---
jupyter:
  jupytext:
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
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

```python
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# les `templates`


Nous allons parler de modèles de classe ou encore *templates*.


nous vous rappelons que les notebooks servent à vous donner des explications suivies de morceaux de code que
vous pouvez copier/coller dans vos éditeurs (VSC) et compiler dans votre terminal.


`c++` est un langage *fortement* typé qui vous demande de définir le type d'un objet pour pouvoir l'utiliser


## on écrit une fonction très simple

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">on écrit une fonction très simple</ins>

<br>

supposons qu'on vous demande d'implémenter une fonction pour diviser deux entiers
    
<br>
    
facile ! vous écrivez le code suivant
    

```c++
int my_div (const int & a , const int & b ) {
  return a/b ;
}
```

vous remarquez
* les arguments sont passés `const`, puisque la fonction `my_div` ne doit pas les modifier (autant le vérifier)
* les arguments sont passés par référence `&` afin d'éviter à  `c++` de les copier... 
    
<br>
    
Les références ne vous semblent pas utiles ?   
* pour des objets de type `int`, effectivement cela pourrait se discuter (d'autant plus qu'on crée ici des références, donc des *indirections*...)  
* mais nous voulons faire du code "générique" aussi nous pensons à d'autres types que les `int`...
* par exemple le produit de deux grosses matrices
* vous devez absolument éviter que la fonction, pour les multiplier, ne les recopie en mémoire avant !  
ce serait un non sens-informatique...
    
<br> 

vous vestez votre code

```c++
// in file main.cpp
#include <iostream>

int my_div (const int & a , const int & b ) {
  return a/b ;
}

int main () {
  std::cout << my_div(10, 3) << std::endl;
  return 0;
}
```

```bash
$ g++ main.cpp
$ ./a.out
3
```
la fonction marche super bien vous êtes très content...
    
</div>    
<!-- #endregion -->

## on désire étendre notre code

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">on désire étendre notre code</ins>

<br>
    
on vous demande alors de faire une fonction qui divise deux `double`...  
`int`  et `double` sont des types numériques, vous essayez votre fonction:
    
```c++
#include <iostream>

int my_div (const int & a , const int & b ) {
  return a/b ;
}

int main () {
   std::cout << div(10., 3.) << std::endl;
   return 0;
}
```
    
```bash
$ g++ main.cpp
$ ./a.out
3
g++ 
```
    
<br>
    
votre fonction fait naturellement une division entière, vous devez donc implémenter une nouvelle fonction  
vous le faites et vous l'appelez    

```c++
#include <iostream>

int my_div (const int & a , const int & b ) {
  return a/b ;
}

double my_div (const double & a , const double & b ) {
  return a/b ;
}

int main () {
  std::cout << my_div(10, 3) << std::endl;
  std::cout << my_div(10., 3.) << std::endl;
  return 0;
}
```

```bash
$ g++ main.cpp
$ ./a.out
3
3.33333
```

<br>
    
vous remarquez:
* que `c++` accepte que deux fonctions portent le même nom (c'est de la surcharge i.e. de l'*overloading*)  
* en effet `c++` arrive parfaitement à les discriminer par leurs arguments  
(nombre ou type, ici type)
* c'est ce qu'on appelle l'**overloading** (i.e. **surcharge**)
    </div>
<!-- #endregion -->

## on désire étendre de nouveau notre code

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">on désire étendre de nouveau notre code</ins>

<br>
    
maintenant on vous demande de diviser `short` des `long int` ...
    
```c++
int my_div (const int & a , const int & b ) {
  return a/b ;
}
double my_div (const double & a , const double & b ) {
  return a/b ;
}
short my_div (const short & a , const short & b ) {
  return a/b ;
}
long int my_div (const long int & a , const long int & b ) {
  return a/b ;
}
 
```
    
bon vous commencez à trouver votre code super redondant...  
et vous remarquez que le code ne diffère que par les types des objets  
à part cela les fonctions sont semblables
    
<br>
    
en programmation, vous ne devez surtout **pas** écrire de code redondant  
vous devez réutiliser le code existant  

<br>
    
d'autant plus que quand vous utilisez le *copier/coller*  
vous pouvez faire des erreurs *stupides* qui prennent un temps fou à être corrigées  
c'est classique en programmation
    
<br>
    

```c++
#include <iostream>

long int my_div_bad ( const int & a , const long int & b ) {
  return a/b ;
}

long int my_div_good ( const long int & a , const long int & b ) {
  return a/b ;
}

int main () {
  // i is the biggest `long int` if `long int` are 8 bytes long
  long int i = 9223372036854775807;
  std::cout << "we take the largest long int (on 8 bytes) " << i << std::endl;
  std::cout << "result when my_div_bad is applied: " << my_div_bad(i, 2) << std::endl;
  std::cout << "result when my_div_good is applied: " << my_div_good(i, 2) << std::endl;
  return 0;
}
```
    
```bash    
$ g++ main.cpp ; ./a.out 
we take the largest long int (on 8 bytes) 9223372036854775807
result when my_div_bad is applied: 0
result when my_div_good is applied: 4611686018427387903
```

un copier/coller raté ...

<br>

il existe dans `c++` un mécanisme pour éviter la redondance **inutile** d'un code
    

</div>
<!-- #endregion -->

## `template` un nouveau mécanisme de `c++`


<div class = "framed-cell">

<ins class = "underlined-title">`template` un nouveau mécanisme de `c++`</ins>

<br>
    
il existe dans `c++` un mécanisme pour éviter la redondance **inutile** des codes
    
<br>
    
lors de l'implémentation d'un code  
* vous déclarez qu'un ou plusieurs types sont de *simples* **arguments** `T`, `U`  
(pour les arguments de templates, on utilise par convention des majuscules)
* vous utilisez ce ou ces types pour écrire votre code
* vous obtenez un *modèle de code* que `c++` pourra ensuite utiliser sur des types réels

<br>
    
<br>

au moment de la compilation
* `c++` va tout d'abord générer *automatiquement* le code nécessaire
* il le fera à partir du modèle que vous avez indiqué
* puis il va compiler le code ...
    
<br>
    
cette possibilité offerte par `c++` a fortement participé au succés du langage 
    
<br>
    
elle est aussi à la base de toute la librairie standard de `c++` encore appelée la **STL** pour Standard Template Librarie
    
<br>
    
vos codes gagnent de *tous les côtés*
* vitesse d'exécution des codes spécifiques générés et compilés
* grande lisibilité du code (avec un peu d'habitude et il faut aimer les `<` et `>`)

</div>


## exemple d'une fonction `template` 

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">exemple d'une fonction `template`></ins>
    
<br>

implémentons un modèle de fonction qui compare deux nombres de types quelconques et de types potentiellement différents
    
```c++
template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;
}
```
    
<br>
    
vous n'implémentez plus la comparaison de deux `int`, `long int`, `double`... mais de deux objets de type `T` et `U`
    
<br>
 
au moment de la compilation
* `c++` ne va générer que le code nécessaire à partir des types reconnus
    
<br>
    
    
```c++
#include <iostream>

template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;
}
int main () {
  std::cout << std::boolalpha;
  std::cout << less_than(10, 12) << std::endl;
  std::cout << less_than(10, 10.1) << std::endl;
  std::cout << less_than('b', 'a') << std::endl;

  return 0;
}
```
                                               
```bash
$ g++ main.cpp ; ./a.out 
true
true
false
```
    
`c++` a *génèré* `less_than` sur
* `const int` et `const int`
* `const int` et `const double`
* `const char` et `const char`   

<br>
    
Voilà vous avez compris l'idée de base des templates
* vous écrivez du code que le compilateur va utiliser pour produire un autre code en instanciant (ici) des types  
(c'est parfois appelé de la méta-programmation)
</div>

<!-- #endregion -->

## problèmes avec les `templates`


<div class = "framed-cell">

<ins class = "underlined-title">problème avec les `templates`</ins>
    
<br>

la fonction `template` que vous implémentez est un modèle de fonction
* qui prend des types en argument
* qui applique, sur des objets des types passés en argument des opérations  
 (par exemple `<=` dans notre exemple précédent)
    
<br>
    
il y a plusieurs erreurs possibles, la fonction `template` instanciée sur un type
* ne fait pas ce que vous attendiez
* échoue i.e. elle ne compile pas, aucun exécutable n'est produit
</div>


### la fonction `template` instancée ne fait pas ce qui est attendu

<!-- #region -->
   
<div class = "framed-cell">

<ins class = "underlined-title">la fonction `template` instanciée ne fait pas ce qui est attendu</ins>
    
<br>
    
une fois la fonction `template` instanciée, le résultat peut ne pas être ce que vous attendiez !  
    
    
<br>
    
que se passe-t-il dans le code suivant ?  
que compare donc `c++` ?
    
<br>
    
```c++
// in file main.cpp
#include <iostream>

template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;
}


int main () {
  std::cout << std::boolalpha;
  std::cout << less_than("a", "b") << std::endl;
  return 0;
}
```
                                               
```bash
$ g++ main.cpp
$ ./a.out
false
```

</div>
<!-- #endregion -->

### la fonction `template` instanciée ne compile pas

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">la fonction `template` instanciée ne compile pas</ins>
    
<br>
    
l'opération appliquée dans la fonction `template` peut ne pas être définie sur les types que vous passez à la fonction  

    
<br>
    
`c++` va alors refuser de compiler votre code

<br>
    
    
```c++
#include <iostream>

template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;

class X {
public:
  X (int value) : value(value) {}
private:
  int value;
};

int main () {
  X x(1), y(2);
  std::cout << less_than(x, y) << std::endl;
  return 0;
}
```
        
```bash
$ g++ main.cpp
main.cpp: In instantiation of ‘bool less_than(const T&, const U&) [with T = X; U = X]’:
main.cpp:17:30:   required from here
main.cpp:5:12: error: no match for ‘operator<=’ (operand types are ‘const X’ and ‘const X’)
    5 |   return a <= b ;
      |          ~~^~~~
```

<br>

ce mécanisme de génération de code à la compilation est extrêmement puissant

</div>
<!-- #endregion -->

## comment résoudre les deux problèmes précédents ?

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">comment résoudre les deux problèmes précédents ?</ins>
    
<br>
    

1) si vous voulez que votre fonction `template` fonctionne correctement sur les chaînes de caractères  
vous devez écrire une fonction spécifique pour ce type
    
                                   
```c++
#include <iostream>
#include <cstring>  // pour strcmp

template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;
}

bool less_than (const char* a, const char* b) {
  return strcmp(a, b) < 0;
}

int main () {
  std::cout << std::boolalpha;
  std::cout << less_than("a", "b") << std::endl;
  return 0;
}
```

```bash
$ g++ main.cpp
$ ./a.out
true
```

   
<br>
    
2) si vous voulez que la fonction `template` puisse fonctionner sur des objets de type `X`  
ce type doit redéfinir l'`operator<=`

```c++
#include <iostream>
#include <cstring>

template <typename T, typename U>
bool less_than (const T & a, const U & b ) {
  return a <= b ;
}

class X {
  friend bool operator<= (const X&, const X&);
public:
  X (int value) : value(value) {}
private:
  int value;
};

bool operator<= (const X& rx, const X& ry) {
  return rx.value <= ry.value;
}

int main () {
  std::cout << std::boolalpha;
  
  X x(1), y(2);
  std::cout << less_than(x, y) << std::endl;
  return 0;
}
```

```bash
$ g++ main.cpp
$ ./a.out
true
```
 
</div>                                     
<!-- #endregion -->

## exemple de `class` `template`

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">exemple de `class` `template`</ins>
    
<br>
    

de même que les fonctions peuvent devenir des modèles de fonctions, les classes peuvent devenir des modèles de classes

<br>

voici un tout petit exemple, il s'agit d'une classe `Toy`
* composée d'un élément de type `T`
* dans laquelle on définit la méthode `play` qui affiche un message puis l'objet de type `T`
* ainsi que la méthode `rest` qui affiche un message puis l'objet de type `T`

<br>

```c++
// in file toy.h
#pragma once
#include <iostream>

template <typename T>
class Toy {
public:
  Toy (T element): element(element) {}
    
  void play () {
    std::cout << "Playing with " << element << std::endl;
  }
  
  void rest () {
    std::cout << "Resting with " << element << std::endl;
  }
    
private:
  T element;
};

```

<br>

pour utiliser cette classe, on va devoir indiquer à `c++` sur quel type on veut l'instancier    
i.e. par quoi le type `T` doit être remplacé dans chacun des objets qu'on définit

<br>

pour le faire on va indiquer le (ou les) type entre `<` et `>` pour le type `template` `Toy`

<br>

par exemple

```c++
#include "toy.h"

int main () {
  Toy<const char*> t1("c++");
  t1.play();
  Toy<int>* pt2 = new Toy<int>(12);
  pt2->rest();
  return 0;
}
```

on compile et on exécute
    
```bash
$ g++ toy.cpp
$ ./a.out
Playing with c++
Resting with 12
```

<br>

Que se passe-t-il dans la classe ?

* la classe est une classe `template` dont l'argument de type s'appelle `T`
* cet argument apparaît comme le type d'un attribut dans la définition de la classe `T`  
et comme argument du constructeur

<br>

Que se passe-t-il dans la fonction `main`
* on crée, dans la pile d'exécution, une variable `t1` de type `Toy` où le type `template` `T` a été instancié par `const char*`  
on passe la chaîne de caractères `"c++"` au constructeur  
on appelle la fonction `play` sur l'objet `t1`
* on alloue, dans le tas, une variable de type `Toy` où le type `template` `T` a été instancié par `int`  
on passe `12` au constructeur  
on appelle la fonction `rest` sur cette variable, à travers le pointeur `pt2`

<br>

pour ce code `c++` ne va définir que les méthodes que vous utilisez donc naturellement
* le constructeur `Toy` pour la classe `Toy` où `T` qui vaut `const char*`
* le constructeur `Toy` pour la classe `Toy` où `T` qui vaut `int`
* la méthode `play` pour la classe `Toy` où `T` est `const char*`
* la méthode `rest` our la classe `Toy` où `T` est `int`
    
...
    
    
* mais aussi le destructeur `Toy` pour la classe `Toy` où `T` qui vaut `const char*`
* et pas de destructeur `Toy` pour la classe `Toy` où `T` qui vaut `int` puisque nous ne faisons pas `delete pt2`

    
<br>
    
`c++` génére les fonctions dont vos objets ont besoin, qu'elles soient utilisées explicitement ou implicitement

<br>
    
ce mécanisme de `template` diffère complètement de la compilation classique des programmes  
il intervient avant la compilation classique  
il construit et compile votre code à la volée

<br>
    
    
en fait, vous écrivez des codes qui permettent de générer d'autres codes  
(meta programmation)
    
</div>
<!-- #endregion -->

## ADVANCED - parlons de syntaxe et d'`inline`

<!-- #region -->
<div class = "framed-cell">

<ins class = "underlined-title">ADVANCED - parlons de syntaxe et d'`inline`</ins>
    
<br>

vous pouvez définir les méthodes en dehors de la classe

```c++
// in file toy.h
#prgama once
#include <iostream>
template <typename T>
class Toy {
public:
  Toy (T);
  void play ();
  void rest ();
private:
  T element;
};

template <typename T>
inline Toy<T>::Toy (T element): element(element) {}

template <typename T>
inline void Toy<T>::play () {
  std::cout << "Playing with " << element << std::endl;
}
    
template <typename T>
inline void Toy<T>::rest () {
  std::cout << "Resting with " << element << std::endl;
}
```

<br>

naturellement, pour pouvoir générer du code, `c++` doit avoir le modèle de ce code  
ici nous mettons la définition de la classe `template`  `Toy` dans un fichier d'entête

<br>

et naturellement dès que vous définissez des méthodes en dehors de leur classe dans un fichier de header    
vous devez spécifier qu'elles sont `inline`  


<br>

si vous ne spécifiez pas  que les méthodes sont `inline` vous pouvez voir apparaître des `multiple defined`  
si le fichier d'entête est inclus dans plusieurs fichier d'implémentation  
qui sont compilés séparemment

</div>
<!-- #endregion -->

## exercice la classe `Stack` `template`


<div class = "framed-cell">

<ins class = "underlined-title">exercice la classe `Stack` `template`</ins>
    
<br>

reprenez le code de votre `IntStack` qui est un conteneur actuellement défini pour des `int`

<br>

naturellement le mécanisme  de ce conteneur est le même quel que soit le type d'objets empilés

<br>

faites une classe `template` sur le type des objets contenus dans la pile

</div>


