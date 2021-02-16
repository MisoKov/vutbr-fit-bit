# [IMP - Microprocessors and Embedded Systems](https://www.fit.vut.cz/study/course/13997/)

#### Assignemnt
* Seznamte se s principem tvorby vestavných aplikací v jazyce C založených na mikrokontroléru Kinetis K60 (s jádrem ARM Cortex-M4) fy Freescale v prostředí Kinetis Design Studio (KDS) nebo MCUXpresso.
* V jazyce C (tak i dále) vytvořte projekt demonstrující možnosti generování přerušení modulem Periodic Interrupt Timer (PIT) dostupném na mikrokontroléru Kinetis K60 z desky platformy FITkit 3. UPŘESNĚNÍ: 1) V rámci jednoduché vestavné aplikace vhodně ověřte a demonstrujte schopnost PIT generovat přerušení v následujících režimech: nezřetězený s jedním časovačem, nezřetězený s více časovači, zřetězený. U nezřetězeného režimu s více časovači si počet časovačů zvolte (na 2 a více), u zřetězeného režimu postačí, omezíte-li se na zřetězení dvou a poté maximálního počtu časovačů. 2) Aplikace nechť umožňuje interakci s uživatelem - přinejmenším, přerušení od PIT vhodně signalizujte (např. pomocí LED, piezzo bzučáku či teminálu). 3) Na základě zkušeností získaných při tvorbě a ověřování funkčnosti aplikace nastavte aplikaci tak, aby byla schopna generovat přerušení s co nejkratší a poté s co nejdelší periodou. Funkčnost nastavení vhodně ověřte a porovnejte s vlastnostmi očekávanými dle technické dokumentace. 
* Vytvořte přehlednou dokumentaci k přípravě, způsobu realizace, k funkčnosti a vlastnostem řešení projektu.

**Obtained points**:<br>
12,7/14

**Notes**:<br>
* Nadprůměrné nadšení pro řešení projektu, snaha řešit projekt včas, kvalitně a/nebo nad rámec vymezený zadáním ( .9 z  1 b. ). 
* Řešení je zcela funkční ( 5 z  5 b. ).
* Dekompozice řešení je na velmi dobré úrovni ( .85 z  1 b. ).
* Způsob řešení je pochopitelný a čitelný ( .85 z  1 b. ). Uživatelská přívětivost řešení je na vysoké úrovni ( .95 z  1 b. ).
* Úvod do řešené problematiky (motivace, přehled použitých HW/SW prostředků (moduly a jejich registry, knihovny a jejich API atp.), s tématem/řešením souvisejících  principů, metod  atd.) v dokumentaci je zpracován chvályhodně ( 1 z  1 b. ).
* Popis způsobu řešení v dokumentaci je zpracován chvályhodně ( 2 z  2 b. ). Zhodnocení řešení v dokumentaci je nedostatečné ( .2 z  1 b. ).
* Prezentace řešení byla chályhodná ( 1 z  1 b. ).

Pozn.: výsledné hodnocení ( 12.7 ) vzniklo zaokrouhlením součtu dílčích, výše uvedených, bodových hodnocení (výsledný součet je 12.75), násobeným ohodnocením míry funkčnosti řešení (míra je K1 + K2 *  5 / 5 [kde K1=0.25, K2=0.75 jsou konstanty hodnoticího algoritmu] = 1).

**Enviroment used**:<br>
Kinetis Design Studio 3.0.0.

**IMPORTANT**:<br>
Project was developed without required hardware and was not tested, may not be running properly.
