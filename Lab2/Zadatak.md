Upravljanje potrošnjom energije mikrokontrolera korištenjem Sleep moda

Proučiti mogućnosti smanjenja potrošnje energije na odabranom procesoru korištenjem sleep modova. 
Razviti program koji efikasno upravlja potrošnjom mikrokontrolera, omogućujući mu da bude u niskopotrošnom režimu dok nije aktivan.
Zadatak
Implementacija Sleep moda
Konfigurirajte mikrokontroler da koristi sleep mode između aktivnih perioda kako bi se smanjila potrošnja energije.
Npr. Koristite odgovarajuću biblioteku (npr. avr/sleep.h za AVR mikrokontrolere ili LowPower.h za Arduino).
Program treba periodično izvršavati osnovnu funkciju, primjerice:
Treptanje LED diode (npr. svijetli 5 sekundi, zatim prelazi u sleep mode).
Uvjeti za izlazak iz Sleep moda
Definirajte i implementirajte mehanizme za buđenje mikrokontrolera:
Pritisak tipkala (korištenjem eksternog prekida – external interrupt).
Timer interrupt (korištenjem watchdog timera ili unutarnjih timera za automatsko buđenje nakon određenog vremena).
Efikasno upravljanje ulaskom i izlaskom iz Sleep moda
Mikrokontroler mora odmah preći u sleep mode nakon što završi zadatak, bez nepotrebnog trošenja energije.
Pravilno konfigurirati podešavanje registara i modula prije ulaska u sleep mode kako bi se spriječili nepotrebni prekidi ili gubitak podataka.
Istraživanje različitih razina Sleep moda
Ispitati različite modove smanjenja potrošnje (npr. Idle, Power-down, Standby) i njihova ograničenja.
Usporediti potrošnju energije između različitih modova i dokumentirati nalaze.
Dokumentacija i testiranje
Detaljno opisati kako program upravlja energijom i kojim metodama se izlazi iz Sleep moda.
Provesti testiranja i analizirati potrošnju energije u različitim modovima.

