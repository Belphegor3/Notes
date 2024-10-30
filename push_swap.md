# Radix	:

- Tout d abord, en binaire (comme dans toutes les bases ou 0 vaut 0), 
on peut ajouter autant de 0 qu on veut devant les nombres, ca ne changera 
pas leurs valeurs !
- Par convention, on ecrit le binaire par paquet de 4 bits parce que 4 bits
en binaire equivaut a une unite en hexadecimal.
- Le plus grand paquet de 4 bits en binaire vaut 1111 qui vaut 15 en decimal
et F en hexadecimal.
- Il est donc tres simple de passer du binaire a l hexa et reciproquement!

## Ca fait quoi radix?

Le but du radix est de trier en connaissant de base le resultat final il
faut donc chercher ces index finaux (**il faut donc avoir une copie de la stack A initiale pour pouvoir incrementer une valeur quand l index final qu on cherche est plus petit que celui avec lequel on le compare**).

## Interet de radix?

Tres simple a mettre en oeuvre et efficace avec un tres grand nombre de valeur a trier (500/700 nombres a trier c est vraiment tres peu).
Ca fait valider a 84% mais rapidement et ca fait voir le bitshifting/les operations binaires si vous connaissez pas encore.  

### Explication illustre des index finaux

Stack.a		: 5 2 1 4 3	     Stack.b :  
Index 		: 0 1 2 3 4	     Stack.b :  
Index finaux: 4 1 0 3 2	     Stack.b :  

Concretement maintenant, on va devoir utiliser des operations binaires ([operations_binaires](#operations-binaires)) et du bitshifting ([bitshifting](#bitshifting)) avec les index finaux pour tout trier.

## Operations binaires

En connaissant les index finaux on va ensuite compare leurs valeurs en binaire. On va comparer bit a bit (exemple 101010: d abord le 0 tout a droite, puis le 1 juste a sa gauche puis le 0 juste a la gauche du 1 qui est a la droite du 0...)  
**Les operations binaires sont |, & et ^**  
- L operateur & vaut 1 si les 2 bits compares valent 1.
- L operateur | vaut 1 si au moins l un des deux bits vaut 1.
- L operateur ^ vaut 1 si et seulement si un seul des deux bits vaut 1.

Pour comparer 2 nombres qui n ont pas le meme nombre de bits, c est tres simple il suffit d ajouter un nombre suffisant de 0 devant le plus petit nombre pour obtenir le meme nombre de bits etant donne que 101 = 0101 = 00101 = 000101 = ...

### Exemples operations binaires
- 8 & 9 (1000 & 1001) = 1000 (8 en decimal)  
- 9 & 5 (1001 & 101) = 0001 (1 en decimal)    1001 & 101 <=> 1001 & 0101  
- 8 | 9 (1000 | 1001) = 1001 (9 en decimal)  
- 9 | 5 (1001 | 101) = 1101 (13 en decimal)	1001 | 101 <=> 1001 | 0101  
- 8 ^ 9 (1000 | 1001) = 0001 (1 en decimal)  
- 9 ^ 5 (1001 ^ 101) = 1100 (12 en decimal)   1001 ^ 101 <=> 1001 | 0101  

Pas besoin de changer de base, l ordinateur sait que les operateurs binaires fonctionnent uniquement en binaire et traduit donc directement en base 2.  
**L ordinateur part du principe qu on ecrit en decimal, si vous vous rappelez de printf, pour que l ordi comprenne qu on ecrit en hexa on ajoutait le prefix '0x'!**  
Ca signifie que 14 vaut 14 en decimal mais 0x14 vaut 20 en decimal (1 x 16 + 4 = 20)

## Bitshifting
Le bitshifting est aussi appele le decallage binaire puisque s agit d ajouter des 0 ou enlever des 0 ou des 1.  
On va notaemment utiliser le bitshifting pour cibler un bit et comparer les "unites" puis les "dizaines" puis les "centaines"... (je sais pas comment ca s appelle en binaire xD)  
Parce que pour utiliser radix il va falloir comparer chaque bit de chaque index final (cf [exemple_radix](#exemple-radix))  
Ca s ecrit "<<" ou ">>" tout depend du sens dans lequel on veut aller.  
En fait ca multiplie ou divise par deux (parce que c est du binaire) le nombre de fois qu on veut.  
Ce qui veut dire qu on multiplie ou divise par 2 puissance ce qu on veut 

### Exemples bitshifting
8 >> 0  equivaux donc a 1000 ou on enleve 0 bit a la fin (puisque dans ce sens on divise) ou encore 8 divise par 2 puissance 0 <=> 8 / 1 (oui n importe quoi puissance 0 fait 1)  
8 >> 1 <=> 1000 ou on enleve 1 bit a la fin donc 100 <=> 8 / 2 = 4 en decimal  
8 >> 2 <=> 1000 ou on enleve 2 bit a la fin donc 10 <=>	8 / 4 = 2 en decimal  
8 >> 3 <=> 1000 ou on enleve 3 bit a la fin donc 1 <=> 8 / 8 = 1 en decimal  
8 >> 4 <=> 1000 ou on enleve 4 bit a la fin donc 0 <=> 8 / 16 = 0 en decimal  
**On peut continuer mais ca fera tjs 0**  


5 >> 0 <=> 101 ou on enleve 0 bit a la fin donc 101 <=> 5 / 1 = 5 en decimal  
5 >> 1 <=> 101 ou on enleve 1 bit a la fin donc 10 <=> 5 / 2 = 2 en decimal  
5 >> 2 <=> 101 ou on enleve 2 bit a la fin donc 1 <=> 5 / 4 = 1 en decimal  
5 >> 3 <=> 101 ou on enleve 3 bit a la fin donc 0 <=> 5 / 8 = 0 en decimal    


9 << 0 <=> 1001 ou on ajoute zero 0 a la fin (**oui forcement un 0, on peut pas rajouter un 1**) donc 1001 <=> 9 x 1 = 9 en decimal  
9 << 1 <=> 1001 ou on ajoute un 0 a la fin donc 10010 (16 + 2 en decimal) <=> 9 x 2 = 18 en decimal  
9 << 2 <=> 1001 ou on ajoute deux 0 a la fin donc 100100 (32 + 4 en decimal) <=> 9 x (2 puissance 2) = 9 x 4 = 36 en decimal    
9 << 3 <=> 1001 ou on ajoute trois 0 a la fin donc 1001000 (64 + 8 en decimal) <=> 9 x (2 puissance 3) = 9 x 8 = 72 en decimal    
   ...					    ...  

# Exemple radix

Le nombre max de bitshift qu on doit donc realiser avec radix est le nombre de bits qu il y a dans l index final le plus grand.  
5 possede 3 bits et on va jusque >> 3 pour atteindre 0  
Mais par exemple si on devait trier un tableau contenant 19 valeurs, on aurait donc pour index final maximum 18 qui vaut 10010 en binaire et contient donc 5 bits.  

Revenons a l exemple du debut :  

Stack.a         : 5 2 1 4 3      Stack.b :  
Index           : 0 1 2 3 4      Stack.b :  
Index finaux    : 4 1 0 3 2      Stack.b :  

On va devoir comparer chaque bit de chaque nombre (enfin premier bit de tous les nombres puis deuxieme bit de tous les nombres...)  
Donc premierement on va comparer tous les bits les plus a droite donc on bitshift de 0 et on compare la valeur avec 1 avec un &.  
**Quand le resultat vaut 1 on rotate_a, quand il vaut 0 on push_b.**  

Exemples :	Stack.a[0] = 5 qui a pour index final 4  
		4 vaut 100 en binaire  
		4 >> 0 & 1 = 0	(pourquoi 0? ne pas oublier qu on compare chaque bit 1 a 1)  
        4 >> 0 = 4  => 4 & 1 <=> 100 & 001 = 001 = 0  
		0 signifie qu on push_b  
Stack.a         : 2 1 4 3     Stack.b : 5  
Index           : 0 1 2 3  
Index finaux    : 1 0 3 2  
		
Maintenant Stack.a[0] = 2 qui a pour index final 1 (en effet l index final est fixe ca ne depend pas du nombre de nombres qu on a dans la stack.a c est le resultat final qui compte)  
		1 vaut 1 en binaire  
		1 >> 0 & 1 = 1  
        1 >> 0 = 1  => 1 & 1 = 1   
		1 signifie qu on rotate_a  
Stack.a         : 1 4 3 2     Stack.b : 5  
Index           : 0 1 2 3  
Index finaux    : 0 3 2 1  

Maintenant Stack.a[0] = 1 qui a pour index final 0  
		0 vaut 0 en binaire  
		0 >> 0 & 1 = 0  
		0 signifie qu on push_b  
Stack.a         : 4 3 2     Stack.b : 1 5  
Index           : 0 1 2  
Index finaux    : 3 2 1  

Maintenant Stack.a[0] = 4 qui a pour index final 3  
		3 vaut 11 en binaire  
		11 >> 0 & 1 = 1  
		1 signifie qu on rotate_a  
Stack.a         : 3 2 4    Stack.b : 1 5  
Index           : 0 1 2  
Index finaux    : 2 1 3  

Maintenant Stack.a[0] = 3 qui a pour index final 2  
		2 vaut 10 en binaire  
		10 >> 0 & 1 = 0  
		0 signifie qu on push_b  
Stack.a         : 2 4    Stack.b : 3 1 5  
Index           : 0 1  
Index finaux    : 1 3  
 
**Maintenant qu on a fini avec tous les premiers bit de droite, on va devoir passer a ceux d apres, mais avant ca il faut tout remettre dans Stack.a.**  
Donc on push_a jusqu a ce que Stack.b soit vide.  

*(PS : reflechir a la condition d arret de cette boucle)*  

Stack.a         : 3 2 4     Stack.b : 1 5  
Index           : 0 1 2  
Index finaux    : 2 1 3  

Stack.a         : 1 3 2 4     Stack.b : 5  
Index           : 0 1 2 3  
Index finaux    : 0 2 1 3  

Stack.a         : 5 1 3 2 4     Stack.b :  
Index           : 0 1 2 3 4  
Index finaux    : 4 0 2 1 3  

**On reitere l operation jusqu a ce qu on ait parcouru tous les bits.**  
**On veut donc maintenant cibler les "dizaines" donc on va bitshift de 1 a chaque fois**  

Maintenant Stack.a[0] = 5 qui a pour index final 4  
        4 vaut 100 en binaire  
        4 >> 1 & 1 = ?  
        4 >> 1 = 2  
        2 & 1 <=> 10 & 01 = 00 = 0  
        0 signifie qu on push_b....  

On compare donc le deuxieme bit le plus a droite de tous les nombres du tableau.  
Puis on le fera avec le troisieme bit le plus a droite qui s avere etre le bit le plus a gauche puisqu on a que 5 nombres dans le tableau et qu on a donc un index final max de 4 qui contient 3 bits.  
Mais si on avait 19 nombres dans le tableau on aurait donc du repeter cette boucle 5 fois.  

Vous n etes pas convaincu ? Finissez l algo sur papier ! Le faire sur papier montre que c est pas opti, il faut bien aller jusqu au bout de l algo mais il est tres simple a transcrire en code qui ne fait que quelques lignes!
