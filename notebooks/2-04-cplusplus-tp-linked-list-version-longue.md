<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12-MINES-ParisTech</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>

```c++
from IPython.display import HTML
HTML('<link rel="stylesheet" href="c++-slides.css" />')
```

# exercice **la liste doublement chaînée**


## les conteneurs d'éléments 


<div class = "framed-cell">
    <ins class = "underlined-title">les
conteneurs d'éléments</ins>

<br/>

certains programmes requièrent la mémorisation d'un certain nombre
éléments tout au long d'une exécution

<br/>

par exemple, dans la calculette, vous avez dû mémoriser, dans un
conteneur, les opérandes jusqu'à trouver l'opérateur qui s'y
appliquait

<br/>

toutes les librairies des langages de programmation comme `c++` ou `Python` proposent différents types de
conteneurs, que vous choisissez suivant les fonctionnalités
recherchées (vecteurs, dictionnaires, ensembles, piles, files...)

<br/>

par exemple, la calculette en notation polonaise inversée
nécessite un conteneur avec un comportement de pile 

</div>


## les types des éléments des conteneurs 


<div class = "framed-cell">
    <ins class = "underlined-title">les types des éléments des
conteneurs</ins>

<br/>

le comportement d'un conteneur ne dépend pas, à proprement parler, du
type des objets qu'il stocke
    
<br/>

qu'une pile empile des entiers ou des chaînes de caractères, elle le fait toujours de la même manière  
en mettant l'élément dans une case du tableau sous-jacent et en incrémentant le nombre d'éléments qu'elle contient

<br/>

nous allons néanmoins vous demander, pour simplifier, de faire une liste d'entiers

<br/>

une fois le conteneur codé sur des éléments de type entier, il sera possible d'en faire un conteneur, indépendant du type des éléments, grâce aux `templates`, nous le ferons lors d'un autre exercice

</div>



## qu'est ce qu'une liste doublement chaînée ?


<div class = "framed-cell">
    <ins class = "underlined-title">qu'est
ce qu'une liste doublement chaînée</ins>

l'idée de la liste chaînée est d'allouer uniquement
la mémoire nécessaire à un élément, au moment de l'ajout de
l'élément dans la liste 

<br/>

contrairement, par exemple, au
vecteur où une zone mémoire continue (permettant de stocker
ensemble un des
éléments) est allouée dès la création
du vecteur (même pour un vecteur initialement vide) 

<br/>

dans le cas de la liste chaînée,
les éléments ne sont pas mis dans une zone mémoire continue  
il
est donc nécessaire de les relier les uns aux autres pour former la
liste 

<br/>

c'est ce qu'on va faire en mettant, l'élément à
ajouter dans la liste, à l'intérieur d'un *maillon de liste* (une
cellule de liste) et en reliant ce maillon à des maillons déjà
créés dans la liste 

<br/>

voici l'exemple d'une liste qui contient
dans l'ordre les éléments `N1`, `N2`, `N3`, `N4` et `N5` 
<img src="media/doubly-linked-list.png" name="Image2" align="bottom" width="690" height="156" border="0"/>

on remarque
* que chaque élément est dans un maillon (encore appelé maillon ou noeud)
* que chaque
maillon contient un lien vers le maillon qui le suit et un lien vers
le maillon qui le précéde
* que le maillon en tête de liste n'a
pas de maillon précédent
* que le maillon en queue de liste n'a pas
de maillon suivant (la liste n'est pas circulaire)
* que la liste est
constituée par un lien vers le maillon en tête de liste et un lien
vers le maillon en queue de liste
* qu'à partir d'un maillon de
liste je peux atteindre (si ils existent) son maillon précédent et
son maillon suivant
* la liste conserve l'ordre demandé lors de l'ajout des éléments

<br/>

à son initialisation, la liste est vide
* il n'y a pas de maillon de tête ni de maillon de queue
* aussi ses liens (vers le maillon de tête et le maillon de queue) ne peuvent-ils
pas être précisés: *ils sont donc initialement nuls* 

</div>


## ajout d'un élément dans une liste doublement chaînée


<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">ajout
d'un élément dans une liste doublement chaînée</ins>

<br/>

dans une liste doublement chaînée, on peut ajouter un élément
* en tête de liste par la méthode `add_front`
* ou en fin de liste
par la méthode `add_back` 

<br/>

ces deux opérations doivent se
faire en temps constant
* pas question de parcourir toute la liste à
partir de son début pour ajouter un élément en fin de liste
* une
opération en temps constant deviendrait alors
une opération en temps linéaire
* voila pourquoi vous avez, dans une
liste, deux liens un vers le maillon de tête et l'autre vers le maillon de queue de liste

<br/>

exemple de l'ajout du premier élément à une liste
```c++
#include <intlist.h>
int main () {
   // création d'une liste vide
   IntList l;
   // ajout de 12 en tête de liste
   l.add_front(12);
   l.print(); // {12 }
   return 0;
}
```
`l.add_front(12)`:
* l'élément `12` est mis dans un maillon
* ce maillon n'a pas de maillon précédent ni de maillon suivant
* le lien de tête de liste pointe vers le maillon
* le lien de queue de liste pointe ... aussi vers ce même maillon 
    
<br/>

exemple de l'ajout d'un second élément en tête de liste

```c++
#include <intlist.h>
int main () {
  IntList l;
  l.add_front(12);

  // ajout de 11 en tête de liste
  l.add_front(11);
  l.print(); // {11 12 }
    
  return 0;
}
```
`l.add_front(11)`    
* il n'a bien sûr pas d'élément précédent
* son élément suivant sera l'élément de
tête de liste avant son ajout
* la tête de liste doit indiquer alors le
maillon qui contient le nouvel élément 

<br/>

voici un exemple de
code

```c++
#include <intlist.h>
int main () {
  // création d'une liste vide
  IntList l;

  // ajout de 11 en tête de liste
  l.add_front(11);
  l.print(); // {11 }

  // ajout de 12 en queue de liste
  l.add_back(12);
  l.print(); // {11 12 }
    
  // ajout de 13 en queue de liste
  l.add_back(13);
  l.print(); // {11 12 13 }

  return 0;
}
```

<br/>

dans une liste, pour accéder aux autres éléments que le premier et le
dernier, on doit parcourir la chaîne formée par les maillons, en
suivant les liens:
* le lien précédent pour se déplacer vers la tête de liste
* le lien suivant pour se déplacer vers la queue de liste 

</div>
<!-- #endregion -->

## les questions ... 


<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">les
questions ...</ins> 

<br/>

*Quelles
sont les classes à définir ?*
    
la classe `IntList` ... mais aussi la
classe pour les maillons i.e. les cellule de liste `IntCell` 

<br/>

*Quels
sont les fichiers à définir ?*
* un fichier `intlist.h` qui
contient les deux classes qui réalisent votre liste
* un fichier
`test-intlist.cpp` qui contient les fonctions pour tester votre liste  
ainsi que la
fonction `main` qui appelle
ces fonctions de tests 

<br/>

*Où sont créés les maillons ?*
* dans les fonctions qui ajoutent des éléments
comme `add_front` et `add_back`
* où vous devez construire une cellule de liste
pour contenir votre nouvel élément et pour le relier aux autres
maillons 

<br/>

*Dans quelle mémoire sont alloués les cellules de
liste ?*

Rappelez vous que vous avez 3 zones mémoires disponibles
    
1. la zone de texte et des variables globales (qui est de taille fixe
donc impossible d'y créer des objets non prévus avant la
compilation)
       
1. la pile d'exécution qui contient les variables
locales aux fonctions (ces variables sont enlevées de la pile d'exécution une
fois la fonction terminée donc impossible d'y créer des objets qui
doivent exister en dehors de la fonction)
    
1. le tas, `heap`, la
mémoire dynamique (`new` et `delete` ) (les objets crées dans cette
zone existent jusqu'à ce que vous décidiez de les en enlever) 

    
</div>
<!-- #endregion -->

## les deux classes sont des **amies**


<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">les
deux classes `IntList` et `IntCell` sont des amies</ins>

dans cet exercice vous n'avez pas une mais deux classes à définir: `IntCell` et
`IntList` 

<br/>

et vous remarquez que ces deux classes sont intimement liées ... 

<br/>

la classe `IntList` va manipuler les cellules de
listes pour les brancher les unes aux autres 

<br/>

le classe
`IntList` doit donc accéder aux attributs de la classe
`IntCell` 

<br/>

mais vous vous rappelez que les attributs des classes sont mis en zone `private`  
pour que personne ne puisse les modifier en dehors du code qui en a besoin 

<br/>

*Comment permettre alors, à la
classe `IntList`, d'accéder aux attributs de la classe `IntCell` ?*
    
* une première solution consiste à implémenter dans la classe `IntCell`, des méthodes en `public` pour accéder et modifier les différents attributs (`get_next`, `set_next`...) mais ce n'est **pas** la solution que `c++`
conseille
    
    
* la solution élégante (que vous ne connaissez pas
encore) consiste pour la
classe `IntCell` à
autoriser la
classe `IntList` à accéder à ses attributs et méthodes privées

<br/>

pour cela la classe `IntCell` va déclarer qu'elle est `friend` avec
la classe `IntList`
* la classe (comme vous) choisit ses amis c'est donc
`IntCell` qui va déclarer que `IntList` est son amie
* et ses amies
peuvent accéder à ses attributs et méthodes
privées
</div>
<!-- #endregion -->

## exemple de `friend` 


<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">exemple de déclaration de `friend`</ins>
    
    
prenons l'exemple d'une classe `Y` qui veut accéder à un attribut `private` d'une classe `X` ici `X.val`

<br>

```c++
// dans le fichier exemple.cpp
class X {
private:
   int val = 0;
};

class Y {
public:
   void foo () {
      X x;  // on définit un objet de type X local à la fonction foo
      x.val = 100; // on accède à un attribut privé de X
   }
};

int main () {
   Y y;
   y.foo();
   return 0;
}
```

<br>
    
naturellement, si on essaie de compiler ... `c++` refuse !  
puisqu'on accède à un attribut privé de `X` en dehors des méthodes de la classe `X` (là dans la fonction `Y.foo`)

    
<br>
on le fait quand même:
    
```bash
g++ exemple.cpp 
exemple.cpp: In member function ‘void Y::foo()’:
exemple.cpp:11:9: error: ‘int X::val’ is private within this context
   11 |       x.val = 100; // on accède à un attribut privé de X
      |         ^~~
exemple.cpp:4:8: note: declared private here
    4 |    int val = 0;
      |        ^~~

Compilation exited abnormally with code 1 at Mon Nov 30 10:41:26

```
</div>
<!-- #endregion -->

<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">on met la déclaration de `friend`</ins>
    
    
dans le code suivant, remarquez la déclaration `friend class Y;`

    
```c++
class X {
   friend class Y;
private:
   int val = 0;
};

class Y {
public:
   void foo () {
      X x;  // on définit un objet de type X local à la fonction foo
      x.val = 100; // on accède à un attribut privé de X
   }
};

int main () {
   Y y;
   y.foo();
   return 0;
}
``` 
    
On compile et on remarque qu'il n'y a plus d'erreur puisque la classe `X` a autorisé la classe `Y` à accéder à ses attributs privés   

```bash
 $ g++ exemple.cpp
 $
 
```

c'est
une méthode simple, élégante il faut l'utiliser quand votre code
s'y prête comme dans le cas de la liste 

</div>
<!-- #endregion -->

## on récapitule les attributs des classes


<div class = "framed-cell">
    <ins class = "underlined-title">on
récapitule les attributs des classes</ins>

<br/>

vous savez désormais que la liste doit posséder
* un lien
vers sa première cellule qui peut ne pas exister
* un lien vers sa
dernière cellule qui peut ne pas exister 

<br/>

vous savez désormais
que la cellule doit posséder
* un lien vers la cellule qui la
précède qui peut ne pas exister
* un lien vers la cellule qui la
suit qui peut ne pas exister
* une valeur entière (parce qu'on
stocke des entiers) 

</div>


## coder de manière incrémentale en testant !

<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">coder de manière incrémentale et en testant</ins>
    
avant de vous donner les méthodes à coder nous allons vous préciser la manière de coder
    
    
<br>
    
il va s'agir pour vous de coder de manière incrémentale (itérative) et de tester votre code au fur et à mesure de son avancée
    
<br>

pour coder de manière itérative, vous devez implémenter les méthodes les unes après les autres ... et pas toutes d'un seul coup !
    
    
<br>
    
à chaque nouvelle méthode que vous écrivez
* testez la méthode seule (par exemple en implémentant une fonction dédiée) ce sont les **tests unitaires**  
naturellement vous pouvez utiliser la fonction `print` pour la tester
* testez les fonctions entre elles
    
<br>
    
conserver tous le code que vous écrivez pour vos tests afin de pouvoir les ré-appliquer sur votre code au fur et à mesure des ajout  
vous allez construire des tests de **non-régression**

<br>
    
nous vous laissons imaginer la manière de faire, inventez votre propre manière de tester
* définition de fonctions de test
* définition d'une classe dédiée aux tests de votre `IntList` avec des méthodes de tests
* afin que lorsque nous vous parlerons ensuite de tests vous ayez refléchi à la question et y soyez sensibilisés

</div>
    
<!-- #endregion -->

## les méthodes à implémenter

<!-- #region -->
 <div class = "framed-cell">
    <ins class = "underlined-title">les méthodes</ins>
    
nous vous donnons la liste de méthodes à implémenter dans l'ordre en les testant
    
<br/>
 
Implémentez:
1. le constructeur sans argument de la `IntList`  
(vous pouvez définir la classe `IntCell` vide avant la classe `IntList`)
```c++
   IntList l;
```
1. la méthode `add_front` qui ajoute l'élément passé en argument en tête de la liste  
et le constructeur de la `IntCell`    
```c++
   l.add_front(10);
``` 
1. la méthode `print` qui parcourt la liste à partir de son premier élément
et affiche les éléments 
```c++
   l.print(); // {10 }
``` 
1. la méthode `add_back` qui ajoute l'élément passé en argument en fin de la liste
```c++
   l.add_back(20);
   l.add_back(30);
   l.print(); // {10 20 30 }
```
1. Avez-vous besoin d'une fonction pour détruire un objet de type `IntList` ?   
si oui programmez la
    
1. la méthode `remove_front` qui enlève de la liste l'élément en tête
de la liste (si il existe sinon ne fait rien) 
```c++
   l.remove_front();
   l.print(); // {20 30 }
``` 
1. la méthode `remove_back` qui enlève de la liste l'élément en fin de la liste (si
il existe sinon ne fait rien) 
```c++
   l.remove_back();
   l.print(); // {20 }
``` 
1. la méthode `search` qui retourne `true` si l'élément, dont la valeur est
passée en argument, existe dans la liste `false` sinon 
```c++
   std::cout << l.search(27); // 0
   l.add_back(123);
   std::cout << std::boolalpha << l.search(123); // true
```
1. pour les plus rapides    
la méthode `remove` enlève de la liste l'élément, dont
la valeur est passée en argument  
notez qu'une liste peut contenir plusieurs
fois le même élément mais vous n'enlevez que le premier trouvé (puisque l'utilisateur a mis plusieurs fois le même élément dans sa liste c'est qu'il en veut plusieurs donc laissez le les gérer ...) 
```c++
   l.add_front(20);
   l.print(); // {20 20 }
    
   l.remove(20);
   l.print(); // {20 }
   l.remove(20);
   l.print(); // {}
```

</div>
<!-- #endregion -->

## une idée des classes 

<!-- #region -->
<div class = "framed-cell">
    <ins class = "underlined-title">une
idée des classes</ins> 

```c++
class IntCell { 
    friend class IntList;
private:
    // tout le code de la IntCell en private
    // le constructeur et ses 3 attributs
    // il n'y a aucune autre méthode en dehors du constructeur
};

class intlist {
public:    
    // constructeur et destructeur
    void add_front (/* argument */) {
       // votre code ici
    }
    void add_back (/* argument */) {
       // votre code ici
    }
    void remove_front () {
       // votre code ici
    }
    void remove_back () {
       // votre code ici
    }
    void remove (/* argument */) {
       // votre code ici
    }
    /* type ici */ search (/* argument */) {
    }
    void print () {
       // votre code ici }
private:
    // les attributs de la liste
    };
```    
<!-- #endregion -->

</div>


## un très bon exercice de c++ 


<div class = "framed-cell">
    <ins class = "underlined-title">un
très bon exercice classique de programmation pour c++</ins>

<br/>

La liste double chaînée est devenue un exercice *classique*
dans l'apprentissage du c++ dans sa version *bas niveau* c'est à
dire proche de la mémoire 

<br/>

Cet exercice permet d'acquérir une
compréhension fine des points suivants:
* les adresses mémoires
(les fameux *pointeurs*)
* l'allocation de données en mémoire
dynamique (*tas*, *heap*)
* la libération, tout au long du
programme, des zones mémoires qui deviennent inutiles 

</div>


## un très mauvais conteneur !



<div class = "framed-cell">
    <ins class = "underlined-title">un
très mauvais conteneur !</ins>

<br/>

les étudiants doivent être conscients que les conteneurs par
liste chaînées ne doivent pas être utilisés dans les programmes

<br/>

il faut leur préférer des conteneurs beaucoup beaucoup plus
efficaces comme les vecteurs (`std::vector`), les dictionnaires
(`std::unordered_map`)... 

<br/>

nous reviendrons en fin de notebook
sur deux raisons de l'inefficacité de la liste chaînée 



## un très mauvais conteneur ! 


<div class = "framed-cell">
    <ins class = "underlined-title">pourquoi
la liste est un très mauvais conteneur !</ins>

<img src="media/doubly-linked-list.png" name="Image3" align="bottom" width="690" height="156" border="0"/>

l'idée derrière les listes chainées est d'ajouter une cellule de
liste à chaque fois qu'on ajoute un élément à la liste 

<br/>

la justification de cette manière de faire est qu'alors seule la
mémoire pour les éléments existants est effectivement créés alors que
les tableaux ou les dictionnaires créent dès leur initialisation, des
espaces pour stocker un *certain nombre* d'éléments

<br/>

mais, les cellules chaînées
* *fragmentent* la mémoire (i.e. allouent
des tas de petits morceaux de mémoire à la place d'allouer un gros
bloc comme pour les tableaux)
* et de créer des indirections qui sont
plus chères que des décalages (indirection = aller chercher un objet à
une adresse en mémoire) (décalage = se déplacer en mémoire
relativement à une adresse mémoire)
