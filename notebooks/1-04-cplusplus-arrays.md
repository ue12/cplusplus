---
jupyter:
  jupytext:
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
      jupytext_version: 1.5.2
  kernelspec:
    display_name: Python 3
    language: python
    name: python3
---

<!-- #region tags=[] -->
<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12-MINES-ParisTech</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>
</div>
<!-- #endregion -->



























```python
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# les tableaux de bas-niveau *à-la-c* en `c++`

Nous allons dans une première partie de ce notebook montrer en `c++` la manière de créer les tableaux les plus proches de la mémoire; ce sont des tableaux qui existent déjà dans le langage `c`.

Il est intéressant, quand on apprend le langage `c++`, de savoir créer et manipuler ces tableaux *très proches de la mémoire*. **Mais**, lorsque vous aurez besoin de stocker des éléments dans vos programmes, vous utiliserez la plupart du temps, un type de tableaux de haut niveau de la librairie standard. Ce type est `std::vector` nous y reviendrons très rapidement dans la seconde partie de ce notebnook. 


## création d'un tableau en `c++`


### *slides*

<!-- #region -->

<div class="framed-cell">

<ins class="underlined-title"> on crée un tableau en `c++` puis on y met des éléments </ins>

<br>

un tableau est une suite d'élément qu'on va accéder par un index `tab[i]`

<br> 

on va créer un tableau comme une zone mémoire d'une taille fixée dès la création du tableau  
et initialiser ses éléments un à un

<br>


```c++
int main () {
  int tab [3];
  tab [0] = 42;
  tab [1] = 17;
  tab [2] = 81;
  return 0;
}
```

<br>

dans la **définition** de la variable `tab`
* vous remarquez `[3]` qui indique à `c++` qu'il doit créer un tableau `[]` pour stocker `3` entiers  
* vous remarquez qu'on indique le type des éléments (ici on a choisi `int`)
</div>
<!-- #endregion -->

<div class="framed-cell">

<ins class="underlined-title">On accède aux éléments de notre tableau `tab`</ins>

<br>

la syntaxe est très directe: pour atteindre le $i^{ème}$ élément du tableau `tab`, on fait `tab[i]`  
(notons que `i` doit être ici entre $0$ et $2$)

<br>

on va afficher le premier et le dernier élément du tableau

>```c++
// in file tab.cpp
>#include<iostream>
>int main () {
  int tab [3];
  tab [0] = 42;
  tab [1] = 17;
  tab [2] = 81;
  std::cout << tab[0] << ' ' << tab[2] << std::endl;
  return 0;
}
```

<br>

on compile et on exécute

>```bash
>$ g++ tab.cpp
$ a.out
42 81
```
</div>

<!-- #region -->
<div class="framed-cell">

<ins class="underlined-title"> on initialise un tableau lors de sa création </ins>

on va créer un tableau sans indiquer une taille  
mais en donnant la liste des éléments

>```c++
// in file tab.cpp
>int main () {
>  int tab [] {17, 23, -3, 41, -9, 75};
  return 0;
}
```

<br>

vous remarquez deux sous-parties dans l'expression

>```c++
>int tab [] {17, 23, -3, 41, -9, 75};
```
$\rightarrow$ à gauche la **définition** de la variable `tab` dans `int tab []`  
$\rightarrow$ à droite, l'initialisation de cette variable `tab`, avec une liste d'éléments 

<br>

dans l'initialisation, on a indiqué les éléments les uns après les autres dans une liste i.e entre `{` et `}`
* les indices des éléments iront de $0$ pour $17$ à l'indice $5$ pour $75$  
* les indices vont de $0$ au nombre d'éléments moins $1$, donc ici $6-1$

<br>

vous pouvez aussi indiquer la taille


>```c++
// in file tab.cpp
>int main () {
>  int tab [6] {17, 23, -3, 41, -9, 75};
  return 0;
}
```

<br>

et si vous fournissez moins d'éléments que la taille indiquée, les éléments restants seront initialisés au $0$ du type  
`0` pour les `int`, `\0` pour les `char`, `false` pour les booléens...

>```c++
>// in file tab.cpp
>#include <iostream>
int main () {
  int tab [9] {17, 23, -3, 41, -9, 75};
  std::cout << tab[6] << std::endl;
  std::cout << tab[7] << std::endl;
  std::cout << tab[8] << std::endl;
  return 0;
}
```

```bash

g++ tab.cpp ; a.out
0
0
0
```
</div>
<!-- #endregion -->

## organisation du tableau en mémoire


### *slides*

<!-- #region tags=[] -->
<div class="framed-cell">

<ins class="underlined-title">Comment est organisé un tableau en mémoire ? </ins>


<br>

un tableau dans la mémoire de `c++` est une **suite contiguë** d'objets de **même type**

<br>
    
ainsi votre tableau `tab`, qui contient **6 entiers**, est $\fbox{17}\fbox{23}\fbox{-3}\fbox{41}\fbox{-9}\fbox{75}$

<br>

(ca vous rappelle quelque chose ? et bien oui les `numpy.ndarray` !)

<br>

tous les éléments d'un tableau ont le **même type** $\Rightarrow$ toutes les cases du tableau sont de la **même taille** en mémoire

<br>


on peut dessiner les **cases mémoires** comme cela  
la valeur de la première est `tab[0]`, de la seconde `tab[1]`...

```
|----------------|----------------|----------------|----------------|----------------|----------------|
|       17       |       23       |       -3       |       41       |       -9       |       75       |
|----------------|----------------|----------------|----------------|----------------|----------------|
      tab[0]           tab[1]           tab[2]           tab[3]           tab[4]            tab[5]
```

<br>

disons aussi que la variable `tab` est en fait l'adresse en mémoire du premier élément du tableau  
essayez d'afficher `tab`


<br>

ainsi si je veux créer une variable `tab1` qui désigne le même tableau que `tab`  
je vais créer une variable de type `pointeur sur un int` et l'initialiser avec `tab`



```c++
#include<iostream>
int main () {
  int tab [3];
  tab [0] = 42;
  tab [1] = 17;
  tab [2] = 81;

  int *tab1 = tab;
  std::cout << tab1[0] << std::endl; 
  std::cout << tab1[1] << std::endl; 
  std::cout << tab1[2] << std::endl; 
  return 0;
}
```

```bash
$ g++ tab.cpp
$ a.out
42
17
81
```

<br>

si vous appliquez la fonction `sizeof` sur un tableau vous obtenez le nombre d'octets total du tableau ainsi si vous divesez pas la taille en octet des éléments vous obtenez le nombre d'éléments du tableau

```c++
#include <iostream>
int main () {
  int tab [9] {17, 23, -3, 41, -9, 75};
  std::cout << sizeof(tab) << std::endl;
  std::cout << sizeof(tab)/sizeof(int) << std::endl;
  return 0;
}
```

```bash
$ g++ tab.cpp
$ a.out
36
9  # mes ints sont sur 4 octets
```

<br>

pour aller (un peu) plus loin dans la compréhension de l'organisation d'un tableau en mémoire vous regarderez l'annexe `2.1  organisation de la mémoire des tableaux de bas niveau`.
</div>
<!-- #endregion -->

## les chaînes de caractères


### *slides*

<!-- #region -->
<div class="framed-cell">

<ins class="underlined-title"> les chaînes de caractères </ins>

<br>

les chaînes de caractères comme `"Hello World !"` en `c++`  
sont stockées en mémoire comme des tableaux de caractères terminés par le caractère `nul` du type `char` (ou `\0`)  

<br>

le caractère de fin de chaîne sert naturellement à arrêter le parcours de la chaîne de caractères (par exemple lors d'un affichage)

<br>


<br>

ainsi `"HELLO"` devient en mémoire le tableau $\fbox{H}\fbox{E}\fbox{L}\fbox{L}\fbox{O}\fbox{\0}$  qui comporte donc un caractère de plus que la chaîne d'origine

<br>

montrons un exemple

```c++
// in file str.cpp
#include <iostream>
int main () {
  char str1 [] = "HELLO"; // naturellement pas besoin de mettre le '\0' en fin de chaîne !
  char str2 [] = {'H', 'E', 'L', 'L', 'O', '\0'}; // là on doit le mettre
  str1[0] ='W';
  str2[0] ='Y';
  std::cout << str1 << std::endl;
  std::cout << str2 << std::endl;
  return 0;
}
```

```bash
$ g++ str.cpp
$ a.out
WELLO
YELLO

```
on remarque que les variables `str` et `str2` peuvent être modifiées

<br>

comme pour les tableaux d'`int` le type d'un tableau de `char` est l'adresse du premier caractère de la chaîne de caractères  
ainsi nous pouvons faire

```c++
int main () {
  char *str1 = "hello";
  char str2 [] = "coucou";
  return 0;
}
```

Quelle est la différence ?  
C'est à dire en quoi l'utilisation de `str1` et celle de `str2` sont-elles différentes ?  

<br>

en fait une chaîne comme `"hello"` est en `c++` un tableau (en mémoire statique) qui est constant c'est à dire que vous ne pouvez pas en modifier les caractères

<br>

dans notre exemple
* `str1` n'est pas un nouveau tableau mais c'est l'adresse d'un tableau existant constant  
donc vous ne pourrez pas modifier le contenu de `str1`  
* `str2` est une *copie* de la chaîne `coucou`, le tableau a été créé et initialisé  
donc vous pouvez modifier le contenu de `str2`

</div>
<!-- #endregion -->

## les tableaux de haut niveau `std::vector`

<!-- #region -->
Les tableaux que nous venons de voir ne sont utiles que pour des séquences de taille fixe, d'objets d'un type donné. C'est une possibilité de très bas niveau et il faut préférer des solutions de haut niveau comme les `std::vector`. Sachant que l'implémentation de ces types de haut niveau se fondent sur les tableaux de bas niveau. 

Très rapidement, la meilleure manière pour apprendre à utiliser ces types de haut niveau de la librairie standard, est de lire la documentation www.cplusplus.com/reference/vector/vector ou https://en.cppreference.com/w/cpp/container/vector ou de comprendre des exemples d'Internet. Il n'y a pas d'intérêt à utiliser du temps en présentiel dans un cours pour apprendre des listes de fonctions.

Nous nous bornons ici à montrer que ce type existe. Pour l'utiliser dans un programme il faut inclure le fichier `vector` de la librairie standard.


```c++
#include <iostream>
#include <vector>

int main () {
  std::vector<int> vec {17, 23, -3, 41, -9, 75};
  std::cout << vec[0];
  return 0;
}
```
<!-- #endregion -->




<!-- #region tags=["level_intermediate"] -->
# annexe
<!-- #endregion -->

<!-- #region tags=["level_intermediate"] -->
##  organisation de la mémoire des tableaux de bas niveau
<!-- #endregion -->

<!-- #region tags=["level_intermediate"] -->
### *slides*
<!-- #endregion -->

<!-- #region tags=["level_intermediate"] -->

<div class="framed-cell">
<ins class="underlined-title"> Pour aller plus loin: </ins>

reprenons notre exemple du tableau `int tab [] = {17, 23, -3, 41, -9, 75};`
<br>

toutes les cases ont la **même taille**, c'est à dire la taille d'un `int`  
qu'on obtient par `sizeof(int)`  
on peut ajouter à notre dessin

```
|----------------|----------------|----------------|----------------|----------------|----------------|
|       17       |       23       |       -3       |       41       |       -9       |       75       |
|----------------|----------------|----------------|----------------|----------------|----------------|
 <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)->

```

<br>

toutes ces cases ont naturellement une **adresse** en mémoire  
dont la première
```
|----------------|----------------|----------------|----------------|----------------|----------------|
|       17       |       23       |       -3       |       41       |       -9       |       75       |
|----------------|----------------|----------------|----------------|----------------|----------------|
 <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)->
^
|
addresse de la première case mémoire
```

<br>

comme toutes les cases ont la même taille mémoire  
si on connait l'adresse de la première case...  
on peut donc en **déduire les adresses des autres cases** 

```
|----------------|----------------|----------------|----------------|----------------|----------------|
|       17       |       23       |       -3       |       41       |       -9       |       75       |
|----------------|----------------|----------------|----------------|----------------|----------------|
 <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)-> <- sizeof(int)->
^                ^                ^ 
|                |                | 
|                |                (addresse de la première case mémoire) déclalée de 2 fois sizeof(int)
|                | 
|                (addresse de la première case mémoire) décalée de sizeof(int)
|
(addresse de la première case mémoire)
```


<br>

donc en fait en `c++` il suffit de connaître l'adresse de la première case du tableau  
pour accéder toutes les autres cases par un **décalage** ou **offset**  
(remarquez comment l'indice de l'élément intervient dans le décalage:  
$\rightarrow$ on décale de $0$ pour la première case, de $1$ pour la seconde, de $2$ pour la troisième...)

<br>

Où est stockée l'adresse d'un tableau ? Et bien dans sa variable: ici dans `tab` !  
donc la variable du tableau, ici `tab` a, comme valeur, l'adresse du premier élément du tableau  

<br>

et comme `c++` connait le type des éléments du tableau (il est indiqué dans le type de `tab` ici `int`)  
il va nous donner une manière beaucoup plus simple de passer d'un élément à un autre  
sans avoir à préciser le type ou sa taille

<br>

si je prends l'adresse du premier élément du tableau `tab` et que je lui ajoute `1`...  
on arrive à la seconde case du tableau

<br>

Allons-y, reprenons notre fichier contenant le tableau `tab`  
`tab` est une adresse, affichons-la

```c++
#include<iostream>
int main () {
  int tab [] = {17, 23, -3, 41, -9, 75};
  std::cout << tab << std::endl;
  return 0;
}```

```bash
$ g++ tab.cpp
$ a.out
0x7fff532cf220
```

<br>

Comment accéder à l'objet qui se trouve à cette adresse ?  
Rappelez-vous l'opérateur `*` (appliqué à une adresse, il donne l'objet à cette adresse - si il existe)  
donc oui en faisant `*tab`, faisons-le 

```c++
#include<iostream>
int main () {
  int tab [] = {17, 23, -3, 41, -9, 75};
  std::cout << *tab   << std::endl;
  std::cout << tab[0] << std::endl;
  return 0;
}
```

<br>

et si j'ajoute `1` à `tab`, j'obtiens l'adresse du deuxième élément du tableau  
et pour accéder à cet élément ? oui j'utilise `*`  
```c++
#include<iostream>
int main () {
  int tab [] = {17, 23, -3, 41, -9, 75};
  std::cout << *(tab+1) << std::endl;
  std::cout << tab[1]   << std::endl;
  return 0;
}
```
</div>
<!-- #endregion -->
