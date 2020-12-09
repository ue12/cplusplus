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
</div>

```{code-cell} ipython3
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# déclenchement et traitement des exceptions

+++

## on a un programme

+++

<div class = "framed-cell">
<ins class = "underlined-title">on a un programme
    </ins>
    
<br>

vous avez implémenté une bibliothèque de programme pour un utilisateur, par exemple une pile d'entiers

<br>

vous avez réalisé toutes les fonctionnalités demandées dans un mode de fonctionnement nominal  
voici votre code:
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  ~IntStack() {
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
</div>

+++

## on a un problème ...

+++

<div class = "framed-cell">
    <ins class = "underlined-title">on a un problème ...</ins>
    
<br>
    
vous avez livré votre programme à un utilisateur qui s'en sert ... 

<br>
    
et tente de dépiler une pile vide ...      
   
```c++
// dans le fichier main.cpp
#include "intstack.h"
int main () {
   IntStack s(12);
   std::cout << s.pop() << std::endl;
   return 0;
}
```
 
```bash
$ g++ main.cpp -g
$ ./a.out
???
```
<br>
    
Que doit afficher ce programme ? Vous n'en avez vraiment aucune idée mais **vous** avez un problème ...
    
<br>
    
oui c'est **vous** le concepteur du programme qui avez un problème ... 

<br>

pasrce que votre utilisateur vous reproche de ne lui avoir donné aucun moyen de savoir qu'il faisait une erreur  
et donc aucun moyen pour récupérer cette erreur

</div>

+++

## le programme *échoue*

+++

<div class = "framed-cell">
<ins class = "underlined-title">le programme échoue</ins>
    
<br>
    
ce code a un sérieux problème
    
<br>
    
si vous ne voyez pas le problème, demandez à `valgrind`

```bash
$ g++ main.cpp -g
$ valgrind ./a.out
==12266== Memcheck, a memory error detector
==12266== Invalid read of size 4
==12266==    at 0x10941F: IntStack::pop() (intstack.h:14)
==12266==    by 0x109281: main (main.cpp:4)
==12266==  Address 0x4dbfc7c is 4 bytes before a block of size 48 alloc d
==12266==    by 0x1093AA: IntStack::IntStack(int) (intstack.h:4)
==12266==    by 0x109275: main (main.cpp:3)
==12266== 
...
    ==12201== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
    
<br>

vous avez une potentielle segmentation-fault 
    
<br>
    
et ce n'est pas le seul endroit où, dans le code de votre pile d'entier, vous pouvez avoir ce genre de problème
* l'utilisateur peut créer une pile avec un nombre négatif ou nul d'éléments
* l'utilisateur peut empiler un élément sur une pile déjà pleine

+++

## qu'est ce qu'une exception ?

+++

<div class = "framed-cell">
<ins class = "underlined-title">qu'est ce qu'une exception ?</ins>
    
<br>

Quel est l'endroit, dans le code de votre pile, où vous pouvez détecter le problème de dépiler une pile vide ?

<br>
    
dans la fonction `pop`  
    
<br>
    
mais à cet endroit de votre code, vous ne **savez pas résoudre ce problème** !
* il n'existe **aucune** valeur *logique* à retourner quand on dépile une pile vide

<br>

lorsque vous détectez une erreur que vous **ne savez pas rattraper localement** ...  
l'exécution de ce programme est gravement compromise !
* vous ne pouvez plus garantir à votre utilisateur une exécution normale de son programme
* vous ne pouvez pas laisser le code continuer à s'exécuter *comme si de rien n'était*

<br>

Quelle est la solution ?  
pour l'instant vous ne savez pas faire c'est une nouvelle construction de `c++`

</div>

+++

## *retour à l'envoyeur*

+++

<div class = "framed-cell">
<ins class = "underlined-title">retour à l'envoyeur</ins>
 
<br>

c'est clairement l'utilisateur qui a laissé passé un problème et il doit donc en être prévenu

<br>

en effet, seul l'utilisateur aurait pu empêcher le problème, par exemple en testant l'état de la pile avant de la dépiler

<br>

et maintemant, seul l'utilisateur peut résoudre ce problème !

<br>

mais encore faut-il qu'il soit prévenu

<br>

c'est là que les **exception** entrent en scène, elles vont permettre aux programmes
* de **renvoyer** aux utilisateurs (*négligeants*) ...
* les problèmes que les codes ne savent pas résoudre localement !
<br>

ce problème se produit lors d'un comportement *non nominal* de votre programme  
c'est à dire dans un cas un peu exceptionnel d'où la fameuse appellation *exception*

</div>

+++

## arrêt de l'exécution *nominale*

+++

<div class = "framed-cell">
<ins class = "underlined-title">arrêt de l'exécution *nominale*</ins>
 
<br>

on récapitule le problème

<br>

lors de l'implémentation de certaines fonctionnalités, vous remarquez
* que des configurations pourraient causer des erreurs
* et que la bonne exécution de votre code pourrait être gravement entravée
* mais localement vous ne savez pas du tout résoudre ces erreurs 

<br>

l'idée fondamentale:
* lorsque, dans votre code, vous voyez un problème que vous ne savez pas résoudre localement
* vous allez renvoyer le problème à celui qui l'a causé

<br>

l'exécution *normale* de votre programme s'arrête
* le contrôle est redonné au block englobant
* soit il attrape et résoud le problème
* soit il envoie le problème plus haut


<br>

et si personne ne résoud le problème alors le programme échoue `Aborted`

</div>

+++

## lancer une exception avec `throw`

+++

<div class = "framed-cell">
<ins class = "underlined-title">lancer une exception avec `throw`</ins>
    
<br>

pour arrêter l'exécution, d'un code qui détecte un problème qu'il ne sait pas résoudre localement
* vous devez utiliser la fonction `throw` qui *envoie* un objet
* cet objet peut être de n'importe quel type  
(`c++` vous propose des types d'exceptions prédéfinies dans sa librarie standard on y reviendra)
</div>

+++

## on lance des exceptions dans notre code

+++

<div class = "framed-cell">
    <ins class = "underlined-title">on lance des exceptions dans notre code</ins>
    
<br>

reprenons le code et lançons avec `throw` des exceptions là où des problèmes peuvent se produire  
(sans qu'on sache comment les résoudre)

<br>

nous lançons des objets, par exemple une chaîne de caractères
    
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s) :top(0),  size(s) {
    if (s > 0)
      tab = new int [size];
    else
      throw("IntStack: size less or equal to 0");
  }
  ~IntStack () {
    delete [] tab;
  }
  void push (int e) {
    if (not is_full()) {
      tab[top] = e;
      top = top + 1;
    } else {
      throw("IntStack: push on full stack");
    }
  }
  int pop () {
    if (not is_empty()) {
      top = top -1;
      return tab[top];
    } else
      throw("IntStack: pop an empty stack");
  }
  bool is_empty () {
    return top == 0;
  }
  bool is_full () {
    return top == size;
  }
  void print () {
    std::cout << '[';
    for (int i = 0; i < top; i++)
      std::cout << tab[i] << ' ';
    std::cout << '[' << std::endl;
  }
private:
  int top;
  int size;
  int *tab;
};
```

</div>

+++

## on déclenche une exception sans la rattraper

+++

 <div class = "framed-cell">
    <ins class = "underlined-title">on déclenche une exception sans la rattraper</ins>
    
<br>  

on produit une erreur dans la fonction `main`

<br>

```c++
// dans le fichiers main.cpp
#include "intstack.h"
int main () {
  IntStack st (-12);  
  return 0;
}
```

<br>

on compile et on exécute le code

```bash
$ g++ main.cpp
$ ./a.out 
terminate called after throwing an instance of 'char const*'
Aborted (core dumped)
$ 
```

<br>

que s'est-il passé ? 
* une exception a été déclenchée
* personne n'a rattrapé l'exception
* le programme a été `Aborted` i.e. le système d'exploitation l'a fait échouer

<br>

plus précisément
* le `throw("IntStack: size less or equal to 0")` a été exécuté
* pour `c++` c'est le `throw` d'un objet de type `const char*`
* l'exception n'est pas rattrapée, dans le programme
* la fonction `std::terminate` est donc appelée
* par défaut la fonction `std::terminate` appelle la fonction `std::abort`
* et la fonction `std::abort` cause une terminaison anormale de votre programme


<br>

`c++` nous donne un moyen de rattraper une exception

</div>

+++

## attraper une exception avec `try` et `catch`

+++

<div class = "framed-cell">
<ins class = "underlined-title">attraper une exception avec `try` et `catch`</ins>
    
<br>

pour attraper une exception et ainsi éviter que le programme n'échoue il vous faut:
* englober le code qui peut déclencher une exception dans un block `try` 
* et indiquer après le block `try` les exceptions que vous voulez rattraper avec `catch`
* les exceptions sont rattrapées suivant le type de l'objet *lancé* par un `throw`

<br>

on va rattraper l'exception `const char*`
  
  
```c++
// dans le fichier main.cpp
#include "intstack.h"
int main () {
  try {
    IntStack st (-12);
  }
  catch (const char* e) {
    std::cout << e << std::endl;
  }
  return 0;
}
```

<br>

vous remarquez
* le block `try` qui englobe la création erronée d'une pile de taille négative
* le block `catch` qui attrape une erreur du type de celle déclenchée dans la construction erronée
* la manière d'accéder au message d'erreur de l'erreur avec la méthode `what`

<br>

compilons et exécutons ce programme

```bash
$ g++ main.cpp
$ ./a.out
IntStack: size less or equal to 0
$
```

<br>

après rattrapage de l'exception, le programme termine tranquillement sa vie 
il n'est pas tué par le système d'exploitation

</div>

+++

## les exceptions standard

+++

<div class = "framed-cell">
    <ins class = "underlined-title">les exceptions de la librairie standard</ins>

<br>

dans le fichier d'en-tête `stdexcept` est défini un ensemble d'exceptions standards

<br>

les programmes peuvent les utiliser pour signaler les erreurs les plus communes

<br>

il y a deux ensemble d'erreurs, les erreurs de *logique* et les erreurs d'*exécution*

<br>

les erreurs de logique `Logic-error`    
`domain_error`  
`invalid_argument`  
`length_error`  
`out_of_range`

<br>

les erreurs d'exécution `runtime_error`    
`range_error`  
`overflow_error`  
`underflow_error` 

<br>

voyons un exemple

</div>

+++

## lancer une exception *standard* `stdexcept`

+++

<div class = "framed-cell">
    <ins class = "underlined-title">lancer une exception *standard* `stdexcept`</ins>

<br>

nous pouvons décider, par exemple, pour notre pile d'exécution que
* si la pile est *créée* avec une taille négative ou nulle, on déclenche une erreur avec un objet de type `std::length_error`
* si on dépile une pile vide ou si on empile sur une pile pleine, on déclenche une exception avec un objet de type `std::domain_error`

<br>

on lance les exceptions dans notre code  
remarquez l'inclusion du fichier des exceptions `stdexcept`

```c++
// dans le fichier intstack.h
#include <stdexcept>
class IntStack {
public:
  IntStack (int s) :top(0),  size(s) {
    if (s > 0)
      tab = new int [size];
    else
      throw std::length_error("IntStack: size less or equal to 0");
  }
  void push (int e) {
    if (not is_full()) {
      tab[top] = e;
      top = top + 1;
    } else
      throw std::domain_error("IntStack: push on full stack");
  }
  int pop () {
    if (not is_empty()) {
      top = top -1;
      return tab[top];
    } else
      throw std::domain_error("IntStack: pop an empty stack");
  }
  // le reste du code ci-dessous
  // ...
};
```

+++

## on déclenche et rattrape une exception standard

+++

<div class = "framed-cell">
    <ins class = "underlined-title">on déclenche et rattrape une exception standard</ins>
    
<br>

on déclenche et rattrape une erreur à partir de la fonction `main`
    
<br>
    
remarquez
* l'inclusion du fichier d'entête `stdexcept`  
* la méthode `what` des objets dle type est une exception standard   
cette méthode renvoie le message de l'exception (qu'on peut afficher)

<br>

```c++
// dans le fichiers main.cpp
#include "intstack.h"
#include <stdexcept>
int main () {
  try {
    IntStack st (12);
    std::cout << st.pop() << std::endl;
  }
  catch (std::domain_error e) { // on reviendra sur le type
    std::cout << e.what() << std::endl;
  }
  return 0;
}
```

<br>
on compile et on exécute le code

```bash
$ g++ main.cpp
$ ./a.out 
IntStack: pop an empty stack
$ 
```

</div>

+++

## on rattrape plusieurs exceptions

+++

<div class = "framed-cell">
<ins class = "underlined-title">on rattrape plusieurs exceptions</ins>

<br>

naturellement, vous pouvez  rattraper plusieurs exceptions en mettant leurs blocs `catch` les uns après les autres
    
<br>

le premier block `catch` dont le type correspond est choisi

```c++
#include "intstack.h"
#include <stdexcept>
int main () {
  try {
    IntStack st (12);
    st.pop();
  }
  catch (std::length_error e) {
    std::cout << e.what() << std::endl;
  }
  catch (std::domain_error e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
```
<br>

```bash
g++ main.cpp
$ ./a.out
IntStack: pop an empty stack
```

</div>

+++

##  `...` rattrape tout !

+++

<div class = "framed-cell">
<ins class = "underlined-title">`...` rattrape tout !</ins>
    
<br>

il est possible de rattraper toute les exceptions possibles avec `...`  
on appelle `...` l'ellipse

<br>


```c++
#include <iostream>

void foo () {
  throw 12;
}

int main () {
  try {
    foo();
  }
  catch (...) {
    std::cout << "je rattrape tout !" << std::endl;
  }
  return 0;
}
```

<br>

```bash
$ g++ main.cpp
$ ./a.out
je rattrape tout !
```

<br>

le `catch` avec `...` doit être le dernier de la liste des `catch`

</div>

+++

## objets locaux et exceptions - advanced

+++

<div class = "framed-cell">
<ins class = "underlined-title">destruction des objets locaux - advanced</ins>

<br>

si vous lancez une exception dans une fonction qui a construit des objets locaux

<br>

si l'exception est rattrapée alors le destructeur de ces objets sera appelé  
sinon non

<br>

exemple avec l'`IntStack` où on a ajouté un affichage dans le destructeur

```c++
class IntStack {
public:
... 
  ~IntStack () {
    std::cout << "on appelle le destructeur de la IntStack\n";
    delete [] tab;
  }
...
};
```

<br>

```c++
// dans le fichier main.cpp
#include <iostream>
#include "intstack.h"
void foo () {
  IntStack st(13);
  throw 12;
}
int main () {
  try {
    foo();
  }
  catch (int) {
    std::cout << "on rattrape l'exception de foo" << std::endl;
  }
  return 0;
}
```

```bash
$ g++main.cpp
$./a.out
on appelle le destructeur de la IntStack
on rattrape l'exception de foo
```
</div>

+++ {"tags": ["level_intermediate"]}

## on relance une exception - advanced

+++ {"tags": ["level_intermediate"]}

<div class = "framed-cell">
<ins class = "underlined-title">on relance une exception - advanced</ins>

<br>

il est possible de relancer à l'identique une exception que vous avez attrapée

<br>

c'est utile quand
* vous vouliez voir si vous pouviez gérer l'exception à cet endroit là du code
* mais en regardant l'exception, vous vous rendez compte que vous ne pouvez pas
* vous la relancer telle quelle avec `throw` **sans argument**


<br>

donnons une exemple
* dans la fonction `bar` on lance une exception
* la fonction `foo` appelle la fonction `bar` et attrape l'exception
* puis elle relance l'exception originale et pas une nouvelle !

<br>

```c++
#include <iostream>

void bar () {
  throw 12;
}
void foo () {
  try {
    bar();
  }
  catch (int i) {
    std::cout << "foo regarde l'exception " << i
              << std::endl;
    throw;
  }
}
int main () {
  try {
    foo();
  }
  catch (int i) {
    std::cout << "main rattrape l'exception " << i
              << std::endl;
  }
  return 0;
}
```

```bash
$ g++ main.cpp
$ ./a.out
foo regarde l'exception 12
main rattrape l'exception 12
```
</div>
