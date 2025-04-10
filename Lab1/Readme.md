# Lab1: Prekidi u Ugradbenim Sustavima

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

---
Reference:
1. Resursi za odabir mikrokontrolera

	Za odabir odgovarajućeg mikrokontrolera i pristup relevantnoj dokumentaciji, **Mouser Electronics** pruža sljedeće resurse:

	## Katalog mikrokontrolera

    **Opis**: Mouser nudi širok izbor mikrokontrolera i omogućuje filtriranje prema različitim parametrima kao što su arhitektura, brzina takta, kapacitet memorije i dostupni periferni uređaji.

    Dostupno na:: [Mikrokontroleri - MCU na Mouseru](https://www.mouser.com/c/semiconductors/processors/microcontrollers-mcu/)

    ## Vodič za mikrokontrolere

   **Opis**: Referentni vodič koji sadrži osnovne informacije o najčešćim arhitekturama mikrokontrolera. Ovaj vodič pomaže vizualizirati ključne građevne blokove modernih mikrokontrolera i njihove međusobne veze, što je korisno za inženjere, hobiste i studente.

   Dostupno na:*: [Vaš esencijalni vodič za MCU](https://emea.info.mouser.com/microcontroller-guide/)
   
2. Wokwi. *Wokwi -  Napredni online simulator mikrokontrolera  i perifereije**. [Online]. Dostupno na: https://wokwi.com/

3. Automatizacija dokumentacije, van Heesch, D. (1997). *Doxygen*. [Online]. Dostupno na: https://www.doxygen.nl/index.html
   Primjer: Atomatsko generiranje i implementaciju Doxygen dokumentacije na GitHub Pages 
        [1] AgarwalSaurav. (n.d.). *Doxygen and GitHub Pages*. GitHub Marketplace. Dostupno na: https://github.c
		[2] Tehničko veleučilište u Zagrebu. *RUS - Repozitorij za razvoj ugradbenih sustava*. GitHub. Dostupno na: https://github.com/vsruk/RUS
	
4. Analiza i vizualizacija podataka snimljenih pomoću Wokwi logičkog analizatora Dostupno na: https://docs.wokwi.com/guides/logic-analyzer#viewing-the-data-in-pulseview	

5. Vizualizacija CFG dijagrama Graphviz diagrama Dostupno [Graphviz Online Editor](https://www.devtoolsdaily.com/graphviz/). 

6. PlantUML, “PlantUML: Open-source tool to draw UML diagrams.”,  Dostupno: https://plantuml







