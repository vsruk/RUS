#include <Arduino.h>
#include "driver/gpio.h"
#include "esp_timer.h"

/** @def LED_TIMER
 *  @brief GPIO pin za LED timer.
 */
#define LED_TIMER 13

/** @def LED_BTN0
 *  @brief GPIO pin za LED gumba 0.
 */
#define LED_BTN0 12

/** @def LED_BTN1
 *  @brief GPIO pin za LED gumba 1.
 */
#define LED_BTN1 14

/** @def LED_BTN2
 *  @brief GPIO pin za LED gumba 2.
 */
#define LED_BTN2 27

/** @def PIR_PIN
 *  @brief GPIO pin za PIR senzor.
 */
#define PIR_PIN GPIO_NUM_33

/** @def BUTTON0
 *  @brief GPIO pin za gumb 0.
 */
#define BUTTON0 GPIO_NUM_2

/** @def BUTTON1
 *  @brief GPIO pin za gumb 1.
 */
#define BUTTON1 GPIO_NUM_4

/** @def BUTTON2
 *  @brief GPIO pin za gumb 2.
 */
#define BUTTON2 GPIO_NUM_5

/** @var volatile bool btn0Pressed
 *  @brief Stanje prekida za gumb 0.
 */
volatile bool btn0Pressed = false;

/** @var volatile bool btn1Pressed
 *  @brief Stanje prekida za gumb 1.
 */
volatile bool btn1Pressed = false;

/** @var volatile bool btn2Pressed
 *  @brief Stanje prekida za gumb 2.
 */
volatile bool btn2Pressed = false;

/** @var volatile bool pirDetected
 *  @brief Stanje prekida za PIR senzor.
 */
volatile bool pirDetected = false;

/** @var volatile bool timerFlag
 *  @brief Zastavica za timer prekid.
 */
volatile bool timerFlag = false;

/** @def PRIORITY_TIMER
 *  @brief Prioritet prekida za timer.
 */
#define PRIORITY_TIMER 0

/** @def PRIORITY_BTN0
 *  @brief Prioritet prekida za gumb 0.
 */
#define PRIORITY_BTN0 3

/** @def PRIORITY_BTN1
 *  @brief Prioritet prekida za gumb 1.
 */
#define PRIORITY_BTN1 4

/** @def PRIORITY_BTN2
 *  @brief Prioritet prekida za gumb 2.
 */
#define PRIORITY_BTN2 1

/** @def PRIORITY_PIR
 *  @brief Prioritet prekida za PIR senzor.
 */
#define PRIORITY_PIR 2

/** @var esp_timer_handle_t timerHandle
 *  @brief Rukovalac za ESP tajmer.
 */
esp_timer_handle_t timerHandle;

/** @fn void IRAM_ATTR handleBtn0(void* arg)
 *  @brief Prekidna rutina za gumb 0.
 *  @param arg Argument prekida (ne koristi se).
 */
void IRAM_ATTR handleBtn0(void* arg) { btn0Pressed = true; }

/** @fn void IRAM_ATTR handleBtn1(void* arg)
 *  @brief Prekidna rutina za gumb 1.
 *  @param arg Argument prekida (ne koristi se).
 */
void IRAM_ATTR handleBtn1(void* arg) { btn1Pressed = true; }

/** @fn void IRAM_ATTR handleBtn2(void* arg)
 *  @brief Prekidna rutina za gumb 2.
 *  @param arg Argument prekida (ne koristi se).
 */
void IRAM_ATTR handleBtn2(void* arg) { btn2Pressed = true; }

/** @fn void IRAM_ATTR handlePIR(void* arg)
 *  @brief Prekidna rutina za PIR senzor.
 *  @param arg Argument prekida (ne koristi se).
 */
void IRAM_ATTR handlePIR(void* arg) { pirDetected = true; }

/** @fn void IRAM_ATTR timerISR(void* arg)
 *  @brief Prekidna rutina za tajmer.
 *  @param arg Argument prekida (ne koristi se).
 */
void IRAM_ATTR timerISR(void* arg) {
    timerFlag = true;
}

/** @fn void setup()
 *  @brief Inicijalizacija hardvera i postavke prekida.
 *  
 *  Ova funkcija konfigurira GPIO pinove, postavlja prekide
 *  i inicijalizira periodični tajmer.
 */
void setup() {
    Serial.begin(115200);

    // Konfiguracija izlaza
    esp_rom_gpio_pad_select_gpio((gpio_num_t)LED_TIMER);
    esp_rom_gpio_pad_select_gpio((gpio_num_t)LED_BTN0);
    esp_rom_gpio_pad_select_gpio((gpio_num_t)LED_BTN1);
    esp_rom_gpio_pad_select_gpio((gpio_num_t)LED_BTN2);
    
    gpio_set_direction((gpio_num_t)LED_TIMER, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)LED_BTN0, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)LED_BTN1, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)LED_BTN2, GPIO_MODE_OUTPUT);

    // Konfiguracija ulaza
    gpio_set_direction(BUTTON0, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON1, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON2, GPIO_MODE_INPUT);
    gpio_set_direction(PIR_PIN, GPIO_MODE_INPUT);

    // Konfiguracija prekida
    gpio_set_intr_type(BUTTON0, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(BUTTON1, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(BUTTON2, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(PIR_PIN, GPIO_INTR_POSEDGE);

    // Inicijalizacija ISR servisa
    gpio_install_isr_service(0);

    // Dodavanje ISR funkcija
    gpio_isr_handler_add(BUTTON0, handleBtn0, (void*)PRIORITY_BTN0);
    gpio_isr_handler_add(BUTTON1, handleBtn1, (void*)PRIORITY_BTN1);
    gpio_isr_handler_add(BUTTON2, handleBtn2, (void*)PRIORITY_BTN2);
    gpio_isr_handler_add(PIR_PIN, handlePIR, (void*)PRIORITY_PIR);

    // Konfiguracija tajmera (1 sekunda)
    esp_timer_create_args_t timerArgs = {};
    timerArgs.callback = &timerISR;
    timerArgs.name = "TIMER1";
    esp_timer_create(&timerArgs, &timerHandle);
    esp_timer_start_periodic(timerHandle, 1000000); // 1 sekunda

    Serial.println("Sustav inicijaliziran.");
}

/** @fn void loop()
 *  @brief Glavna petlja programa koja obrađuje prekide.
 *  
 *  Ova funkcija kontinuirano provjerava zastavice prekida
 *  i izvršava odgovarajuće akcije prema prioritetima.
 */
void loop() {
    // Obrada prekida prema prioritetima
    if (timerFlag) {
        timerFlag = false;
        gpio_set_level((gpio_num_t)LED_TIMER, !gpio_get_level((gpio_num_t)LED_TIMER));
        Serial.println("[TIMER] Aktiviran");
    }

    if (btn0Pressed) {
        btn0Pressed = false;
        gpio_set_level((gpio_num_t)LED_BTN0, 1);
        delay(1000);
        gpio_set_level((gpio_num_t)LED_BTN0, 0);
        Serial.println("[BUTTON0] Pritisnut");
    }

    if (btn1Pressed) {
        btn1Pressed = false;
        gpio_set_level((gpio_num_t)LED_BTN1, 1);
        delay(1000);
        gpio_set_level((gpio_num_t)LED_BTN1, 0);
        Serial.println("[BUTTON1] Pritisnut");
    }

    if (btn2Pressed) {
        btn2Pressed = false;
        gpio_set_level((gpio_num_t)LED_BTN2, 1);
        delay(1000);
        gpio_set_level((gpio_num_t)LED_BTN2, 0);
        Serial.println("[BUTTON2] Pritisnut");
    }

    if (pirDetected) {
        pirDetected = false;
        Serial.println("[PIR] Pokret detektiran!");
    }

    delay(10); // Mala pauza za stabilnost
}
