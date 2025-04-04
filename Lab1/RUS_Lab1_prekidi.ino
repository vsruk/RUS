/**
 * @file RUS_Lab1_prekidi.ino
 * @brief Primjer rada s prekidima i ugnježđeni prekidi za tipkala.
 *
 * Ovaj program prikazuje mogućnosti rada s prekidima, mjeri udaljenost pomoću HC-SR04 senzora, 
 * obrađuje prekide s ugnježđenim prioritetima za tipkala i upravlja odgovarajućim LED indikatorskim lampicama
 * koje trepere tijekom aktivnih prekida.
 * 
 * @section led_indikatori LED indikatori:
 * 
 * | Pin        | Funkcija                 | Prioritet         |
 * |------------|--------------------------|-------------------|
 * | LED_INT0   | Visoki prioritet (INT0)   | Najviši           |
 * | LED_INT1   | Srednji prioritet (INT1)  | Srednji           |
 * | LED_INT2   | Niski prioritet (INT2)    | Niski             |
 * | LED_ALERT  | Alarm udaljenosti         | -                 |
 * | LED_Timer  | Alarm tajmera             | -                 |
 *
 * @section tipkala Tipkala:
 * 
 * | Pin        | Tipka  | Funkcija      |
 * |------------|--------|---------------|
 * | BUTTON0    | Tipka 0 | Ulazni signal |
 * | BUTTON1    | Tipka 1 | Ulazni signal |
 * | BUTTON2    | Tipka 2 | Ulazni signal |
 *
 * @note
 * AVR arhitektura (ATmega2560) ne podržava preempciju, tj. automatsko preklapanje prekida višeg prioriteta unutar ISR-a ni ugnježđene prioritete. 
 * Iako je implementirana programska prioritizacija prekida, stvarni ugnježđeni prekidi nisu podržani na razini sklopovlja.
 * Prekidi su prioritetizirani programski, ali ne stvarno ugnježđeni.
 *
 * @author Vlado Sruk
 * @date 20. ožujak 2025.
 * @version 1.0
 * @note Licenca: MIT Licenca - Slobodno korištenje, modifikacija i distribucija.
 * 
 * @note Zavisnosti: prekidi AVR avr/interrupt.h
 *
 * @dot
 * digraph cfg {
 *     // Nodes
 *     setup [label="setup()", shape=box];
 *     loop [label="loop()", shape=box];
 *     measureDistance [label="measureDistance()", shape=box];
 *     handleInterrupts [label="handleInterrupts()", shape=box];
 *     handleInterrupt [label="handleInterrupt()", shape=box];
 *     triggerDistanceAlert [label="triggerDistanceAlert()", shape=box];
 *     handleTimerInterrupt [label="handleTimerInterrupt()", shape=box];
 *     ISR_INT0 [label="ISR_INT0", shape=box];
 *     ISR_INT1 [label="ISR_INT1", shape=box];
 *     ISR_INT2 [label="ISR_INT2", shape=box];
 *     
 *     // Edges (Control Flow)
 *     setup -> loop;
 *     loop -> measureDistance;
 *     loop -> handleInterrupts;
 *     loop -> handleInterrupt;
 *     loop -> triggerDistanceAlert;
 *     loop -> handleTimerInterrupt;
 *     
 *     measureDistance -> handleInterrupts;
 *     handleInterrupts -> handleInterrupt;
 *     
 *     handleInterrupt -> handleInterrupts;
 *     handleInterrupts -> triggerDistanceAlert;
 *     
 *     handleTimerInterrupt -> handleInterrupts;
 *     
 *     ISR_INT0 -> handleInterrupts;
 *     ISR_INT1 -> handleInterrupts;
 *     ISR_INT2 -> handleInterrupts;
 *     
 *     handleInterrupts -> triggerDistanceAlert;
 *     
 *     // Special handling for conditions (arrows point to actions)
 *     loop -> measureDistance [label="distance check", color=blue];
 *     loop -> handleTimerInterrupt [label="timerFlag", color=red];
 *     handleInterrupts -> triggerDistanceAlert [label="distanceAlert", color=green];
 * }
 * @enddot
 */
/**
 * @section diagrami Dijagram prioriteta prekida
 * 
 * Dijagram prioriteta prekida prikazuje raspored prioriteta za prekide u sustavu:
 * - INT0 ima najviši prioritet.
 * - INT1 ima srednji prioritet.
 * - INT2 ima najniži prioritet.
 *
 * @dot
 * digraph G {
 *   rankdir=TB;
 *   LED_INT0 [label="INT0\nVisoki prioritet" shape=box style=filled color=red];
 *   LED_INT1 [label="INT1\nSrednji prioritet" shape=box style=filled color=yellow];
 *   LED_INT2 [label="INT2\nNiski prioritet" shape=box style=filled color=green];
 *   LED_ALERT [label="Alarm udaljenosti" shape=ellipse];
 *   LED_Timer [label="Alarm tajmera" shape=ellipse];
 *   BUTTON0 [label="Tipka 0" shape=ellipse];
 *   BUTTON1 [label="Tipka 1" shape=ellipse];
 *   BUTTON2 [label="Tipka 2" shape=ellipse];
 * }
 * @enddot
 *
 * Ovaj dijagram prikazuje prioritet prekida (INT0, INT1, INT2) i njihove povezane LED indikatore. 
 * Također uključuje tipke za aktiviranje prekida i alarme za udaljenost i tajmer.
 */

/**
 * @section funkcije Dijagram toka funkcija
 * 
 * Dijagram toka funkcija prikazuje povezanost između funkcija za upravljanje prekidima i LED indikatorima:
 * - Prekidi (ISR) pozivaju funkcije za obradu prekida.
 * - Glavna petlja (loop) obrađuje zastavice postavljene ISR funkcijama.
 *
 * @dot
 * digraph G {
 *   rankdir=LR;
 *   ISR_INT0 -> handleInterrupt;
 *   ISR_INT1 -> handleInterrupt;
 *   ISR_INT2 -> handleInterrupt;
 *   handleInterrupt -> handleInterrupts;
 *   handleInterrupts -> handleTimerInterrupt;
 *   handleTimerInterrupt -> blinkLed;
 * }
 * @enddot
 *
 * Ovaj dijagram prikazuje tok poziva između prekidnih funkcija, obrade prekida, upravljanja timerom i bljeskanja LED dioda.
 */
#include <avr/interrupt.h>

// LED pinovi
/** Pinovi za LED indikatore */
#define LED_INT0 11   ///< Visoki prioritet (INT0)
#define LED_INT1 12   ///< Srednji prioritet (INT1)
#define LED_INT2 13   ///< Niski prioritet (INT2)
#define LED_ALERT 10  ///< Alarm udaljenosti
#define LED_Timer 9   ///< Alarm tajmera

// Tipkala
/** Pinovi za tipke */
#define BUTTON0 2 ///< Tipka 0
#define BUTTON1 3 ///< Tipka 1
#define BUTTON2 21 ///< Tipka 2

// HC-SR04 senzor
/** Pinovi za HC-SR04 ultrazvučni senzor */
#define TRIG_PIN 4  ///< Pin za TRIG signal
#define ECHO_PIN 5  ///< Pin za ECHO signal

// Timer1 konstante
/** Timer konfiguracija za Timer1 */
#define TIMER1_PRESCALER 1024 ///< Preskaler za Timer1
#define TIMER1_COMPARE 15624 ///< Vrijednost za usporedbu timer-a

// Globalne varijable za prekide
/** Globalne varijable za upravljanje prekidima */
volatile bool intFlag[3] = {false, false, false};  ///< Zastavice za prekide
volatile unsigned long lastInterruptTime[3] = {0, 0, 0}; ///< Vrijeme zadnjih prekida
volatile unsigned long lastTimerTime = 0; ///< Vrijeme zadnjeg tajmera
const unsigned long BLINK_INTERVAL = 200; ///< Interval za treptanje LED-a
const int DEBOUNCE_DELAY = 50; ///< Delay za debounce
const int TIMER_DELAY = 1000; ///< Kašnjenje za timer
const int ALARM_DISTANCE = 100; ///< Granica za alarm udaljenosti

volatile bool distanceAlert = false; ///< Zastavica za alarm udaljenosti
volatile bool timerFlag = false; ///< Zastavica za alarm tajmera
volatile bool interruptInProgress = false; ///< Označava obradu prekida s visokim prioritetom

/**
 * @brief Inicijalizacija svih pinova, prekida, timer-a i serijske komunikacije.
 * 
 * Ova funkcija postavlja sve pinove, aktivira prekide na tipkama, postavlja timer i omogućava serijsku komunikaciju.
 */
void setup() {
    pinMode(LED_INT0, OUTPUT);
    pinMode(LED_INT1, OUTPUT);
    pinMode(LED_INT2, OUTPUT);
    pinMode(LED_ALERT, OUTPUT);
    pinMode(LED_Timer, OUTPUT);

    pinMode(BUTTON0, INPUT_PULLUP);
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(BUTTON0), ISR_INT0, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_INT1, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_INT2, FALLING);

    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = TIMER1_COMPARE;
    TIMSK1 = (1 << OCIE1A);

    Serial.begin(9600);
    sei(); // Omogućavanje globalnih prekida
}

/**
 * @brief Glavna petlja koja obrađuje sve funkcionalnosti uređaja.
 * 
 * Funkcija stalno mjeri udaljenost, upravlja prekidima i alarmima te pokreće odgovarajuće radnje.
 */
void loop() {
    float distance = measureDistance(); ///< Mjeri udaljenost s HC-SR04 senzorom
    distanceAlert = (distance > 0 && distance < ALARM_DISTANCE); ///< Provjerava udaljenost za alarm

    handleInterrupts(); ///< Obrada prekida sa višim prioritetom

    if (interruptInProgress) {
        return; ///< Ako je u tijeku prekid s visokim prioritetom, preskoči ostatak petlje
    }

    // Obrada najvišeg prioriteta (tajmer)
    if (timerFlag) {
        handleTimerInterrupt(); ///< Obrada tajmera
    }

    if (distanceAlert) {
        triggerDistanceAlert(); ///< Pokretanje alarma udaljenosti
    }
}

/**
 * @brief Prekidna funkcija za Timer1.
 * 
 * Funkcija koja se poziva kada Timer1 dostigne zadanu usporedbu.
 */
ISR(TIMER1_COMPA_vect) {
    timerFlag = true; ///< Postavljanje zastavice za obradu u petlji
    lastTimerTime = millis(); ///< Snimanje vremena kada je prekid nastao
}

/**
 * @brief Obrada prekida od Timer1.
 * 
 * Funkcija koja obavlja radnje povezane s prekidom timer-a.
 */
void handleTimerInterrupt() {
    interruptInProgress = true; ///< Označavanje da je obrada tajmera u tijeku
    digitalWrite(LED_Timer, HIGH); ///< Aktivacija LED za tajmer
    Serial.println("TIMER INTERRUPT (HIGHEST PRIORITY)"); ///< Ispis na serijski monitor
    delay(200); ///< Kratka pauza
    digitalWrite(LED_Timer, LOW); ///< Gašenje LED-a
    timerFlag = false; ///< Resetiranje zastavice za tajmer
    interruptInProgress = false; ///< Završetak obrade
}

/**
 * @brief Prekidna funkcija za INT0.
 * 
 * Funkcija koja se poziva kada je pritisnuta tipka 0 (INT0).
 */
void ISR_INT0() {
    if (!interruptInProgress) {
        handleInterrupt(0, "INT0 (HIGH priority) triggered"); ///< Obrada prekida INT0 s visokim prioritetom
    }
}

/**
 * @brief Prekidna funkcija za INT1.
 * 
 * Funkcija koja se poziva kada je pritisnuta tipka 1 (INT1).
 */
void ISR_INT1() {
    if (!interruptInProgress) {
        handleInterrupt(1, "INT1 (MEDIUM priority) triggered"); ///< Obrada prekida INT1 sa srednjim prioritetom
    }
}

/**
 * @brief Prekidna funkcija za INT2.
 * 
 * Funkcija koja se poziva kada je pritisnuta tipka 2 (INT2).
 */
void ISR_INT2() {
    if (!interruptInProgress) {
        handleInterrupt(2, "INT2 (LOW priority) triggered"); ///< Obrada prekida INT2 s niskim prioritetom
    }
}

/**
 * @brief Obrada prekida s prioritetima
 * 
 * Ova funkcija obrađuje prekide postavljanjem odgovarajuće zastavice (`intFlag`) na temelju indeksa prekida i ispisuje poruku na serijski monitor.
 * Funkcija također implementira zaštitu od "debounce" efekta, čime se sprječava višestruko prepoznavanje prekida unutar kratkog vremenskog perioda.
 * 
 * @param index Indeks prekida (0 za INT0, 1 za INT1, 2 za INT2)
 * @param message Poruka koja se ispisuje na serijski monitor kada se prekid obradi
 * 
 * @note Funkcija koristi globalnu varijablu `lastInterruptTime` za praćenje vremena posljednjeg prekida i sprječavanje "debounce" efekta.
 * 
 * @see handleInterrupts() Za daljnju obradu prekida prema prioritetima.
 * 
 * @dot
 * digraph G {
 *   rankdir=LR;
 *   handleInterrupt [label="handleInterrupt\nObrada prekida" shape=box style=filled color=lightblue];
 *   debounceCheck [label="Debounce provjera\n(millis() - lastInterruptTime[index] < DEBOUNCE_DELAY)" shape=ellipse];
 *   setFlag [label="Postavljanje intFlag[index] = true" shape=ellipse];
 *   serialPrint [label="Ispis poruke na serijski monitor" shape=ellipse];
 *   
 *   handleInterrupt -> debounceCheck;
 *   debounceCheck -> setFlag [label="Ako je debounce prošao"];
 *   setFlag -> serialPrint;
 * }
 * @enddot
 */
 
void handleInterrupt(int index, const char* message) {
    if (millis() - lastInterruptTime[index] < DEBOUNCE_DELAY) return;
    lastInterruptTime[index] = millis();
    intFlag[index] = true;
    Serial.println(message);
}

/**
 * @brief Obrada svih prekida prema prioritetu.
 * 
 * Funkcija koja provjerava i obrađuje prekide prema prioritetu.
																 
 */
void handleInterrupts() {
    if (intFlag[0]) {
        blinkLed(LED_INT0); ///< LED za INT0 (najviši prioritet)
        intFlag[0] = false;
    }
    else if (intFlag[1]) {
        blinkLed(LED_INT1); ///< LED za INT1 (srednji prioritet)
        intFlag[1] = false;
    }
    else if (intFlag[2]) {
        blinkLed(LED_INT2); ///< LED za INT2 (niskim prioritetom)
        intFlag[2] = false;
    }
}

/**
 * @brief Bljeskanje LED-a na temelju vremena.
 * 
 * Funkcija koja blinka LED-om za zadani pin.
 * 
 * @param ledPin Pin na kojem je spojena LED dioda
 */
void blinkLed(int ledPin) {
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        digitalWrite(ledPin, !digitalRead(ledPin)); ///< Bljeskanje LED-a
        delay(BLINK_INTERVAL); ///< Interval treptanja
    }
    digitalWrite(ledPin, LOW); ///< Gašenje LED-a
}

/**
 * @brief Mjerenje udaljenosti pomoću HC-SR04 senzora.
 * 
 * Funkcija koja pokreće senzor i vraća izmjerenu udaljenost.
 * 
 * @return Udaljenost u centimetrima, ili 0 ako nije valjana
 */
float measureDistance() {
    digitalWrite(TRIG_PIN, LOW); ///< Osiguranje da je TRIG_PIN na LOW
    delayMicroseconds(2); ///< Kratka pauza
    digitalWrite(TRIG_PIN, HIGH); ///< Pokretanje signala
    delayMicroseconds(10); ///< Trajanje impulsa
    digitalWrite(TRIG_PIN, LOW); ///< Zaustavljanje signala

    long duration = pulseIn(ECHO_PIN, HIGH); ///< Mjerenje trajanja ECHO signala
    float distance = (duration / 2.0) * 0.0344; ///< Izračunavanje udaljenosti
    return distance;
}

/**
 * @brief Pokretanje alarma udaljenosti.
 * 
 * Funkcija koja pokreće alarm kada udaljenost padne ispod definirane granice.
 */
void triggerDistanceAlert() {
    digitalWrite(LED_ALERT, HIGH); ///< Uključivanje LED-a za alarm
    Serial.println("Distance alert triggered!"); ///< Ispis na serijskom monitoru
    delay(500); ///< Kratka pauza
    digitalWrite(LED_ALERT, LOW); ///< Gašenje LED-a
}
