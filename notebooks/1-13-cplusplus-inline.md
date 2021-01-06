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

# les fonctions *classiques* et les fonctions `inline`

+++

## les fonctions classiques

+++

<div class="framed-cell">
    
<ins class="underlined-title">les fonctions et méthodes *classiques*</ins>

<br>
    
définissons deux fonctions une fonction globale `foo` et la méthode `bar` de la classe `X`  
et appelons ces deux fonctions dans la fonction `main`
    
```c++
void foo () {
}

class X {
public:
  void bar ();
};

void X::bar () {
}

int main () {
  foo();
  X x;
  x.bar();
  return 0;
}

```    
<br>
    
 
pour appeler ces fonctions, le systeme d'exploitation de votre ordinateur va:  
* **rechercher** la définition de la fonction (i.e. aller à son adresse en mémoire)
* **empiler** les arguments de la fonction (si elle en a) sur la pile d'exécution
* **exécuter** la fonction dans cet environnement (qui possède les arguments passés à la fonction)
* enfin, à la terminaison de la fonction, **dépiler** les arguments de la pile d'exécution


`c++` fait cela que la fonction soit une méthode de classe ou une fonction globale
    
<br>
 
avec les définitions ci-dessus, les fonctions `foo` et `X::bar` ont une **adresse en mémoire**
    
</ins>

+++

## exercice

+++

<div class="framed-cell">
<ins class="underlined-title">exercice</ins>
    
<br>
    
1. mettez dans un fichier d'entête `file.h`, les définitions
    * de la fonction `foo`
    * de la classe `X`
    * de la méthode `X::bar` en dehors de la classe (comme dans l'exemple précédent)
    * protégez votre fichier d'entête contre l'inclusion multiple avec `#pragma once`

-----
    
2. créez un fichier `test.cpp` qui définit une fonction `test` qui
    * appelle la fonction `foo`
    * crée un objet `x` de type `X`
    * appelle la méthode `bar` de cet objet
    * ce fichier doit inclure le fichier `file.h`

-----

3. créez un fichier `test.h` qui déclare la fonction `test`
    
-----

4. créez un fichier `main.cpp` qui inclut le fichier `file.h` et le fichier `test.h` 

-----
    
6. définissez dans le fichier `main.cpp` la fonction `main` qui
    * appelle la fonction `foo`
    * appelle la fonction `test`
    * crée un objet `x` de type `X`
    * appelle la méthode `bar` de cet objet
    
<br>
 
compilez séparemment les fichiers `test.cpp` et `main.cpp`
    
```bash
$ g++ -c main.cpp
$ g++ -c test.cpp
$
```


<br>

linkez les fichiers `test.o`  et `main.o`

```bash
$ g++ main.o test.o
???
``` 
    
<br>
    
    
Vous constatez un problème ! de `multiple defined` de fonctions et de méthodes
    
<br>
    
Comprenez-vous ce qui se passe ?

    
</div>

+++

## un exemple de code pour l'exercice

+++

<div class="framed-cell">
<ins class="underlined-title">un exemple de code</ins>

<br>

le fichier `file.h` 
    
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
-----
    
<br>

le fichier `test.h`
    
```c++
void test ();
```
-----

<br>
    
le fichier `test.cpp` 
    
```c++
#include "file.h"

void test () {
  foo();
  X x;
  x.bar();
}
```
-----
    
<br>

le fichier `main.cpp`
    
```c++
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
-----

+++

## `multiple definition` de fonctions et méthodes

+++

<div class="framed-cell">
    
<ins class="underlined-title">`multiple definition` de fonctions et méthodes</ins>

<br>
    
    
```bash
$ g++ -c main.cpp
$ g++ -c test.cpp
$ g++ main.o test.o
/usr/bin/ld: test.o: in function `foo()':
test.cpp:(.text+0x0): multiple definition of `foo()'; main.o:main.cpp:(.text+0x0): first defined here
/usr/bin/ld: test.o: in function `X::bar()':
test.cpp:(.text+0xc): multiple definition of `X::bar()'; main.o:main.cpp:(.text+0xc): first defined here
collect2: error: ld returned 1 exit status
$
```   

    
<br>
    
   
lors du *link* des deux fichiers `main.o` et `test.o` pour générer un exécutable
* `main.o` contient la définition de la fonction `foo` et de la méthode `bar`
* `test.o` contient la définition de la fonction `foo` et de la méthode `bar`
* au link on a deux `foo`  et deux `X::bar` pour un exécutable ... c'est trop !   

    
<br>
    
la première solution est de ne pas définir de fonctions dans un fichier d'entête ...  
    
    
<br>
    
faire donc comme pour la fonction `test()` qui est 
* définie dans un fichier d'implémentation
* déclarée dans un fichier d'entête (pour pouvoir l'utiliser ailleurs)
* donc compilée dans un code objet qui ne sera que dans `test.o`
 
    
<br>
    
ou déclarer des fonctions `inline`    
</div>

+++

## les fonctions `inline`

+++

<div class="framed-cell">
    
<ins class="underlined-title">les fonctions `inline`</ins>

<br>
    
en `c++` si vous déclarez une fonction comme `inline`   
à chaque appel de la fonction `c++`  remplace l'appel par le corps de la fonction
    
   
    
<br>

et ceci que la fonction soit une méthode de classe ou une simple fonction globale


<br>
    
le nouveau fichier `file.h`
    
```c++
#pragma once

inline void foo () {
}

class X {
public:
  void bar ();
};

inline void X::bar () {
}
    
```
<br>

il y a plusieurs bénéfices
1. on évite le surcoût dû à l'appel de la fonction
1. aucun code n'est généré pour une fonction `inline`
1. les fonctions `inline` sont utilisées lors de la compilation
1. et n'existent plus après
    
<br>
    
mais alors ! pas de corps $\Rightarrow$ pas de `multiple definition`
    
<br>
    
en déclarant les fonctions `foo` et `X::bar` `inline` dans `file.h`  
aucun code ne sera généré pour ces deux fonctions  
c'est la solution à votre problème


<br>
    
maintenant si nous recompilons séparemment `main.cpp` et `test.cpp`  
avec le nouveau fichier `file.h` (i.e. qui contient les déclarations en `inline` de `foo`  et `X::bar`),  
tout se passe bien !
    
```bash
$ g++ -c main.cpp
$ g++ -c test.cpp
$ g++ main.o test.o
$ ./a.out
$
```
    
    
</div>

+++

## exercice

+++

<div class="framed-cell">
<ins class="underlined-title">exercice</ins>
    
<br>
    
dans la classe `X` du fichier `file.h`  
définissez une nouvelle méthode `gee` que vous placez dans le bloc de définition de la classe
sans la déclarer `inline`
    
<br>
    
le fichier `file.h` 
    
    
```c++
#pragma once

inline void foo () {
}

class X {
public:
  void bar ();
  void gee () {
  }
};

inline void X::bar () {
}

```

<br>
    
vous pouvez appeler la méthode `gee`  sur l'objet `x` dans la fonction `main` du fichier `main.cpp` et dans la fonction `test` du fichier `test.cpp` 
    
    
<br>
    
compilez séparemment les fichiers `main.cpp` et `test.cpp`
    
<br>
    
linkez les ...
    
<br>
    
Que se passe-t-il ? 
    
<br>

lors du link, il n'y a pas d'erreur de multiple définition ...  

<br>
    
par défaut les méthodes de classe définies dans le bloc de la classe (et non en dehors) sont `inline` 
    
</div>

+++

## fonction `inline`  dans un fichier d'implémentation

+++

<div class="framed-cell">
    
<ins class="underlined-title">fonction `inline`  dans un fichier d'implémentation</ins>
    
<br>
    
    
essayez dans le fichier `test.cpp`  de déclarer que la fonction `test` est `inline`

<br>
    
recompilez vos fichiers
    
<br>
    
que se passe-t-il ? vous avez lors du link une erreur disant `undefined reference to test()`

    
    
<br>
    
la fonction `test` est `undefined` !
    
<br>

en effet, si vous définissez une fonction comme `inline` dans un fichier d'implémentation  
comme celle-ci n'aura pas de corps, elle n'existera pas dans le code produit  
donc vous ne pourrez pas l'utiliser à l'extérieur du fichier d'implémentation 
    
</div>

+++

## exercice: mettez `inline` et dans un `.h` les méthodes de vos classes

+++

<div class="framed-cell">
    
<ins class="underlined-title">exercices: faites de même dans vos propres fichiers d'entête</ins>
    
faites le a minima pour un de vos fichiers d'entête comme `intstack.h` ou `intlist.h`
    
</div>
