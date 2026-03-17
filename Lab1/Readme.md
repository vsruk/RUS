# Lab1: Prekidi u ugradbenim sustavima

## Opis zadatka

Ovaj zadatak uključuje korištenje mikrokontrolera za demonstraciju rada s višestrukim prekidima u ugradbenim sustavima. Implementacija obuhvaća obradu signala s tipkala, tajmera, senzora udaljenosti i drugih komponenti. Razvoj i ispitivanje koda provodi se u Wokwi simulatoru.

---

## Cilj zadatka

### 1. Razumijevanje prekida odabranog mikrokontrolera  
   - Detaljno proučiti mehanizme prekida mikrokontrolera i mogućnosti obrade višestrukih prekida.  
   - Razumjeti prioritizaciju prekida i mehanizme preklapanja (*nested interrupts*).

### 2. Implementacija i ispitivanje prioriteta prekida  
   - Osmisliti primjer s jasno definiranim prioritetima prekida.
   - Osigurati pravilno upravljanje višestrukim prekidima u stvarnom vremenu.
   - Provesti ispitivanja u simulatoru i stvarnim uvjetima.
   
   **Cilj je razviti robusno rješenje za višestruke prekide, testirati ga u simulatoru te dokumentirati svaki korak implementacije!**

> #### Različiti izvori prekida
>   - Detekcija pritisaka tipkala s različitim prioritetima (visoki, srednji, niski).
>   - Generiranje prekida pomoću tajmera.
>   - Mjerenje udaljenosti pomoću senzora (HC-SR04) i sl.
>   - Definiranje hijerarhije prioriteta svih prekida.
>
> #### Postavljanje prioriteta prekida
>   - Postaviti različite prioritete za prekide kako bi se osigurala prednost kritičnih događaja.
>   - Omogućiti preklapanje prekida (*nested interrupts*) ako razvojna platforma to podržava.
>
>#### Efikasno upravljanje resursima
>   - Spriječiti konflikte pristupa resursima korištenjem **semafora, kritičnih sekcija i zastavica (flags)**.
>   - Minimizirati vrijeme izvršavanja ISR funkcija kako bi se izbjegle blokade drugih prekida.
>   - Implementirati logiku koja pokazuje utjecaj prioriteta na obradu događaja.
>   - Razviti mehanizam za detekciju i rješavanje sukoba višestrukih prekida koji se javljaju istovremeno.

---

### 3. Rad s GitHub platformom
   - Praćenje verzija koda i dokumenata.
   - Razvoj i ispitivanje sustava u Wokwi simulatoru.
   - Dokumentacija projekta kroz GitHub Wiki. Za dokumentaciju ovih vježbi možete iskoristiti Readme.md
   - Jasno dokumentirati način rada programa, uključujući opis svakog prekida i njegovog prioriteta.
   - Izraditi i priložiti Control Flow Graph (CFG) koji jasno prikazuje tijek izvršavanja glavnog programa i skokove u prekidne rutine (ISR).

### 4. Korištenje logičkog analizatora  
   - Snimanje i analiza rada prekida pomoću logičkog analizatora.
   - Verifikacija ispravnosti izvršavanja prioriteta i pravovremene reakcije na događaje.
   - U dokumentaciji prikažite jedno ispitivanje redosljeda prekida, uključujući opis.
     
### 5. Automatizacija dokumentacije koda  
   - Generiranje tehničke dokumentacije pomoću **Doxygen** alata.
   - Održavanje ažurirane dokumentacije bez potrebe za ručnim pisanjem.
   - Provođenje ispitivanja kako bi se osigurala pravilna reakcija sustava na različite događaje i spriječili konflikti između prekida.

> Napomena: Komentari u programu trebaju biti usmjereni na objašnjavanje specifičnih dijelova koda, kao što su funkcionalnost ISR funkcija, ključne varijable, ili razlozi zbog kojih je implementacija izvedena na određeni način. Cilj komentara je omogućiti programerima da razumiju što određeni dio koda radi, a ne pružati opširne tehničke informacije o principima i teoriji.
> FAQ: Zašto su u primjeru komentari opsežniji?
> U dokumentaciji programa (npr. Doxygen komentari) poželjno je uključiti osnovne informacije o tome kako, na primjer, prekidi i ISR funkcije rade, kao i smjernice vezane uz dizajn. Ova vrsta dokumentacije namijenjena je programerima i inženjerima i pruža detaljne informacije o arhitekturi sustava, dizajnu koda, te implementacijskim odlukama. Takva dokumentacija pomaže u održavanju i nadogradnji sustava, osiguravajući dugoročnu kvalitetu.
> Zašto koristiti automatiziranu dokumentaciju?
> Automatizacija dokumentacije osigurava da je kod ugradbenih sustava kvalitetan, održiv i lak za razumijevanje, što doprinosi uspjehu projekta i povećava zadovoljstvo krajnjih korisnika.

#### Smjernice za korištenje Doxygena:
   - **Dodati komentare u kod koristeći Doxygen sintaksu**
    primjer je dan u rpozitoriju
    
   - **Po potrebi modificirati  Doxygen konfiguracijsku datoteku**
     - `OUTPUT_DIRECTORY = ./docs` (sprema generirane dokumente u direktorij `docs`)
   -  **Dokumentacija  generirati  u HTML ** te je dostupna u direktoriju `docs/html/index.html`.
   
#### Automatizacija generiranja dokumentacije pomoću GitHub Actions
   - Modificirati datoteku `.github/workflows/doxygen.yml` s konfiguracijom za automatsko generiranje dokumentacije:
             with:
               github_token: ${{ secrets.GITHUB_TOKEN }}
               publish_dir: ./docs/html
   - https://docs.github.com/en/actions/security-for-github-actions/security-guides/automatic-token-authentication
   - U vašem  tokenu npr. https://github.com/settings/tokens/22000326xx označite workflow! tako da primjer u Lab1 radi!
   - **Važno**: Kako bi ova akcija radila, potrebno je omogućiti **GitHub Pages** u postavkama repozitorija i osigurati da se generirani HTML dokumenti nalaze u `docs/html` direktoriju.
   - **Napomena**: OPcionalno, u slučaju privatnog repozitorija, potrebno je kreirati **Personal Access Token (PAT)** i dodati ga u `secrets` pod imenom `GH_PAT` umjesto `GITHUB_TOKEN`.
   - **Modifikacija Doxygen konfiguracije**: U `Doxyfile` postaviti `OUTPUT_DIRECTORY = ./docs` kako bi se osigurala kompatibilnost s GitHub Actions.
### Završna napomena
>Sve gore navedene stavke su obvezne.
>ne postoji obvezni i dodatni dio zadatka
>ne postoji izbor između pojedinih elemenata
>zadatak se smatra cjelovito izvršenim isključivo ako su implementirani, ispitani i dokumentirani svi navedeni zahtjevi
---
### Reference: Suvremeni resursi za razvoj ugradbenih sustava

1. **Razvojna okruženja i simulacija**
   * **PlatformIO**: Moderni ekosustav i alat za profesionalni razvoj ugradbenih sustava (C/C++), integriran u VS Code. [Dostupno na: platformio.org](https://platformio.org/)
   * **Wokwi Simulator**: Napredni online simulator mikrokontrolera (ESP32, Pi Pico, Arduino) i periferije, idealan za testiranje prekida bez hardvera. [Dostupno na: wokwi.com](https://wokwi.com/)

2. **Odabir mikrokontrolera i arhitekture**
   * **Mouser MCU Vodič**: Referentni vodič za vizualizaciju ključnih građevnih blokova modernih mikrokontrolera. [Dostupno na: Mouser MCU Guide](https://emea.info.mouser.com/microcontroller-guide/)
   * Za specifične implementacije složenih prekida (Nested Interrupts) preporuča se proučavanje arhitektura poput **ARM Cortex-M (NVIC)** ili **ESP32**.

3. **Dokumentacija kao kod (Docs-as-Code) i CI/CD**
   * **Doxygen**: Standardni alat za generiranje dokumentacije iz izvornog koda. [Dostupno na: doxygen.nl](https://www.doxygen.nl/index.html)
   * **Doxygen Awesome Theme**: Suvremena CSS tema koja Doxygen HTML izlaz čini modernim, responzivnim i prilagođenim za tamni način rada (Dark Mode). [Dostupno na: GitHub - jothepro/doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)
   * **GitHub Actions za Doxygen**: Automatizacija generiranja dokumentacije i objava na GitHub Pages. [Dostupno na: Doxygen Action (Marketplace)](https://github.com/marketplace/actions/doxygen-and-github-pages)

4. **Analiza signala i debagiranje**
   * **Sigrok & PulseView**: Industrijski standard za open-source analizu logičkih signala. Koristi se za vizualnu potvrdu prioriteta i vremenskog izvršavanja prekidnih rutina (ISR). [Dostupno na: sigrok.org](https://sigrok.org/wiki/PulseView)

5. **Vizualizacija arhitekture i logike programa**
   * **Mermaid.js**: Alat za generiranje dijagrama (uključujući Control Flow Graph) pomoću teksta. Nativno podržan unutar GitHub Markdown datoteka. [Dostupno na: mermaid.js.org](https://mermaid.js.org/)
   * **PlantUML**: Otvoreni alat za crtanje UML dijagrama (razred, sekvenca, stanja). [Dostupno na: plantuml.com](https://plantuml.com/)
     
  Primjer integracije u .md"http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/vsruk/RUS/main/Lab1/docs/Primjer_RUS_prekidi.puml":
  ![Primjer_prekida](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/vsruk/RUS/main/Lab1/docs/Primjer_RUS_prekidi.puml)








