Vězňovo dilema - simulace
=========================

Published on 4. března 2020 12:45

Minule jste si všichni naprogramovali svoji strategii pro vězňovo dilema, dneska si s vašimi strategiemi trošku pohrajeme. Zkusíme si nasimulovat život strategií, stejně jako to dělal Axelrod, a také si zkusíme vyvinout strategii proti nějaké skupině jiných strategií.

Při dnešních cvičeních budete potřebovat strategie z minula (posílal jsem vám je všem mailem, pokud je nemáte, dejte mi vědět).

Představme si, že máme populaci jedinců, kteří mezi sebou opakovaně hrají vězňovo dilema a na základě získaných bodů mají možnost se rozmnožit a přežít do další generace.

V takové simulaci můžeme objevit různé zajímavé věci. Například, existuje strategie, která ve všech případech přežije? Existuje nějaká skupina strategií, kterou nelze napadnout tím, že se objeví nová strategie?

Vezměme si například populaci, ve které všichni jedinci hrají podle strategie Always Cooperate (AC). Co se stane, když se najedou v populaci objeví jedinec, který bude hrát Always Defect (AD)? Všichni AC strategie proti sobě vždy získají 3 body za každý tah. Nově příchozí AD získá ale za každý tah proti AC 5 bodů. Protože je AD jedna a AC je hodně, tak se stane to, že AD bude mít více bodů než AC a začne se v populaci množit. Zastaví se někdy množení AD nebo AD ovládne celou populaci? Co by se stalo, kdyby v populaci místo AC byly Tit-for-tat?

Simulace života strategií je implementována v souboru `simulation.py` ([zdrojové kódy](https://martinpilat.com/media/materials/mas/prisoners-dilemma.zip) jsou stejné jako minule, jen je v nich tento soubor navíc). Simuluje se tedy populace s několika jedinci, kde každý hraje nějakou strategii Počáteční počty strategií jsou v konfiguračním souboru `config.json` - pokud neexistuje, program si vytvoří defaultní, kde je každá strategie desetkrát. Cestu ke konfiguračnímu souboru můžete také zadat jako parametr z příkazové řádky. Potom se simuluje několik generací a v každé generaci každý jedinec potká několik (default 10) jiných náhodných jedinců z populace a sehraje s nimi iterované vězňovo dilema. Za tuto hru oba jedinci dostanou body. Na konci každé generace se body pro jednotlivé jedince sečtou a rozhodne se, kteří jedinci postoupí do další generace. Pravděpodobnost, že jedinec postoupí je úměrná jeho fitness, výběr z populace je s opakováním (v zásadě jde o ruletovou selekci z evolučních algoritmů). Nakonec program zobrazí (a uloží) graf s vývojem počtu jedinců hrajících každou ze strategií a s vývojem počtu bodů, které tyto strategie průměrně získaly.

Nápady na pokusy
----------------

Dnes není žádný domácí úkol, pohrajeme si ale během cvičení s našimi strategiemi/simulátorem.

1.  Zkuste si odsimulovat směsi strategií zmíněné výše - populaci se skoro samými AC/TFT a několika málo jedinci hrajícími AD nebo naopak.
2.  Najděte skupinku strategií, ve které se té vaší bude dařit a skupinku, kde brzy vymře.
3.  Dá se změnit výsledek simulace (nebo i turnaje) tím, že změníte odměny za jednotlivé kombinace tahů? Je vaše hra stále vězňovo dilema?
4.  Uměli byste naimplementovat strategii, která by se učila a postupně měnila tak, aby se jí dařilo v různých populacích?