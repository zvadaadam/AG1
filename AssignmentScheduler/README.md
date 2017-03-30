Nevyřešené progtestové úlohy už vám poněkud přerostly přes hlavu. Na projití do dalšího semestru, kdy můžete začít s čistým štítem a konečně začít dělat všechno včas, naštěstí není zapotřebí splnit všechny úlohy. To je dobře, jelikož vyřešit bez výjimky všechny úlohy nemusí být ve vašich silách. Je ale nutné stanovit si bojový plán a vybrat právě ty úlohy, které splnit zvládnete, resp. stihnete.

V systému na vás čeká hned N nevyřešených úloh. Každá úloha má stanovený určitý počet bodů, který získáte za její úspěšné vyřešení, a dále časový interval, od kdy do kdy je úloha otevřená. Čas otevření však v zásadě nikoho nezajímá, protože, jak všichni velmi dobře víme, na úloze se začíná pracovat na poslední chvíli, přesně v okamžik, aby se stihla odevzdat těsně před deadline. Ze stejného důvodu, jakmile začnete na úloze pracovat, není možné práci přerušit (to byste ji nestihli dodělat). U každé úlohy je tedy zadán časový interval, kdy na ní musíte začít pracovat, a deadline odevzdání. Intervaly jednotlivých úloh se mohou překrývat.

Vaším cílem bude vybrat takovou podmnožinu úloh, které jednak zvládnete vyřešit (nemůžete pracovat na dvou úlohách současně, ani práci na úloze jakkoli přerušit) a zároveň chcete získat maximální možný počet bodů. Pokud má úloha X deadline ve stejný okamžik, jako má úloha Y začátek, je možné pracovat na obou. Pokud je více možných řešení, vypište libovolné z nich.

A protože chcete udělat radost L. Vagnerovi, tak ve svých řešeních nepoužijete knihovnu STL.


Formát vstupu:

Na prvním řádku je číslo N, 1 ≤ N ≤ 105 - počet úloh v systému.
Pak následuje N řádků, kde i-tý řadek obsahuje tři čísla zi, ki a bi, kde zi udává čas, kdy je třeba začít pracovat na i-té úloze, ki udává deadline i-té úlohy, a bi počet bodů za i-tou úlohu.
Můžete se spolehnout, že platí 0 ≤ zi, ki, bi ≤ 109 a zároveň zi < ki.
Formát výstupu:


Na prvním řádku vypište číslo s maximálním možným počtem bodů, které lze získat.
Na druhém řádku vypište mezerami oddělený vzestupně uspořádaný seznam pořadových čísel úloh, které tvoří optimální řešení (úlohy číslujeme od 0 dle pořadí v jakém se objevily na vstupu).


Bodové podmínky:

Pro splnění povinných testů (test základními a malými daty) je zapotřebí, aby program fungoval korektně pro vstupy o nejvýše 20 úlohách.
Pro splnění testu středními daty musí program splnit časový a paměťový limit pro vstupy o nejvýše 5 000 úlohách.
Pro splnění testu velkými daty musí program splnit časový a paměťový limit pro vstupy o nejvýše 100 000 úlohách.


Ukázka práce programu:


Příklad vstupu 1:
5
1 7 2
5 11 2
3 9 5
7 12 12
10 14 10


Příklad výstupu 1:
15
2 4


Příklad vstupu 2:
10
13 14 10
12 13 10
11 12 10
10 11 10
9 10 10
7 8 10
5 6 10
3 4 10
2 3 10
1 2 10


Příklad výstupu 2:
100
0 1 2 3 4 5 6 7 8 9

