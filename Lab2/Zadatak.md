# Upravljanje potrošnjom energije mikrokontrolera korištenjem Sleep moda

## Cilj zadatka

Proučiti mogućnosti smanjenja potrošnje energije na odabranom mikrokontroleru korištenjem različitih sleep modova. Razviti program koji efikasno upravlja energetskim režimima, omogućujući mikrokontroleru da pređe u niskopotrošni način rada dok nije aktivan.

## Implementacija Sleep moda

### 1. Konfiguracija Sleep moda

Odabrani mikrokontroler treba biti konfiguriran da koristi odgovarajući sleep mode između aktivnih perioda rada.

Preporučene biblioteke:
- `avr/sleep.h` za AVR mikrokontrolere
- `LowPower.h` za Arduino
- ESP-IDF ili Arduino-ESP32 biblioteke za ESP32/ESP8266 (npr. `esp_sleep.h`)

### 2. Osnovna funkcionalnost

Implementirati osnovni zadatak koji uključuje:
- Izvršavanje funkcije (npr. LED treptanje: LED svijetli 5 sekundi)
- Ulazak u sleep mode
- Povremeno buđenje i ponavljanje zadatka

## Uvjeti za buđenje iz Sleep moda

Podržati različite mehanizme buđenja:
- Eksterni prekid (npr. pritisak tipkala)
- Timer interrupt (automatsko buđenje nakon određenog vremena)

ESP mikrokontroleri posebno podržavaju:
- Timer wakeup: pomoću `esp_sleep_enable_timer_wakeup()`
- Touch wakeup: buđenje dodirom (ESP32)
- GPIO wakeup: putem odabranih pinova
- ULP coprocessor: za napredne funkcije bez buđenja glavnog CPU-a

## Efikasno upravljanje energijom

- Mikrokontroler treba odmah ući u sleep mode po završetku zadatka.
- Prije ulaska:
  - Onemogućiti nepotrebne periferne uređaje
  - Sačuvati stanje ako je potrebno (posebno kod ESP32 – može koristiti RTC memoriju)
- Nakon buđenja, obnoviti potrebne module i nastaviti rad

## Istraživanje različitih razina Sleep moda

### Za AVR/Arduino:
- Idle: CPU pauziran, ali periferija radi
- Power-down: većina modula isključena
- Standby: brže buđenje, ali niža ušteda

### Za ESP32:
- Light Sleep: CPU pauziran, RAM i periferija ostaju aktivni
- Deep Sleep: gotovo svi dijelovi se isključuju, ali RTC i ULP ostaju aktivni
- Hibernation: najniža potrošnja, samo RTC memorija može se sačuvati

Usporediti modove po:
- Potrošnji energije (ako je moguće mjeriti)
- Vremenu buđenja
- Fleksibilnosti konfiguracije

## Dokumentacija i ispitivanje

U izvještaju navesti:
- Korištene sleep modove i uvjete za buđenje
- Razliku u potrošnji između modova (ako moguće: realna mjerenja)
- Posebnosti implementacije na ESP platformi ako se koristi
- Savjete za optimizaciju energetske učinkovitosti

## Napomena za ESP korisnike

Ako koristite ESP8266 ili ESP32:
- Prednost je što nude integrirane funkcije za sleep i detaljne kontrole buđenja
- Dokumentacija: [ESP32 Sleep Modes – Espressif Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html)
- Primjeri: Arduino ESP32 `esp_sleep_enable_timer_wakeup()`, `esp_deep_sleep_start()` itd.

## Završna napomena

Kôd neka bude čitljiv i modularan. Svaka faza (aktivna funkcija, ulazak u sleep, buđenje) treba biti jasno razdvojena. Obavezno komentirati ključne dijelove.
### WokWI simulator
- pruža osnovne funkcionalnosti za testiranje sleep modova
- 
> Ograničena simulacija stvarnog ponašanja potrošnjom energije
> 
> Nema podrške za napredne sleep modove (npr. Deep Sleep)
> 
> 
 Nije  precizan za ozbiljno ipitivanje napajanja, optimizaciju sleep modova i stvarnu procjenu potrošnje energije. Za takve potrebe, preporučuje se rad u stvarnom okruženju s odgovarajućim mjeračima energije ili upotreba alata specifičnih za ispitivanje  potrošnje energije u stvarnim uvjetima.
 
> Napredniji alati:
> 
> ARM Energy Profiler https://developer.arm.com/documentation/102732/1910/Energy-profiling
> 
> ARM nudi vlastite alate za analizu potrošnje energije, uključujući ARM Energy Profiler. Ovaj alat pomaže u vizualizaciji potrošnje energije tijekom razvoja aplikacija. Pruža uvid u potrošnju energije temeljem vremenskih intervala u aplikaciji, što omogućava optimizaciju na temelju stvarnih podataka.
> 
> Ključne značajke:
> 
> Pruža grafove za praćenje potrošnje energije po različitim fazama.
> 
> Integracija s IDE-ovima poput Keil uVision i ARM Development Studio.
> 
> Mogućnost simulacije rada mikrokontrolera i predviđanja potrošnje energije za različite operacije.
> 
