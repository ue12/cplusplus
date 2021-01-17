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
notebookname: "point d'entr\xE9e"
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

https://code.visualstudio.com/docs/cpp/config-linux

+++

# faire un projet et debugger en `vs-code`

+++

très rapidement pour ceux qui ne l'ont pas encore fait, nous allons
1. utiliser `vs-code` pour créer un projet dont le `makefile` est généré automatiquement (ouf)
1. debugger une `segmentation fault` qui apparaît en lançant l'exécutable généré par notre projet

+++

## les deux fichiers 

+++

créer un répertoire `mon-projet` et mettez dedans les deux fichiers ci-dessous, il s'agit d'une minuscule `intlist` avec un très gros bug  
ou plutôt un gros bug avec une petite `intlist` autour

+++

file `intlist.h`

```c++
#pragma once

class IntCell
{
  friend class IntList;

private:
  IntCell(int value,
          IntCell *prev = nullptr,
          IntCell *next = nullptr) : value(value),
                                     prev(prev),
                                     next(next) {}

  int value;
  IntCell *prev;
  IntCell *next;
};

class IntList
{

public:
  IntList() : head(nullptr),
              tail(nullptr) {}

  void add_front(int e)
  {
    IntCell *front = new IntCell(e, nullptr, head);
    head->prev = front;
    head = front;
  }

private:
  IntCell *head;
  IntCell *tail;
};
```

+++

file `main.cpp`
```c++
#include <iostream>
#include <cstdlib>
#include "intlist.h"

void test_add_front()
{
  std::cout << "test add front one element with value";
  IntList list;
  int e1 = rand();
  list.add_front(e1);
}

int main()
{
  test_add_front();
  return 0;
}
```

+++

## on crée le projet et on compile

+++

lancer `vs-code` dans le répertoire où se trouvent les deux fichiers

+++

installez l'extension pour le debugging de `c/c++`
* dans les extensions tapez `c/c++` et installez le package `C/C++ IntelliSense, debugging, and ... microsoft`

+++

installez l'extension pour générer les `makefile`
* dans les extensions tapez `c/c++ makefile project` et installez le package `C/C++ Makefile Project adriano-markovic.c-cpp-makefile-project de Adriano Markovic`

+++

ouvrez le fichier `main.cpp` dans `vs code`

ne pas oublier de se mettre dans un fichier `cpp` sinon le contexte sera différent et vous ne trouverez pas les bonnes commandes dans la palette ... 

+++

ouvrez la `command palette` (menu `view`)

tapez `make` dans la `command palette`

choississez `C/C++ Make:INIT Projet`

puis choisissez `C++ Projet`

+++

il apparaît un `Makefile` dans votre éditeur ! (que vous devez comprendre un petit peu)

vous allez le customizer pour votre propre projet en en modifiant les variables suivantes:

* `APPNAME = test-intlist` pour changer le nom de l'exécutable (de l'application) 
* `SRCDIR = ./src` pour changer le nom du répertoire des fichiers source les `.h` et les `.cpp`
* `OBJDIR = ./obj` pour changer le nom du répertoire où seront mis les binaires

dans les options de compilation rajoutez `-g` (pour le debug)
* `CXXFLAGS = -std=c++11 -Wall -g`

+++

ouvrez un terminal dans `vscode`

+++

vous devez être dans votre répertoire `mon-projet` 

vous voyez deux nouveaux répertoire `obj` et `src` à côté du fichier `Makefile`

déplacez les deux fichiers `intlist.h` et `main.cpp` dans le répertoire `src`

```bash
$ mv intlist.h main.cpp src
```

+++

lancez la commande `make` sans cible (elle fera sa première cible)

```bash
$ make
```

si vous avez `g++ fatal error no input file` c'est que vous n'avez pas mis les fichiers `main.cpp` et `intlist.h` dans le répertoire `src`

+++

regardez ce qui a été généré
* les `.o` sont dans le répertoire `obj`
* l'exécutable porte le nom choisi `test-intlist` il se trouve directement dans le répertoire du projet
* regardez le fichier `main.d` qui indique les dépendances pour le code objet du fichier `main.cpp`

+++

## on exécute

+++

ouvrez un terminal et lancez l'exécutable généré

```bash
$ ./test-intlist
Segmentation fault (core dumped)
```

on va maintenant debugger le code pour trouver la provenance de la segmentation-fault

+++

si vous souhaitez tout bien recompiler, allez dans le terminal, tapez `make clean` et `make`

+++

## on installe le debugger `gdb`

+++

allez dans le menu `Run` et faites `Start Debugging`, la palette s'ouvre et vous choisissez `C++ (GDB/LLDB)`, il apparaît un fichier `launch.json`

dans ce fichier, après `"configuration": [`, trouvez la ligne

```
"program": "enter program name, for example ...",
```

et remplacez `"enter ..."` par `"./test-intlist"`


vous ve nez de dire à votre projet quel exécutable vous voulez debugger

+++

sauvez le `launch.json`

+++

## on utilise le debugger

+++

on va exécuter notre programme sous le debugger pour trouver d'où provient l'erreur

re-exécuter `Start Debugging` du menu `Run`

+++

là une fenêtre rouge foncé apparaît elle dit qu'une `exception has occured` qui est une  `Segmentation Fault` (quelque chose s'est clairement mal passé...)

l'exécution est arrêtée sur une ligne (repérée par une flèche jaune à gauche)

la fenêtre rouge foncé souligne un morceau de code surligné en un jaune (terne tirant vers le vert)

ce rectangle accuse une ligne de code (on est en plein thriller, on va savoir qui est le coupable) 

c'est `head->prev = front;` !

mais pourquoi ?

on met la souris sur le `head`, un petit texte nous donne la valeur de `head` ... c'est `0x0` (c'est à dire en hexadécimal `0x` la valeur `0` donc le `nullptr` 

mais oui on fait `nullptr->prev` et `c++` n'a pas aimé !

mais comment en est-on arrivé là dans ce code ? 

+++

pour le savoir, en bas à gauche vous voyez écrit `CALL STACK`

oui, c'est la pile des appels de fonctions qui ont mené à l'erreur !

cliquez sur `main()` vous allez voir surlignée en vert la ligne `test_add_front();`

cliquez sur `test_add_front();` vous allez voir apparaître `list.add_front(e1);`

on s'approche, cliquez sur `IntList::add_front(...)` vous allez voir re-apparaître la ligne qui déclenche l'erreur

+++

## on arrête de debugger

+++

maintenant vous voyez en haut au milieu (vers l'endroit ou apparaît normalement la palette) une petite frise d'icones qui concernent votre debugger avec


* un triangle bleu qui dit `Continuer` (quand on passe la souris dessus)

* 3 flèches pour `Step Over`, `Step into` et `Step Over` concernant l'exécution pas-à-pas

* le restart

* le stop (carré orange)

cliquez sur la flèche bleue ou le stop

+++

## les variables et la call-stack

+++

à gauche dans le bandeau qui contient les icones de l'explorer et des extensions (les 4 carrés), cliquez sur le triangle pointe vers la droite qui s'appelle `Run` (quand on passe la souris dessus)

si vous cliquez dessus après avoir lancé `Run` `Start Debugging` vous avez des menus
* avec les `VARIABLES` (dont vous pouvez voir la valeur)
* `WATCH` qui permet lors d'une exécution pas-à-pas de suivre les valeurs de variables
* la `CALL STACK`

+++

## les breakpoints

+++

vous pouvez mettre un point d'arrêt
* il arrête l'exécution à un endroit de votre code
* pour vous permettre de continuer pas-à-pas en regardant la valeur de vos variables

+++

pour mettre un `beakpoint` dans le texte d'un fichier
* cliquez à gauche de la colonne des numéros des lignes du fichier
* un point rouge apparaît

+++

stoper l'exécution courante de votre debugger

mettez un breakpoint dans le fichier `main.cpp` devant la ligne `test_add_front();`

relancer le debug

la flèche jaune s'arrête sur le breakpoint

vous pouvez alors utiliser les 3 flèches `Step Over`, `Step into` et `Step Out` pour vous balader dans le code pas-à-pas
* sur une ligne qui contient un appel de fonction
* `Step Over` l'exécute d'un coup
* `Step into` entre dans la fonction
* `Step Out` termine la fonction et remonte

faites `Step Into` pour entrer dans la fonction `test_add_front`

puis `Step Over` pour passer l'affichage etc...

essayer aussi `Watch` ...

+++

il y a d'autres manière de faire des projets, celle-ci est plutôt facile et directe
