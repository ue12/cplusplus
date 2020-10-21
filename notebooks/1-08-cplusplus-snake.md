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

#  SNAKE c++ --- version Old-School !!


## A l'issue de la séance 

<!-- #region -->
L'objectif est qu'à l'issue de la séance vous-soyze capable de de jouer à Snake comme illustré ci-dessous. 


<video width="600" controls src="media/snake.mp4" />
<!-- #endregion -->

## Pré-requis 

Pour réaliser cet exercice il a volontairement été choisi de réaliser un programme excessivement simple, si si je vous promets c'est simple. 

Cela se traduit par : 

* Uniquement des tableaux statiques à la C 
* Pas de dépendances externes, la partie graphique se fait uniquement dans le terminal à coup de `std::cout` et de `clear`

Étant d'une grande mansuétude je vous ai préparé un squelette de code : 

Dans le dossier `src` les fichiers `.cpp` contenant un certain nombre de fonctions utilitaires que j'ai faites pour vous. Dans le dossier `include` les headers files associés. Vous trouverez également un fichier `makefile` s'occupant de la chaîne de compilation pour vous. 


## Travail à faire 

Alors oui pour avoir le droit de jouer au Snake il va falloir que vous vous bougiez un peu !! 

Dans le dossier que vous venez de cloner vous avez un fichier `main.cpp` c'est le point d'entré de notre programme. Dans ce fichier vous trouvez une fonction `main` et une fonction `startGame`. Ces deux fonctions sont déjà implémentées pour vous pas besoin d'y toucher. 

En revanche .... ces fonctions appellent elles-même d'autre fonctions comme vous pouvez le voir qui elles ne sont pas implémentées. Donc vous me voyez venir c'est à vous d'implémenter les fonctions manquantes. 

Enjoy !! 

Pour repérer les fonctions manquantes c'est facile, VSCode les soulignes en rouge et vous dit un truc du genre "identificateur truc inconnu". 



### Bon ok je suis sympa je vous aide 

Du coup pour que vous puissiez vous en sortir avec toutes ces fonctions à définir i lfaut déjà que vous compreniez le fonctionnement général du programme. 

Le programme comporte deux tableaux que l'on se trimballe partout : 
* `int snake[2*SNAKE_LEN]` c'est le tableau définissant la position du serpent
* `int background[nx*ny]` c'est le tableau définissant le "terrain de jeu"

Il s'agit de tableau unidimensionel qui nous permettent de stocker des valeurs 2D `(x, y)`. 
Le tableau `background` contient les valeurs associées à chaque point de la grille `nx*ny`. Pour accéder à l'élement `(i,j)` avec `i` l'indice des abscisses et `j` l'indice des ordonnées la relation est la suivante : 

$$val_{ij} = background[ i + j*nx ]$$

Tandis que le tableau `snake` permet de stocker la position `(x,y)` de chaque élément du serpent. Pour obtenir les coordonnées `(x_i, y_i)` du i-ème élément du serpent il faut procéder de la manière suivante : 

$$
x_i=snake[i] ; y_i=snake[SNAKE\_LEN+i]
$$




Vous pourriez demandez pourquoi se prendre la tête à faire des tableau 1D pour représenter du 2D. Vous auriez raison mais cela s'explique tout simplement par le fait que vous ne savez pas faire de tableaux 2D :D !! Et puis c'est pas plus compliqué en plus. 

```python

```
