---
jupytext:
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

# makefile

+++

Lorsque vous faites de la compilation séparée, vous êtes amenés à recompiler des fichiers lorsque les fichiers dont ils dépendent sont modifiés. La commance `make` et son fichier de règles `makefile` ou `Makefile` va vous permetter d'automatiser le recompilation des fichiers entrant dans la constitution d'un exécutable.

+++

## compilation séparée

+++

<div class="framed-cell">
<ins class="underlined-title">compilation séparée</ins>
  
Rappelez-vous nous avons un programme qui prend une expression arithmétique en notation polonaise inversée comme arguments de la ligne de commande et qui évalue l'expression.

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
puisque dans de gros codes la phase de compilation peut prendre beaucoup beaucoup de temps alors autant ne pas la faire inutilement !  
et puis les `.o` peuvent être utilisés dans d'autres exécutables   

<br>

d'autre part, recompiler *à-la-main* que ce qui est utile, est très  fastidieux et source d'erreur ...

<br>

on sait aussi qu'à partir du moment où on connaît
* les dépendances entre les fichiers (comme par exemple que `rpn_calc.cpp` dépend de `stack.h`)
* comme le système d'exploitation connaît la date de la dernière modification de nos fichiers
* c'est quelque chose qui peut s'automatiser

<br>

dans les années 1970 la commande `make` a été inventée pour cela ... et elle est toujours utilisée malgré son utilisation de la tabulation comme séparateur ... et son grand age


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

un makefile est un fichier de texte utilisé par la commande `make`  
pour exécuter un ensemble d'actions facilitant la compilation et l'édition de liens de vos programes (mais pas que)
  
<br>

un `Makefile` est constitué de **définitions** et de **règles** avec des noms de cibles (`all`, `clean`...)

<br>

pour une **cible**, on va indiquer
* ses **dépendances** c'est à dire les cibles qui doivent être résolues avant de résoudre cette cible
* les **actions** à faire pour résoudre cette cible

<br>

```bash
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

## problème de tabulation

+++

<div class="framed-cell">
<ins class="underlined-title">attention à la tabulation</ins>

les actions sont introduites par une `tabulation` (le fameux caractère ascii `\t`)

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
    
la commande `make` va tenter de réaliser toutes les actions requise par une règle pour accomplir sa cible  
la règle étant mise dans un fichier `makefile`
    
<br>

à vous de jouer, éditez un fichier `makefile` dans le répertoire où se trouvent vos fichiers de la calculette  
et créez-y une cible `clean` qui supprime des fichiers de compilation dont nous n'avons plus besoin  
notons qu'avec `-f` la commande `rm` ignore les fichiers qui existent pas i.e. ne fait pas d'erreur


donc attention avant `rm -f` il **faut** mettre une **tabulation**


>```bash
># dans le fichier makefile
clean:
        rm -f test.o rpn_calc.o stack.o calc
```


<br>

vous remarquez 

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
CXXFLAGS=-Wall -ggdb
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

vous remarquez `$(RM)$` qui est la manière d'accéder à la variable contenant la commande de `rm -f` 
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

tous les fichiers `.o` sont produits de la même manière en appelant le compilateur avec ses options de compilation  
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
# das le fichier makefile
%.o: %.cpp; $(CXX) $(CXXFLAGS) -o $@ -c $<
```    
 
<br>
   

notons que le caractère `%` est appelé un `wildcard` (il matche tout ou une partie d'un nom de fichier)

<br>

comment cela se lit-il ?
* pour réaliser la cible `%.o` (par exemple prenons `%.o` à `rpn_calc.o`), il faut que la cible `%.cpp` existe (donc `rpn_calc.cpp`)


* puis il faut appliquer l'action `$(CXX) $(CXXFLAGS) -o $@ -c $<` où  
  * `$@` est le nom de la cible que vous cherchez à faire (`rpn_calc.o`)  
  * et `$<` le nom de la dépendance donc `rpn_calc.cpp`

</div>

+++

## les règles de dépendances

+++

<div class="framed-cell">
<ins class="underlined-title">les règles de dépendances</ins>
    
<br>
    
la calculette utilise les fonctions de la `stack` (`pop`, `push`...) pour cela, elle inclut le header `stack.h`  
qui déclare ces fonctions

<br>

supposons qu'une fonction vienne à changer, par exemple `print_stack` prend désormais un argument en plus `int* top`

<br>

il faudra naturellement modifier le code de la pile donc recompiler le fichier `stack.cpp`

<br>

afin que `make` sache que si `stack.cpp` a été modifié depuis que `stack.o` a été produit  
on va indiquer la dépendance dans le `makefile` 


```bash
# dans le fichier makefile
stack.o: stack.cpp stack.h
```    

<br>

mais il faudra aussi recompiler le fichier `rpn_calc.cpp` puisqu'il dépend de la pile !

<br>

on va donc indiquer dans les dépendances de `rpn_calc.o`
* qu'il dépend de `rpn_calc.cpp` et aussi de `stack.h`

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
* avoir compilé les fichiers `stack.o`, `rpn_calc.o` et `stack.o`
* donc avoir leur fichier `.cpp` correspondants
* appeler le `linker` en lui donnant un nom pour l'exécutable

<br>

```
# dans le fichier makefile
calc : stack.o rpn_calc.o test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ 
```

comment cette expression se lit-elle ?
* il faut que les dépendances `stack.o`,  `rpn_calc.o` et `test.o` soient vraies  
i.e. que ces 3 fichiers existent
* ensuite le `linker` de nom `$(CXX)` donc `g++` avec les options déjà indiquées est appelé
* il fait l'édition de lien des fichiers `$^` qui sont les dépendances
* et il met l'exécutable dans `$@` qui est le nom de la cible ici `calc`

</div>

+++

## un `makefile` simple

+++

voici un `makefile` très simple

attention si vous coupez le texte de cette cellule, les tabulations deviendront sûrement des espaces quand vous le collerez dans un fichier

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
