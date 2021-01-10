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

# opérateur d'affectation

+++

## rappel de l'implémentation de la pile d'entiers

+++

<div class = "framed-cell">
<ins class = "underlined-title">Rappel de l'implémentation de la pile d'entiers</ins>

<br>

on prend une version simple de la pile d'entiers sans les exceptions  
la classe comporte le constructeur de copie et le destructeur
    
```c++
// dans le fichier intstack.h
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}
  ~IntStack () {
    delete [] tab;
  }
  IntStack (const IntStack& rst) :
    size(rst.size), top(rst.top), tab(new int[size]) {
        for (int i = 0; i < top; ++i)
            tab[i] = rst.tab[i];
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

vous remarquez:
* le constructeur alloue un morceau de mémoire dans le tas avec `new int [size]`
* le destructeur dés-alloue (libère) le morceau de mémoire alloué avec `delete [] tab`  
(notez bien qu'on appelle `delete []` parce qu'on détruit un tableau d'`int` et pas un seul `int`)
* le constructeur de copie, pour construire un nouvel objet de type `IntStack` alloue un morceau de mémoire dans le tas et y recopie le contenu de l'objet copié
</div>

+++

## affectation d'objets de types de base

+++

<div class = "framed-cell">
<ins class = "underlined-title">type de base</ins>
  
  
<br>

pour les type de base de `c++` on peut affecter un objet existant à un objet existant du *même* type
par exemple:

```c++
int main () {
  int i = 12;
  int j = 98;
  j = i;
  return 0;
}
```

dans cet exemple
* `i` existe, `j` existe
*  la valeur `j` est remplacée par celle de `i`
</div>

+++

<div class = "framed-cell">
<ins class = "underlined-title">type utilisateur</ins>
  
  
<br>
    
`c++` traite les objets de vos propres types (*types utilisateur* i.e. *définis par l'utilisateur*),  
comme il traite les objets de ses types de base (par exemple les entiers)
    
<br>
ainsi si vous définissiez un type `X` dont le constructeur prend un entier en argument
    
    
```c++
// in file file.h
class X {
    int value;
public:
    X (int value) : value(value) {}
};
```
    
<br>
    
`c++` va définir *implicitement* les méthodes pour affecter un objet de type `X` pré-existant (ici `i`) à un objet de type `X` pré-existant (ici `j`)
    
```c++
#include "file.h"    
int main () {
  X i = 12;
  X j = 98;
  j = i;
  return 0;
}
```

+++

## opération d'affection d'objets de types utilisateur

+++

<div class = "framed-cell">
<ins class = "underlined-title">opération d'affectation d'objets de types utilisateur</ins>
  
donc `c++` permet de:
* traiter les objets des types définis par l'utilisateur (par exemple la pile d'entiers),   
    de la même manière que les objets de ses types de base (par exemple les entiers)

<br>

ainsi, vous allez pouvoir affecter un objet d'un de vos types comme la `IntStack`  
avec un objet existant de ce même type  

<br>

à vous de jouer
* initialisez deux objets de type `IntStack`
* ajoutez-y des éléments
* affecter le second au premier
* manipulez ces deux objets, ajoutez des éléments et imprimez les
* que constatez-vous ?

<br>

voici un exemple, en commentaire ce qu'on attend  
ensuite l'exécution (et ca ne se passe pas vraiment comme nous l'attendions !)
    
```c++
#include "intstack.h"
int main () {
  IntStack st1 (100);
  st1.push (10);
  st1.print(); // [10 [
  IntStack st2 (10);
  st2.push (20);
  st2.print(); // [20 [
  st1 = st2; // l'affectation !
  st1.print(); // [10 [
  st2.print(); // [20 [
  return 0;
}
```
    
```bash
$ g++ main.cpp
$ ./a.out
[10 [
[20 [
[20 [
[20 [
free(): double free detected in tcache 2
Aborted (core dumped) ./a.out    
```
     
<br>
    
on constate
* des erreurs dans les impressions `st1` ne contient plus `10`
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
    
Ce code se compile parfaitement donc `c++` a accepté notre programme
    
<br>

Que fait `c++` dans ce code ?
```c++
  IntStack st1 (100);
  st1.push (10);
    
  IntStack st2 (10);
  st2.push (20);
  
  st1 = st2;
```
    
* `c++` construit l'objet `st1` de type `IntStack` en appelant le constructeur `IntStack(100)`  
il y empile l'élément `10`
* `c++` construit l'objet `st2` de type `IntStack` en appelant le constructeur `IntStack(10)`  
il y empile l'élément `20`
* puis il affecte l'objet `st2` à l'objet existant `st1`

<br>
    
première remarque
* si `c++` accepte l'affectation entre objets de type `IntStack`, c'est que `c++` a (*implicitement*) défini une telle fonction (puisqu'il l'appelle !)

<br>
    
deuxième remarque
* dans le cas de notre classe `IntStack` la fonction proposée par `c++` ne nous convient clairement **pas**

</div>

+++

## affectation implicite entre objets de type `IntStack`

+++

<div class = "framed-cell">

<ins class = "underlined-title">affectation implicite entre objets de type `IntStack`</ins>
    
<br>
    
`c++` a remplacé le contenu de `st1` par celui de `st2`
* il a affecté `st1.top` par `st2.top` et `st1.size` par `st2.size`
* mais surtout il a remplacé `st1.tab` par `st2.tab`  
l'objet `st1` pointe désormais sur le tableau d'entiers de `st2`

<br> 

remarques
1. le tableau d'entiers sur lequel pointait `st1` avant son affectation par `st2` a été perdu en mémoire  
il ne peut plus être des-alloué par l'appel du destructeur de `st1`
2. par contre, comme les destructeurs des objets `st2` et `st1` sont appelés et que les objets partagent désormais l'adresse du même tableau d'entier, celui-ci sera détruit deux fois d'où la `segmentation fault`
    
    <br>

nos problèmes sont survenus parce nous avons laissé `c++` décider comment affecter des objets de notre type utilisateur `IntStack`

<br>

quand `c++` affecte à objet à un objet existant (du **bon** type)
* il regarde si votre classe propose la méthode d'**affectation** (l'**opérateur d'affectation**)
* si cette méthode n'est pas définie dans votre classe, il réalise une **affectation implicite membre à membre** de l'objet

  
</div>

+++

## ajout d'un opérateur d'affectation à votre classe

+++

<div class = "framed-cell">

<ins class = "underlined-title"> ajout d'un opérateur d'affectation à votre classe</ins>

<br>
    
quand l'opérateur d'affectation, implicitement généré par `c++`, n'a pas le comportement souhaité, vous avez deux solutions:
* redéfinir votre **propre méthode** pour l'affectation
* refuser (suprimer avec `delete`) l'affectation pour les objets de votre type (nous y reviendrons)
    
    
<br>
    
pour la redéfinir, vous devez redéfinir une méthode spéciale appelée l'**opérateur d'affectation** ou encore `operator=`
* il va prendre en argument l'objet pour affecter l'objet courant  
par **référence** pour ne pas recopier l'objet et `const` puisque l'argument n'est pas modifié
* il va modifier l'objet courant (i.e. l'objet pointé par `this` dans les méthodes de la classe)
* et (pour l'instant) nous demandons que la méthode retourne `void` (nous y reviendrons)
    
<br>
    
    
ainsi nous devons définir cette nouvelle méthode:    
  
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s);
  IntStack (const IntStack& rst);
  ~IntStack ();
    
  void operator= (const IntStack& rt) {
    // votre code ici 
  }
                                
  void push (int e);
  int pop ();
  void print ();
private:
  int size;
  int top;
  int* tab;
};
```
    
```c++
#include "intstack.h"                                 
int main () {
  IntStack st1 (10); // appel de IntStack::IntStack (10)
  IntStack st2 (100); // appel de IntStack::IntStack (100)
  st1 = st2; // appel de IntStack::operator=(st2)
  return 0;
}
```
      
<br>

Quel est son code ?
</div>

+++

## le code de l'`operator=`

+++

<div class = "framed-cell">

<ins class = "underlined-title">quel est le code de l'`operator=` ? </ins>

<br>
    
Que devez-vous faire ?
* affecter: `top`, `size` et `tab` de l'objet courant à partir des valeurs de la pile d'entier passée en argument par **référence**
* en n'oubliant pas que `tab` pointe sur un tableau d'entiers en mémoire dynamique, qu'il faut donc des-allouer avant de modifier la seule variable qui pointe dessus 
<br>
    
faites le 
    
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s);
  IntStack (const IntStack& rst);
  ~IntStack ();
    
  void operator= (const IntStack& rt) {
    // votre code ici 
  }
                                
  void push (int e);
  int pop ();
  void print ();
private:
  int size;
  int top;
  int* tab;
};
```

<br>
    
pour le tester faites fonctionner ce code suivant  
sans aucune erreur avec `valgrind`    
    
```c++
#include "intstack.h"
int main () {
  IntStack st1 (100);
  st1.push (10);
  st1.print(); // [10 [
  IntStack st2 (10);
  st2.push (20);
  st2.print(); // [20 [
  st1 = st2; // l'affectation !
  st1.print(); // [10 [
  st2.print(); // [20 [
  return 0;
}
```
   
</div>

+++

## `operator=` et `class Integer`

+++

<div class = "framed-cell">
<ins class = "underlined-title"> Avons-nous besoin d'un opérateur d'affectation pour la `class integer`?</ins>

  
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
  Integer j = 9;
  j.decr();
  i = j; // que se passe-t-il ici ?
  j.print();
  return 0;
} 
  
```
    
<br>

ne définissez jamais un opérateur d'affectation si vous n'en n'avez pas besoin  
laissez le compilateur le générer implicitement  
il sera beaucoup plus efficace que le vôtre !


</div>

+++

## - ADVANCED - chaîner les affectations

+++

<div class = "framed-cell">
    
<ins class = "underlined-title">- ADVANCED - chaîner les affectations </ins>

<br>
   
en `c++` vous pouvez chaîner les affectations, comme par exemple pour les `int`

<br>

  
```c++
int main () {
  int i = 10;
  int j = 20;
  int k = 30;
  i = j = k;
  return 0;
}
```

<br>
    
la valeur de `k` est mise dans `j` et la valeur de `j` est mise dans `i`
    
<br>
    
    
comme d'habitude si `c++` accepte *quelque chose* pour ses types de base, il va le permettre aussi pour vos types à vous (les types utilisateur)

<br>
    
comment faire ? 
    

```c++
int main () {
  IntStack i (10);
  IntStack j (20);
  IntStack k (30);
  i = j = k;
    
  // ou encore en décomposant l'expression
  i.operator=(j.operator=(k));
  return 0;
}
```
    
<br>
    
vous constatez dans l'expression `i.operator=(j.operator=(k))`
* que `i.operator=` prend en argument ce qui est retourné par la méthode `j.operator=(k)`
* clairement, pour chaîner les affectation, la méthode `operator=` doit renvoyer autre chose que `void`    
    
<br>
    
    
Que doit-elle retourner ?
    
    
</div>

+++

## - ADVANCED - empêcher l'affectation entre objets d'une classe

+++

<div class = "framed-cell">
<ins class = "underlined-title">- ADVANCED - empêcher l'affectation entre objets d'une classe</ins>

<br>
   
si vous souhaitez interdire l'affectation entre objets d'un de vos types  
déclarez son `operator=` comme `= delete`

<br>

  
```c++
#include <iostream>
class IntStack {
public:
  IntStack (int s): size(s), top (0), tab (new int [size]) {}  
  void operator= (const IntStack&) = delete;
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
  IntStack st2 (10);
  st1 = st2;
  return 0;
}
```
    
```bash
$ g++ main.cpp
main.cpp: In function ‘int main()’:
main.cpp:4:18: error: use of deleted function ‘IntStack::operator=(const IntStack&)’
    4 |   st1 = st2;
      |                  ^~~
In file included from main.cpp:1:
intstack.h:6:3: note: declared here
    6 |   void operator= (const IntStack&) = delete;
      |   ^~~~~~~~
$ 
```
    
<br>
    
les messages sont très clairs ...
    
</div>

+++

END
