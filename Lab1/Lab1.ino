/**
 * @file smart_safe_system.ino
 *
 * @mainpage Pametni Sef s PIN Zaštitom, Resetiranjem PIN-a i Temperaturnim Ograničenjem
 *
 * @section description Opis
 * Ovaj projekt implementira sustav pametnog sefa koristeći LCD zaslon, matricu tipki (keypad),
 * temperaturni senzor i servo motor. Korisnik mora unijeti ispravan PIN kako bi otvorio sef.
 * Sustav uključuje sigurnosnu provjeru temperature, kao i mogućnost resetiranja PIN-a pomoću posebne kombinacije (*201*).
 *
 * @section hardware Hardverska postava
 * - LCD zaslon povezan na pinove 12, 11, 10, A5, A4, A3.
 * - Matrica tipki (4x4) povezana na digitalne pinove 2-9.
 * - Servo motor povezan na pin 13.
 * - NTC temperaturni senzor spojen na analogni pin A0.
 *
 * @section libraries Korištene biblioteke
 * - Keypad: za upravljanje matricom tipki.
 * - LiquidCrystal: za upravljanje LCD zaslonom.
 * - Servo: za upravljanje servo motorom.
 *
 * @section notes Napomene
 * - Zadnja znamenka PIN unosa je vidljiva, dok su ostale maskirane.
 * - Ako je temperatura izvan granica (manja od 0°C ili veća od 38°C), sef se neće otvoriti.
 * - Resetiranje PIN-a vrši se unosom kombinacije trenutnog pina i *201*, nakon čega se unosi novi PIN.
 *
 * @section author Autor
 * - Autor: Renato Rak, Ivan Prekratić
 */

// Biblioteke
#include <Keypad.h>         ///< Biblioteka za upravljanje matricom tipki.
#include <LiquidCrystal.h>  ///< Biblioteka za upravljanje LCD zaslonom.
#include <Servo.h>          ///< Biblioteka za upravljanje servo motorom.

// Inicijalizacija LCD zaslona (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 10, A5, A4, A3);

// Konfiguracija tipkovnice (keypad)
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
uint8_t colPins[COLS] = {5, 4, 3, 2};
uint8_t rowPins[ROWS] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servo;

// Varijable za upravljanje PIN-om i stanjem sustava
String correctPin = "1234";                 ///< Ispravni PIN za otključavanje sefa.
String enteredPin = "";                     ///< Trenutno uneseni PIN.
volatile bool hasEntered = false;           ///< Status pristupa (otključano/zatvoreno).
String resetPasswordPattern = "*201*";      ///< Kod za resetiranje PIN-a.
volatile bool isResetPasswordMode = false;  ///< Status načina za postavljanje novog PIN-a.
String inputPassword = "";                  ///< Novi uneseni PIN.

// Pinovi senzora i serva
const int ntcPin = A0;                      ///< Analogni pin za NTC temperaturni senzor.
const int servoPin = 13;                    ///< Digitalni pin za servo motor.

/**
 * @brief Inicijalna funkcija postavljanja.
 * Inicijalizira servo motor, LCD zaslon i prikazuje početnu poruku.
 */
void setup()
{
  servo.attach(servoPin);
  servo.write(-90); ///< Zaključaj sef.
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Dobrodosli!");
  lcd.setCursor(0, 1);
  lcd.print("Unesite PIN:");
}

/**
 * @brief Glavna petlja programa.
 * Čita unos s tipkovnice i prikazuje maskirani PIN unos.
 */
void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY && !hasEntered)
  {
    if (key == '#')
    {
      checkPin();
    }
    else if (key == 'C')
    {
      enteredPin = "";
      lcd.setCursor(0, 2);
      lcd.print("                ");
    }
    else
    {
      enteredPin += key;
    }

    // Prikaz maskiranog unosa (zadnja znamenka ostaje vidljiva)
    lcd.setCursor(0, 2);
    lcd.print("Unos: ");
    int start = 6;
    for (int i = 0; i < enteredPin.length(); i++) {
      if (i == enteredPin.length() - 1) {
        lcd.print(enteredPin[i]);
      } else {
        lcd.print("*");
      }
    }
    int totalLength = start + enteredPin.length();
    while (totalLength < 20) {
      lcd.print(" ");
      totalLength++;
    }
  }
}

/**
 * @brief Provjerava uneseni PIN.
 * Ako je PIN ispravan i temperatura unutar granica, sef se otključava. 
 * Ako je unesena šifra za reset, prelazi se u način postavljanja novog PIN-a.
 */
void checkPin()
{
  lcd.clear();

  if (enteredPin == correctPin)
  {
    float temp = readTemperature();

    if (temp > 38.0 || temp < 0.0)
    {
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temp, 1);
      lcd.setCursor(0, 1);
      lcd.print("Ne moze se otvoriti");
      lcd.setCursor(0, 2);
      lcd.print("Temp izvan granica");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unesite PIN:");
      enteredPin = "";
      return;
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Tocan PIN!");
      lcd.setCursor(0, 1);
      lcd.print("Sef otvoren!");
      hasEntered = true;
      unlock();
      delay(5000); // sef ostaje otvoren 5 sekundi

      // Zaključavanje
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sef zakljucan.");
      lock();
      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unesite PIN:");
      hasEntered = false;
      enteredPin = "";
    }
  }
  else if (enteredPin == (correctPin + resetPasswordPattern))
  {
    isResetPasswordMode = true;
    newPin();
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Pogresan PIN!");
    lcd.setCursor(0, 1);
    lcd.print("Pokusajte opet");
    enteredPin = "";
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unesite PIN:");
  }
}

/**
 * @brief Omogućuje korisniku postavljanje novog PIN-a.
 * Unos mora biti različit od reset koda.
 */
void newPin()
{
  inputPassword = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Postavi novi PIN");

  while (isResetPasswordMode)
  {
    char key = keypad.getKey();

    if (key)
    {
      if (key == 'C')
      {
        inputPassword = "";
        lcd.setCursor(0, 2);
        lcd.print("                ");
      }
      else if (key == '#')
      {
        if (inputPassword == resetPasswordPattern)
        {
          lcd.setCursor(0, 3);
          lcd.print("Neispravan PIN!");
          inputPassword = "";
          delay(2000);
          lcd.setCursor(0, 3);
          lcd.print("                ");
        }
        else
        {
          correctPin = inputPassword;
          enteredPin = "";
          isResetPasswordMode = false;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PIN postavljen!");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Unesite PIN:");
        }
      }
      else
      {
        inputPassword += key;
      }

      lcd.setCursor(0, 2);
      lcd.print("Unos: ");
      int start = 6;
      lcd.setCursor(start, 2);

      int len = inputPassword.length();

      if (len < 4) {
        for (int i = 0; i < len - 1; i++) {
          lcd.print("*");
        }
        lcd.print(inputPassword[len - 1]);
        for (int i = len; i < 4; i++) {
          lcd.print(" ");
        }
      }
      else {
        lcd.print("***");
        lcd.print(inputPassword[len - 1]);
      }
      int totalLength = start + inputPassword.length();
      while (totalLength < 20) {
        lcd.print(" ");
        totalLength++;
      }
    }
  }
}

/**
 * @brief Čita temperaturu s NTC senzora i pretvara je u stupnjeve Celzijusa.
 * @return Temperatura u °C.
 */
float readTemperature()
{
  const float BETA = 3950;
  int analogValue = analogRead(ntcPin);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return celsius;
}

/**
 * @brief Otključava sef (pomakne servo motor).
 */
void unlock()
{
  servo.write(180);
}

/**
 * @brief Zaključava sef (pomakne servo motor).
 */
void lock()
{
  servo.write(-90);
}
