/**
 * @file
 * @brief Simulates interrupts on an Arduino Mega (Wokwi project).
 *
 * This project demonstrates how to simulate external and timer interrupts using an Arduino Mega.
 * It uses three buttons (BUTTON0, BUTTON1, BUTTON2) to trigger external interrupts and a timer
 * (via Timer1) for periodic events. The program also measures distance using an ultrasonic sensor
 * and triggers a distance alert if an object is too close.
 *
 * Various LEDs indicate the interrupt events and sensor/timer actions. Additionally, several
 * digital pins are toggled to simulate signals for a logic analyzer.
 *
 * @note This code is designed for use with the Wokwi simulation environment.
 */

#include <avr/interrupt.h>

// Pin definitions
#define BUTTON0      2
#define BUTTON1      3
#define BUTTON2      21
#define LED_INT0     8
#define LED_INT1     9
#define LED_INT2     10
#define LED_Sensor   11
#define LED_Timer    12
#define TRIG_PIN     6
#define ECHO_PIN     5

#define TIMER1_PRESCALER   1024
#define TIMER1_COMPARE     15624
#define BLINK_INTERVAL     200
#define DEBOUNCE_DELAY     50
#define TIMER_DELAY        1000
#define ALARM_DISTANCE     100

// Logic analyzer pin definitions
#define LOGIC_ANALYZER_PIN0 14 
#define LOGIC_ANALYZER_PIN1 15 
#define LOGIC_ANALYZER_PIN2 16 
#define LOGIC_ANALYZER_TIMER 17
#define LOGIC_ANALYZER_SENSOR 18 

// Global volatile variables used in ISRs and main loop
volatile bool     intFlag[3] = {0};
volatile unsigned long lastInterruptTime[3] = {0}, lastTimerTime = 0;
volatile bool     distanceAlert = 0, timerFlag = 0;
volatile bool     pastDistanceAlert;

/**
 * @brief Arduino setup function.
 *
 * Configures the pin modes for LEDs, buttons, ultrasonic sensor, and logic analyzer pins.
 * Attaches external interrupts for BUTTON0, BUTTON1, and BUTTON2.
 * Sets up Timer1 to generate an interrupt when it reaches the compare value.
 * Initializes serial communication and enables interrupts.
 */
void setup(){
    pinMode(LED_INT0,    OUTPUT);
    pinMode(LED_INT1,    OUTPUT);
    pinMode(LED_INT2,    OUTPUT);
    pinMode(LED_Sensor,  OUTPUT);
    pinMode(LED_Timer,   OUTPUT);

    pinMode(BUTTON0, INPUT_PULLUP);
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(LOGIC_ANALYZER_PIN0, OUTPUT);
    pinMode(LOGIC_ANALYZER_PIN1, OUTPUT);
    pinMode(LOGIC_ANALYZER_PIN2, OUTPUT);
    pinMode(LOGIC_ANALYZER_TIMER, OUTPUT);
    pinMode(LOGIC_ANALYZER_SENSOR, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(BUTTON0), ISR_INT0, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_INT1, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_INT2, FALLING);

    // Configure Timer1 in CTC mode with prescaler 1024
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A  = TIMER1_COMPARE;
    TIMSK1 = (1 << OCIE1A);
    
    Serial.begin(9600);
    sei();  // Enable global interrupts
    float d = measureDistance();
    pastDistanceAlert = (d > 0 && d < ALARM_DISTANCE);
    if (pastDistanceAlert == 1){
      digitalWrite(LOGIC_ANALYZER_SENSOR, HIGH);
      Serial.println("ALARM: Predmet preblizu (<100cm, LOWEST PRIORITY)!");
    }
}

/**
 * @brief Arduino main loop.
 *
 * Continuously measures the distance using the ultrasonic sensor.
 * Sets the distanceAlert flag based on the measured distance.
 * Checks for a timer interrupt flag and handles it.
 * Processes any pending external interrupts by blinking the corresponding LEDs.
 * Triggers a distance alert if an object is detected too close.
 */
void loop(){
    float d = measureDistance();
    distanceAlert = (d > 0 && d < ALARM_DISTANCE);
    if(pastDistanceAlert != distanceAlert){
      if (distanceAlert == 0){
        digitalWrite(LOGIC_ANALYZER_SENSOR, LOW);
      } else {
        digitalWrite(LOGIC_ANALYZER_SENSOR, HIGH);
        Serial.println("ALARM: Predmet preblizu (<100cm, LOWEST PRIORITY)!");
      }
      pastDistanceAlert = distanceAlert;
    }

    if(timerFlag){ 
        handleTimerInterrupt(); 
    }
    
    handleBlinking();
    
    triggerDistanceAlert();
}

/**
 * @brief Timer1 Compare Match Interrupt Service Routine.
 *
 * This ISR is called when Timer1 reaches the compare value.
 * It toggles the logic analyzer timer pin, sets the timer flag, and updates the last timer time.
 */
ISR(TIMER1_COMPA_vect){
    digitalWrite(LOGIC_ANALYZER_TIMER, HIGH);
    timerFlag = true;
    lastTimerTime = millis();
    digitalWrite(LOGIC_ANALYZER_TIMER, LOW);
}

/**
 * @brief Handles the timer interrupt event.
 *
 * Activates the LED for timer events, prints a message to the Serial Monitor,
 * waits for a short duration, then turns off the LED and resets the timer flag.
 */
void handleTimerInterrupt(){
    digitalWrite(LED_Timer, HIGH);
    Serial.println("TIMER INTERRUPT (HIGHEST PRIORITY)");
    delay(200);
    digitalWrite(LED_Timer, LOW);
    timerFlag = false;
}

/**
 * @brief Interrupt Service Routine for BUTTON0 (INT0).
 *
 * Toggles the logic analyzer pin for INT0 and calls the generic interrupt handler.
 */
void ISR_INT0(){
    digitalWrite(LOGIC_ANALYZER_PIN0, HIGH);
    handleInterrupt(0, LED_INT0, "INT0 (HIGH priority) triggered");
    digitalWrite(LOGIC_ANALYZER_PIN0, LOW);
}

/**
 * @brief Interrupt Service Routine for BUTTON1 (INT1).
 *
 * Toggles the logic analyzer pin for INT1 and calls the generic interrupt handler.
 */
void ISR_INT1(){ 
    digitalWrite(LOGIC_ANALYZER_PIN1, HIGH);
    handleInterrupt(1, LED_INT1, "INT1 (MEDIUM priority) triggered");
    digitalWrite(LOGIC_ANALYZER_PIN1, LOW);
}

/**
 * @brief Interrupt Service Routine for BUTTON2 (INT2).
 *
 * Toggles the logic analyzer pin for INT2 and calls the generic interrupt handler.
 */
void ISR_INT2(){
    digitalWrite(LOGIC_ANALYZER_PIN2, HIGH);
    handleInterrupt(2, LED_INT2, "INT2 (LOW priority) triggered");
    digitalWrite(LOGIC_ANALYZER_PIN2, LOW);
}

/**
 * @brief Common interrupt handler.
 *
 * Debounces the interrupt by checking the elapsed time since the last interrupt.
 * If the debounce delay has passed, the corresponding interrupt flag is set,
 * the current time is saved, and a message is printed to the Serial Monitor.
 *
 * @param i Interrupt index (0 for INT0, 1 for INT1, 2 for INT2).
 * @param led LED pin associated with this interrupt.
 * @param msg Message to print indicating which interrupt was triggered.
 */
void handleInterrupt(int i, int led, const char* msg){
    if((millis() - lastInterruptTime[i]) < DEBOUNCE_DELAY) return;
    lastInterruptTime[i] = millis();
    intFlag[i] = true;
    Serial.println(msg);
}

/**
 * @brief Processes external interrupt flags by blinking LEDs.
 *
 * Iterates over the interrupt flags array; if a flag is set, calls blinkLed for the
 * corresponding LED and then clears the flag.
 */
void handleBlinking(){
    for(int i = 0; i < 3; i++){
        if(intFlag[i]){
            blinkLed((i == 0) ? LED_INT0 : (i == 1) ? LED_INT1 : LED_INT2);
            intFlag[i] = false;
        }
    }
}

/**
 * @brief Blinks the specified LED for 1 second.
 *
 * Toggles the LED state every BLINK_INTERVAL milliseconds for a duration of 1 second.
 *
 * @param led Pin number of the LED to blink.
 */
void blinkLed(int led){
    unsigned long start = millis();
    while(millis() - start < 1000){
        digitalWrite(led, !digitalRead(led));
        delay(BLINK_INTERVAL);
    }
    digitalWrite(led, LOW);
}

/**
 * @brief Measures distance using an ultrasonic sensor.
 *
 * Sends a trigger pulse on TRIG_PIN, then measures the duration of the echo on ECHO_PIN.
 * Converts the pulse duration to a distance in centimeters.
 *
 * @return Measured distance in centimeters, or -1 if no valid echo is received.
 */
float measureDistance(){
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long t = pulseIn(ECHO_PIN, HIGH, 30000);
    return (t > 0) ? t / 58.0 : -1;
}

/**
 * @brief Triggers a distance alert if an object is detected too close.
 *
 * If the measured distance is within the alarm threshold, this function toggles the sensor LED
 * at 200 ms intervals and prints an alarm message. Otherwise, it ensures the LED is turned off.
 */
void triggerDistanceAlert() {
    static unsigned long prev = 0;
    if (distanceAlert) {
        if (millis() - prev >= 200) {
            prev = millis();
            digitalWrite(LED_Sensor, !digitalRead(LED_Sensor));
        }
    } else {
        digitalWrite(LED_Sensor, LOW);  // Turn off LED when distance is safe
    }
}
