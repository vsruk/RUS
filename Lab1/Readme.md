# Lab1: Prekidi u Ugradbenim Sustavima

## Opis zadatka

U ovom zadatku potrebno je koristi mikrokontroler za demonstraciju rada s prekidima na ugradbenom sustavu (vlastiti odabir). Implementirati različite prekidi za obradu signala s tipkala, tajmera, senzora udaljenosti i sl.. Koristiti  Wokwi simulator za razvoj i ispitivanje koda.

Projekt uključuje:
- Detekciju pritisaka tipkala s prioritetima (visoki, srednji, niski).
- Generiranje prekida pomoću tajmera za generiranje signala na svakih 1 sekundi.
- Mjerenje udaljenosti pomoću senzora (HC-SR04) i aktivacija LED indikatora.
- Obrada svih prekida u glavnoj petlji sa zadanim prioritetima.

## **Cilj zadatka**
1. **Proučiti prekide odabranog mikrokontrolera**  
   U okviru ovog zadatka potrebno je detaljno proučiti i implementirati prekide mikrokontrolera, posebno s naglaskom na njihove prioritete i obradu u sustavu.

2. **Napisati i ispitati program za primjer prioriteta prekida**  
   Osmisliti  primjer s  definiranim prioritetima prekida kako bi se osiguralo pravilno upravljanje prekidima. Ovaj cilj omogućava razumijevanje kako mikrokontroler upravlja višestrukim prekidima u stvarnom vremenu i kako se različiti prioriteti prekida mogu učinkovito implementirati u sustavu. Ispitivanje programa u simulacijama i u stvarnim uvjetima omogućava potvrdu ispravnosti implementacije.

3. **Uporaba GitHub platforme**:
   - **Vježbanje inačice dokumenata** – Praćenje promjena rad na projektu.
   - **Familiarizacija s Wokwi platformom** – Razvoj i ispitivanje mikrokontrolerskih sustava u simulatoru.
   - **Savladavanje pisanja Wiki dokumentacije** – Dokumentiranje sustava.

4. **Primjena logičkog analizatora**:
   Rad prekida mora biti snimljen i analiziran pomoću **logičkog analizatora** kako bi se potvrdilo ispravno izvršavanje prioriteta i pravovremena reakcija na događaje.

5. **Automatizacija dokumentacije koda**  
   Cilj je automatski generirati dokumentaciju iz izvornog koda projekta, koristeći alat Doxygen. Automatizacija ovog procesa omogućuje održavanje ažurirane i točne dokumentacije bez potrebe za ručnim pisanjem uputa. Korištenjem alatnih sustava za automatsko generiranje dokumentacije, možemo osigurati da svi budu u mogućnosti razumjeti i koristiti kod bez potrebe za dugim razdobljem objašnjavanja svakog detalja.
