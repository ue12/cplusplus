---
jupytext:
  formats: md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.12
    jupytext_version: 1.7.1
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

```{code-cell} ipython3
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# constructeur de copie

+++

## rappel de l'implémentation de la pile d'entiers

+++

<div class = "framed-cell">
<ins class = "underlined-title">Rappel de l'implémentation de la pile d'entiers</ins>

<br>

on prend la version de la pile d'entiers sans les exceptions
    
```c++
// dans le fichier intstack.h
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  ~IntStack () {
    delete [] tab;
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
private:
  int size;
  int top;
  int* tab;
};
```
        
<br>

vous vous rappelez:
* le constructeur alloue un morceau de mémoire dans le tas avec `new int [size]`
* le destructeur dés-alloue ce morceau de mémoire  avec `delete [] tab`  
(notez bien qu'on appelle `delete []` parce qu'on détruit un tableau d'`int` et pas un seul `int`)

</div>

+++

## construction d'objets de types de base

+++

<div class = "framed-cell">
<ins class = "underlined-title">type de base</ins>
  
  
<br>

pour les type de base de `c++` on peut construire un nouvel objet à partir d'un objet existant du *même* type, par exemple:

```c++
int main () {
  int i = 12;
  int j = i;
  j = i;
  return 0;
}
```

dans cet exemple
* un objet, de type entier, de nom `j` est construit
* en recopiant un objet de type entier existant de nom `i`

</div>

+++

<div class = "framed-cell">
<ins class = "underlined-title">type utilisateur</ins>
  
  
<br>
    
`c++` traite les objets de vos propres types (*types utilisateur* i.e. *définis par l'utilisateur*),  
comme il traite les objets de ses types de base (par exemple les entiers)
    
<br>
ainsi supposons que vous définissiez un type `X` dont le constructeur prend un entier en argument
    
    
```c++
// in file file.h
class X {
    int value;
public:
    X (int value) : value(value) {}
};
```
    
<br>
    
`c++` va définir *implicitement* les méthodes pour construire un objet de type `X` (ici `j`) à partir d'un objet de type `X` existant (ici `i`),  
et même affecter un objet de type `X` (ici `i`) à un objet de type `X` pré-existant (ici `j`)
    
```c++
#include "file.h"    
int main () {
  X i = 12;
  X j = i;
  j = i;
  return 0;
}
```

+++

## construction d'objets de types utilisateur

+++

<div class = "framed-cell">
<ins class = "underlined-title">construction d'objets de types utilisateur</ins>
  
donc `c++` permet de:
* traiter les objets des types définis par l'utilisateur (par exemple la pile d'entiers),   
    de la même manière que les objets de ses types de base (par exemple les entiers)

<br>

ainsi, vous allez pouvoir construire un objet d'un de vos types comme la `IntStack`  
à partir d'un objet existant de ce même type  

<br>

à vous de jouer
* initialisez un objet de type `IntStack` à partir d'un objet de type `IntStack` existant
* manipulez ces deux objets, imprimez les
* que constatez-vous ?

<br>
voici un exemple, en commentaire ce qu'on attend  
ensuite l'exécution
    
```c++
#include "intstack.h"
int main () {
  IntStack st1 (100);
  IntStack st2 = st1;
  st1.push (10);
  st2.push (20);
  st1.print (); // ? [10 [
  st2.print (); // ? [20 [
  return 0;
}
```
    
```bash
$ g++ main.cpp
$ ./a.out
g++ -g -Wall main.cpp ; ./a.out
[20 [
[20 [
free(): double free detected
Aborted (core dumped)
```
     
<br>
    
on constate
* des erreurs dans les impressions
* une erreur indiquant qu'une zone mémoire est détruite deux fois  
(`free` c'est comme `delete`)
* un arrêt brusque de l'exécution
    
</div>

+++

## qu'a fait `c++` ?

+++

<div class = "framed-cell">
    
<ins class = "underlined-title">qu'a fait `c++`</ins>

<br>
    
Ce code se compile parfaitement donc `c++` a parfaitement accepté notre programme
    
<br>

Que fait `c++` dans ce code ?
```c++
  IntStack st1 (100);
  IntStack st2 = st1;
```
* `c++` construit l'objet `st1` de type `IntStack` en appelant le constructeur `IntStack(100)`
* puis il construit l'objet `st2` qui est de type `IntStack` comment le fait-il ? 

<br>
    
`IntStack st2 = st1` ou encore `IntStack st2 (st1)` nous indique
* que `st2` est construit en appelant un constructeur `IntStack`
* pas celui auquel on passe un entier `st1` n'est **pas** un entier
* mais un constructeur auquel on passe `st1` donc un objet de type `IntStack`

    
<br>
    
deux remarques
1. c ++ doit avoir *implicitement* défini une telle fonction puisqu'il l'appelle !
1. dans le cas de notre classe, la fonction proposée par `c++` n'est **pas correcte**

</div>

+++

## construction par copie implicite d'un objet

+++

<div class = "framed-cell">
<ins class = "underlined-title">construction par copie implicite d'un objet</ins>
    
<br>
    
`c++` a construit `st2` en **copiant** `s1`
  
<br>

nos problèmes sont survenus parce nous avons laissé `c++` décider comment copier des objets de notre type utilisateur `IntStack`

<br>

quand `c++` créé un nouvel objet
* si il est confronté à l'initialisation de cet objet par un objet existant (du **bon** type)
* il va regarder si votre classe propose la méthode de **construction par copie**
* si cette méthode n'est pas définie dans votre classe, il réalise la **copie implicite membre à membre** de l'objet

<br>

donc pour initialiser `st2`  
`c++` a copié membre les 3 attributs de `st1` dans l'espace mémoire réservé à `st2` (zone mémoire ?)
* `st1.top` a été copié dans la zone mémoire réservée à `st2.top`
* `st1.size` a été copié dans `st2.size`
* `st1.tab` a été copié dans `st2.tab` ...

<br>

.... vous voyez le problème ?
* `top` et `size` étant des entiers `c++` les a simplement copiés aucun problème
* mais `tab` est une adresse donc `c++` a copié dans `st2.tab` l'adresse de `st1.tab`
* donc les deux objets ont la même adresse pour leur attribut `tab`
    

<br>

vos deux piles partagent le même tableau d'entiers ...

<br>

la première erreur vient du fait que vous écrasez le premier élément de `st1.tab` lors du `st2.push(20)`

<br>

la deuxième erreur vient du fait que
* les deux objets `st` et `st2` sont mis par `c++` dans sa zone mémoire qui est sa pile d'exécution
* leur constructeur sera automatiquement appelé lorsque ces deux objets arriveront en fin de vie (du `main`)
* `delete [] tab` est donc appelé deux fois une fois pour détruire `st2` et ensuite pour détruire `st1`
    
</div>

+++

## deux autres exemples de ce même problème

+++

<div class = "framed-cell">
<ins class = "underlined-title">deux autres exemples de ce même problème</ins>

à vous de jouer
* codez les codes ci-dessous
* exécutez-les directement
* exécutez-les en utilisant `valgrind` (pour ceux qui ne l'ont pas, je vous donne la sortie de `valgrind`)
* et essayez de comprendre ce qui se passe  
</div>

+++

la classe `IntStack`

```c++
      1 #include <iostream>
      2 class IntStack {
      3 public:
      4   IntStack (int s): size(s), top (0), tab (new int [size]) {}
      5   ~IntStack() {
      6     delete [] tab;
      7   }
      8   void push (int e) {
      9     tab[top] = e;
     10     top = top+1;
     11   }
     12   int pop () {
     13     top = top-1;
     14     return tab[top];
     15   }
     16   void print () {
     17     std :: cout << "[";
     18     for (int i=0; i<top; i++) {
     19       std::cout << tab[i] << ' ';
     20     }
     21     std::cout << '[' << std::endl;
     22   }
     23 private:
     24   int size;
     25   int top;
     26   int* tab;
     27 };
```

+++

<div class = "framed-cell">
<ins class = "underlined-title">premier exemple</ins>

```c++
1 #include "intstack.h"
2 int main () {
3   IntStack st1 (100);
4   {
5     IntStack st2 = st1;
7   }
8   st1.push (10);
9   return 0;
10 }
```
    
<br>

```bash
$ valgrind ./a.out
==19544== Memcheck, a memory error detector
==19544== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==19544== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==19544== Command: ./a.out
==19544== Invalid write of size 4
==19544==    at 0x109392: IntStack::push(int) (intstack.h:9)
==19544==    by 0x109242: main (main.cpp:7)
==19544==  Address 0x4dbfc80 is 0 bytes inside a block of size 400 free'd
==19544==    at 0x483D74F: operator delete[](void*) (in ...)
==19544==    by 0x109364: IntStack::~IntStack() (intstack.h:6)
==19544==    by 0x109231: main (main.cpp:5)
==19544==  Block was alloc'd at
==19544==    at 0x483C583: operator new[](unsigned long) (in ...)
==19544==    by 0x109328: IntStack::IntStack(int) (intstack.h:4)
==19544==    by 0x109215: main (main.cpp:3)
==19544== 
==19544== Invalid free() / delete / delete[] / realloc()
==19544==    at 0x483D74F: operator delete[](void*) (in ...)
==19544==    by 0x109364: IntStack::~IntStack() (intstack.h:6)
==19544==    by 0x109253: main (main.cpp:3)
==19544==  Address 0x4dbfc80 is 0 bytes inside a block of size 400 free'd
==19544==    at 0x483D74F: operator delete[](void*) (in ...)
==19544==    by 0x109364: IntStack::~IntStack() (intstack.h:6)
==19544==    by 0x109231: main (main.cpp:5)
==19544==  Block was alloc'd at
==19544==    at 0x483C583: operator new[](unsigned long) (in ...)
==19544==    by 0x109328: IntStack::IntStack(int) (intstack.h:4)
==19544==    by 0x109215: main (main.cpp:3)
==19544== 
==19544== 
==19544== HEAP SUMMARY:
==19544==     in use at exit: 0 bytes in 0 blocks
==19544==   total heap usage: 2 allocs, 3 frees, 73,104 bytes allocated
==19544== 
==19544== All heap blocks were freed -- no leaks are possible
==19544== 
==19544== For lists of detected and suppressed errors, rerun with: -s
==19544== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```
</div>

+++

<div class = "framed-cell">
<ins class = "underlined-title">second exemple</ins>
    
```c++
1 #include "intstack.h"
2 void foo (IntStack st) {
3 }
4 int main () {
5   IntStack st1 (100);
6   foo(st1);
7   st1.push(10);
8   return 0;
9 }
```
    
```bash
valgrind ./a.out 
==19596== Memcheck, a memory error detector
==19596== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==19596== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==19596== Command: ./a.out
==19596== 
==19596== Invalid write of size 4
==19596==    at 0x1093AC: IntStack::push(int) (intstack.h:9)
==19596==    by 0x10925D: main (main.cpp:7)
==19596==  Address 0x4dbfc80 is 0 bytes inside a block of size 400 free'd
==19596==    at 0x483D74F: operator delete[](void*) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==19596==    by 0x10937E: IntStack::~IntStack() (intstack.h:6)
==19596==    by 0x10924C: main (main.cpp:6)
==19596==  Block was alloc'd at
==19596==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==19596==    by 0x109342: IntStack::IntStack(int) (intstack.h:4)
==19596==    by 0x109224: main (main.cpp:5)
==19596== 
==19596== Invalid free() / delete / delete[] / realloc()
==19596==    at 0x483D74F: operator delete[](void*) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==19596==    by 0x10937E: IntStack::~IntStack() (intstack.h:6)
==19596==    by 0x10926E: main (main.cpp:5)
==19596==  Address 0x4dbfc80 is 0 bytes inside a block of size 400 free'd
==19596==    at 0x483D74F: operator delete[](void*) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==19596==    by 0x10937E: IntStack::~IntStack() (intstack.h:6)
==19596==    by 0x10924C: main (main.cpp:6)
==19596==  Block was alloc'd at
==19596==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==19596==    by 0x109342: IntStack::IntStack(int) (intstack.h:4)
==19596==    by 0x109224: main (main.cpp:5)
==19596== 
==19596== 
==19596== HEAP SUMMARY:
==19596==     in use at exit: 0 bytes in 0 blocks
==19596==   total heap usage: 2 allocs, 3 frees, 73,104 bytes allocated
==19596== 
==19596== All heap blocks were freed -- no leaks are possible
==19596== 
==19596== For lists of detected and suppressed errors, rerun with: -s
==19596== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
``` 

</div>

+++

## ajout d'un constructeur de copie à votre classe

+++

<div class = "framed-cell">
<ins class = "underlined-title"> ajout d'un constructeur de copie à votre classe</ins>

<br>
    
quand le constructeur de copie implicite (généré par `c++`) n'a pas le comportement souhaité  
vous devez **définir votre propre méthode** pour la construction de copie  
c'est une méthode spéciale appelée le **constructeur de copie** (copy-constructor)  

  
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  
  // le constructeur de copie
  // c'est un constructeur donc il porte le nom de la classe
  IntStack (???) { // quel est le type de son argument ?
     // quel est son code ?
  }
    
  ~IntStack() {
    delete [] tab;
  }
  // et le reste du code ...
...
};
```
    
```c++
#include "intstack.h"                                 
int main () {
  IntStack st1 (10); // appel de IntStack (10)
  IntStack st2 = st1; // appel de IntStack (st1)
  return 0;
}
```
      
<br>

Quel est le type de l'argument du constructeur de copie ?  
Quel est son code ?
</div>

+++

## quel est le type de l'argument du constructeur de copie ?

+++

<div class = "framed-cell">
<ins class = "underlined-title">type de l'argument du constructeur de copie</ins>

<br>
    
vous savez deux choses:    
* vous n'avez pas besoin de modifier l'argument, vous devez le passer `const`
* `st1` est de type `IntStack`

<br>
    
Pourquoi cette solution est-elle fausse ?

```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  
  // le constructeur de copie
  IntStack (const IntStack st) { // ceci est faux !
     // ...
  }
...
```
    
<br>

c'est faux parce que vous utiliserez la copie dans la méthode supposée définir la copie !

<br> 
    
il vous manque une construction de `c++` qui est la référence
    

</div>

+++

## la référence en `c++` est `&`

+++

<div class = "framed-cell">
    
<ins class = "underlined-title">la référence en `c++` est `&`</ins>


<br>
    
un référence c'est un pointeur (comme `pi`) qui est automatiquement déréférencé quand on l'utilise (comme `*pi`)

<br>
    
les références ont été introduites dans `c++` afin de pouvoir passer son argument au constructeur de copie  
qui justement ne doit pas copier son argument  
tout en ne pouvant pas non plus passer l'adresse de son argument  
puisque le code attend bien un objet pas l'adresse d'un objet
 
```c++
#include "intstack.h"
int main () {
   IntStack st1 (100);
   IntStack st2 = st1; // c'est IntStack(st1) pas IntStack(&st1) !!!
   return 0;
}
```
</div>

+++

## référence vers des entiers

+++

   <div class = "framed-cell">
<ins class = "underlined-title">référence vers des entiers</ins>
 
`c++` a introduit la référence `&`
* pour les variables
* pour le passage des arguments des fonctions
* pour les types de retour des fonctions  
(attention même problème que pour les types de retours des fonctions qui sont des pointeurs: il ne faut pas renvoyer un pointeur ou une référence vers un objet local !) 
    
    
<br>
    
```c++
int main () {
  int i = 12;
  int* pi = &i;
  int& ri = i;
  ri++;   // i is now 13
  ri=156; // i is now 156
  return 0;
}    
```

<br>
  
vous remarquez
* `i` est un entier
* `ri` est une référence vers un entier existant ici `i`
* `i` et `ri` référence bien le **même entier** donc la **même zone de mémoire**
* mais `ri` n'est pas une adresse quand on *modifie*  `ri` c'est `i` qu'on modifie !

<br>
    
une référence est comme un alias, un nouveau nom pour un objet existant

<br>
    
c'est **très** puissant pour le passage d'arguments des fonctions
* parce que le comportement est celui d'un pointeur mais on le manipule comme un objet
    
<br>
    
à vous de jouer  
re-programmer en utilisant les références la fonction `swap_ref`  
on vous redonne l'exemple avec les arguments qui sont des adresses (pointeurs) 
   
```c++
// in file swap.cpp
#include<iostream>

void swap_ref (int& x, int& y) {
   int i = x;
   x = y;
   y = i;
}
void swap_ptr (int* x, int* y) {
  int i = *x;
  *x = *y;
  *y = i;
}
    
int main () {
   int a = 11;
   int& ra = a;
   ra = ra + 1;
   int b = 81;
   std::cout << a << ' ' << b << std::endl; // 12 81 
   swap_ptr(&a, &b);
   std::cout << a << ' ' << b << std::endl; // 81 12 
   swap_ref(a, b);
   std::cout << a << ' ' << b << std::endl; // 12 81
   return 0;
}
```

+++

    
```c++    
#include "intstack.h"
void foo (IntStack& rst) {
    rst.push(10);
    rst.print();
}
void bar (IntStack rst) {
    rst.push(10);
    rst.print();
}
int main () {
    IntStack st(100);
    foo(st);
    st.print();
    bar(st);
    st.push(20);
    return 0;
}
    
```
    
</div>

+++

## quel est le code du constructeur de copie

+++

<div class = "framed-cell">
<ins class = "underlined-title"> quel est le code du constructeur de copie</ins>

<br>
    
réfléchissez à ce que vous devez faire
* vous devez initialiser: `top`, `size` et `tab` de la nouvelle pile d'entiers
* à partir des valeurs de la pile d'entier passée en argument par **référence**
    
 
<br>
    
faites le 
    
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  
  IntStack (const IntStack& rst) : /* liste d'intialisation */ {
    // le reste du code
  }

  ~IntStack() {
    delete [] tab;
  }
...
};
```

<br>
    
pour le tester faites fonctionner ce code suivant  
sans aucune erreur avec `valgrind`    
    
```c++
#include "intstack.h"
int main () {
  IntStack st1 (10);
  st1.push(1);
  st1.push(2);
  IntStack st2 = st1;
  st2.push(30);
  st1.print(); // doit imprimer [1 2 [
  st2.print(); // doit imprimer [1 2 30 [
  return 0;
}
```
   
</div>

+++

  
```c++
// dans le fichier main.cpp
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  
  IntStack (const IntStack& rst) : size(rst.size), top(rst.top), 
    tab(new int[size]) {
        for (int i = 0; i < top; ++i)
            tab[i] = rst.tab[i];
  }

  ~IntStack() {
    delete [] tab;
  }
...
private:
    int size;
    int top;
    int* tab;
};
```
```bash
$ g++ -g -Wall main.cpp
```

+++

## un constructeur de copie est un constructeur...

+++

<div class = "framed-cell">
    
<ins class = "underlined-title">un constructeur de copie est un constructeur... </ins>

<br>

cela vous semble évident ? 
    
<br>
    
dans le code ci-dessous qui compile et fonctionne parfaitement
1. rajoutez un constructeur de copie dont le code sera d'écrire "je suis le constructeur de copie !"
1. faites en sorte que votre code compile ...


```c++
class Foo {
    public:
    Foo (const Foo&) {}
};

int main () {
  Foo x;
  Foo y = x;
  y = x;
  return 0;
}

```    

<br>

Qu'en concluez-vous ?     
    
<br>   
    
    
un constructeur de copie est un constructeur ... en effet 
* si vous ne déclarez aucun constructeur dans une classe
* celle-ci possède alors le constructeur par défaut (automatiquement généré par `c++`),  
(par défaut = par défaut d'argument)
* si dans cette classe, vous ajoutez alors un constructeur de copie
* alors le constructeur par défaut n'existe plus !
* vous avez écrit un constructeur pour la classe, `c++` n'en génère plus !
* c'est **vous** le responsable des constructions désormais
    
</div>

+++

## - ADVANCED - code par défaut (celui de `c++`) des méthodes spéciales

+++

<ins class = "underlined-title"> - ADVANCED - code par défaut (celui de `c++`) des méthodes spéciales</ins>
   

comme il vaut toujours mieux laisser `c++` générer implicitement les méthodes spéciales quand vous le pouvez  
vous allez pouvoir lui demander de mettre à la place de votre constructeur sans argument  
son propre constructeur sans argument avec `default`

```c++
class Foo {
public:
  Foo(const Foo&) {}
  Foo () = default;
};

int main () {
  Foo x;
  Foo y = x;
  return 0;
}
```
   
</div>

+++

## constructeur de copie et `class Integer`

+++

<div class = "framed-cell">
<ins class = "underlined-title"> Avons-nous besoin d'un constructeur de copie pour la `class integer`?</ins>

  
```c++
 #include <iostream>
class Integer {
public:
  Integer (int value = 0) : value(value) {}
  void incr () { value+=1; }
  void decr () { value-=1; }
  void print () { std::cout << value; }
private:
  int value;
};
int main () {
  Integer i = 7;
  i.incr();
  Integer j = i; // Que se passe-t-il ? 
  j.decr();
  j.print();
  return 0;
} 
  
```
    
<br>

ne définissez jamais un constructeur de copie quand vous n'en n'avez pas besoin  
laissez le compilateur le générer implicitement  
il sera beaucoup plus efficace que le vôtre


</div>

+++

## - ADVANCED - empêcher la construction par copie pour une classe

+++

<div class = "framed-cell">
<ins class = "underlined-title">- ADVANCED - empêcher la construction par copie pour une classe</ins>

<br>
   
si vous souhaitez interdire la copie d'objets d'un de vos types  
déclarez son constructeur de copie comme `= delete`

<br>

  
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  
  IntStack (const IntStack&) = delete;

  ~IntStack() {
    delete [] tab;
  }
  // le reste du code ...
};
```
    
```c++
// dans le fichier main.cpp    
#include "intstack.h"
int main () {
  IntStack st1 (100);
  IntStack st2 = st1;
  return 0;
}
```
    
```bash
$ g++ main.cpp
main.cpp: In function ‘int main()’:
main.cpp:4:18: error: use of deleted function ‘IntStack::IntStack(const IntStack&)’
    4 |   IntStack st2 = st1;
      |                  ^~~
In file included from main.cpp:1:
intstack.h:6:3: note: declared here
    6 |   IntStack (const IntStack&) = delete;
      |   ^~~~~~~~
$ 
```
    
<br>
    
les messages sont très clairs ...
    
</div>

+++

END
