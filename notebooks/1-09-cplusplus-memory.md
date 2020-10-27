<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12-MINES-ParisTech</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>
</div>

```c++
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

<br>

# les zones mémoires d'un programme
</div>


Pour faire ce notebook, il faudrait avoir fait et corrigé le TP sur la calculette en notation polonaise inversée dans sa version simple où toutes les fonctions sont dans un même fichier `rpn_calc.cpp` mais nous pouvons nous en passer.

Nous allons réduire le code de la calculette à la partie sur la gestion de la pile pour empiler les opérandes de la calculette.


dans le main nous créons un tableau, local à la pile d'exécution, de `argc` entiers, nous mettons `12` dans notre pile d'opérandes, puis nous dépilons une valeur que nous affichons. 

>```c++
>// dans le fichier simple-pile.cpp
>#include<iostream>
void push (int stack [], int* top, int token) {
   stack[*top] = token; 
   *top = *top + 1;
}
int pop (int stack [], int* top) {
  *top = *top - 1;
  return stack[*top];
}
int main (int argc, char* argv []) {
   int stack [argc];
   int top = 0;
   push(stack, &top, 12);
   std::cout << pop(stack, &top) << std::endl;
  return 0;
}
```

```bash
$ g++ simple-pile.cpp -o pile
$ ./pile
12
```


## nombre de zones mémoires


<div class="framed-cell">
<ins class="underlined-title">les zones mémoires d'un programme</ins>

<br>

Nous avons vu jusqu'à présent deux zones mémoires
* la `mémoire statique` (qui contient le programme) et les données globales
* la `pile d'exécution` qui contient les variables locales aux fonctions  
et les arguments des fonctions lors de leur appel

<br>

Posons nous un nouveau problème:
* comment définir un objet dans le corps d'une fonction
* de manière à ce que cet objet continue d'exister quand la fonction termine ?

<br>

par exemple, dans le code ci-dessus (extrait de notre calculette en notation polonaise inversée), demandons nous comment implémenter une fonction `init_stack` qui alloue donc le tableau bas-niveau de $n$ `int` dont nous avons besoin ...

</div>

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">une très très très très très très très fausse manière de faire</ins>

dans la fonction `init_stack` j'alloue un tableau de bas-niveau dans la pile d'exécution  
je renvoie l'adresse de ce tableau  
j'appelle cette fonction dans le `main`  
et j'accède à ce tableau ... (malheur !)

<br>


>```c++
>// dans le fichier faux.cpp
>#include<iostream>
int* init_stack (int n) {
  int tab [n];
  return tab; // mais c'est horrrrrible !!
}
...
int main (int argc, char* argv []) {
  int* stack = init_stack(argc);
  int top = 0;
  push(stack, &top, 12);
  std::cout << pop(stack, &top) << std::endl;
  return 0;
}
```

```bash
$ g++ faux.cpp
faux.cpp: In function ‘int* init_stack(int)’:
faux.cpp:3:7: warning: address of local variable ‘tab’ returned [-Wreturn-local-addr]
   int tab [n];
       ^~~
$ ./a.out 12
Segmentation fault (core dumped)
```

<br>

c'est tellement faux que `c++` se venge avec une segmentation-fault !

</div>
<!-- #endregion -->

<div class="framed-cell">
<ins class="underlined-title">Que se passe-t-il dans ce code très très très très très faux</ins>
    
<br>

* le tableau de bas-niveau `tab` renvoyé par la fonction `init_stack` a été alloué dans la `pile d'exécution` de votre process
* cette zone sera dépilée à la sortie de la fonction `init_stack`
* la zone mémoire d'adresse `tab` n'existera plus !     
* ainsi `stack` dans le `main` qui contient la valeur de `tab` pointe vers une zone qui n'existe plus  
* d'où la segmentation-fault

<br>

avec ce que vous savez actuellement du langage, vous ne pouvez pas faire ce qui vous est demandé !

<br>
Que vous manque-t-il ?

<br>

il vous manque une manière d'allouer de la mémoire dans une zone
* qui existe pendant toute l'exécution du programme
* où vous pouvez demander de la mémoire quand vous en avez besoin
* l'utiliser tout au long d'un code avec des appels de fonctions
* et libérer cette mémoire quand vous n'en avez plus besoin

<br>

c'est le rôle de la **mémoire dynamique** et de ses deux opérateurs `c++` `new` et `delete`  
et donc `new []` et `delete []` quand on veut des tableaux

</div>


## mémoire dynamique ou `tas`


<div class="framed-cell">
    <ins class="underlined-title">allocation et desallocation dans la mémoire dynamique</ins>

<br>

l'allocation d'un objet, dans la mémoire dynamique, est décidée par le programmeur  
quand son code nécessite la définition d'un objet dans une zone mémoire  
qui existera tant qu'il a besoin de l'objet
  
<br>

l'opérateur `new` réserve un morceau de mémoire dans la mémoire dynamique (encore appelé le `tas` ou `heap`)  
pour y stocker un objet  
il vous renvoie l'adresse de la zone  
que vous mettez dans une variable afin de pouvoir l'atteindre

</div>


## allocation d'un entier dans le tas

<!-- #region -->
<div class="framed-cell">
    <ins class="underlined-title">allocation d'un entier dans la mémoire dynamique</ins>

<br>

on met un entier dans la mémoire dynamique  
pour cela `c++` va y réserver une zone mémoire taille de `sizeof(int)` octets  
et l'initialiser, ici,  avec la valeur `12`  
on peut atteindre l'objet avec `*pi`


<br>

```c++

int main () {
   int* pi = new int(12);
   *pi = *pi + 1;
   return 0;
}
```
</div>
<!-- #endregion -->

## allocation d'un tableau dans le tas

<!-- #region -->
<div class="framed-cell">
    <ins class="underlined-title">allocation d'un tableau d'entiers dans la mémoire dynamique</ins>

<br>

`new []` est la même chose pour les tableaux

<br>

là on met `size` entiers dans la mémoire dynamique (remarquez les `[` `]` autour de `size`)  
donc on réserve une taille de `sizeof(int)*size` octets  
et on l'initialise avec les valeur `{10, 20, 30, 40, 50, 60, 70, }`  
on peut atteindre l'objet avec `*pi`

```c++
#include <iostream>
int main () {
  int size = 7;
  int* tab = new int[size] {10, 20, 30, 40, 50, 60, 70};
  for (int i = 0; i < size; i++) {
    std::cout << tab[i] << std::endl;
  }
  return 0;
}

```
</div>
<!-- #endregion -->

## libération de la mémoire dynamique


L'opérateur `delete` appliqué à l'adresse d'une zone mémoire allouée dans le `tas` libère cette zone  
si la zone est un tableau il faut le dire à `c++` par un `delete []`


## libération d'un entier du tas

<!-- #region -->
<div class="framed-cell">
    <ins class="underlined-title">libération de la mémoire allouée pour un objet</ins>

<br>

on a mis un entier dans la mémoire dynamique  

pour libérer cette zone on va appliquer l'opérateur `delete` à l'adresse de la zone  
(l'adresse que nous a retourné `new`)


<br>

```c++

int main () {
   int* pi = new int(12);
   *pi = *pi + 1;
   delete pi;
   return 0;
}
```

<br>

remarquez que la zone libérée est toujours accessible à travers l'objet `pi` qui est l'adresse d'un entier...

<br>

que se passe-t-il si vous essayer d'accéder à l'objet pointé par cette adresse ?  
oui une segmentation-fault

</div>
<!-- #endregion -->

## libération d'un tableau du tas

<!-- #region -->
<div class="framed-cell">
    <ins class="underlined-title">libération de la mémoire allouée pour un tableau</ins>

<br>

on a mis un tableau d'entiers dans la mémoire dynamique  

pour libérer cette zone on va appliquer l'opérateur `delete[]` à l'adresse de la zone  
(l'adresse que nous a retourné `new[]`)


<br>

```c++

int main () {
   int* tab = new int[7];
   delete [] tab;
   return 0;
}
```

<br>

de même vous remarquez que la zone libérée est toujours accessible à travers le tableau `tab`  
(qui est l'adresse du premier entier d'un tableau d'entier)

<br>

que se passe-t-il si vous essayer d'accéder aux objets dans ce tableau ?  
oui une segmentation-fault  
parce que la zone a été libérée par `c++`

</div>
<!-- #endregion -->

## fonctionnement de l'allocation dynamique de mémoire

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title"> fonctionnement de l'allocation dynamique de mémoire</ins>

<br>

lorsque l'allocation est invoquée par un `new`
* `c++` (grâce à un algorithme intelligent) recherche un bloc mémoire qui correspond à la taille demandée
* quand il l'a trouvé, il le marque comme réservé
* et il renvoie l'adresse de l'emplacement sous la forme d'un pointeur du type demandé
  
<br>

l'allocation dynamique est une opération chère !
* préférez mettre des objets dans la pile quand c'est possible
* et sinon, préférez utiliser les conteneurs de la librairie standard 


</div>
<!-- #endregion -->

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">un autre exemple `c++`</ins>
  
  
<br>
  
prenons un exemple `c++` 


```c++
int i;                  
int main () {           
  int j;               
  int k = 12;
  char* pc = new char; 
  *pc = 'y';
  delete pc;           
  return 0;            
} 
```

<br>
    
dans ce code, on définit quatre variables
* `i` une variables globale initialisée, par défaut, à `0`
* `j` une variable locale au `main` non initialisée dont la valeur restera indéterminée
* `k` une variable locale du `main` bien initialisée
* `pc` l'adresse d'un `char`

<br>

Où se retrouvent toutes ces données ?
* `i` est un `int` alloué dans le `segment statique` de données
* `j` est un `int` alloué dans la `pile d'exécution`
* `k` est un `int` alloué dans la `pile d'exécution`
* `new char` on a demandé au `tas` d'allouer un caractère `mémoire dynamique`
* `pc` est un `char*` (pointeur vers un caractère) alloué dans la `pile d'éxécution`  
* l'objet pointé par `pc` est affecté avec la valeur `'y'` 
* la zone en mémoire dynamique est libérée

```
      Les segments de mémoire
    + ======================= +
    | int i;                  |
    | int main () {           |
    |    int j;               |
    |    int k = 12;          | segment de texte
    |    char* pc = new char; |
    |    *pc = 'y';           | <- you are here
    |    delete pc;           |
    |    return 0;            |
    | }                       |
    + ======================= +
    |         i: 0            | segment de données
    + ======================= +
----|         pc              | segment de pile
|   |         k: 12           | d'exécution
|   |         j:?             |
|   + ======================= +
|   |                         | | augmentation de la pile
|   |                         | v
|   |                         |
|   |                         |  free zone 
|   |                         |
|   |                         | ^
|   |                         | | augmentation du tas
|   + ======================= +
--->|         'y'             | segment pour la mémoire
    |-------------------------| dynamique (tas)
    |                         |
    + ======================= +
```
</div>
<!-- #endregion -->

## ne pas les mélanger !


<div class="framed-cell">
    <ins class="underlined-title">ne pas mélanger `new` et `delete []` et `new[]` et `delete`</ins>

<br>

si vous allouez une zone avec un `new` vous devez la libérer avec `delete`

<br>

si vous allouez une zone avec un `new []` vous devez la libérer avec `delete []`

<br>

si vous faites autrement, le comportement de votre programme sera indéterminé  
donc ce n'est pas bien du tout

</div>


## ces opérateurs en `c`


<div class="framed-cell">
    <ins class="underlined-title">dans le langage `c`</ins>

<br>

dans le langage `c`
* ces fonctions d'allocation et de libération de la mémoire dynamique sont `malloc` et `free`
* `malloc` et `free` peuvent être utilisées dans un programme `c++` mais préférez naturellement `new` et `delete`

<br>

notons enfin que ces fonctions ne s'intervertissent pas:
* vous ne devez pas libérer avec `free` une zone allouée par `new`
* ni libérer avec `delete` une zone allouée par `malloc` 

</div>


</div>



## exercice

<!-- #region -->
Dans votre code de la calculette en notation polonaise inversée, ajouter une fonction qui alloue le tableau de la pile d'opérandes en mémoire dynamique et une fonction qui libère la mémoire dynamique occupée par ce tableau.

Ajoutez une fonction qui affiche la pile d'opérande avant les opérations  
de la manière suivante:

```c++
$ ./calc 5 6 x 8 2 ! / +
[[
[5 [
[5 6 [
[30 [
[30 8 [
[30 8 2 [
[30 8 -2 [
[30 -4 [
26
```
<!-- #endregion -->

Pour comprendre un peu plus les mécanismes, vous pourrez lire la suite de ce notebook lorsque vous en aurez le temps


## (optionnel) mémoire adressable par un programme

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">la mémoire qu'un programme peut adresser(1)</ins>

<br>

un programme exécuté par un système d'exploitation a son propre espace d'adressage  
i.e. sa propre mémoire
  
  

<br>

votre programme voit cet espace mémoire comme un **bloc continu** de mémoire
  

<br>

la taille de la mémoire que votre programme peut voir est le nombre d'adresses auxquelles vous pouvez accéder

<br>

ainsi, sur un processeur 64 bits, comme le type `*` pointeur (la valeur d'une adresse) va stocker ses objets sur 64 bits  
on peut dire (très approximativement) que votre programme peut adresser près de $2^{64}$ octets de mémoire vive (la `RAM`)
  
<br>

c'est beaucoup (combien ?) ! et votre `RAM` naturellement ne peut pas vous proposer toute cette mémoire 

</div>

----------
    
(1) http://www.cprogramming.com/tutorial/virtual_memory_and_heaps.html

<!-- #endregion -->

## la mémoire virtuelle

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">la mémoire virtuelle</ins>
    
<br>
    
c'est l'espace d'adressage de vos process  
un process (processus) est un programme en cours d'exécution, par le système d'exploitation, sur le processeur  
c'est au tour de ce process de profiter du processeur pour un tout petit laps de temps

<br>

votre process peut accéder à plus de mémoire `RAM` que votre l'ordinateur a physiquement  
pour cela il va utiliser un mécanisme d'*adressage virtuel*
  
<br>

dans ce mécanisme, une partie de la mémoire du disque dur de votre ordinateur va être utilisé pour stocker des objets qui devraient être en mémoire `RAM` si elle était suffisante


<br>

le processus ne sait pas si l'adresse est physiquement stockée dans la `RAM` ou sur le disque dur
  
  

<br>

le système d'exploitation maintient naturellement une table de correspondance entre les adresses virtuelles, auxquelles votre programme veut accéder, et les adresses physiques correspondantes
  

</div>
<!-- #endregion -->

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">organisation de la mémoire virtuelle</ins>

<br>

la mémoire virtuelle ( qui est donc l'espace d'adressage d'un processus) est généralement organisée en plusieurs **segments**
  
<br>

* un segment de **texte** où le **code** de votre programme (dans sa version compréhensible par le processeur) est conservé
  
<br>

* un segment de données où les variables statiques globales sont conservées  
(leur taille étant connue dès la compilation par votre exécutable même sans être exécuté en connaît la taille) 
 
<br>

* la pile d'exécution utilisée pour stocker les arguments des fonctions, leur valeur de retour et leurs variables locales (dites automatiques)


<br>

le `tas` utilisé pour les allocations dynamiques 
  
<br>

ainsi les objets en `c++` peuvent être stockés dans trois zones mémoire différentes:
* la zone de *texte*
* la pile d'exécution **pile**
* le tas
  
</div>

<!-- #endregion -->

## les segments de mémoire


<div class="framed-cell">
<ins class="underlined-title">  les segments (très schématiquement)</ins>

```
+-----------+ ^
|   zone    | |
| de texte  | |
+-----------+ | MÉMOIRE
|   zone    | | STATIQUE
|des données| |
|  globales | |
+-----------+ v
|           |
|   pile    | |
|d'exécution| | (growth)
|   (pile)  | v
+-----------+ 
|   zone    |
|  mémoire  |
|   libre   |
+-----------+ ^
|           | | 
|    tas    | | (growth)
|   (heap)  | |
+-----------+

```

<br>

la taille du segment (zone) de texte est déterminée à la compilation
      
<br>

la taille du segment de données est établie à la compilation

<br>

la taille du segment pour la pile d'exécution grandit et rétrécit pendant l'exécution du programme

<br>

la taille du segment pour le tas augmente et rétrécit pendant l'exécution du programme 
      
</div>




  
  


## lorsque vous faites quelque chose de mal

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title"> Lorsque vous faites quelque chose de mal</ins>

Prenons un exemple qui va mal tourner:

<br>

```c++
// dans le fichier bad.cpp
int main () {
  float* pf;
  *pf = 17.3;
  return 0;
}
```
    
```bash
$ g++ bad.cpp
$ ./a.out
Segmentation fault (core dumped)
```

<br>

Que se passe-t-il lorsqu'un programme tente d'accéder à une zone mémoire à laquelle il n'est pas autorisé d'accéder ?


<br>

vous obtenez un `erreur de segmentation`

<br>

votre programme se termine d'une manière tragique: il est tué par le système d'exploitation (il l'a bien cherché !)

</div>
<!-- #endregion -->
