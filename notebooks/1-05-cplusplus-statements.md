---
jupytext:
  formats: md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: '0.9'
    jupytext_version: 1.5.2
kernelspec:
  display_name: Python 3
  language: python
  name: python3
---

<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12-MINES-ParisTech</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>
</div>

```{code-cell} ipython3
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# les instructions du langage `c++`

+++

(les instruction `c++` se terminent par un `;`)

+++

## variables locales et globales

+++

<div class="framed-cell">
<ins class="underlined-title"> variables locales et variables globales </ins>

<br>

une variable est dite
* **locale** quand elle est  définie à l'intérieur d'une fonction  
* **globale** quand elle est définie en dehors de toute fonction

</div>

+++

## définition, initialisation et affectation des variables locales et globales

+++

<div class="framed-cell">
<ins class="underlined-title"> la définition des variables locales </ins>

<br>

lors de la définition d'une variable locale vous devez indiquer le type de la variable, ou utiliser `auto` et vous pouvez (*devriez*) lui indiquer une valeur initiale

```c++
int main () {
   int i = 10;
   bool b = true;
   return 0;
}
```

<br>

vous pouvez naturellement changer la valeur d'une variable locale après sa définition par une affectation


```c++
int main () {
   int i = 10;
   bool b = true;
   i = i + 1;
   b = false;
   return 0;
}
```
</div>

+++

<div class="framed-cell">
<ins class="underlined-title"> problème de la définition d'une variable locale sans initialisation </ins>

<br>

**premier problème**: vous pouvez ne pas initialiser une variable locale lors de sa définition (heu ... c'est pas top ca !)  

```c++
int main () {
   int i;
   i = i + 1;
   return 0;
}
```

<br>

Quelle est le problème de la *définition sans initialisation* d'une variable locale ?  
$\rightarrow$ vous vous donnez la possibilité d'utilisez une variable **sans** l'avoir initialisée  (et c'est mal !)

<br>


et `c++` ne verra rien    
puisqu'il sait parfaitement trouver une valeur pour une variable:
* il prend la mémoire allouée pour votre variable  
* il transforme les $0$ et les $1$ qu'il y trouve en une valeur du bon type


<br>

à vous les bugs et problèmes divers dans vos programmes... on verra plus tard que `valgring` pourra vous aider, dans la mesure des ses capacités, à détecter ce genre de problème
</div>

+++

<div class="framed-cell">
    
<ins class="underlined-title"> ce n'est pas parce que le compilateur fait quelque chose</ins>
* <ins class="underlined-title"> que tous les compilateurs feront la même chose !</ins>
* <ins class="underlined-title"> ou que lui même fera la même chose dans un autre contexte</ins>
* <ins class="underlined-title"> ou que c'est la chose à faire</ins>

<br>

compilateur $\neq$ norme du langage

<br>

**deuxième problème**:  
pour une variable locale à une fonction, la mémoire initiale pourra valoir $0$ par *hasard*  

```c++
#include <iostream>
int main () {
   int i;
   i = i + 1;
   std::cout << i << std::endl;
   return 0;
}
```

```bash
$ g++ bad.cpp -o bad
$ bad 
1  # c'est un pur hasard !!
```
<br>

c'est un pur hasard ! vous ne devez pas vous appuyer là dessus dans votre code  
* si une variable locale doit valoir $0$ il **faut** l'indiquer **explicitement**


```c++
#include <iostream>
int main () {
   int i = 0;
   i = i + 1;
   std::cout << i << std::endl;
   return 0;
}
```

```bash
$ g++ bad.cpp -o bad
$ bad 
1  # vous en êtes sûrs !
```
<br>
</div>

+++

<div class="framed-cell">
<ins class="underlined-title"> définition des constantes locales</ins>

<br>

vu que l'essence même d'une valeur constante est de ne pas pouvoir être modifiée  
vous devez impérativement indiquer la valeur de la constante lors de sa création

```c++
int main () {
   const int i = 10;
   const bool b = true;
   return 0;
}
```
<br>

si vous ne le faites pas, votre constante ne sera pas intialisée  
mais comme sa mémoire existe et contient une valeur  
votre constante aura cette valeur qui est **n'importe quelle valeur**
</div>

+++

<div class="framed-cell">
<ins class="underlined-title"> les variables globales sont initialisées par défaut à $0$</ins>

<br>

une variable globale est une variable définie en dehors de toute fonction

<br>

si vous ne lui donnez pas de valeur initiale `c++` l'initialise au zéro qui correspond au type de la variable  
(`'\0'` pour les `char`,  `0` pour les `int`, `false` pour les `bool`...) 

<br>

>```c++
>// in file good.cpp
>#include <iostream>
int i;
bool b;
int main () {
    i = i + 1;
    std::cout << b << std::endl;
   return 0;
}
```

>```bash
>$ g++ good.cpp -o good 
$ good
0
```
<br>

à la compilation 
* `c++` **connait** toutes les variables et les constantes globales$^{(2)}$  
* alors qu'il ne **connait pas** encore quelles fonctions seront appelées donc quelles variables locales il sera amené à créer  


<br>

ainsi les variables et constantes globales de `c++` sont stockées dans la **mémoire statique** des programmes    
comme le texte du programme

<br>

donc la norme `c++` vous assure que toutes les variables globales, par défaut d'initialisation, seront initialisées au zéro qui correspond à leur type

-----------------
(2) les constantes globales doivent être initialisées
</div>

+++

## les expressions

+++

<div class="framed-cell">
<ins class="underlined-title"> une instruction peut être une seule expression </ins>

<br>

```c++
int main () {
   10 + 2;
   return 0;
}
```
<br>

l'expression est évaluée et sa valeur est ignorée après le `;`

<br>

si un problème apparaît à l'évaluation d'une expression
* le résultat n'est pas défini mathématiquement  
* le résultat est en dehors de la plage des valeurs représentables pour son type
* ...

alors le comportement de votre programme **sera indéfini**
</div>

+++

## les blocs d'instructions

+++

<div class="framed-cell">
<ins class="underlined-title"> les blocs d'instructions </ins>

<br>

un bloc d'instruction est composée d'une instruction (ou d'une suite d'instructions terminées par des `;`) mise entre `{` et `}`

<br>

vous pouvez définir des blocs
  
```c++
int main () {
  { 10 + 2; }
  { 20 + 1; }
  return 0;
}
```
<br>

vous pouvez imbriquer des blocks

  
```c++
int main () {
  int i = 1; // définition de i
  {
    int j = i; // définition de j
    {
      int k = j; // définition de k
    }
  }
  return 0;
}
```
<br>

il est très important ici de comprendre qu'un identificateur existe
* à partir de sa définition
* jusqu'à la fin du bloc dans lequel il a été défini

<br>

ainsi:
```c++
int main () {
  int i = 12;
  {
    int j = i;
    {
      int k = j;
    } // k n'existe plus
  } // j n'existe plus
  return 0;
} // i n'existe plus
```
</div>

+++

<div class="framed-cell">
<ins class="underlined-title"> n'oubliez pas d'indenter votre code! </ins>

<br>

en `c++`, contrairement à `Python`, vous explicitez les blocs entre `{` et `}`  
en `Python` c'est avec l'indentation que vous pouvez le faire

<br>

**mais** en `c++` aussi vous devez indenter votre code afin qu'il soit **lisible** !!!

<br>

```c++
int main () {
int i = 12;
{
int j = i; 
{
int k = j; 
}
}
return 0;
}
```

vous y comprenez quelque chose ?  
on vous rassure: nous non plus !  
mais c'est pourtant ce genre de programme que vous allez nous montrer pour qu'on y trouve une erreur    

<br>

alors **indentez vos code** pour cela des **fonctionnalités orientées syntaxe** existent dans votre éditeur qui vous aideront beaucoup !
</div>

+++

## la mémoire pour stocker les variables locales

+++

<div class="framed-cell">
<ins class="underlined-title"> où sont stockées les variables locales ? </ins>

<br>
    
quand un programme s'exécute, le système d'exploitation lance un process:  

* il lui donne déjà un espace mémoire avec le code de son programme et ses variables globales  
(puisque les variables globales doivent être accessibles de tout le programme)  
appelons cette mémoire la `mémoire statique`

<br>

ensuite le programme commence à exécuter le `main` il rencontre des variables `i`, `j` et `k`, il doit bien les mettre quelque part ... pour comprendre comment il fait regardons le programme suivant

on réfléchit au type de mémoire dont on a besoin


<br>

```c++
int main () {
  int i = 1; // définition de i
  {
    int j = i; // définition de j
    {
      int k = j; // définition de k
    } // k n'existe plus
  } // j n'existe plus
  return 0;
} // i n'existe plus
```


quel est le comportement de cette mémoire:

<br>


```
|     |
|     |
|     |
|     |<- haut de mémoire
-------
```

on définit `i`

```
|     |
|     |<- haut de mémoire
|-----|
|  i  |
-------
```

on définit `j` dans la mémoire on a les deux variables `i` et `j`


```
|     |<- haut de mémoire 
|-----|
|  j  |
|-----|
|  i  |
-------
```

on définit `k` dans la mémoire on a les trois variables `i`, `j` et `k`

```
|     |<- haut de mémoire 
|-----|
|  k  |
|-----|
|  j  |
|-----|
|  i  |
-------
```

on sort du bloc contenant `k` 

```
|     |<- haut de mémoire 
|-----|
|  j  |
|-----|
|  i  |
-------
```

on sort du `main`

```
|     |
|     |<- haut de mémoire
|-----|
|  i  |
-------
```

on sort du bloc contenant `k`

```

|     |
|     |
|     |
|     |<- haut de mémoire
-------
```

<br>


cela ressemble au comportement d'une pile et oui cette zone mémoire est appelée la **pile d'exécution**
</div>

+++

## l'instruction conditionnelle `if`

+++

<div class="framed-cell">
<ins class="underlined-title"> l'instruction conditionnelle `if`</ins>
    

<br>

```c++
if (condition)
    instruction-1
```

<br>

```c++
if (condition)
    instruction-1
else
    instruction-2
```

<br>


```c++
if (condition1)
    instruction-1
else if (condition2) 
    instruction-2
else
    instruction-3
```

<br>

```c++
#include<iostream>
int main () {
  int i = 0;
  std::cout << "entrez un entier: ";
  std::cin >> i;
  if (i == 0)
    std::cout << "je suis nul\n";
  else if (i%2 == 0) 
    std::cout << "je suis pair\n";
  else 
    std::cout << "je suis impair\n";
  return 0;
}

```

<br>

si vous vouliez mettre plus d'une instruction dans une des branches ?  
Que pensez-vous de ce (mauvais) code ?

```c++
// dans le fichier mauvais-code.cpp
#include<iostream>
int main () {
  int i = 0;
  std::cout << "entrez un entier: ";
  std::cin >> i;
  if (i%2 == 0) 
    std::cout << "je suis pair\n";
  else 
    std::cout << i << std::endl;
    std::cout << "je suis impair\n";
  return 0;
}
```

<br>

compilons-le et exécutons le:

```bash
$ g++ mauvais-code.cpp
$ ./a.out 
entrez un entier: 2
je suis pair
je suis impair
```

et bien oui ! la branche `else` n'est constituée que le premier affichage 

+++

## l'instruction  `switch`

+++

<div class="framed-cell">
<ins class="underlined-title"> l'instruction `switch`</ins>
    
```
switch (c) {
  case v0:
    instruction0;
  case v1:
    instruction1;
  case v2:
    instruction2;
  default:
    instruction3;
}    
<br>
```

`c++` compare la valeur de `c` avec toutes les valeurs dans les cas `v0` puis `v1` puis `v2`
* si aucune comparaison ne réussit il exécute l'instruction `default`
* si une comparaison réussit, il exécute l'instruction (ou le bloc d'instruction) correspondant ...
* et il exécute aussi toutes les instructions des cas suivants !

<br>

donnons un exemple
```c++
// dans le fichier switch.cpp
#include<iostream>
int main () {
  char c = '\0';
  std::cout << "entrez un caractère: ";
  std::cin >> c;
  switch (c) {
  case 'a':
    std::cout << "je suis 'a'\n";
  case 'b':
    std::cout << "je suis 'b'\n";
  case 'c':
    std::cout << "je suis 'c'\n";
  default:
    std::cout << "défaut\n";
  }    
  return 0;
}
```


```bash
$ g++ switch.cpp
entrez un caractère: a
je suis 'a'
je suis 'b'
je suis 'c'
défaut
```

<br>

pour éviter ce comportement, il faut interrompre le `switch` avec l'opérateur `break` 

<br>


```c++
#include<iostream>
int main () {
  char c = '\0';
  std::cout << "entrez un caractère: ";
  std::cin >> c;
  switch (c) {
  case 'a':
    std::cout << "je suis 'a'\n";
    break;
  case 'b':
    std::cout << "je suis 'b'\n";
    break;
  case 'c':
    std::cout << "je suis 'c'\n";
    break;
  default:
    std::cout << "défaut\n";
  }    
  return 0;
}

```

```bash
$ g++ swich-break.cpp
$ a.out
entrez un caractère: a
je suis 'a'
```

<br>


remarquez que nous n'avons pas besoin de blocs `{` `}` dans les instructions des `case`

+++

## les instruction d'itération `while`

+++

<div class="framed-cell">
<ins class="underlined-title"> les instructions d'itération </ins>

```
while (condition)
   instruction
```
   
<br>

donnons un exemple 
```c++
#include<iostream>
int main () {
  char c = '\0'; // j'initialise une variable
                 // de type char avec le caractère null
  
  while (c != 'q') { // tant que ce caractère n'est pas 'q'
    std::cin >> c;   // je lis un caractère
    std::cout << c << std::endl; // je l'affiche
  }
  return 0;
}
```

<br>

on remarque dans cet exemple que le permier test de la condition est inutile  
puisque `c` vaut `'\0'` et non pas `'q'`

<br>

on peut utiliser la forme `do` `while` à la place

<br>

```c++
#include<iostream>
int main () {
  char c = '\0'; // j'initialise une variable
                 // de type char avec le caractère null
  do {
    std::cin >> c;   // je lis un caractère
    std::cout << c << std::endl; // je l'affiche
  } while (c != 'q'); // j'itére de nouveau si lfe caractère n'est pas 'q'
  return 0;
}
```

+++

## l'instruction `for` 

+++

<div class="framed-cell">
<ins class="underlined-title"> l'instruction `for` </ins>

c'est une instruction qui itére sur un index, on a trois formes:

* ```
for (initialisation; condition de continuation; expression)
   instruction
```


* ```
for (; condition de continuation; expression)
   instruction
```


* ```
for (;;)
   instruction
```

<br>

tant que la condition de continuation est vraie, l'itération continue  
l'expression modifie la valeur de l'index

<br>

```c++
#include<iostream>
int main () {
  int tab [] {1, 2, 3, 4, 5, 6};
  for (int i = 0; i < 6; i++)
    std::cout << tab[i] << std::endl;
  return 0;
}
```

<br>

depuis `c++11` il existe un `for-range` qui s'applique à un objet itérable  
il parcourt ses eléments à la suite


```c++
#include<iostream>
int main () {
  int tab [] {1, 2, 3, 4, 5, 6};
  for (int e : tab)
    std::cout << e << std::endl;
  return 0;
}
```

l'utiliser avec un `auto` est encore mieux !

```c++
#include<iostream>
int main () {
  int tab [] {1, 2, 3, 4, 5, 6};
  for (auto e : tab)
    std::cout << e << std::endl;
  return 0;
}
```


<br>

```c++
#include<iostream>
int main () {
  int tab [] {1, 2, 3, 4, 5, 6};
  int i = 0; // on peut sortir l'initialisation
  for (; i < 6; i++)
    std::cout << tab[i] << std::endl;
  return 0;
}
```

<br>

```c++
#include<iostream>
int main () {
  char c = '\0';
  for (;;) {
    std::cin >> c;
    std::cout << c << std::endl;
  }
  return 0;
}
```

on remarque que l'exécution de ce dernier programme ne termine jamais ...  
dans votre terminal il faut tuer le process qui tourne avec un `ctrl-c`

<br>
