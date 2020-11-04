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

# destructeur de classe

+++

## `new`, `delete` sur les classes

+++

<div class = "framed-cell">
<ins class = "underlined-title">new et delete sur les classes</ins>

<br>

avec `new` on peut allouer, dans le tas, des objets des classes (que nous avons préalablement définies)  
avec `delete` on desalloue ces mêmes objets

<br>

notons que les constructeurs des objets sont appelés  
il faut leur passer leurs arguments

<br>

exemple avec la classe `Integer`

```c++
// dans le fichier integer.cpp
class Integer {
public:
  Integer (int v = 0) : value(v) {
  }
private:
  int value;
};
int main () {
  Integer* pi = new Integer(12);
  delete pi;
  return 0;
}
```

<br>

`new Integer` alloue dans le tas (heap, mémoire dynamique) assez de mémoire pour stocker un objet de type `Integer` i.e. `sizeof(Integer)`

<br>

si l'allocation réussit, le constructeur est appelé pour initialiser l'espace de stockage

<br>

`delete pi` libère la mémoire allouée

<br>

notons qu'après le `delete pi` le pointeur `pi` pointe vers une zone des-allouée  
y aller vous coûtera un `segmentation-fault`
</div>

+++

## `new[]`, `delete[]` sur les classes

+++

<div class = "framed-cell">
<ins class = "underlined-title">new[] et delete[] sur les classes</ins>

<br>

avec `new[]` on peut allouer, dans le tas, un tableau d'objets de classes (préalablement définies)     
avec `delete[]` on desalloue ce tableau

<br>

notons que les constructeurs des objets du tableau sont appelés donc il faut leur passer leurs arguments  
remarquez la manière d'utiliser la liste d'initialisation des tableaux

<br>

exemple avec la classe `Integer`

```c++
// dans le fichier integer-tab.cpp
#include <iostream>
class Integer {
public:
  Integer (int v = 0) : value(v) {
  }
private:    
  int value;
};
int main () {
  Integer* tab = new Integer[4] {12, 56};
  // un tableau de 4 objets de type Integer est créé
  // les attributs `value` des objets sont 12, 56, 0 et 0
  delete [] tab;
  return 0;
}
```

<br>

`new Integer [4]` alloue dans le tas (heap, mémoire dynamique) assez de mémoire pour stocker 4 objets de type `Integer` (les zones mémoires des objets sont contigus en mémoire)     
(*si l'allocation réussit*) les constructeurs sont appelés pour initialiser les 4 objets

<br>

`delete [] tab` libère le tableau allouée
         

<br>
</div>

+++

## rappel de la classe *pile d'entiers*

+++

<div class = "framed-cell">
<ins class = "underlined-title">la classe *pile d'entiers*</ins>

<br>

voici le fichier d'entête `intstack.h` qui définit la classe pour la pile d'entiers  
tout le code est dans le fichier d'entête i.e. pas besoin ici de fichier d'implémentation

<br>

```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {
  }
  void push (int e) {
    tab[top] = e;
    top = top+1;
  }
  int pop () {
    top = top-1;
    return tab[top];
  }
  void print () {
    std :: cout << "[";
    for (int i=0; i<top; i++) {
      std::cout << tab[i] << ' ';
    }
    std::cout << '[' << std::endl;
  }
  void delete_stack () {
    delete [] tab;
  }
private:
  int size;
  int top;
  int* tab;
};
```

</div>

+++

## exemple d'utilisation de la pile d'entiers

+++

<div class = "framed-cell">
    <ins class = "underlined-title">exemple d'utilisation de cette classe</ins>
    
on utilise la classe pile d'entiers dans une fonction `main`

<br>

```c++
// dans le fichier main.cpp
#include "intstack.h"

int main () {
  IntStack st (12);
  st.print();
  st.delete_stack();
  return 0;
}
```

<br>

on vérifie avec `valgrind` que tout se passe bien dans cette exécution

<br>

```bash
$ g++ main.cpp -o stack
$ valgrind ./stack
==8821== Memcheck, a memory error detector
==8821== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==8821== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==8821== Command: ./stack
==8821== 
[[
==8821== 
==8821== HEAP SUMMARY:
==8821==     in use at exit: 0 bytes in 0 blocks
==8821==   total heap usage: 3 allocs, 3 frees, 73,776 bytes allocated
==8821== 
==8821== All heap blocks were freed -- no leaks are possible
==8821== 
==8821== For counts of detected and suppressed errors, rerun with: -v
==8821== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Compilation finished at Sun Nov  1 17:33:39
```

<br>

nous sommes contents:
* nous n'avons pas d'erreurs mémoire `0 errors from 0 contexts`
* nous n'avons pas de fuite mémoire `All heap blocks were freed -- no leaks are possible`
</div>

+++

## Que se passe-t-il dans ce code ?

+++

<div class = "framed-cell">
<ins class = "underlined-title">Que se passe-t-il dans ce code ?</ins>

<br>

Que pensez-vous du code ci-dessous  ?

<br>


```c++
// dans le fichier foo.cpp
#include "intstack.h"
void foo () {
    IntStack st (100);
}
int main () {
  foo();
  return 0;
}
```

<br>
 
demandons à `valgrind` ce qu'il en pense

<br>

```bash
$ g++ foo.cpp -o foo
$ valgrind ./foo
==9098== Memcheck, a memory error detector
==9098== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==9098== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==9098== Command: ./foo
==9098== 
==9098== 
==9098== HEAP SUMMARY:
==9098==     in use at exit: 400 bytes in 1 blocks
==9098==   total heap usage: 2 allocs, 1 frees, 73,104 bytes allocated
==9098== 
==9098== LEAK SUMMARY:
==9098==    definitely lost: 400 bytes in 1 blocks
==9098==    indirectly lost: 0 bytes in 0 blocks
==9098==      possibly lost: 0 bytes in 0 blocks
==9098==    still reachable: 0 bytes in 0 blocks
==9098==         suppressed: 0 bytes in 0 blocks
==9098== Rerun with --leak-check=full to see details of leaked memory
==9098== 
==9098== For counts of detected and suppressed errors, rerun with: -v
==9098== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

<br>

`HEAP SUMMARY: in use at exit: 400 bytes in 1 blocks`

<br>

`valgrind` indique avoir perdu 400 octets ($100 \times 4 $ avec `sizeof(int)` qui vaut `4`) 
</div>

+++

## un problème de fuite mémoire !

+++

<div class = "framed-cell">
<ins class = "underlined-title">un problème de fuite mémoire !</ins>

<br>

```c++
#include "intstack.h"
void foo () {
    IntStack st (100);
}
int main () {
  foo();
  return 0;
}
```

<br>

la fonction `main` appelle une fonction `foo`

<br>

dans la fonction `foo` un objet de type `IntStack` est alloué dans la pile d'exécution de c++

<br>

le constructeur de la `IntStack`  est appelé avec une taille de `100`

<br>

le constructeur alloue donc un tableau de `100` entiers en mémoire dynamique (dans le tas)  
notons que cette mémoire est repéré par l'attribut `st.tab` 
   
<br>
      
quand la fonction `foo` est terminée, l'objet `st` est enlevé de la pile d'exécution de c++  

<br>

la mémoire pointée par `st.tab` est devenue inaccessible mais elle n'a pas été libérée !!

<br>

`100` entiers restent bien alloués dans le tas

<br>

votre programme a un sérieux problème de fuite de mémoire !

<br>

ne pas libérer une mémoire allouée dynamiquement est considéré comme une erreur de programmation  
ce n'est pas anodin ! ce n'est pas un simple oubli !

<br>

si cette fonction `foo` était appelée très souvent, votre mémoire (tas) va se remplir  
il va devenir de plus en plus difficile pour votre programme de trouver des morceaux dans cette mémoire  
bientôt il va en reléguer même sur votre disque dur  
votre exécution va devenir de plus en plus lente (swap) jusqu'à finir par échouer  

</div>

+++

## comment éviter un tel problème ?

+++

<div class = "framed-cell">
<ins class = "underlined-title">comment éviter un tel problème ?</ins>
    
<br>

réfléchissons un peu ...
* une fonction crée un objet local
* le constructeur de cet objet alloue de la mémoire dynamique
* quand la fonction termine, l'objet arrive en fin de vie ...

<br>

`c++` sait parfaitement qu'il va enlever l'objet local de sa pile d'exécution

<br>
  
donc `c++` pourrait se charger d'exécuter automatiquement les instructions nécessaires pour nettoyer un objet quand il arrive en fin de vie

<br>

par exemple dans notre cas, on aimerait bien qu'il exécute automatiquement `delete [] tab`

<br>

dans un autre cas, on aimerait (par exemple) qu'il ferme un fichier que l'objet a ouvert durant sa vie...


<br>

`c++` introduit pour cela un **destructor**
* le destructeur est une fonction membre spéciale
* qui est automatiquement appelée lorsqu'un objet de cette classe arrive en fin de vie


<br>

un destructeur porte le nom de la classe préfixé par le caractère `~`

<br>

donnons un exemple simple de destructeur

```c++
class IntStack {
public:
  IntStack (int s) {
    tab = new int [s];
  }
  ~IntStack () {
    delete [] tab;
  }
private:
  int* tab;
};
```

<br>

vous remarquez que le code pour *nettoyer les objets qui arrivent en fin de vie* est ici la libération de la mémoire dynamique allouée

<br>

pour tester notre code, réutilisons la fonction `foo` et utilisons `valgrind` 

```c++
void foo () {
  IntStack st (100);
}
int main () {
  foo();
  return 0;
}
```

<br>

```bash
$ g++ foo.cpp -o foo
$ valgrind ./foo 
==9366== Memcheck, a memory error detector
==9366== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==9366== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==9366== Command: ./foo
==9366== 
==9366== 
==9366== HEAP SUMMARY:
==9366==     in use at exit: 0 bytes in 0 blocks
==9366==   total heap usage: 2 allocs, 2 frees, 73,104 bytes allocated
==9366== 
==9366== All heap blocks were freed -- no leaks are possible
==9366== 
==9366== For counts of detected and suppressed errors, rerun with: -v
==9366== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

<br>

la mémoire a bien été libérée par l'appel automatique de la fonction spéciale `~IntStack` (destructeur) sur l'objet local de `foo`  

qui est le destructeur  

<br>


notons que le constructeur est aussi une fonction spéciale  
qui est automatiquement appelée quand un objet d'une classe est créé (débute sa vie)

</div>

+++

## le destructeur une fonction membre spéciale

+++

<div class = "framed-cell">
<ins class = "underlined-title">le destructeur une fonction membre spéciale</ins>

<br>

si une classe n'a rien à faire de spécial quand ses objets arrivent en fin de vie: il ne faut **pas** mettre de destructeur !

<br>

quand vous ne définissez pas de destructeur dans une classe, `c++` en génère un implicitement

<br>

</div>

+++ {"tags": ["level_intermediate"]}

## supprimer le destructeur

+++ {"tags": ["level_intermediate"]}

<div class = "framed-cell">
<ins class = "underlined-title">supprimer un destructeur</ins>

<br>

il existe une manière d'interdire qu'un destructeur soit généré pour une classe

<br>

ainsi si un destructeur n'existe pas pour une classe, `c++` ne peut pas l'appeler

<br>

donc votre code ne doit mettre `c++` en situation de le faire

<br>

donc aucun objet local ne peut exister dans un code pour une classe dont le constructeur est supprimé

<br>

voici un exemple

```c++
// dans le fichier delete.cpp
class X {
  ~X () = delete;
};
int main () {
  X x;
  return 0;
}
```
<br>

on tente de compiler un programme où on a demandé que le destructeur de la classe `X` n'existe pas  
mais qui demande à `c++` de l'appeler en créant un objet de type `X` local au `main`

<br>

```bash
$ g++ delete.cpp
delete.cpp: In function ‘int main()’:
delete.cpp:6:5: error: use of deleted function ‘X::~X()’
   X x;
     ^
delete.cpp:3:3: note: declared here
   ~X () = delete;
   ^
Compilation finished at Sun Nov  1 19:07:35
```

</div>

+++ {"tags": ["level_intermediate"]}

## imposer le destructeur par défaut

+++ {"tags": ["level_intermediate"]}

<div class = "framed-cell">
<ins class = "underlined-title">imposer le destructeur par défaut</ins>

<br>

si on n'implémente pas le destructeur d'une classe, `c++` génère un destructeur qui, par défaut, est `public`

<br>

supposons que vous voulez
1. qu'une classe ait un destructeur `private`
1. et que `c++` génère le destructeur

<br>

parce qu'il fait cela tellement mieux que nous !  
(ses codes sont plus rapides)

<br>

vous pouvez le faire en déclarant que le destructeur *par défaut* est `private` mais sans le définir

<br>

naturellement il ne faut alors pas mettre `c++` en situation de devoir l'utiliser

<br>

`c++` ne peut pas appeler une fonction `private` en dehors des fonctions membres de la classe

<br>

il en sera de même pour le destructeur par défaut qui est mis dans la partie `private` d'une classe

<br>

```c++
class X {
private:
  ~X () = default;
};
void foo () {
  X x;
}
int main () {
  return 0;
}
```

<br>

ce code ne peut pas être compilé !  
puisque le destructeur `X::~X` devra être appelé lorsque l'objet `x` de la fonction `foo` arrivera en fin de vie

<br>


```bash
$ g++ delete.cpp
delete.cpp: In function ‘void foo()’:
delete.cpp:6:5: error: ‘X::~X()’ is private within this context
   X x;
     ^
delete.cpp:3:3: note: declared private here
   ~X () = default;
   ^
```

<br>

nous pouvons par contre utiliser le destructeur par défaut `private`  
dans les fonctions membres privées !

```c++
class X {
private:
  ~X () = default;
  void foo () {
    X x;
  }
};
int main () {
  return 0;
}
```

```bash
$ g++ delete.cpp
$

```

</div>
