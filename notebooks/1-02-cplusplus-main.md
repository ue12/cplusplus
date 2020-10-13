---
jupytext:
  cell_metadata_filter: all,-hidden,-heading_collapsed
  notebook_metadata_filter: all,-language_info,-toc,-jupytext.text_representation.jupytext_version,-jupytext.text_representation.format_version
  text_representation:
    extension: .md
    format_name: myst
kernelspec:
  display_name: Python 3
  language: python
  name: python3

notebookname: point d'entrée
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

+++

# le point d'entrée des programmes `c++`

+++

### *slides*

+++

<ins>le point d'entrée des programmes c++</ins>

*Quand vous exécutez un programme `c++`,  
par quel **endroit du code, l'exécution commence-t-elle** ?*

Que fait `Python` ?

```python
# dans le fichier foo.py
print(42)
```

`Python` fait de l'exécution en mode *inter-actif*


```bash
$ python foo.py
42
```

+++

### *notes*

+++

Vous avez écrit un programme `c++`, vous l'avez compilé avec succès et vous avez obtenu un exécutable. Nous reparlerons de tout cela bien sûr mais pour l'instant demandons-nous: *Quand vous exécutez un programme `c++`, par quel **endroit du code, l'exécution commence-t-elle** ?*

+++

En `Python`, si vous avez, par exemple, un fichier `foo.py` qui contient:

```python
print(42)
```

Et que vous en lancez l'interprétation en faisant:

```bash
$ python foo.py
42
```

`Python` interprète le code pas à pas en effectuant les actions que vous demandez, ici afficher `42` sur la sortie standard.
 
(attention le `$` personnifie le *prompt* de votre système d'exploitation, il ne fait pas partie de la commande !).

+++

## le point d'entrée est unique

+++

### *slides*

+++

<ins>en `c++` le point d'entrée est unique</ins>

Si on fait *pareil* en `c++` qu'en `Python`  
on doit définir la fonction ` main`

+++

### *notes*

+++

En `c++` c'est très différent:, un exécutable doit comporter un **unique point d'entrée**. Dans votre code, pour indiquer ce point d'entrée (qui est donc l'endroit où vous voulez commencer l'exécution de votre programme) vous allez définir une unique fonction globale de nom (standardisé) `main`. 

Allons-y montrons ce que nous devons faire en `c++` pour écrire un code qui fait la même chose que le code `Python` précédent. Créons un répertoire de nom `code-cplusplus` (`mkdir`) déplacons-nous y (`cd code-cplusplus`). Éditons le programme suivant sous `vs-code` et sauvons-le dans le fichier `foo.cpp` dans ce répertoire. `.cpp` indique que notre fichier est un fichier d'implémentation.

>```c++
>// fichier foo.cpp
>#include <iostream>
int main () {
    std::cout << 42 << std::endl;
   return 0;
}
```

On voit plusieurs choses dans le texte de ce programme mais nous allons parler tout d'abord de la fonction `main`.

+++

## la fonction `main`

+++

### **slides**

+++

<ins>On remarque la fonction `main`</ins>

>```c++
>int main () { // pas d'argument (dans cette version)
    ...
    return 0; // retourne un int (code d'erreur c-style)
}
```

une seule `main` est définie dans un programme  
on ne peut pas l'appeler

+++

### *notes*

+++

Cette fonction est un reliquat du `c`. L'entête de la fonction `int main ()` montre qu'elle ne prend pas d'argument `()` (dans cette version là), et qu'elle **retourne un `int`**.

Cet entier servait, à l'origine, à renvoyer des codes d'erreurs de vos programmes vers le système d'exploitation. Celui-ci recevait ce code et vous pouviez alors gérer ces différents cas d'erreurs. Là comme mon code ne fait pas d'erreurs, je l'indique en retournant `0`. C'est une convention, vous pouvez retourner n'importe quoi. Cette technique de gestion d'erreurs de `c` a été *remplacé* en `c++`, par l'ajout d'un système d'exception, un peu comme le `try`, `except` et `raise` de `Python`.

Là on retourne `0` en utilisant l'instruction `return`.

+++

La fonction `main` ne peut pas être appelée dans un programme, elle ne peut qu'être définie. Et naturellement, il ne peut y avoir qu'une seule fonction `main` dans un exécutable.

+++

## l'inclusion de la librairie des entrées/sorties

+++

### *slides*

+++

<ins>inclusion de la librairie des entrées/sorties</ins>

>```c++
>#include <iostream>
```

* `#` : on parle à `cpp` (le `c pré-processeur`)
* `include` : directive qui inclut un segment de code source (dans un autre fichier)
* `<` et `>` : librarie standard
* `iostream` : librarie pour les entrées/sorties

----------

<ins>inclusion de fichiers *utilisateurs*</ins>

`include` + `"` et `"`

>```c++
>#include "foo.h"
```
* il faut que `foo.h` soit accessible (1)

+++

### *notes*

+++

On remarque ensuite:

```c++
#include <iostream>
```

où un `#` est suivi par la commande `include`. Le `#` indique qu'on désire parler à un système qui pré-traite nos programmes `c++` et pas encore au compilateur lui-même. On appelle cela le **c-pré-processeur**. Ici on lui demande d'importer la librairie standard de `c++`, qui gère les entrées/sorties, `iostream`.

La commande `include` du c-pré-processeur, va chercher le fichier qui contient les entêtes des fonctions d'entrées/sorties et l'insère dans le code de notre programme à la place de la commande `#include <iostream>`.

Comment sait-il où aller chercher le fichier ? Là, comme on désire que `c++` aille chercher un fichier de la librairie standard de `c++`, on le lui indique en entourant le nom du fichier par `<>`.

Si c'est un fichier qui n'appartient **pas** à la librarie standard que l'on souhaite inclure, on l'indiquerait en entourant le nom du fichier par `""`. Par exemple `#include "foo.h"` indiquerait qu'on souhaite inclure le fichier `foo.h`. L'extension du fichier est `.h` parce qu'on importe des **fichier d'entête** (*header*) pas des fichiers d'implémentation (ceux qui sont post-fixés par `.cpp`).

Il existe d'autres commandes dans le langage du c-pré-processeur, parmi celles qui sont très utilisées se trouvent l'inclusion de fichiers (`include`) et les commandes pour faire de la compilation conditionnelle `if`, `else`... on en reparlera (peut être) plus tard.

Ces entêtes ainsi inclues, notre code peut utiliser les fonctions de la librairie `iostream`.

+++

(1)
* dans le même répertoire que le fichier contenant l'instruction `#include`
* le long des chemins spécifiés par une option du compilateur `I` (Include)
* ...
* essayer: `gcc -xc -E -v -`

+++

## l'affichage de l'entier `42`

+++

### *slides*

+++

<ins>affichage de l'entier 42</ins>

>```c++
>  std::cout << 42 << std::endl;
```

* `std` : librarie standard de `c++`
* `std::cout` : sortie standard de `c++`
* `std::endl` "caractère de fin de ligne (`\n` en ascii ou encore )

+++

### *notes*

+++

On remarque ensuite 

```c++
  std::cout << 42 << std::endl;
```
`std` (pour *standard*) est le petit nom de la librarie standard de `c++`, cette librairie étend le langage `c++` avec de nombreuses fonctionnalités comme les entrées/sorties. `std` est le nom de l'espace de nommage `namespace` dans lequel est définie La bibliothèque standard. Notez qu'une première façon de *programmer* consiste à utiliser les fonctions prédéfinies dans la bibliothèque standard de `c++`.

`std::cout` est le nom de la sortie standard de `c++`. C'est une sorte de tableau-tampon (ou *buffer*) où vont être stockées les valeurs avant qu'il ne soit *vidé* sur la sortie standard. Remarquez la manière très particulière de `c++` d'afficher `42` qui est de rediriger la valeur dans le tableau-tampon avec la commande `<<`. Una utre sortie est la sortie d'erreur `std::cerr`. C'est pour être cohérent avec les systèmes d'exploitation qui gèrent ces deux sorties différentes: standard et erreur.

`std::endl` est le caractère de fin de ligne qui permet donc de passer à la ligne après l'affichage de `42`. Remarquez qu'après avoir redirigé `42` dans le tableau-tampon de la sortie standard, on y redirige le caractère de fin de ligne qui est aussi connu comme le caractère ascii `\n`.

+++

Nous allons maintenant compiler et exécuter ce programme.

+++

## la compilation + édition de liens d'un programme

+++

### *slides*

+++

<ins>compilation + édition de liens d'un programme</ins>

code objet  
* `g++ -o bar foo.cpp` $\rightarrow$ exécutable de nom `bar`
* `g++ foo.cpp` $\rightarrow$ exécutable de nom `a.out` ou `a.exe`


on remarque qu'on passe des arguments à `g++` sur la ligne de commande comme `-o bar` ou encore `foo.cpp`

+++

### *notes*

+++

Maintenant, dans un terminal `bash`, je **compile** mon fichier. Si je suis sous `windows 10` j'ai installé `WSL` puis le compilateur `g++`, sous `macos` j'ai tapé `g++` et il s'est débrouillé et sous `linus` j'ai utilisé un installateur de package.

```bash
$ g++ foo.cpp -o bar
$ ls
bar foo.cpp
```

J'ai compilé en utilisant la commande `g++` à laquelle j'ai demandé de mettre l'exécutable dans un fichier de nom `bar`, Pour cela j'ai précisé l'option `-o` (*output*) suivie du nom du fichier.

`g++ foo.cpp -o bar`  est ce qu'on appelle une `ligne de commande` (*command line*) et `-o` est appelé un argument de la ligne de commande. On ré-utilisera cette terminologie plus tard dans le cours.

Quand on liste les fichiers de ce répertoire on en voit deux: `foo.cpp` et `bar`.

Et si je n'indique pas le nom de l'exécutable, suivant les operating-systems il va gènérer un nom par défaut 

```bash
$ g++ foo.cpp
$ ls
a.out foo.cpp
```

+++

## l'exécution du programme

+++

### *slides*

+++

<ins>exécution d'un programme</ins>

```bash
$ ./bar
42
$ ./a.out
42
```

`./` parce que mon répertoire courant n'est **pas** trouvé (dans le *chemin* ou encore `PATH` sous linux)

+++

### *notes*

+++

Maintenant, dans un terminal `bash`, je lance l'exécution contenue dans le fichier `bar`.


```bash
$ ./bar
42
```

Le `./` devant `bar` indique que je veux lancer l'exécutable de nom `bar` qui se trouve dans le répertoire courant (celui où on est) qui est `.`.

Pour éviter cela, je dois mettre le répertoire courant `.` dans ma variable d'environnement (sous linux) `PATH`. Ainsi il cherchera `bar` aussi dans le répertoire courant et y trouvera `bar`.

`42` s'est affiché dans mon terminal.

Et si mon exécutable a pris le nom par défaut sous linux/MacOS il s'appelle `a.out` et sous Windows `a.exe`

+++

## compilation seule

+++

### *slides*

+++

<ins>compilation seule d'un programme</ins>

code objet  
* `g++ -c foo.cpp` $\rightarrow$ fichier de code objet `foo.o`
* `g++ -c -o my-foo.o foo.cpp` $\rightarrow$ fichier de code objet `my-foo.o`

besoin d'une édition de lien pour en faire un exécutable

+++

### *notes*

+++

Si nous ne voulons pas créer directement un exécutable, le compilateur de `c++` peut générer un fichier contenant vos instruction `c++` traduites en **code machine** plus tout ce dont `c++` a besoin pour le relier à d'autres fichiers afin d'obtenir un exécutable.

On appelle ces fichiers des fichiers de *code objet*.

Par exemple dans `foo.o` (ou `my-foo.o`) vous allez avoir la définition de la fonction `main` mais les fonctions de la librairie standard sont simplement appelées, leur définitions ne sont pas encore incluses, il faudra pour cela procéder à une édition de lien.

```bash
$ g++ foo.o -o bar
$ ./bar
42
```

+++

## <ins>exercice</ins>: déterminer la norme `c++` que votre compilateur `g++`  utilise

+++

Il existe plusieurs standards de `c++`:
* 199711 pour C++98 (novembre 1997)
* 201103 pour C++11
* 201402 pour C++14
* 201703 pour C++17

Votre compilateur utilise par défaut une de ces normes. Pour savoir laquelle, faites un programme `cmd-std.cpp` qui affiche la valeur de la variable globale `__cplusplus` qui est le nom de la norme utilisée par défaut.

```bash
$ g++ cmd-std.cpp -o cmd
$ ./cmd
201402
```

Un argument de compilation `std=c++xx` permet d'indiquer la norme `xx` que vous voulez utiliser. Compiler votre programme en lui passant, lors de la compilation, une nouvelle norme.

```bash
$ g++ -std=c++11 cmd-std.cpp -o cmd
$ ./cmd
201103
```

+++

## passer des arguments à la fonction `main`

+++

### *slides*

+++

<ins>passer des arguments à la fonction main</ins>

* on passe des arguments quelconques à notre exécutable `bar`
>```bash
>$ g++ -o bar foo.cpp
$ ./bar 1 2 Hello World ! 17.9
42
```

* rien ne se passe mais notre exécutable a *accepté sans rien en faire* les arguments

* on remarque que `g++` traite bien les arguments qui lui sont passés  
--------------------------

* une seconde version `main`  
avec des paramètres pour accéder aux *arguments de la ligne de commande*
>```c++
> int main (int argc, char* argv[]) {
    return 0;
}
```

* `argc` nombre d'arguments sur la ligne (argument-count)
* `argv` tableau de ces arguments sous format de chaînes de caractères (*argument-values*)  
(y compris le nom de l'exécutable)

+++

### *notes*

+++

Reprenez un de vos exécutables, par exemple `bar` (qui affiche `42`) et exécutez le en mettant des valeurs quelconques lors de son appel, comme:

```bash
$ ./bar 1 2 Hello World ! 17.9
42
```

Que se passe-t-il ? Rien de visible: le programme `bar` a fait ce qu'on lui a demandé de faire (afficher `42`), il n'a pas traité les valeurs passées, mais il ne s'est pas plaint de les avoir.

Mais à quoi correspondent donc les valeurs mises à cet endroit là ?

Prenons un autre exemple pour bien comprendre. Quand vous utilisez la commande `g++` vous lui passez:
* le nom du fichier dans lequel se trouve le texte de votre programme
* ou encore avec l'option `-o` vous lui indiquez le nom de l'exécutable
* ou encore vous lui indiquez avec `std=...` la norme que vous voulez qu'il utilise pour compiler votre programme ...

Ces arguments et autres valeurs sont lues et traitées par le programme `g++`, c'est donc qu'il y accède ! ... mais comment-fait-il donc ?

+++

En fait il existe une seconde version de la fonction `main` qui vous permet d'accéder à ces arguments. On les appelle les *arguments de la ligne de commande*.

Cette nouvelle fonction `main` prend deux paramètres:
* le nombre des arguments
* le tableau des valeurs des arguments

```c++
int main (int argc, char* argv[]) {
    return 0;
}
```

* `argc` est le nombre des arguments passés sur la ligne (argument-count)
* `argv` est le tableau des arguments passés sous la forme de chaînes de caractères (*argument-values*)
(y compris le nom de l'exécutable)

Les deux arguments `argc` et `argv` contiennent aussi le nom de votre exécutable. Donc si vous ne donnez aucun arguments lors de l'appel d'un exécutable `argc` sera à `1` et `argv[0]` indiquera la chaîne de caractères qui contient le nom de votre programme.

+++

### <ins>exercice</ins>: afficher le nombre des arguments `argc`

+++

À vous de jouer. Faites un programme `nb-args.cpp` qui affiche le nombre des arguments passés sur la ligne de commande lors de l'appel de l'exécutable.

+++

Compilez et exécutez le programme.

```bash
$ g++ nb-args.cpp -o nb-args
$ ./n-bargs
1
$ ./nb-args 1  2 Hello World !       17.9
7 ```

Remarquez qu'il considère toutes les valeurs: même l'exécutable ! Et sauter plusieurs espaces ou des tabulations entre les arguments ne l'a pas gêné.

+++

### <ins>exercice</ins>: le nom des arguments

+++

Le type de `argv` est `char*[]` c'est à dire que c'est un tableau `[]` de chaînes de caractères `char*` (on y reviendra).

Dans l'appel `$ ./nb-args 1 2 Hello World ! 17.9`, le tableau va contenir les chaînes de caractères suivantes: `"./nb-args"`, `"1"`, `"2"`, `"Hello"`, `"World"`, `"!"`, `"17.9"`.

Les arguments seront donc fournis, dans le tableau `argv` à partir de la case `argv[0]` avec `argv[0]` qui sera la chaîne de caractères `"./nb-args"`, `argv[1]` qui sera `"1"` ...

Comment a-t-il séparé les arguments ? En tenant compte des caractères s'apparentant à des caractères blancs (un ou plusieurs ' ', des tabulations...).

À vous de jouer. Faites un programme de nom `args-first-last.cpp` qui affiche le premier et le dernier argument de manière à ce que le programme soit vrai quelque soit le nombre d'arguments.

i.e. faites en sorte que le programme n'accède pas à des choses qui n'existent pas ! Par exemple si vous ne passez aucun argument après que le nom de l'exécutable ... et bien seul `argv[0]` existe ! `argv[1]` il n'existe pas !  et si vous essayez quand même d'y accédez votre exécution peut très mal se passer ...

On remarque que `c++` a mis tous les objets dans des chaînes de caractères: les entiers, les flottants, les chaînes...
