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

# les classes `class`

+++

Nous allons dans ce notebook présenter les types définis par l'utilisateur.

+++

## exemple du code de la pile d'entiers *à-la-c*

+++

<div class = "framed-cell">
<ins class = "underlined-title">exemple du code de la pile d'entiers</ins>

<br>

reprenons notre exemple de la pile d'entier que nous avons programmée dans un `c++` très basique c'est à dire dans un code `à-la-c`

<br>

voici le code de la pile qui
* empile un argument de type entier dans un tableau d'entier passé en argument à la position indiquée
* dépile le haut de la pile c'est à dire l'entier à l'indice indiqué
* et affiche le contenu de la pile (i.e. les éléments empilés non encore dépilés)

<br>

```c++
#include <iostream>
void push (int stack [], int* top, int token) {
  stack[*top] = token; 
  *top = *top + 1;
}
int pop (int stack [], int* top) {
  *top = *top - 1;
  return stack[*top];
}
void print_stack (int stack [], int top) {
  std::cout << '[';
  for (int i = 0; i < top; i++)
    std::cout << stack[i] << ' ';
  std::cout << '[' << std::endl;
}
```
<br>

Que remarquez-vous dans ce code ?

<br>

que les fonctions prennent toutes en argument les deux mêmes objets:
* le tableau d'entier `int stack []`
* et l'indice où empiler le prochain élément dans la pile `int* top`  
(que ce soit son adresse ou sa valeur)

<br>
  
que ces fonctions, alors qu'elles sont censés constituer notre pile d'entier, n'ont *rien à voir entre elles*
* elles ne sont aucunement regroupées dans une même entité de pile
* ce sont des fonctions globales quelconques ...
* alors que dans notre idée elles *travaillent ensemble* i.e. ce sont les fonctions implémentant une pile d'entier

</div>

+++

## exemple de l'utilisation de la pile d'entiers  *à-la-c*

+++

<div class = "framed-cell">
<ins class = "underlined-title">exemple de l'utilisation de la pile d'entiers à-la-c</ins>

<br>

voici l'utilisation du code de la pile d'entiers dans une fonction (ici la fonction `main`)

```c++
#include "stack.h"
int main () {
  int tab [10];
  int top = 0;
    
  print_stack(tab, top);

  push(tab, &top, 71);
  print_stack(tab, top);

  push(tab, &top, -128);
  print_stack(tab, top);

  pop(tab, &top);
  print_stack(tab, top); 

  return 0;
}
```

```bash
$ g++ stack.cpp
$ /a.out
[[
[71 [
[71 -128 [
[71 [
```
<br>

Que remarquez-vous dans ce code ?

<br>
  
que les deux variables censés constituer notre pile d'entier  n'ont *rien à voir entre elles* !

<br>

en effet: le tableau pour la pile d'entiers `tab` et son indice de haut de pile `top`
* ne sont aucunement regroupées dans une même entité de pile  
(oui ok on aurait pu les mettre dans une `struct` `c` mais pas les fonctions)
* ce sont deux variables locales à la fonction `main`
* alors que dans notre idée elles *travaillent ensemble* à la représentation en mémoire d'une pile d'entier

<br>

voila pourquoi `c++` vous permet de définir un **nouveau type de données** en regroupement dans une même définition de **classes** `class`:
* les données qui composent le type appelées les **membre de données** ou les **attributs**
* les fonctions travaillant sur ces données appelées les **fonctions membres** ou les **méthodes**

</div>

+++

## définition d'une `class Integer`

+++

<div class = "framed-cell">
<ins class = "underlined-title">définition d'une `class`</ins>

<br>

nous allons définir dans `c++` le nouveau type `Integer`

<br>

la donnée qui va constituer chaque objet de type `Integer` est un simple `int` qui représentera la valeur de l'`Integer`

<br>

les méthodes qui vont travailler sur les objets de type `Integer` sont 
* `incr` qui augmente la valeur de l'objet de `1`
* `decr` qui diminue la valeur de l'objet de `1`
* `print` qui affiche la valeur de l'objet

<br>
    
```c++
class Integer {

public:
  void incr () {
    value = value + 1;
  }
  void decr () {
    value = value - 1;
  }
  void print () {
    std::cout << value;
  }

private:
    int value = 0;
}; // attention ';' obligatoire !
```

<br>

Que faut-il remarquer ?

<br>

que tout objet de type `Integer` est composé d'un attribut `int` dont la valeur initiale est toujours `0`

<br>

que tout objet de type `Integer` possède trois méthodes `incr`, `decr` et `print`

<br>

que les attributs et les méthodes sont définies *à l'intérieur* de la `class` i.e. entre le `{` et son `};` de la `class`

<br>

que les méthodes **manipulent** (lisent et écrivent)  les **attributs** de la classe sans les redéclarer

<br>

que le `;` en fin de définition de `class` est **obligatoire** (son oubli peut causer parfois des erreurs incompréhensibles) 

<br>

précisons que tous les membres d'une classe attributs et méthodes doivent être déclarés à l'intérieur de la définition de la classe

<br>

une définition de `class` est unique et doit **déclarer** l'ensemble complet des membres de la classe (données et fonctions)  
notons que les méthodes pourront être définies en dehors de la classe mais elles auront été déclarées dans la `class`
  <br>

on reviendra sur `private` et `public`

</div>

+++

## utilisation de notre `class Integer`

+++

<div class = "framed-cell">
<ins class = "underlined-title">utilisation de notre `class Integer`</ins>

<br>

une `class` introduit un nouveau `type` dans le langage `c++`
  
<br>

dans un programme où la `class Integer` a été définie  
le nom de la `class` ici `Integer` devient le nom d'un type

<br>

```c++
// la class Integer est définie
int main () {
    
  Integer i;
  i.print();
    
  i.incr();
  i.print();
    
  i.decr();
  i.print();
    
  return 0;
}
```      

<br>

dans ce programme `i` est une variable, de type `Integer`, locale à la fonction `main`

<br>

vous remarquez que pour appeler les méhodes de la classe `Integer` sur les objets on utilise le `.`  
comme dans `i.print()` ou encore `i.incr()`

<br>


À vous de jouer, essayez d'accéder à l'attribut `value` de l'objet `i` dans la fonction `main`  
compilez votre programme

<br>

```c++
int main () {
  Integer i;
  i.value;
  return 0;
}
```

<br>

et oui `c++` refuse de compiler un tel programme
```bash
$ g++ integer.cpp
integer.cpp: In function ‘int main()’:
integer.cpp:19:5: error: ‘int Integer::value’ is private within this context
   i.value;
     ^~~~~
integer.cpp:14:15: note: declared private here
   int value = 0;
$ 
```

<br>

avez-vous compris pourquoi ?

</div>

+++

## membres `private` et `public` d'une `class`

+++

<div class = "framed-cell">
<ins class = "underlined-title">membres `private` et `public` d'une `class`</ins>

<br>

dans la définition de le `class Integer` vous remarquez les mots clés `public` et `private`

<br>

l'attribut `value` a été mis dans une partie privée `private`  de la `class`

<br>

ainsi seules les méthodes de la `class Integer` pourront toucher (lire et écrire) `value`

<br>

en effet
* `value` est notre manière d'implémenter nos objets de type `Integer`
* on va la garder privée à notre code 
* tout le monde peut voir que les `Integer` sont constitués par un `int value`  
* mais personne ne peut y toucher en dehors des méthodes de la classe

<br>

les méthodes `incr`, `decr` et `print` ont été définies dans la partie publique `public` de la `class`

<br>

afin que tout le monde puissent utiliser ces méthodes sur les objets de type `Integer`

<br>

`c++` vous permet de **conserver** certaines informations privées à votre classe
* lorsque vous ne voulez pas que les utilisateurs de vos classes accèdent à leurs détails d'implémentation
* lorsque vous ne souhaitez pas que les utilisateurs accèdent à certains algorithmes

<br>

**par défaut** les membres d'une `class` sont `private`

</div>

+++

## les constructeurs

+++

<div class = "framed-cell">
<ins class = "underlined-title">les constructeurs</ins>
 
<br>

supposons maintenant que vous désiriez donner une valeur initiale lors de la création d'un objet de type `Integer` 

<br>

vous voulez construire un objet de type `Integer` avec une valeur initiale, faire quelque chose comme

```c++
// la définition de la classe Integer
int main () {
  Integer j;
  Integer i(12);
  i.incr();
  i.print();
  return 0;
}
```
<br>

si nous réfléchissons en regardant ce code  
il s'agit de définir un constructeur de la classe `Integer` qui prenne un `int` en argument  
comme dans `Integer i(12)`
<br>

on voit que la méthode qui se prête bien à cette construction...est la fonction qui a le même nom que la classe

<br>

cette méthode est un constructeur et les constructeurs sont des fonctions membres spéciales

<br>

dans un constructeur on va initialiser les attributs des objets comme ici l'attribut `value` 

<br>

```c++
#include <iostream>
class Integer {
public:
    
  Integer (int v) : value(v) {
  }
    
  void incr () { value = value + 1; }
  void decr () { value = value - 1; }
  void print () { std::cout << value; }
    
private:
  int value = 0;
};

int main () {
  Integer i(12);
  i.print();
  return 0;
}
```
<br>

on remarque la **liste d'initialisation** `value(v)` du constructeur juste avant le corps du constructeur  
`attribut(valeur-initiale)`

<br>

essayez de construire dans le `main` un objet de type `Integer` comme vous le faisiez auparavant  
`Integer j;`

<br>

Que se passe-t-il ? 

<br>

et oui le fait de définir un constructeur qui prend un argument vous a fait *perdre* le constructeur *par défaut*

<br>

c'est normal vous venez de décider que votre classe a un seul constructeur qui prend un argument `c++` ne va pas vous y coller le constructeur par défaut !

<br>

mais vous pouvez définir plusieurs constructeurs dans une classe  
ou vous pouvez mettre des valeurs par défaut aux arguments du constructeur  

<br>

à vous de jouer écrivez le code qui permet de construire des `Integer` dont la valeur initiale, par defaut est  `0` en utilisant les arguments par défaut des fonctions

```c++
void foo (int v = 0) {
}
int main () {
    foo();   // (par défaut) v vaudra 0
    foo(10); // v vaudra 10
```

</div>

+++

<div class = "framed-cell">
<ins class = "underlined-title">liste d'initialisation des constructeurs</ins>
 
<br>

regardez les deux implémentations du constructeur de la classe `Integer`, quelle est la différence ?

<br>

la première implémentation `IntegerGood` utilise la liste d'initialisation, la seconde `IntegerBad` ne l'utilise pas

<br>

```c++
class IntegerGood {
public:
  IntegerGood (int v) : value(v) {
  }
private:
  int value = 0;
};

class IntegerBad {
public:
  IntegerBad (int v) {
    value = v;
  }
private:
  int value = 0;
};
```

<br>

dans la première implémentation  les attributs des objets sont **initialisés** avant d'entrer dans le corps du constructeur

<br>

dans la seconde implémentation, les attributs sont **affectés** dans le corps de la fonction et non initialisés  
pourtant à l'entrée du corps du constructeur de `IntegerBad`, les attributs ont une valeur  
elle est quelconque mais elle existe  
vous avez fait du travail pour rien et votre code est moins lisible

<br>

préférez toujours la liste d'initialisation 

</div>

+++

## pointeur vers un objet de type `Integer`

+++

<div class = "framed-cell">
<ins class = "underlined-title">pointeur vers un objet de type `Integer`</ins>

<br>

pour l'instant nous avons manipulé des objets de type `Integer` comme dans `Integer i;`

<br>

et nous avons accédé aux méthodes des objets par un `.` comme dans `i.incr()`

<br>

naturellement nous pouvons prendre l'adresse d'un objet de type `Integer`

<br>

la variable contenant "l'adresse d'un objet de type `Integer`" sera de type `Integer*`


<br>

on peut donc prendre l'objet à l'adresse `pi` par `*pi` et accéder aux méthodes `(*pi).incr();`


<br>

il ont mis un raccourci à `(*pi).incr();` qui est `pi->incr();`

<br>

ainsi `(*pi).incr();` est équivalent à `pi->incr();` 

<br>

```c++

int main () {
  Integer i;
  Integer* pi = &i;
  pi->incr();
  pi->decr();
  pi->print();
  return 0;
}
```

<br>

utilisez le raccourci !

</div>

+++

## exercice la `class IntStack`

+++

<div class = "framed-cell">
<ins class = "underlined-title">exercice  la `class IntStack`</ins>
    
<br>

à vous de jouer implémentez un `class IntStack`
* ses attributs sont le tableau d'entier, l'indice de la pile et la taille de la pile
* vous passez la taille de la pile au constructeur
* faites un constructeur
* faites les fonctions `pop`, `push` et `print`
* faites les deux fonctions pour tester si la pile est vide ou si la pile est peine

<br>

```c++
class IntStack {
public: 
  IntStack (int size) /* votre code ici */
  void push (int element) /* votre code ici */
  int pop () /* votre code ici */
  bool is_empty () /* votre code ici */
  bool is_full () /* votre code ici */
  void print () /* votre code ici */
private:
  /* vos attributs - données membres ici */
};
```

<br>

pour la tester faites fonctionner ce code  
vous pouvez aussi tester en remplacant, dans votre calculette, la pile par un objet de type `IntStack`

```c++
int main () {
  IntStack s1 (3);
  s1.push(1);
  s1.push(2);     
  int e = s1.pop();
  s1.push(3);
  return 0;
}
```

```bash
$ g++ instack.cpp
$ ./a.out
[ [
[ 1 2 [ 
[ 1 3 [
```

<br>

Que se passe-t-il dans ce code ?

```c++
int main () {
  IntStack s1 (3);
  int e = s1.pop();
  s1.push(3);
  return 0;
}
```

<br>

Que se passe-t-il dans ce code ?

```c++
int main () {
  IntStack s1 (3);
  s1.push(10);
  s1.push(20);
  s1.push(30);
  s1.push(40);
  return 0;
}
```
 <br>
 
 créez des objets de type `IntStack` en mémoire statique, en mémoire dynamique et dans la pile d'exécution  
i.e dans les trois zones de mémoire </ins>

</div>
