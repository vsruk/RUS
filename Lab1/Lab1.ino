/**
 * @file patient_info_display.ino
 * @brief ESP32 application for displaying patient info via keypad and LCD using Firebase and offline mode.
 *
 * This sketch connects to WiFi and Firebase to retrieve patient appointment information using a card number entered via keypad.
 * If the network is unavailable, it uses a local database. The LCD displays info and uses LEDs and buzzer to indicate success or failure.
 */

 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
 #include <WiFi.h>
 #include <FirebaseESP32.h>
 #include <ArduinoJson.h>
 
 // Firebase configuration
 #define FIREBASE_HOST "rus---projekt-default-rtdb.europe-west1.firebasedatabase.app"
 #define FIREBASE_AUTH "Fg4FnkVhu41qNTYb4XjFI9VVeBmYBg9oYOblnfZJ"
 
 // WiFi credentials
 #define WIFI_SSID ""
 #define WIFI_PASSWORD ""
 
 const int buzzerPin = 25;
 #define BUZZER_CHANNEL 0
 #define BUZZER_RESOLUTION 8
 // Simulation mode for Wokwi
 bool simulatedWiFiMode = true;
 bool hasInternet = false;
 
 // LCD configuration
 LiquidCrystal_I2C lcd(0x27, 16, 2);
 
 // Keypad configuration
 const byte ROWS = 4;
 const byte COLS = 4;
 char keys[ROWS][COLS] = {
   {'1','2','3','A'},
   {'4','5','6','B'},
   {'7','8','9','C'},
   {'*','0','#','D'}
 };
 byte rowPins[ROWS] = {12, 13, 14, 15};
 byte colPins[COLS] = {5, 18, 19, 23};
 
 // LED indicators
 const int ledSuccessPin = 2;
 const int ledErrorPin = 4;
 
 // Firebase objects
 FirebaseData firebaseData;
 FirebaseConfig firebaseConfig;
 FirebaseAuth firebaseAuth;
 
 /**
  * @struct PatientInfo
  * @brief Structure to hold patient information
  */
 struct PatientInfo {
   String cardNumber;
   String fullName;
   String roomNumber;
   String appointmentTime;
   String doctor;
   bool isValid;
 };
 
 /**
  * @struct LocalPatient
  * @brief Structure for offline patient data
  */
 struct LocalPatient {
   const char* cardNumber;
   const char* fullName;
   const char* roomNumber;
   const char* appointmentTime;
   const char* doctor;
 };
 
 // Local fallback database
 LocalPatient localPatients[] = {
   {"12345", "Ivan Horvat", "101", "09:30", "Dr. Perić"},
   {"67891", "Ana Kovač", "102", "10:15", "Dr. Marić"},
   {"54321", "Marko Novak", "103", "11:00", "Dr. Jurić"},
   {"11111", "Petra Kralj", "104", "13:45", "Dr. Kovačić"},
   {"22222", "Josip Tomić", "105", "14:30", "Dr. Babić"}
 };
 
 // System state machine
 enum SystemState {
   WAITING_FOR_INPUT,
   PROCESSING_CARD,
   DISPLAY_PATIENT_INFO,
   DISPLAY_ERROR
 };
 
 SystemState currentState = WAITING_FOR_INPUT;
 String inputBuffer = "";
 unsigned long lastActionTime = 0;
 const unsigned long TIMEOUT = 30000;
 
 // Function declarations
 void connectToWiFi();
 void resetToInputMode();
 void handleCardInput();
 void processCardNumber();
 void beepSuccess();
 void beepError();
 char getKey();
 PatientInfo getPatientData(String cardNumber);
 void displayPatientInfo(PatientInfo patient);
 String truncateString(String str, int maxLength);
 
 /**
  * @brief Setup function initializes LCD, WiFi, Firebase and sets the system to input mode.
  */
 void setup() {
   Serial.begin(115200);
 
   lcd.init();
   lcd.backlight();
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Inicijalizacija");
   lcd.setCursor(0, 1);
   lcd.print("sustava...");
   ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RESOLUTION); 
  ledcAttachPin(buzzerPin, BUZZER_CHANNEL);
   for (byte r = 0; r < ROWS; r++) {
     pinMode(rowPins[r], INPUT_PULLUP);
   }
   pinMode(buzzerPin, OUTPUT);
   pinMode(buzzerPin, OUTPUT);
   ledcAttachPin(buzzerPin, BUZZER_CHANNEL);
   ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RESOLUTION);

   pinMode(ledErrorPin, OUTPUT);
 
   connectToWiFi();
 
   if (hasInternet) {
     firebaseConfig.host = FIREBASE_HOST;
     firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
     Firebase.begin(&firebaseConfig, &firebaseAuth);
     Firebase.reconnectWiFi(true);
     Firebase.setReadTimeout(firebaseData, 1000 * 60);
     Firebase.setwriteSizeLimit(firebaseData, "tiny");
     Serial.println("Firebase initialized");
   }
 
   resetToInputMode();
 }
 
 /**
  * @brief Main loop handles state machine and key input.
  */
 void loop() {
   if (millis() - lastActionTime > TIMEOUT) {
     resetToInputMode();
   }
 
   static unsigned long lastWiFiCheck = 0;
   if (millis() - lastWiFiCheck > 30000) {
     connectToWiFi();
     lastWiFiCheck = millis();
   }
 
   switch (currentState) {
     case WAITING_FOR_INPUT:
       handleCardInput();
       break;
 
     case PROCESSING_CARD:
       processCardNumber();
       break;
 
     case DISPLAY_PATIENT_INFO:
     case DISPLAY_ERROR:
       char key = getKey();
       if (key == 'D') {
         resetToInputMode();
       }
       break;
   }
   delay(100);
 }
 
 /**
  * @brief Plays success beep pattern (higher pitch, single beep)
  */
 void beepSuccess() {
  Serial.println("Playing beep...");
  ledcWriteTone(BUZZER_CHANNEL, 2000); // 2kHz
  delay(200);
  ledcWrite(BUZZER_CHANNEL, 0); // Off
}
 
 /**
  * @brief Plays error beep pattern (lower pitch, double beep)
  */
 void beepError() {
  for(int i=0; i<2; i++) {
    ledcWriteTone(BUZZER_CHANNEL, 1200); // 1.2kHz
    delay(100);
    ledcWrite(BUZZER_CHANNEL, 0);
    if(i==0) delay(50); // Gap between beeps
  }
}
 
 /**
  * @brief Connects to WiFi (real or simulated).
  */
 void connectToWiFi() {
   if (simulatedWiFiMode) {
     WiFi.begin("Wokwi-GUEST", "", 6);
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Simulirani WiFi");
 
     int attempt = 0;
     while (WiFi.status() != WL_CONNECTED && attempt < 10) {
       delay(500);
       lcd.print(".");
       attempt++;
     }
 
     if (WiFi.status() == WL_CONNECTED) {
       hasInternet = true;
       lcd.setCursor(0, 1);
       lcd.print("IP: 192.168.1.100");
       Serial.println("Simulated WiFi connected");
     } else {
       hasInternet = false;
       lcd.setCursor(0, 1);
       lcd.print("Offline mod");
       Serial.println("Simulated WiFi not available");
     }
     delay(2000);
     return;
   }
 
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Spajanje na WiFi");
   lcd.setCursor(0, 1);
   lcd.print(WIFI_SSID);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
   int attemptCount = 0;
   while (WiFi.status() != WL_CONNECTED && attemptCount < 10) {
     delay(500);
     Serial.print(".");
     attemptCount++;
   }
 
   if (WiFi.status() == WL_CONNECTED) {
     hasInternet = true;
     Serial.println("WiFi connected");
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("WiFi spojen");
     lcd.setCursor(0, 1);
     lcd.print(WiFi.localIP());
   } else {
     hasInternet = false;
     Serial.println("WiFi connection failed");
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("WiFi greška!");
     lcd.setCursor(0, 1);
     lcd.print("Offline način");
   }
   delay(2000);
 }
 
 /**
  * @brief Resets system to input state and clears indicators.
  */
 void resetToInputMode() {
   inputBuffer = "";
   currentState = WAITING_FOR_INPUT;
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Unesite broj:");
   lcd.setCursor(0, 1);
   lastActionTime = millis();
   digitalWrite(ledSuccessPin, LOW);
   digitalWrite(ledErrorPin, LOW);
 }
 
 /**
  * @brief Handles numeric input from keypad.
  */
 void handleCardInput() {
   char key = getKey();
   if (key) {
     lastActionTime = millis();
     if (key == '#') {
       if (inputBuffer.length() > 0) {
         currentState = PROCESSING_CARD;
       }
     } else if (key == '*') {
       if (inputBuffer.length() > 0) {
         inputBuffer = inputBuffer.substring(0, inputBuffer.length() - 1);
         lcd.setCursor(0, 1);
         lcd.print("                ");
         lcd.setCursor(0, 1);
         lcd.print(inputBuffer);
       }
     } else if (key >= '0' && key <= '9' && inputBuffer.length() < 16) {
       inputBuffer += key;
       lcd.setCursor(0, 1);
       lcd.print(inputBuffer);
     }
   }
 }
 
 /**
  * @brief Processes card number and fetches patient info.
  */
 void processCardNumber() {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Provjera...");
 
   PatientInfo patient = getPatientData(inputBuffer);
 
   if (patient.isValid) {
     displayPatientInfo(patient);
     currentState = DISPLAY_PATIENT_INFO;
     beepSuccess();
   

     digitalWrite(ledSuccessPin, HIGH);
     digitalWrite(ledErrorPin, LOW);
   } else {
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Greška:");
     lcd.setCursor(0, 1);
     lcd.print("Nema u bazi");
     currentState = DISPLAY_ERROR;
     beepError();
     digitalWrite(ledSuccessPin, LOW);
     digitalWrite(ledErrorPin, HIGH);
   }
   lastActionTime = millis();
 }
 
 /**
  * @brief Gets patient data from Firebase or local database.
  * @param cardNumber The card number to look up.
  * @return PatientInfo structure with patient details.
  */
 PatientInfo getPatientData(String cardNumber) {
   PatientInfo patient;
   patient.isValid = false;
 
   if (hasInternet) {
    String path = "/pacijenti/";
    path.concat(cardNumber);    
     if (Firebase.getJSON(firebaseData, path)) {
       String json = firebaseData.jsonString();
       if (json.isEmpty()) return patient;
 
       DynamicJsonDocument doc(1024);
       if (deserializeJson(doc, json) == DeserializationError::Ok) {
         patient.cardNumber = cardNumber;
         patient.fullName = doc["fullName"].as<String>();
         patient.roomNumber = doc["roomNumber"].as<String>();
         patient.appointmentTime = doc["appointmentTime"].as<String>();
         patient.doctor = doc["doctor"].as<String>();
         patient.isValid = true;
         return patient;
       }
     }
   }
 
   for (LocalPatient lp : localPatients) {
     if (cardNumber == lp.cardNumber) {
       patient.cardNumber = cardNumber;
       patient.fullName = lp.fullName;
       patient.roomNumber = lp.roomNumber;
       patient.appointmentTime = lp.appointmentTime;
       patient.doctor = lp.doctor;
       patient.isValid = true;
       break;
     }
   }
   return patient;
 }
 
 /**
  * @brief Displays patient info on the LCD screen.
  * @param patient The patient info to display.
  */
 void displayPatientInfo(PatientInfo patient) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Ime: ");
   lcd.print(truncateString(patient.fullName, 11));
   lcd.setCursor(0, 1);
   lcd.print("Soba: ");
   lcd.print(patient.roomNumber);
   delay(3000);
 
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Termin: ");
   lcd.print(truncateString(patient.appointmentTime, 8));
   lcd.setCursor(0, 1);
   lcd.print("Dr: ");
   lcd.print(truncateString(patient.doctor, 12));
   delay(3000);
 
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Pritisnite 'D'");
   lcd.setCursor(0, 1);
   lcd.print("za povratak");
 }
 
 /**
  * @brief Truncates string to fit LCD line.
  * @param str String to truncate.
  * @param maxLength Max characters.
  * @return Truncated string.
  */
 String truncateString(String str, int maxLength) {
   return (str.length() <= maxLength) ? str : str.substring(0, maxLength);
 }
 
 /**
  * @brief Gets pressed key from keypad.
  * @return Pressed key character or 0 if none.
  */
 char getKey() {
   for (byte c = 0; c < COLS; c++) {
     pinMode(colPins[c], OUTPUT);
     digitalWrite(colPins[c], LOW);
     for (byte r = 0; r < ROWS; r++) {
       if (digitalRead(rowPins[r]) == LOW) {
         delay(50);
         while (digitalRead(rowPins[r]) == LOW);
         pinMode(colPins[c], INPUT_PULLUP);
         return keys[r][c];
       }
     }
     pinMode(colPins[c], INPUT_PULLUP);
   }
   return 0;
 }
