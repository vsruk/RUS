#include <Arduino.h>

#define BUTTON1 6   // Tipkalo 1 - koristi digitalRead()
#define BUTTON2 3   // Tipkalo 2 - vanjski prekid 1 (INT1)
#define TRIG 4      // HC-SR04 Trig pin
#define ECHO 2      // HC-SR04 Echo pin (prekid 0) (INT0 - Viši prioritet od INT1)
#define LED 13      // LED za tajmerski prekid

volatile bool button2_pressed = false;
volatile bool sensor_triggered = false;
volatile bool timer_flag = false;
volatile bool nested_occurred = false; // Novi flag za detekciju preklapanja
volatile unsigned long echo_start_time = 0; // Vrijeme početka ECHO signala
volatile unsigned long echo_duration = 0;   // Trajanje ECHO signala

bool lastButton1State = HIGH;  // Za praćenje stanja tipkala 1

// ISR za tipkalo 2 (pokreće slanje ultrazvučnog impulsa) - Srednji prioritet
void ISR_button2() {
  sei(); // Omogući nested interrupts

  // Pošalji TRIG impuls samo kada je gumb pritisnut
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2); // Kratka pauza, u redu unutar ISR
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);// Kratka pauza, u redu unutar ISR
  digitalWrite(TRIG, LOW);

  button2_pressed = true;
  // Serial.println("DEBUG: ISR_button2 finished"); // Može se dodati za debug
}

// ISR za Echo signal s HC-SR04 (detektira reflektirani val) - Najviši prioritet
void ISR_sensor() {
  sei(); // Omogući nested interrupts
  
  // Ako je ovo RISING edge, počinjemo mjeriti vrijeme
  if (digitalRead(ECHO) == HIGH) {
    echo_start_time = micros();
  } else {
    // Ako je FALLING edge, računamo trajanje
    echo_duration = micros() - echo_start_time;
    sensor_triggered = true;
  }

  // Ako je ovaj ISR pozvan dok je drugi (npr. Timer) bio aktivan,
  // ovo će se postaviti. Provjeravamo u loop().
  nested_occurred = true;

  // Serial.println("DEBUG: ISR_sensor finished"); // Može se dodati za debug
}

// ISR za Timer1 (LED treptanje) - Najniži prioritet od ova tri
ISR(TIMER1_COMPA_vect) {
  sei(); // Omogući nested interrupts (ključno za test)

  timer_flag = true; // Signaliziraj loop-u da je vrijeme za toggle LED
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n--- Arduino Prekidi Test ---");
  Serial.println("Prioriteti (HW): ECHO(INT0) > BUTTON2(INT1) > Timer1");
  Serial.println("Nested interrupts su OMOGUĆENI u ISR-ovima.");
  Serial.println("Ultrazvučni senzor će ispisivati udaljenost samo kada je < 100cm");

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW); // Osiguraj da je TRIG isključen na početku
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  // Vanjski prekidi
  attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_button2, FALLING); // INT1
  attachInterrupt(digitalPinToInterrupt(ECHO), ISR_sensor, CHANGE);      // INT0, mijenjamo na CHANGE da uhvatimo i rising i falling edge

  // Timer1 konfiguracija za LED treptanje (1 Hz)
  cli(); // Isključi prekide tijekom podešavanja
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624; // 1 sekunda (16 MHz / 1024 / 1Hz - 1)
  TCCR1B |= (1 << WGM12); // CTC mod
  TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
  TIMSK1 |= (1 << OCIE1A); // enable compare match interrupt
  sei(); // Omogući prekide globalno
}

void loop() {
  bool processed_nested = false;

  // Obrada flag-a postavljenog u ISR_sensor
  // Koristimo noInterrupts/interrupts da osiguramo atomičnost čitanja i resetiranja flag-a
  noInterrupts();
  if (nested_occurred) {
    processed_nested = true; // Zapamti da smo ovo obradili
    nested_occurred = false; // Resetiraj flag
    interrupts(); // OMOGUĆI PREKIDE što prije

    Serial.println("### LOOP: DETEKTIRANO PREKLAPANJE (nested_occurred flag) ###");
    // Ovdje možemo dodati dodatnu logiku ako je potrebno znati da se preklapanje dogodilo
  } else {
    interrupts(); // Obavezno omogući prekide i ako flag nije bio postavljen
  }

  // Tipkalo 1 se čita ručno, za dodatno testiranje
  bool button1State = digitalRead(BUTTON1);
  if (lastButton1State == HIGH && button1State == LOW) {
    Serial.println("[GUMB] Tipkalo 1 pritisnuto!");
  }
  lastButton1State = button1State;

  // Obrada Echo prekida (reflektirani signal)
  noInterrupts();
  if (sensor_triggered) {
    // Kopiraj vrijednost u lokalnu varijablu prije resetiranja
    unsigned long duration = echo_duration;
    sensor_triggered = false; // Resetiraj flag
    interrupts(); // OMOGUĆI PREKIDE

    // Izračunaj udaljenost u centimetrima (brzina zvuka = 343m/s)
    float distance_cm = duration * 0.0343 / 2; // Pretvori mikrosekunde u centimetre
    
    // Ispiši samo ako je udaljenost manja od 100cm
    if (distance_cm < 100) {
      Serial.print("[PREKID OBRADA] Ultrazvučni senzor: ");
      Serial.print(distance_cm);
      Serial.println(" cm (< 100cm)");
    }
  } else {
    interrupts();
  }

  // Obrada prekida gumba 2
  noInterrupts();
  if (button2_pressed) {
    button2_pressed = false; // Resetiraj flag
    interrupts(); // OMOGUĆI PREKIDE

    Serial.println("[PREKID OBRADA] Tipkalo 2 pritisnuto i poslan TRIG!");
  } else {
    interrupts();
  }

  // Obrada timer prekida - LED treptanje
  noInterrupts();
  if (timer_flag) {
    timer_flag = false; // Resetiraj flag
    interrupts(); // OMOGUĆI PREKIDE

    Serial.println("[PREKID OBRADA] Pocetak obrade Timer1 eventa...");
    digitalWrite(LED, !digitalRead(LED)); // Toggle LED
    Serial.println("[PREKID OBRADA] ...Zavrsetak obrade Timer1 eventa (LED toggle).");

    // Ako smo u istom prolazu loop-a detektirali preklapanje, ispiši to OVDJE
    // da se vidi da se dogodilo "unutar" logičkog bloka Timer-a.
    if (processed_nested) {
        Serial.println("    >>> Napomena: Preklapanje (ISR_sensor) detektirano tijekom ovog Timer ciklusa! <<<");
    }
  } else {
    interrupts();
  }
}

// serialEvent se izvršava između prolaza loop() funkcije
void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print("Primljeno preko Serial: ");
    Serial.println(c);
  }
}
