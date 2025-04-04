/**
 * @file RUS_Lab1_prekidi.ino
 * @mainpage RUS - Rad s Ugradbenim Sustavima
 * @section overview Pregled modula
 *
 * Ovaj projekt demonstrira rad s vanjskim prekidima, uključujući prioritizaciju prekida na AVR mikroprocesoru.
 * Program omogućuje upravljanje prekidima putem tipkala (INT0, INT1, INT2) i koristi Timer1 za generiranje vremenskih prekida.
 * Također se koristi HC-SR04 ultrazvučni senzor za mjerenje udaljenosti, pri čemu se aktivira LED alarm ako je udaljenost manja od definiranog praga.
 * U ovom programu implementirana je programska prioritizacija prekida, pri čemu Timer1 ima najviši prioritet, a prekidi INT0, INT1 i INT2 su obrađeni u skladu s definiranom hijerarhijom.
 *
 * @section modules Pregled funkcionalnosti
 * - @ref group_prekidi "Funkcije za upravljanje prekidima"
 * - Shema spajanja
 *
 * @section links Projekt i simulacija
 * Projekt i simulacija dostupni su na Wokwi platformi: [Wokwi projekt](https://wokwi.com/)@brief Primjer rada s prekidima i programska prioritizacija prekida za tipkala.
 *
 *
 * @section led_indikatori LED indikatori
 *
 * | Pin        | Funkcija                     | Prioritet         |
 * |------------|------------------------------|-------------------|
 * | LED_INT0   | Indikator prekida INT0       | Najviši           |
 * | LED_INT1   | Indikator prekida INT1       | Srednji           |
 * | LED_INT2   | Indikator prekida INT2       | Niski             |
 * | LED_ALERT  | Indikator alarma udaljenosti | -                 |
 * | LED_Timer  | Indikator alarma tajmera     | Najviši (programski)|
 *
 * @section tipkala Tipkala za prekide
 *
 * | Pin        | Tipka   | Funkcija aktivacije prekida |
 * |------------|---------|-----------------------------|
 * | BUTTON0    | Tipka 0 | Aktivira INT0 (najviši)     |
 * | BUTTON1    | Tipka 1 | Aktivira INT1 (srednji)     |
 * | BUTTON2    | Tipka 2 | Aktivira INT2 (niski)       |
 *
 * @section hcsr04 HC-SR04 Ultrazvučni senzor
 *
 * Koristi se za mjerenje udaljenosti. Ako izmjerena udaljenost padne ispod definiranog praga,
 * aktivira se LED indikator alarma udaljenosti (LED_ALERT).
 *
 * @section timer1 Timer1 prekid
 *
 * Timer1 je konfiguriran za generiranje periodičkog prekida. Ovaj prekid ima programski najviši
 * prioritet i koristi se za demonstraciju prekida s najvišim prioritetom putem LED indikatora (LED_Timer).
 *
 * @note
 * Iako je implementirana programska prioritizacija prekida, stvarni ugnježđeni prekidi (preemption)
 * nisu podržani na AVR arhitekturi (ATmega2560) na razini sklopovlja. Program osigurava da se
 * prekidi obrađuju prema definiranom prioritetu u glavnoj petlji. Prekid tajmera ima najviši
 * prioritet, zatim INT0, INT1 i na kraju INT2.
 *
 * @author Vlado Sruk
 * @date 20. ožujak 2025.
 * @version 1.0
 * @note Licenca: MIT Licenca - Slobodno korištenje, modifikacija i distribucija.
 *
 * @note Zavisnosti: <avr/interrupt.h> za rad s prekidima.
 */

/**
 * @section cfg_tok Dijagram toka programa
 * @brief Prikazuje kontrolni tok glavnih funkcija programa.
 * 
 * Ovaj dijagram prikazuje
 *
 * @dot
 *
 * digraph cfg {
 * node [shape=box];
 * setup -> loop [label="Jednom pri pokretanju"];
 * loop -> measureDistance [label="U svakoj iteraciji"];
 * loop -> handleInterrupts [label="U svakoj iteraciji"];
 * loop -> handleTimerInterruptCheck [label="U svakoj iteraciji"];
 * handleTimerInterruptCheck -> handleTimerInterrupt [label="timerFlag == true"];
 * measureDistance -> handleInterrupts [label="Nakon mjerenja"];
 * handleInterrupts -> triggerDistanceAlert [label="distanceAlert == true"];
 *
 * subgraph cluster_interrupts {
 * label = "Obrada prekida";
 * handleInterrupts -> handleInterrupt [label="ako je intFlag[i] true"];
 * ISR_INT0 -> handleInterrupt [label="prekid INT0"];
 * ISR_INT1 -> handleInterrupt [label="prekid INT1"];
 * ISR_INT2 -> handleInterrupt [label="prekid INT2"];
 * }
 *
 * style setup fillcolor=#CCEEFF, stroke=blue, strokewidth=2;
 * style loop fillcolor=#FFEECC, stroke=orange, strokewidth=2;
 * style measureDistance fillcolor=#DDFFDD, stroke=green, strokewidth=2;
 * style handleInterrupts fillcolor=#FFFFDD, stroke=purple, strokewidth=2;
 * style triggerDistanceAlert fillcolor=#FFDDDD, stroke=red, strokewidth=2;
 * style handleTimerInterruptCheck fillcolor=#DDDDFF, stroke=brown, strokewidth=2;
 * style handleTimerInterrupt fillcolor=#E0FFFF, stroke=cyan, strokewidth=2;
 * style ISR_INT0 fillcolor=#F0FFF0, stroke=gray;
 * style ISR_INT1 fillcolor=#F0FFF0, stroke=gray;
 * style ISR_INT2 fillcolor=#F0FFF0, stroke=gray;
 * }
 * @enddot
 */
 
 
/**
 * @section priority_diagram Dijagram prioriteta prekida
 * @brief Vizualizacija prioritetnog rasporeda prekida.
 *
 * Ovaj dijagram prikazuje relativni prioritet programski implementiranih prekida.
 * Prekid tajmera ima najviši prioritet, zatim slijede vanjski prekidi INT0, INT1 i INT2.
 *
 * @dot
 * digraph priority {
 * rankdir=TB;
 * Timer1 [shape=box, style=filled, fillcolor=red, label="Timer1\n(Najviši programski prioritet)"];
 * subgraph cluster_external_interrupts {
 * label = "Programski prioritet vanjskih prekida";
 * style = rounded;
 * INT0 [shape=box, style=filled, fillcolor=yellow, label="INT0"];
 * INT1 [shape=box, style=filled, fillcolor=green, label="INT1"];
 * INT2 [shape=box, style=filled, fillcolor=lightblue, label="INT2\n(Najniži programski prioritet)"];
 * INT0 -> INT1 [label="ima viši prioritet od"];
 * INT1 -> INT2 [label="ima viši prioritet od"];
 * }
 * Timer1 -> INT0 [label="ima najviši prioritet od"];
 * }
 * @enddot
 */

/**
 * @section function_flow Dijagram toka obrade prekida
 * @brief Prikazuje kako se prekidi detektiraju i obrađuju.
 *
 * Kada se aktivira prekid (tipka pritisnuta ili Timer1 istekne), odgovarajuća ISR funkcija postavlja
 * zastavicu. Glavna petlja periodički provjerava te zastavice i poziva odgovarajuće funkcije za
 * obradu prekida prema definiranom prioritetu.
 *
 * @dot
 * digraph interrupt_flow {
 * rankdir=LR;
 * subgraph cluster_interrupt_service_routines {
 * label = "Prekidne rutine (ISR)";
 * ISR_INT0 [shape=ellipse];
 * ISR_INT1 [shape=ellipse];
 * ISR_INT2 [shape=ellipse];
 * TIMER1_COMPA_vect [shape=ellipse, label="TIMER1_COMPA_vect\n(ISR)"];
 * }
 * handleInterrupts [shape=box];
 * handleInterrupt [shape=box];
 * blinkLed [shape=box];
 * handleTimerInterrupt [shape=box];
 *
 * TIMER1_COMPA_vect -> handleTimerInterrupt;
 * handleTimerInterrupt -> blinkLed [label="za LED_Timer"];
 *
 * ISR_INT0 -> handleInterrupt [label="index=0"];
 * ISR_INT1 -> handleInterrupt [label="index=1"];
 * ISR_INT2 -> handleInterrupt [label="index=2"];
 *
 * handleInterrupt -> handleInterrupts;
 * handleInterrupts -> blinkLed [label="ako je intFlag[0] true,\nbljesni LED_INT0"];
 * handleInterrupts -> blinkLed [label="ako je intFlag[1] true,\nbljesni LED_INT1"];
 * handleInterrupts -> blinkLed [label="ako je intFlag[2] true,\nbljesni LED_INT2"];
 * }
 * @enddot
 */

#include <avr/interrupt.h>

// Definiranje pinova za LED indikatore
#define LED_INT0 11    ///< Pin za LED indikator prekida INT0 (visoki prioritet)
#define LED_INT1 12    ///< Pin za LED indikator prekida INT1 (srednji prioritet)
#define LED_INT2 13    ///< Pin za LED indikator prekida INT2 (niski prioritet)
#define LED_ALERT 10   ///< Pin za LED indikator alarma udaljenosti
#define LED_Timer 9    ///< Pin za LED indikator alarma tajmera

// Definiranje pinova za tipkala
#define BUTTON0 2      ///< Pin za tipkalo koje aktivira prekid INT0
#define BUTTON1 3      ///< Pin za tipkalo koje aktivira prekid INT1
#define BUTTON2 21     ///< Pin za tipkalo koje aktivira prekid INT2

// Definiranje pinova za HC-SR04 senzor
#define TRIG_PIN 4     ///< Pin za TRIG signal HC-SR04 senzora
#define ECHO_PIN 5     ///< Pin za ECHO signal HC-SR04 senzora

// Konstante za konfiguraciju Timer1
#define TIMER1_PRESCALER 1024  ///< Preskaler za Timer1 (F_CPU / 1024)
#define TIMER1_COMPARE 15624   ///< Vrijednost za usporedbu (za generiranje prekida svakih 1 sekundu pri 16MHz)

// Globalne varijable za upravljanje prekidima i stanjima
volatile bool intFlag[3] = {false, false, false};    ///< Zastavice koje signaliziraju aktivaciju prekida (INT0, INT1, INT2)
volatile unsigned long lastInterruptTime[3] = {0, 0, 0}; ///< Vremenski žigovi posljednjih prekida za debounce
volatile unsigned long lastTimerTime = 0;             ///< Vremenski žig posljednjeg prekida tajmera
const unsigned long BLINK_INTERVAL = 200;             ///< Interval treptanja LED indikatora (u milisekundama)
const int DEBOUNCE_DELAY = 50;                        ///< Vrijeme ignoriranja uzastopnih pritisaka tipke (u milisekundama)
const int TIMER_DELAY = 1000;                         ///< Periodičnost Timer1 prekida (u milisekundama)
const int ALARM_DISTANCE = 100;                       ///< Prag udaljenosti (u centimetrima) za aktivaciju alarma

volatile bool distanceAlert = false;                 ///< Zastavica koja označava da je udaljenost ispod praga
volatile bool timerFlag = false;                     ///< Zastavica koja označava da je Timer1 prekid nastupio
volatile bool interruptInProgress = false;           ///< Zastavica koja sprječava obradu prekida nižeg prioriteta tijekom obrade višeg

/**
 * @brief Inicijalizacija pinova, konfiguracija prekida i Timer1.
 *
 * Ova funkcija postavlja smjer pinova za LED indikatore i tipkala, aktivira vanjske prekide
 * na padajući brid signala s tipkala, konfigurira Timer1 za generiranje periodičkih prekida
 * i inicijalizira serijsku komunikaciju za ispis poruka. Globalni prekidi su omogućeni na kraju.
 */
void setup() {
  // Postavljanje pinova LED indikatora kao izlaze
  pinMode(LED_INT0, OUTPUT);
  pinMode(LED_INT1, OUTPUT);
  pinMode(LED_INT2, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
  pinMode(LED_Timer, OUTPUT);

  // Postavljanje pinova tipkala kao ulaze s internim pull-up otpornicima
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  // Postavljanje pinova HC-SR04 senzora
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Povezivanje prekidnih rutina s odgovarajućim pinovima i načinom okidanja
  attachInterrupt(digitalPinToInterrupt(BUTTON0), ISR_INT0, FALLING); // INT0 na pinu 2
  attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_INT1, FALLING); // INT1 na pinu 3
  attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_INT2, FALLING); // INT2 na pinu 21

  // Konfiguracija Timer1 za generiranje prekida svakih 1 sekundu
  TCCR1A = 0; // Resetiraj registar kontrole timera A
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC način rada (Clear Timer on Compare Match), preskaler 1024
  OCR1A = TIMER1_COMPARE; // Postavi vrijednost za usporedbu
  TIMSK1 = (1 << OCIE1A); // Omogući prekid kod usporedbe A

  // Inicijalizacija serijske komunikacije
  Serial.begin(9600);
  Serial.println("Inicijalizacija zavrsena.");

  sei(); // Omogući globalne prekide
}

/**
 * @brief Glavna programska petlja.
 *
 * U ovoj petlji se periodički mjeri udaljenost, provjeravaju se i obrađuju aktivirani prekidi
 * prema prioritetu, te se aktivira alarm udaljenosti ako je potrebno. Obrada prekida s
 * najvišim prioritetom (tajmera) se provjerava na početku petlje.
 */
void loop() {
  // Provjera i obrada prekida tajmera s najvišim prioritetom
  if (timerFlag) {
    handleTimerInterrupt();
  }

  // Mjerenje udaljenosti
  float distance = measureDistance();
  // Postavljanje zastavice alarma udaljenosti ako je udaljenost manja od praga
  distanceAlert = (distance > 0 && distance < ALARM_DISTANCE);

  // Obrada aktiviranih prekida prema prioritetu (INT0, INT1, INT2)
  handleInterrupts();

  // Ako je aktiviran alarm udaljenosti i nema prekida u tijeku, pokreni alarm
  if (distanceAlert && !interruptInProgress) {
    triggerDistanceAlert();
  }
}

/**
 * @brief Prekidna rutina za Timer1 Compare Match A.
 *
 * Ova funkcija se poziva kada Timer1 dosegne vrijednost usporedbe OCR1A. Postavlja zastavicu
 * koja signalizira da je potrebno obraditi prekid tajmera u glavnoj petlji.
 */
ISR(TIMER1_COMPA_vect) {
  timerFlag = true; // Postavi zastavicu za obradu prekida tajmera u glavnoj petlji
  lastTimerTime = millis(); // Zabilježi vrijeme prekida
}

/**
 * @brief Obrada prekida tajmera.
 *
 * Ova funkcija se poziva iz glavne petlje kada je postavljena zastavica `timerFlag`. Označava
 * početak obrade prekida s najvišim prioritetom, aktivira LED indikator tajmera, ispisuje poruku
 * na serijski monitor, kratko pauzira, gasi LED indikator i resetira zastavice.
 */
void handleTimerInterrupt() {
  interruptInProgress = true; // Označi da je obrada prekida s visokim prioritetom u tijeku
  digitalWrite(LED_Timer, HIGH); // Uključi LED indikator tajmera
  Serial.println("TIMER INTERRUPT (NAJVIŠI PRIORITET)");
  delay(200);
  digitalWrite(LED_Timer, LOW); // Isključi LED indikator tajmera
  timerFlag = false; // Resetiraj zastavicu prekida tajmera
  interruptInProgress = false; // Završi obradu prekida s visokim prioritetom
}

/**
 * @brief Prekidna rutina za vanjski prekid INT0.
 *
 * Ova funkcija se poziva kada se detektira padajući brid na pinu spojenom na BUTTON0.
 * Ako trenutno nije u tijeku obrada prekida s višim prioritetom, poziva funkciju za obradu
 * prekida s indeksom 0 (za INT0).
 */
void ISR_INT0() {
  if (!interruptInProgress) {
    handleInterrupt(0, "INT0 (VISOKI prioritet) aktiviran");
  }
}

/**
 * @brief Prekidna rutina za vanjski prekid INT1.
 *
 * Ova funkcija se poziva kada se detektira padajući brid na pinu spojenom na BUTTON1.
 * Ako trenutno nije u tijeku obrada prekida s višim prioritetom, poziva funkciju za obradu
 * prekida s indeksom 1 (za INT1).
 */
void ISR_INT1() {
  if (!interruptInProgress) {
    handleInterrupt(1, "INT1 (SREDNJI prioritet) aktiviran");
  }
}

/**
 * @brief Prekidna rutina za vanjski prekid INT2.
 *
 * Ova funkcija se poziva kada se detektira padajući brid na pinu spojenom na BUTTON2.
 * Ako trenutno nije u tijeku obrada prekida s višim prioritetom, poziva funkciju za obradu
 * prekida s indeksom 2 (za INT2).
 */
void ISR_INT2() {
  if (!interruptInProgress) {
    handleInterrupt(2, "INT2 (NISKI prioritet) aktiviran");
  }
}

/**
 * @brief Obrada pojedinačnog prekida.
 *
 * Ova funkcija provjerava debounce za pritisak tipke, postavlja odgovarajuću zastavicu
 * prekida (`intFlag`) i ispisuje poruku o aktiviranom prekidu na serijski monitor.
 *
 * @param index Indeks prekida (0 za INT0, 1 za INT1, 2 za INT2).
 * @param message Poruka koja se ispisuje prilikom aktivacije prekida.
 *
 * @note Koristi se za sprječavanje višestrukog okidanja prekida uslijed "debounce" efekta tipkala.
 */
void handleInterrupt(int index, const char* message) {
  if (millis() - lastInterruptTime[index] < DEBOUNCE_DELAY) {
    return; // Ignoriraj prekid ako je unutar debounce perioda
  }
  lastInterruptTime[index] = millis();
  intFlag[index] = true;
  Serial.println(message);
}

/**
 * @brief Obrada svih aktiviranih prekida prema prioritetu.
 *
 * Ova funkcija provjerava zastavice prekida redom prioriteta (INT0, zatim INT1, zatim INT2).
 * Ako je zastavica postavljena, poziva funkciju za bljeskanje odgovarajućeg LED indikatora
 * i resetira zastavicu. Obrada prekida nižeg prioriteta se preskače ako je u tijeku obrada
 * prekida višeg prioriteta (što se kontrolira globalnom varijablom `interruptInProgress`).
 */
void handleInterrupts() {
  interruptInProgress = true; // Označi da je započela obrada prekida

  if (intFlag[0]) {
    blinkLed(LED_INT0); // Bljesni LED za INT0 (najviši prioritet)
    intFlag[0] = false;
  } else if (intFlag[1]) {
    blinkLed(LED_INT1); // Bljesni LED za INT1 (srednji prioritet)
    intFlag[1] = false;
  } else if (intFlag[2]) {
    blinkLed(LED_INT2); // Bljesni LED za INT2 (niski prioritet)
    intFlag[2] = false;
  }

  interruptInProgress = false; // Završi obradu prekida
}

/**
 * @brief Bljeskanje LED indikatora.
 *
 * Ova funkcija uključuje i isključuje LED diodu spojenu na zadani pin tijekom određenog vremenskog
 * perioda, stvarajući efekt treptanja.
 *
 * @param ledPin Pin na kojem je spojena LED dioda.
 */
void blinkLed(int ledPin) {
  unsigned long startTime = millis();
  while (millis() - startTime < 1000) {
    digitalWrite(ledPin, !digitalRead(ledPin)); // Promijeni stanje LED-a
    delay(BLINK_INTERVAL);
  }
  digitalWrite(ledPin, LOW); // Osiguraj da je LED isključena nakon bljeskanja
}

/**
 * @brief Mjerenje udaljenosti pomoću HC-SR04 ultrazvučnog senzora.
 *
 * Ova funkcija šalje kratki impuls na TRIG pin senzora i zatim mjeri trajanje ECHO pulsa.
 * Iz trajanja pulsa se izračunava udaljenost do objekta.
 *
 * @return Izmjerena udaljenost u centimetrima. Vraća 0 ako mjerenje nije uspjelo.
 */
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH); // Mjeri trajanje ECHO pulsa

  // Brzina zvuka je oko 343 m/s ili 0.0343 cm/mikrosekundi
  // Ukupno vrijeme pulsa uključuje put do objekta i natrag, stoga se dijeli s 2
  float distance = (duration / 2.0) * 0.0343;

  // Vrati izmjerenu udaljenost, ili 0 ako je trajanje predugo ili prekratko
  return (duration > 0 && duration < 30000) ? distance : 0; // Ograničenje za realna mjerenja
}

/**
 * @brief Aktivacija alarma udaljenosti.
 *
 * Ova funkcija se poziva kada izmjerena udaljenost padne ispod definiranog praga (`ALARM_DISTANCE`).
 * Uključuje LED indikator alarma udaljenosti i ispisuje poruku na serijski monitor.
 */
void triggerDistanceAlert() {
  digitalWrite(LED_ALERT, HIGH); // Uključi LED alarm udaljenosti
  Serial.println("ALARM UDALJENOSTI AKTIVIRAN!");
  delay(500);
  digitalWrite(LED_ALERT, LOW);  // Isključi LED alarm udaljenosti
}
