# Programsko inženjerstvo

> Ime projekta u naslovu ima cilj opisati namjenu projekta te pomoći u podizanju početnog interesa za projekt prezentirajući osnovnu svrhu projekta.
> Isključivo ovisi o Vama!
> 
> Naravno, nijedan predložak nije idealan za sve projekte jer su potrebe i ciljevi različiti. Ne bojte se naglasiti Vaš cilj u ovoj početnoj stranici projekta, podržat ćemo ga bez obzira usredotočili se Vi više na tenologiju ili marketing.
> 
> Zašto ovaj dokument? Samo manji dio timova je do sada propoznao potrebu (a i meni je lakše pratiti Vaš rad).  

# Opis projekta
Ovaj projekt je reultat timskog rada u sklopu projeknog zadatka kolegija [Programsko inženjerstvo](https://www.fer.unizg.hr/predmet/proinz) na Fakultetu elektrotehnike i računarstva Sveučilišta u Zagrebu. 

Kratko opisati cilj Vašeg projekta. Vaša motivacija?  (Napomena: odgovor nije »Zato što je to bio zadatak i nismo imali ideje za drugo.«). Koji problem rješavate?
> Obzirom da je ovo zadani projekt navedite i što želite/jeste novo  naučili.

Cilj eZgrade jest kreiraiti jedinstvenu web-aplikaciju koja olakšava svakodnevne dogovore i organiziciju suživota u stambenim objektima. Prestavnici stanara i suvlasnici bit će u mogućnosti brzo i jednostavno kreirati i najaviti sastanke stanara koji će olakšati svakodnevni život. Problemi od jednostavnih točaka dnevnog reda pa sve do riješavanja većih nedoumica od sada je jednostavno uz sve pogodnosti koje nudi naša aplikacija. O sastancima pojedine zgrade upravljati će administrator, a sastanci će biti vidljivi svim sadašnjim i budućim suvlasnicima i administratorima stambenog objekta.

> Dobro izrađen opis omogućuje vam da pokažete svoj rad drugim programerima, kao i potencijalnim poslodavcima. Ne samo da prvi dojam na stranici opisa često razlikuje dobar projekt od lošeg projekta već i predstavlja dobru praksu koju morate savladati.

# Funkcijski zahtjevi
> Aplikacija mora omogućiti predstavniku kreiranje novog sastanka.<br />
> Prilikom kreiranja sastanka, aplikacija mora omogućiti dodavanje naslova, sažetak namjere sastanka, vrijeme, mjesto i niz točaka dnevnog reda. <br />
> Aplikacija mora omogućiti predstavniku prevođenje kreiranog sastanka u stanje "Objavljen", osim ako sastanak nema definiranu nijednu točku dnevnog reda. <br />
> Aplikacija predstavniku mora omogućiti dodavanje novih točaka dnevnog reda za sastanke u stanju "Objavljen". <br />
> Za sastanak u stanju "Objavljen" aplikacija mora poslati obavijest na e-mail suvlasnicima i prikazati ga na oglasnoj ploči aplikacije. <br />
> Za sastanak u stanju "Objavljen" aplikacija mora omogućiti označavanje sudjelovanja suvlasnicima. <br />
> Aplikacija mora prikazivati broj potvrđenih sudjelovanja na početnom prikazu sastanka.<br />
> Aplikacija mora omogućiti predstavniku prevođenje sastanka iz stanja "Objavljen" u stanje "Obavljen" nakon isteka termina sastanka.<br />
> Aplikacija mora omogućiti predstavniku dodavanje zaključka svakoj točki dnevnog reda za sastanke u stanju "Objavljen".<br />
> Aplikacija mora omogućiti predstavniku svrstavanje pojedinog zaključka u onaj s pravnim učinkom ili onaj bez pravnog učinka.<br />
> Aplikacija mora omogućiti predstavniku svrstavanje zaključka s pravnim učinkom u "Izglasan" ili "Odbijen".<br />
> Aplikacija mora omogućiti predstavniku prevođenje sastanka iz stanja "Objavljen" u stanje "Arhiviran", osim ako nisu dodani zaključci na točke dnevnog reda koje imaju pravni učinak.<br />
> Za sastanak u stanju "Arhiviran" aplikacija mora poslati obavijest na e-mail suvlasnicima.<br />
> Aplikacija mora omogućiti suvlasnicima pregledavanje zaključaka arhiviranih sastanaka.<br />
> Aplikacija se mora moći spojiti kao klijent na aplikacijsko sučelje aplikacije StanBlog, preuzeti listu diskusija i njihove poveznice.<br />
> Aplikacija mora moći postaviti poveznicu na diskusiju u aplikaciji StanBlog za neku točku dnevnog reda.<br />
> Aplikacija mora omogućiti administratoru kreiranje profila predstavnika i suvlasnika.<br />
> Aplikacija za svaki profil omogućuje kreiranje korisničkog imena, lozinke i e-mail adrese.<br />
> Aplikacija mora korisnicima omogućiti promjenu lozinke koristeći prethodnu lozinku.<br />
> Aplikacija realizira aplikacijsko sučelje koje će koristiti aplikacija StanBlog, a preko kojegje moguće kreirati sastanak kriran iz specifične diskusije.<br />
> Proces registracije i prijave bit će pojednostavljen korištenjem vanjskih servisa za autentifikaciju.<br />

# Nefunkcijski zahtjevi
> Osigurani su podaci o korisnicima. <br />
> Omogućiti korisnicim da kreiraju najmanje jedan sastanak. <br />
> Usluga će biti dostupna 24 sata na dan. <br />
> Svi podaci o korisnicima i sastanicma biti će zaštićeni. <br />



# Tehnologije
> Dizajn - Figma <br />
> Frontend - React <br />
> Backend - .Net Core <br />
> Development -  <br />
> Baze podataka - PostgreSQL <br />
> Testiranje - <br />
> Dokumentacija -  <br />


# Članovi tima 
> Ivan Gulubić ivan.golubic@fer.unizg.hr - Voditelj projekta <br />
> Marin Rossini marin.rossini@fer.unizg.hr <br />
> Sara Lazarušić sara.lazarusic@fer.unizg.hr <br />
> Kristian Lovey kristian.lovey@fer.unizg.hr <br />
> Vedran Vrabec vedran.vrabec@fer.unizg.hr <br />
> Dino Dervišević dino.dervisevic@fer.unizg.hr <br />
> Lovro Nidogon lovro.nidogon@fer.unizg.hr <br />

# Kontribucije
>Pravila ovise o organizaciji tima i su često izdvojena u CONTRIBUTING.md



# 📝 Kodeks ponašanja [![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](CODE_OF_CONDUCT.md)
Kao studenti sigurno ste upoznati s minimumom prihvatljivog ponašanja definiran u [KODEKS PONAŠANJA STUDENATA FAKULTETA ELEKTROTEHNIKE I RAČUNARSTVA SVEUČILIŠTA U ZAGREBU](https://www.fer.hr/_download/repository/Kodeks_ponasanja_studenata_FER-a_procisceni_tekst_2016%5B1%5D.pdf), te dodatnim naputcima za timski rad na predmetu [Programsko inženjerstvo](https://wwww.fer.hr).
Očekujemo da ćete poštovati [etički kodeks IEEE-a](https://www.ieee.org/about/corporate/governance/p7-8.html) koji ima važnu obrazovnu funkciju sa svrhom postavljanja najviših standarda integriteta, odgovornog ponašanja i etičkog ponašanja u profesionalnim aktivnosti. Time profesionalna zajednica programskih inženjera definira opća načela koja definiranju  moralni karakter, donošenje važnih poslovnih odluka i uspostavljanje jasnih moralnih očekivanja za sve pripadnike zajenice.

Kodeks ponašanja skup je provedivih pravila koja služe za jasnu komunikaciju očekivanja i zahtjeva za rad zajednice/tima. Njime se jasno definiraju obaveze, prava, neprihvatljiva ponašanja te  odgovarajuće posljedice (za razliku od etičkog kodeksa). U ovom repozitoriju dan je jedan od široko prihvačenih kodeks ponašanja za rad u zajednici otvorenog koda.
>### Poboljšajte funkcioniranje tima:
>* definirajte načina na koji će rad biti podijeljen među članovima grupe
>* dogovorite kako će grupa međusobno komunicirati.
>* ne gubite vrijeme na dogovore na koji će grupa rješavati sporove primjenite standarde!
>* implicitno podrazmijevamo da će svi članovi grupe slijediti kodeks ponašanja.
 
>###  Prijava problema
>Najgore što se može dogoditi je da netko šuti kad postoje problemi. Postoji nekoliko stvari koje možete učiniti kako biste najbolje riješili sukobe i probleme:
>* Obratite mi se izravno [e-pošta](mailto:vlado.sruk@fer.hr) i  učinit ćemo sve što je u našoj moći da u punom povjerenju saznamo koje korake trebamo poduzeti kako bismo riješili problem.
>* Razgovarajte s vašim asistentom jer ima najbolji uvid u dinamiku tima. Zajedno ćete saznati kako riješiti sukob i kako izbjeći daljnje utjecanje u vašem radu.
>* Ako se osjećate ugodno neposredno razgovarajte o problemu. Manje incidente trebalo bi rješavati izravno. Odvojite vrijeme i privatno razgovarajte s pogođenim članom tima te vjerujte u iskrenost.

# 📝 Licenca
Važeča (1)
[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

Ovaj repozitorij sadrži otvoreni obrazovni sadržaji (eng. Open Educational Resources)  i licenciran je prema pravilima Creative Commons licencije koja omogućava da preuzmete djelo, podijelite ga s drugima uz 
uvjet da navođenja autora, ne upotrebljavate ga u komercijalne svrhe te dijelite pod istim uvjetima [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License HR][cc-by-nc-sa].
>
> ### Napomena:
>
> Svi paketi distribuiraju se pod vlastitim licencama.
> Svi upotrijebleni materijali  (slike, modeli, animacije, ...) distribuiraju se pod vlastitim licencama.

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: https://creativecommons.org/licenses/by-nc/4.0/deed.hr 
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

Orginal [![cc0-1.0][cc0-1.0-shield]][cc0-1.0]
>
>COPYING: All the content within this repository is dedicated to the public domain under the CC0 1.0 Universal (CC0 1.0) Public Domain Dedication.
>
[![CC0-1.0][cc0-1.0-image]][cc0-1.0]

[cc0-1.0]: https://creativecommons.org/licenses/by/1.0/deed.en
[cc0-1.0-image]: https://licensebuttons.net/l/by/1.0/88x31.png
[cc0-1.0-shield]: https://img.shields.io/badge/License-CC0--1.0-lightgrey.svg

### Reference na licenciranje repozitorija
