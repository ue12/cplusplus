<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12-MINES-ParisTech</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>
</div>

```python
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

#  Calculatrice notation polonaise inversée 


## A l'issue de la séance 


L'objectif de cet exercice est qu'à l'issue de la séance vous ayez un programme c++ vous permettant d'évaluer une expression arithmétique fournie en notation polonaise inversée en argument à votre programme.


## Pré-requis 

La notation polonaise inversée (**R**everse **P**olish **N**otation) est une notation mathématique dans laquelle les opérateurs suivent leurs opérandes et dans laquelle vous n'avez pas besoin de parenthéser des sous-expressions.

Par exemple l'expression : 

$$1+2*(3-1)$$ 

se traduit en notation polonaise inversée de la manière suivante : 

$$ 1\, 2\, 3\, 1\, -\, *\, +$$

ou encore l'expression :

$$2\ *\ 5\ +\ 4\ /\ 2$$

se traduit en notation polonaise inversée de la manière suivante :

$$2\, 5\, *\, 4\, 2\, /\, +\,$$


## Travail à faire

<!-- #region -->
L'objectif du TP est donc que vous fassiez un programme C++ auquel vous fournissez en argument une expression en notation polonaise inversée, qui évalue l'expression et qui l'affiche.

Par exemple, $3 + 5$ en notation infixe s'écrit $3\, 5 \, +$ en notation polonaise inversée, sur cette expression, notre programme `rpn_cpp` s'appelle de la manière suivante :

```bash 
$ ./rpn_cpp 3 5 +
8
```
et affiche la valeur `8`



Notez que les termes de l'expressions doivent être séparés par des espaces, ainsi ne donnez pas `3 5+` mais bien `3 5 +`.

Une autre précision, la multiplication sera indiquée par l'opérateur `x` et non `*` (pour éviter les problèmes d'interprétation du `*` dans le bash)

```bash 
$ ./rpn_cpp 3 5 x
15
```

Encore une précision, le moins unaire sera indiqué par l'opérateur `!` et non `-`.

```bash 
$ ./rpn_cpp 3 5 ! x
-15
```
<!-- #endregion -->
## Quelques explications


### lire des arguments de la ligne de commande

<!-- #region -->
Comme vous devez passer des arguments à votre programme, vous allez utiliser la fonction `main` qui lit les arguments i.e. : 

```c++
int main(int argc, char* argv[]){
   // sûrement du code ici
   return 0;
}
```

Où
* `argc` est le nombre des token sur la ligne de commande
* et `argv` un tableau de taille `argc` qui contient les adresses des chaînes de caractères passées sur la ligne de commande.

Ainsi, quand vous appelez le programme de cette manière :
```bash
./rpn_cpp 3 5 ! x
```

* `argc` vaut `5`
* et `argv` est le tableau `["./rpn_cpp", "3", "5", "!", "x"]`, donc la partie qui nous intéresse est la partie `["3", "5", "!", "x"]`
<!-- #endregion -->
### analyser l'expression donnée en argument


Dans un premier temps vous allez considérer que **les expressions données en argument au programme sont bien formées** i.e. elles ne comportent pas d'erreurs. Quand vous avez terminé cette version, nous vous invitons à réfléchir aux différentes erreurs possibles mais pas avant.
<!-- #region -->
**Comment analyser une expression donnée sous la forme d'un tableau de ses tokens ?**

Un token étant
* soit une chaîne de caractères représentant une opérande (ici un entier)
* soit une chaîne de caractères représentant un des $5$ opérateurs

Notez que vous allez devoir tranformer une chaîne de caractères qui contient des `char` entre `0` et `9` en un entier. Pour cela, nous vous conseillons d'utiliser la fonction `atoi` de la librairie `cstdlib` (une librairie qui provient de `c`).

Nous allons analyser l'expression `["3", "5", "!", "x"]`:

Le premier token est l'opérande `"3"`, je ne sais encore pas quoi en faire, j'en profite pour le transformer en un entier, et je le garde *quelque part*.

Le second token est l'opérande `"5"`, je ne sais encore pas quoi en faire, j'en profite pour le transformer en un entier, et je le garde *quelque part*.

Le troisième token est l'opérateur `"!"`. C'est le moins unaire donc il s'applique à une seule opérande. À quelle opérande ? À la dernière que nous avons lue, donc `5`. L'opérateur remplace, la valeur de l'opérande `5` par `-5`.

Le quatrième token est l'opérateur `"x"`. C'est le multiplier binaire donc il s'applique à deux opérandes. Ca tombe bien, il nous en reste deux ! L'opérateur est appliqué aux deux opérandes, que nous n'avons plus besoin de garder, par contre il faut garder le résultat donc on met `-15` *quelque part*.

Et il n'y a plus de token ! L'évaluation de l'expression est terminée et je retourne `-15`.

Plusieurs remarques à ce moment là. Déjà nous avons besoin de fonctions, comme celle qui prend un token et qui dit si ce token est une opérande, ou celle qui prend un token et qui dit si ce token est un opérateur.

Ensuite quelle genre de structure avez-vous besoin pour stoker vos entiers durant l'analyse ?
* on doit pouvoir y mettre un entier
* et on doit pouvoir reprendre le dernier entier qui y a été mis

Ca ressemble à une pile ! Pour la faire, vous allez utiliser un tableau d'entier et vous allez implémenter deux fonctions: `push` (empiler) et `pop` (dépiler) auxquelles vous allez passer en argument, entre autre, le tableau d'entier.

Nous terminerons en vous demandant d'écrire du code propre (bien indenté, avec des identificateurs bien choisis) et de bien structurer votre code en fonctions. Enfin nous vous donnons la fonction `main`:

```c++
// dans le fichier rpn_main.cpp
#include <iostream>

// votre code ici

int main (int argc, char argv []) {
    std::cout << rpn_eval(argc, argv[1]);
    return 0;
}
```
<!-- #endregion -->
