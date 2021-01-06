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

# makefile

+++

https://www.gnu.org/software/make/manual/make.html

+++

Lorsque vous faites de la compilation séparée, vous êtes amenés à recompiler des fichiers lorsque les fichiers dont ils dépendent sont modifiés.

Par exemple, si vous modifiez la fonction `main` du fichier `main.cpp`, le `main.o` pré-existant à cette modification n'est plus à jour ou du moins ne comportera pas votre modification.

La commance `make` et son fichier de règles `makefile` ou `Makefile` va vous permetter d'automatiser le recompilation des fichiers entrant dans la constitution d'un exécutable, parce que ca devient très vite difficile de ne pas se tromper en le faisant *à-la-main*.

+++

## exemple de compilation séparée

+++

<div class="framed-cell">
<ins class="underlined-title">exemple de compilation séparée</ins>
  
<br>

vous avez un programme qui
* prend une expression arithmétique entière en notation polonaise inversée (en argument de la ligne de commande) et qui évalue l'expression.
* et qui se sert pour l'évaluation d'une pile d'entiers
<br>

Pour cela, nous avons les fichiers suivants:
* le code d'une pile d'entiers dans les fichiers `stack.h` et `stack.cpp`
* le code d'une calculette en notation polonaise inversée, avec son fichier de déclaration `rpn_calc.h` et son fichier d'implémentation `prn_calc.cpp`  
cette calculette utilise la pile d'entier et pour cela doit inclure le fichier `stack.h` (contenant les déclarations des fonctions de la pile)
* le fichiers de test `test.cpp`  
ce fichier utilise la calculette et pour cela inclut le fichier `rpn_calc.h`

<br>

pour produire mon exécutable *à-la-main* je peux
* tout compiler ensemble

```bash
$ g++ stack.cpp rpn_calc.cpp test.cpp
```

* ou je peux faire de la compilation séparée

```bash
$ g++ -c stack.cpp
$ g++ -c rpn_calc.cpp
$ g++ -c text.cpp
$ g++ stack.o rpn_calc.o test.o
```

<br>

maintenant quand je modifie un fichier, je ne veux **pas** tout recompiler !  
je ne veux recompiler que ce qui est obligatoire  

<br>

d'une part, dans de gros codes la phase de compilation peut prendre beaucoup beaucoup de temps  
alors autant ne pas la faire inutilement !

<br>

d'autre part, les `.o` peuvent être utilisés dans d'autres exécutables   

<br>

et enfin, recompiler *à-la-main* que ce qui est utile, est très  fastidieux et source d'erreur ...

<br>

on sait aussi qu'à partir du moment où
* on connaît les dépendances entre les fichiers (comme par exemple que `rpn_calc.cpp` dépend de `stack.h`)
* sachant que le système d'exploitation connaît la date de la dernière modification de nos fichiers
* c'est quelque chose qui peut s'automatiser !

<br>

dans les années 1970 la commande `make` a été inventée pour cela ... et elle est toujours utilisée  
malgré son choix de la tabulation comme séparateur ...


<br>

dans un fichier `makefile` vous allez mettre les règles pour compiler votre programme  
la commande `make` va permettre de réaliser ces règles

<br>

nous allons voir rapidement un embryon de `makefile` et l'utilisation de la commande `make`
</div>

+++

## qu'est ce qu'un `makefile`

+++

<div class="framed-cell">
<ins class="underlined-title">introduction minimaliste au `Makefile`</ins>

<br>

un `makefile` est un fichier de texte utilisé par la commande `make`  
pour exécuter un ensemble d'actions facilitant la compilation et l'édition de liens de vos programes (mais pas que)
  
<br>

un `makefile` est constitué de **définitions** et de **règles** avec des noms de cibles (`all`, `clean`...)

<br>

pour une **cible**, on va indiquer
* ses **dépendances** c'est à dire les cibles qui doivent être résolues avant d'essayer de résoudre cette cible
* les **actions** à faire pour résoudre cette cible

<br>

```c++
cible-1: dependance-1 ... dependance-m
    action-1
    ...
    action-n
```

<br>

cela se lit de la maière suivante:
* pour faire la cible `cible-1`
* les cibles `dependance-1` ... `dependance-m` doivent avoir été faites avant
* une fois qu'elles sont faites on fait les actions `action-1` ... `action-n` dans l'ordre
* si tout se passe bien notre `cible-1` est considérée comme faite
<br>
</div>

+++

## le problème de la tabulation

+++

<div class="framed-cell">
<ins class="underlined-title">attention à la tabulation</ins>

les actions sont introduites par une `tabulation` (le fameux caractère ascii '`\t`')

<br>

ainsi, une cible est 

```bash
cible-1: dependance-1 ... dependance-m
<tabulation>action-1
<tabulation>...
<tabulation>action-n
```

<br>

attention quand vous coupez/collez des actions où quand vous tapez la tabulation dans votre éditeur  
la tabulation peut être transformée en des espaces (genre 8)  
la commande `make` fera alors des erreurs dans le genre  

```bash
$ make
makefile:3: *** missing separator (did you mean TAB instead of 8 spaces?).  Stop. 
```
<br>

ce sera malheureusement le cas pour `vs-code` regardez  
là https://stackoverflow.com/questions/34937092/why-does-visual-studio-code-insert-spaces-when-editing-a-makefile-and-editor-in/56060185  
et/ou là https://github.com/microsoft/vscode/issues/8227

<br>

nous vous conseillons de configurer votre `vs-code` de la manière indiquée

+++

## la commande `make`

+++

<div class="framed-cell">
<ins class="underlined-title">la commande `make`</ins>
    
la commande `make` va essayer de résoudre une cible
    
<br>
    
pour cela, elle tente de réaliser toutes les actions requise par la règle décrivant la cible  
   
<br>
    
la ou les règles étant mises dans un fichier `makefile`
    
<br>

à vous de jouer
* éditez un fichier `makefile` dans le répertoire où se trouvent vos fichiers de la calculette (ou autre projet)
* créez-y une cible `clean` qui supprime des fichiers de compilation dont nous n'avons plus besoin    
notons qu'avec `-f` la commande `rm` ignore les fichiers qui existent pas i.e. ne fait pas d'erreur

<br>
    
donc attention avant `rm -f` il **faut** mettre une **tabulation**


```bash
# dans le fichier makefile
clean:
        rm -f test.o rpn_calc.o stack.o calc
```



<br>

pour demander à faire l'action `clean`, dans un terminal faites  

```bash
$ make clean
rm -f test.o rpn_calc.o stack.o calc
```

<br>

pour regarder ce que ferait une action sans la faire faites `-n`   

```bash
$ make -n clean
rm -f test.o rpn_calc.o stack.o calc
```

<br>

enfin `make` par défaut réalise la première action du fichier `makefile`  
donc l'action `clean` dans notre cas

```bash
$ make
rm -f test.o rpn_calc.o stack.o calc
```

</div>

+++

## les définitions

+++

<div class="framed-cell">
<ins class="underlined-title">définitions dans le `makefile`</ins>

+++

<div class="framed-cell">
<ins class="underlined-title">les définitions</ins>

<br>

on peut mettre des définitions dans un fichier `makefile` pour introduire
* les diverses commandes utilisées (comme le compilateur, le linker, la commande de suppression de fichiers ...)
* les diverses options de compilation ou de link
  
<br>

```bash
# dans le fichier makefile
CXX=g++
CXXFLAGS=-Wall -g
RM=rm -f

clean:
        $(RM) test.o rpn_calc.o stack.o calc
```
<br>

ici `CXX` sera le compilateur que vous voulez utiliser  
et `CXXFLAGS` les options passées lors de la compilation ici
* `-ggdb` l'option de debug (on y reviendra)
* `-Wall` l'option pour que le compilateur vous donne *tous les warnings possibles*

<br>

vous remarquez `$(RM)$` qui est la **manière** d'accéder à la variable contenant la commande de `rm -f` 
</div>

+++

## la règle de compilation

+++

<div class="framed-cell">
<ins class="underlined-title">la règle de compilation</ins>

le `makefile` doit indiquer comment compiler les fichiers i.e. comment produire les `.o` à partir des `.cpp`

<br>

cette règle doit indiquer que pour produire le fichier `rpn_calc.o` nécessaire à mon exécutable  
je dois compiler le fichier `rpn_calc.cpp`

<br>

tous les fichiers de code objet `.o` sont produits de la même manière en appelant le compilateur avec ses options de compilation  
i.e. `g++ -Wall -ggdb -c`


<br>

on va donc indiquer dans notre `makefile` une règle globale de compilation et pas une règle par fichier `rpn_calc.cpp`, `stack.cpp`...

<br>

l'idée est donc de pouvoir utiliser la commande `make` pour générer la cible `rpn_calc.o`   
de la même manière que pour la cible `stack.o`  
ou encore `test.o` 

```bash
$ make rpn_calc.o
g++ -Wall -ggdb -c rpn_calc.cpp
```
```bash
$ make stack.o
g++ -Wall -ggdb -c stack.cpp
```
```bash
$ make test.o
g++ -Wall -ggdb -c test.cpp
```

<br>

donc pour généraliser une telle règle, dans le fichier `makefile` nous allons faire  

```bash
# dans le fichier makefile
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -o $@ -c $<
```    
 
<br>

```bash
# dans le fichier makefile
%.o: %.cpp; $(CXX) $(CXXFLAGS) -o $@ -c $<
```    
 
<br>
   

notons que le caractère `%` est appelé un `wildcard` (il matche tout ou une partie d'un nom de fichier)

<br>

comment cela se lit-il ?
* pour réaliser la cible `%.o` (par exemple prenons `%.o` égal à `rpn_calc.o`), il faut que la cible `%.cpp` existe (donc que `rpn_calc.cpp` existe)


* puis il faut appliquer l'action `$(CXX) $(CXXFLAGS) -o $@ -c $<` où  
  * `$@` est le nom de la cible que vous cherchez à faire (`rpn_calc.o`)  
  * et `$<` le nom de la dépendance donc `rpn_calc.cpp`  
  (la ou les dépendances sont à droite du signe `:` de la règle et avant le `;`)

</div>

+++

## les règles de dépendances

+++

<div class="framed-cell">
<ins class="underlined-title">les règles de dépendances</ins>
    
<br>
    
la calculette utilise les fonctions de la `stack` (`pop`, `push`...) pour cela, elle inclut le header `stack.h` qui déclare ces fonctions

<br>

supposons qu'une fonction vienne à changer, par exemple `print_stack` prend désormais un argument en plus `int* top`

<br>

il faudra naturellement modifier le code de la pile et recompiler le fichier `stack.cpp`

<br>

afin que `make` sache si `stack.cpp` a été modifié depuis que `stack.o` a été produit  
on va indiquer la dépendance dans le `makefile` 


```bash
# dans le fichier makefile
stack.o: stack.cpp stack.h
```    

<br>

mais il faudra aussi recompiler le fichier `rpn_calc.cpp` puisqu'il dépend de la pile !

<br>

on va donc indiquer dans les dépendances de `rpn_calc.o` qu'il dépend de `rpn_calc.cpp` et aussi de `stack.h`

```bash
# dans le fichier makefile
rpn_calc.o: rpn_calc.cpp stack.h rpn_calc.h
```    
      
si un fichier d'implémentation inclut son propre fichier d'entête  
il faut le rajouter dans les dépendances

<br>

ainsi le `makefile` va indiquer toutes les règles de dépendances entre les fichiers  
afin d'indiquer les fichiers qui doivent être recompilés lorsque d'autres fichiers ont été modifiés

<br>

à vous de jouer, mettez les dépendances dans votre `makefile` avec les bons noms de vos fichiers

</div>

+++

## l'exécutable

+++

<div class="framed-cell">
<ins class="underlined-title">les règles de dépendances</ins>
    
    
<br>

il nous reste maintenant à faire l'édition de lien de tous les fichiers `.o` c'est à dire de `stack.o`, de `rpn_calc.o` et `test.o`

<br>

donc pour faire l'exécutable, il faut
* avoir les fichiers `stack.o`, `rpn_calc.o` et `stack.o`
* ou les refaire et dans ce cas avoir leur fichier `.cpp` correspondant
* appeler le `linker` en lui donnant un nom pour l'exécutable

<br>

```
# dans le fichier makefile
calc : stack.o rpn_calc.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ 
```

comment cette expression se lit-elle ?
* il faut que les dépendances `stack.o`,  `rpn_calc.o` et `test.o` soient vraies et *à jour*  
  sinon il les refera
* ensuite le `linker` de nom `$(CXX)` donc `g++` avec les options déjà indiquées est appelé
* il fait l'édition de lien des fichiers `$^` qui sont la liste des fichiers `.o` mis comme dépendances de la règle
* et il met l'exécutable dans `$@` qui est le nom de la cible ici `calc`

</div>

+++

## un `makefile` simple

+++

voici un `makefile` très simple

attention si vous coupez le texte de cette cellule, les tabulations deviendront sûrement des espaces quand vous le collerez dans un fichier    
il faudra le vérifier ...

<div class="framed-cell">
<ins class="underlined-title">le ficher makefile</ins>

```bash
CXX=g++
CXXFLAGS=-Wall -ggdb
RM=rm -f

calc : stack.o rpn_calc.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ 

stack.o : stack.cpp stack.h
rpn_calc.o : rpn_calc.cpp rpn_calc.h stack.h
test.o : test.cpp rpn_calc.h

%.o:%.cpp; $(CXX) $(CXXFLAGS) -o $@ -c $<
                                             
clean :
	$(RM) test.o rpn_calc.o stack.o calc
```

+++

ensuite vous pourrez faire des makefiles plus compliqués, par exemple en répartissant les sources d'un programme entre plusieurs répertoires, ou pour générer une librarie pour distribuer votre code:
* `src` les fichiers d'implémentation et headers *locaux*
* `include` les fichiers d'entête (header) que l'extérieur doivent accéder
* `build` les fichiers objets `.o`
* `lib` les librairies

+++

## un `makefile` un peu plus compliqué

+++

vous pouvez regarder le TP sur le `snake` où
* les sources sont distribuées entre les répertoires `src` et `include`, `build` et `lib`
* les sources construisent une librarie (`libsnake.a`)
* ... et vous poserez vos questions par e_mail

+++

```bash
CXX=g++
CXXFLAGS=-Wall -ggdb -Iinclude
AR=/usr/bin/ar -r 

src := $(wildcard src/*.cpp)
obj := $(subst src, build, $(src:.cpp=.o))


.PHONY: directories

all: directories libsnake.a snake 
	@echo $(obj)

snake: main.cpp libsnake.a
	$(CXX) $(CXXFLAGS) $^ -o $@ 

libsnake.a: ${obj}
	${AR} $@ $^

build/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<


directories: build
build:
	mkdir -p $@


clean: 
	rm snake
	rm libsnake.a
	rm -r build
```

+++

## exercices

+++

<div class="framed-cell">
<ins class="underlined-title">exercices</ins>

mettez dans un même répertoire les fichiers de la calculette plus le `makefile` et testez avec la commande `make`:

<br>

on regarde ce que fera la commande par défaut

```bash
$ make -n
make -n
g++ -Wall -ggdb -o stack.o -c stack.cpp
g++ -Wall -ggdb -o rpn_calc.o -c rpn_calc.cpp
g++ -Wall -ggdb -o test.o -c test.cpp
g++ -Wall -ggdb stack.o rpn_calc.o test.o -o calc
```

<br>

qui est la même que `make cacl`, on l'exécute

```bash
$ make
g++ -Wall -ggdb -o stack.o -c stack.cpp
g++ -Wall -ggdb -o rpn_calc.o -c rpn_calc.cpp
g++ -Wall -ggdb -o test.o -c test.cpp
g++ -Wall -ggdb stack.o rpn_calc.o test.o -o calc 
```

<br>

on essaie de la refaire sans rien toucher


```bash
$ make
make: 'calc' is up to date.
```

<br>

on supprime le fichier `stack.o` et on demande à refaire l'exécutable

```bash
$ rm stack.o
$ make
g++ -Wall -ggdb -o stack.o -c stack.cpp
g++ -Wall -ggdb stack.o rpn_calc.o test.o -o calc 
```

il ne recompile que `stack.cpp`  et il refait l'édition de lien

<br>

on modifie le fichier `stack.cpp`   
(ou on le `touch` c'est à dire qu'on change sa date  
il est alors plus récent que son `.o`)

```bash
$ touch stack.cpp 
$ make
g++ -Wall -ggdb -o stack.o -c stack.cpp
g++ -Wall -ggdb stack.o rpn_calc.o test.o -o calc 
```

il refait de même `stack.o` et l'édition de lien

<br>

si on modifie `test.cpp` il recompile `test.cpp` et refait l'édition de lien

<br>

enfin si on touche `stack.h`  
pour faire semblant d'avoir modifié les prototypes des fonctions

```bash
$ touch stack.h
$ make
g++ -Wall -ggdb -o stack.o -c stack.cpp
g++ -Wall -ggdb -o rpn_calc.o -c rpn_calc.cpp
g++ -Wall -ggdb stack.o rpn_calc.o test.o -o calc 
```

il refait bien tout 

</div>

+++ {"tags": ["level_intermediate"]}

## `phony`

+++ {"tags": ["level_intermediate"]}

https://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile

quand les targets ne sont pas des noms de fichiers  
si un fichier porte, par hasard, ce nom dans le répertoire (concerné par la commande) `make` les confond et ne va pas faire la cible !

`make` doit comprendre qu'il doit lancer la commande sans condition de date du fichier homonyme  

ces commandes seront déclarées `.PHONY`

```make
.PHONY: clean

clean:
  rm -rf *.o
```

i.e. une cible `phony` est considérée comme toujours *out-of-date*   
    d'exemple sont `all`, `install`, `clean`, 'check ...

+++

## exercice: mettez un `makefile` dans chacun de vos projets

+++

<div class="framed-cell">
    
<ins class="underlined-title">exercice: mettez un `makefile` dans chacun de vos projets</ins>
    
le sujet du TP est dans le titre   
    
</div>
