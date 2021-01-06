---
jupytext:
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

# problème de l'inclusion multiple d'un fichier d'entête

+++

## exercice: on inclut plusieurs fois un fichier d'entête lors d'une compilation

+++

<div class="framed-cell">
<ins class="underlined-title">exercice: on inclut plusieurs fois un fichier d'entête lors d'une compilation</ins>

<br>
    
1. mettez dans un fichier d'entête `file.h`, les définitions
    * d'une fonction `foo` (qui ne fait rien)
    * de la classe `X` et de sa méthode `X::bar` (qui ne fait rien)
    * définissez `bar` en dehors de la classe
    
-----    
    
2. créez un fichier `test.h` et incluez le fichier `file.h` dedans 
   
-----    
    
3. définissez dans le fichier `test.h` une fonction `void test ()` qui
    * appelle la fonction `foo`
    * crée un objet `x` de type `X`
    * appelle la méthode `bar` de cet objet
    
-----
    
4. créez un fichier `main.cpp` et incluez les fichiers `file.h` et `test.h` dedans

-----
    
5. définissez dans le fichier `main.cpp` la fonction `main` qui
    * appelle la fonction `test` définie dans le fichier `test.h`
    * appelle la fonction `foo` définie dans le fichier `file.h`
    * crée un objet `x` de type `X` définie dans le fichier `file.h`
    * appelle la méthode `bar` de cet objet
    
<br>
 
compilez le fichier `main.cpp`
    
```bash
$ g++ main.cpp
???
```
       
<br>
    
    
Vous constatez un problème qui arrive plusieurs fois et la compilation échoue  !
    
<br>
    
Comprenez-vous ce qui se passe ?

    
</div>

+++

## le code concerné

+++

<div class="framed-cell">
    
<ins class="underlined-title">le code concerné par les erreurs</ins>
    
    
afin de reproduire l'erreur nous vous donnons le code
    
<br>
    
le fichier `file.h` 
    
```bash
void foo () {
}

class X {
public:
  void bar ();
};

void X::bar () {
}
```
    
<br>

le fichier `test.h` 
    
```bash
#include "file.h"

void test () {
  foo();
  X x;
  x.bar();
}
```
    
<br>
    
     
le fichier `main.cpp` 
    
```bash
#include "file.h"
#include "test.h"

int main () {
  test();
  foo();
  X x;
  x.bar();
  return 0;
}
```
    
<br>

+++

## erreur de redéfinition de fonctions

+++

<div class="framed-cell">
    
<ins class="underlined-title">erreur de redéfinition de classes et de fonctions</ins>    
    

<br>
    
`c++` déclenche des erreurs de `redefinition of` pour: la fonction globale `foo`, la classe `X` et sa méthode `bar` et 
    
```bash
$ g++ main.cpp 
In file included from test.h:1,
                 from main.cpp:2:
file.h:1:6: error: redefinition of ‘void foo()’
    1 | void foo () {
      |      ^~~
In file included from main.cpp:1:
file.h:1:6: note: ‘void foo()’ previously defined here
    1 | void foo () {
      |      ^~~
In file included from test.h:1,
                 from main.cpp:2:
file.h:4:7: error: redefinition of ‘class X’
    4 | class X {
      |       ^
In file included from main.cpp:1:
file.h:4:7: note: previous definition of ‘class X’
    4 | class X {
      |       ^
In file included from test.h:1,
                 from main.cpp:2:
file.h:9:6: error: redefinition of ‘void X::bar()’
    9 | void X::bar () {
      |      ^
In file included from main.cpp:1:
file.h:9:6: note: ‘void X::bar()’ previously defined here
    9 | void X::bar () {
      |      ^
```
  
</div>

+++

## Que se passe-t-il ?

+++

<div class="framed-cell">
    
<ins class="underlined-title">Que se passe-t-il ?</ins>    
    

<br>
    
lors de la compilation du fichier `main.cpp`, `c++`:
* **inclut** le fichier `file.h` qui **définit** la fonction globale `foo`, la classe `X` et sa méthode `bar`
* **inclut** le fichier `test.h` qui **inclut** `file.h` (puisqu'il veut utiliser `foo` et `X`)
* le fichier `file.h` inclus **définit de nouveau**  la fonction globale `foo`, la classe `X` et sa méthode `bar`
    
<br>
    
vous remarquez alors que les redéfinitions proviennent d'un **seul et unique fichier** `file.h`
    
<br>

un **même** fichier a été inclus **deux fois** lors de la **même** compilation  
celle du fichier `main.cpp`
    
<br>
    
pour éviter ce problème, il suffit de vérifier que, lors d'une compilation, un même fichier d'entête ne soit inclus qu'une seule fois  
à la première inclusion, on marque qu'il a été inclus  
aux futures inclusions on refuse de l'inclure de nouveau
    
<br>
    
on utilise pour cela les directives du pré-processeur de `c++` (celui dont les directives commencent par `#`)
* dans un fichier d'entête, on teste si il a déjà été inclus
* si il ne l'a pas été on laisse l'inclusion se faire
* si il a déjà été inclus, on évite l'inclusion
    
</div>

+++

## les *vieilles*  directives `ifndef`-`endif` et `define` du  pré-processeur

+++

<div class="framed-cell">
    
<ins class="underlined-title">les *vieilles* directives `ifndef`-`endif` et `define` du  pré-processeur</ins>    
    

<br>
   
c'est la manière *ancienne* de faire
    
<br>
    
pour chaque fichier d'entête, on choisit un nom de variable spécifique au fichier  
(i.e. qu'on espère *unique*)

<br>
    
en début du fichier d'entête, on demande au pre-processeur de `c++` de tester si cette variable existe   
(ifndef signifie If Not Defined)
    
```c++
#ifndef NOTRE_NOM_UNIQUE_H
```
    
<br>
    
si elle n'existe pas, on la définit et on fait suivre cette définition par le code du fichier  
et par la fin du `ifndef` qui est `endif`

```c++
#ifndef NOTRE_NOM_UNIQUE_H
#define NOTRE_NOM_UNIQUE_H

// le code du fichier ci dessous

#endif    
```   

<br>
    
si cette variable existe alors on ne fait rien
    
<br>
    
ainsi le fichier sera inclus une seule fois par compilation
    
<br>
    
à la fin de la compilation, cette variable m'existe plus
    
<br>
    
notons que la variable est locale à une compilation faite à un instant donné
    
<br>

il existe une manière beaucoup plus moderne de faire ! 
   
</div>

+++

## la *nouvelle* directive `#pragma once`

+++

<div class="framed-cell">
    
<ins class="underlined-title"> la *nouvelle* directive `#pragma once`</ins>
    
<br>
    
avec la directive `#pragma once`  
vous demandez au pré-processeur de `c++` de n'inclure le fichier d'entête que lorsque c'est nécessaire  
sinon d'ignorer la directive `#include`
    
<br>
    
```c++
#pragme once
// le code du fichier ci dessous
```   
    
<br>
    
cela évite une gestion à-la-main avec `indef`, `define` et `endif` de l'inclusion des fichiers d'entête
    
<br>
    
et surtout, cette méthode évite le problème du choix d'un nom *unique*  
puisque si ce nom a déjà été défini dans un autre cadre, alors votre code va générer des problèmes de `refefinition` et ne va plus compiler...
    
<br>    

le mécanisme `#pragma once` est défini au niveau du compilateur et non du pré-processeur  
il sera ainsi beaucoup plus rapide que la gestion faite par le pré-processeur avec `ifndef/define/endif`
    
<br>
    
ce mécanisme n'est pas normalisé mais il existe désormais dans la plupart des compilateurs
    
  
</div>

+++

## exercice: essayez les deux manières de faire

+++

<div class="framed-cell">
    
<ins class="underlined-title">exercice: essayez les deux manières de faire</ins>
    
  
pour vérifier mettez les gardes dans le fichier `file.h`  
compilez et faites l'édition de lien pour obtenir un exécutable     
<br>
    
la manière `#include guard macro`
    
```c++
#ifndef FILE_H
#define FILE_H

void foo () {
}

class X {
public:
  void bar ();
};

void X::bar () {
}
    
#endif
```
    
<br>
    
la manière `#pragma once`

```c++
#pragma once

void foo () {
}

class X {
public:
  void bar ();
};

void X::bar () {
}
```
<br>

```bash
$ g++ main.cpp
$ ./a.out
$
``` 
</div>

+++

## exercices: faites de même dans vos propres fichiers d'entête

+++

<div class="framed-cell">
    
<ins class="underlined-title">exercices: faites de même dans vos propres fichiers d'entête</ins>
    
le sujet de l'exercice est dans le titre
    
</div>
