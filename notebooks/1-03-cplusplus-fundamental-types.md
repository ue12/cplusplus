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

# les types fondamentaux de `c++`

+++

Nous allons voir dans ce notebook les différents types de base du langage `c++` (*core language*).

Quand les explications sont simples et directes, seules les *slides* sont indiquées, les *notes* ne sont pas données.

+++

<ins>Les types fondamentaux de base de `c++` (que nous allons détailler ici)</ins>
* types et identificateurs
* booléen
* nombres entiers
* nombres à virgule flottante
* le type vide `void`
* `typeid`
* pointeurs
* caractères et chaînes de caractères

+++

## les identificateurs en `c++`

+++

### *slides*

+++

<ins>en `c++` les objets et les identificateurs sont typés</ins>

les objets sont typés `12` est un  `signed int`, `3.1459` un `double`, `'a'` un caractère...

tous les identificateurs sont typés
* les variables sont typées
* les fonctions, leurs arguments, leur type de retour sont typés...

types $\Rightarrow$ opérations

+++ {"tags": []}

À vous de jouer. Dans un fichier de nom `divint.cpp` implémentez une fonction `main` incluez les `iostream`.

Définissez un entier (`int`) `i` qui vaut `1`, un autre entier `j` qui vaut `3`. Faites la division de `i` par `j` et affichez le résultat. Que constatez-vous ?

Faites de même dans un fichier `divfloat.cpp` pour deux nombres à virgule flottante (`float`) avec `f` et `g` qui sont des `float` qui valent respectivement `1` et `3`. Que constatez-vous

+++

### *notes*

+++

En `c++` tout les identificateurs sont typés que ce soient des objets comme `12`, `17.8`, `true` mais aussi les variables, les arguments des fonctions, leur type de retour...

Les types vont déterminer les opérations qui peuvent s'appliquer sur des identificateurs.

+++

Si je fais la division de deux entiers j'obtiens un entier. 

>```c++
>// fichier divint.cpp
>#include <iostream>
int main () {
   int i = 1;
   int j = 3;
   std::cout << i/j << std::endl ;
   return 0;
}
```
    
```bash
$ g++ divint.cpp -o divint
$ ./divint
0
```

+++

Si je fais la division de deux flottants, j'obtiens un flottant. 
    
>```c++
>// fichier divfloat.cpp
>#include <iostream>
int main () {
   float f = 1;
   float g = 3;
   std::cout << f/g << std::endl ;
   return 0;
}
```

```bash
$ g++ divfloat.cpp -o divfloat
$ ./divfloat
0.333333
```

+++

## `sizeof` ou la taille en octet d'un type ou d'objet

+++

### *slides*

+++

<ins>taille en octet d'un type ou objet</ins>

la fonction `sizeof` appliquée à un type ou à un objet ou à une variable

renvoie la quantité de mémoire en octets occupée par le type, l'objet ou la variable

par exemple:

```c++
int x = 17;
std::cout << sizeof(x);
std::cout << sizeof(17);
```

+++

À vous de jouer, dans un fichier `sizeof.cpp` affichez la taille en octets des valeurs `int` et `17`.

+++

## le type booléen (`bool`)

+++

### *slides*

+++ {"cell_style": "center"}

<ins>le type booléen</ins>

c'est le type `bool`

il comporte deux valeurs `true` et `false`

les opérateurs qui travaillent sur des booléens sont:
* `and` (s'écrit aussi `&&`)
* `or` (s'écrit aussi `||`)
* `not` (s'écrit aussi `!`)

+++ {"cell_style": "center"}

```c++
// in file types.cpp
#include <iostream>
int main () {
  bool b1 = true;
  bool b2 = false;
  bool b3 = b1 and not b2;
  std::cout << "b1 est " << b1;
  std::cout << "b2 est " << b2;
  std::cout << std::boolalpha    // pour afficher des booléens à la place de 0 et de 1
            << "b3 est " << b3;
  return 0;
}
```
```bash
$ g++ types.cpp
$ ./a.out
b1 est 1
b2 est 0
b3 est true    <-- true à la place de 1
```

+++

<ins>le type `bool`</ins>

c'est le type renvoyé par une condition dans une instruction `if` ou une itération conditionnelle `while`

les valeurs de type `bool` peuvent être implicitement promues en valeurs entières:
* `true` vaut `1`
* `false` vaut `0`

les valeurs entières peuvent être implicitement converties en valeurs booléennes:
* les entiers non nuls sont convertis en `true`
* `0` est converti en `false`
    
-------------------

Plus généralement:

tout objet dont la valeur n'est pas nulle peut être converti en `true`

tout objet dont la valeur est nulle peut être converti en `false`

+++

À vous de jouer, dans le fichier `sizeof.cpp` rajoutez l'affichage de la taille en octets de `true` et `bool`.

+++

## les types entiers

+++

### *slides*

+++

<ins> les types pour représenter des entiers <ins>
    
cinq types d'entiers signés standard:
* `signed char`
* `short int` (ou `short`)
* `int`
* `long int` (ou `long`)
* `long long int` (ou `long long`)

`signed char` $\leq$ `short int` $\leq$ `int` $\leq$ `long int` $\leq$ `long long int`

une version non-signée (de même taille en octets)

* `unsigned char`
* `unsigned short int` (ou `unsigned short`)
* `unsigned int`
* `unsigned long int` (ou `unsigned long`)
* `unsigned long long int` (ou `unsigned long long`)


les `int` sont signés par défaut


nous reviendrons plus loin sur le type `char` qui est celui qui encode les caractères

+++

### *notes*

+++

Il existe cinq types pour représenter les entiers signés (i.e. qui peuvent être positifs ou négatifs) en `c++`.

Ces types sont différenciés par leur *taille* en mémoire i.e. le nombre d'octets (8 bits) sur lequel les objets d'un type est représenté.

Nous avons vu rapidement en `numpy` que des entiers peuvent être stockés sur `8`, `16`, `32` ou `64` bits. En `c++`, ce n'est pas la même chose. La norme `c++` ne standardise pas la taille de ses types. Elle va simplement vous donner des indications en comparant leur grandeur. La phrase: `signed char` $\leq$ `short int` $\leq$ `int` $\leq$ `long int` $\leq$ `long long int` vous indique que chaque type fournit au moins autant de stockage que ceux qui le précèdent. Ainsi vous ne pourrez pas avoir dans un même programme un `int` stocké sur `2` octets et un `char` stocké sur `32` bits ! 

La taille des types dépend de l'implémentation, elle est *implementation-defined*. Par exemple sur 

Pour chaque type entier signé, il existe un type entier non signé, donc où toutes les valeurs sont positives ou nulles, qui occupera la même quantité d'octets que le type entier signé correspondant.

La norme nous dit que tous les `int` sont signés par défaut i.e. `int` ou `signed int` sont les mêmes types. Le fait que le type `char`, qui pourtant est un type entier (nous y reviendrons) soit signé ou non dépend de l'implémentation.

+++

## le type pour les nombres à virgule flottante

+++

### slides

+++

<ins>les nombres à virgule flottante</ins>

sont des approximations de nombres réels $17.3\ 10^{-8}$ est `17.3e-8` avec: 
* une partie entière `17`
* un point décimal `.`
* une partie fraction `3`
* un exposant entier (optionnel) introduit par `e` ou `E`

(pour vous amuser avec la représentation binaire des nombres à virgule flottantes allez là: www.binaryconvert.com)

(pour vous amuser avec la norme allez ici https://fr.wikipedia.org/wiki/IEEE_754)

+++

```c++
#include<iostream>
int main () {
  
  // des nombres à virgule flottante
  std: cout << 15.47 // 15.47
            << 17.3e-8; // 1.73e-7
  
  // des entiers avec la notation en exposant
  std::cout << 12.  // 12
            << 12e2   // 1200
            << .7E-10; //7e11
  return 0;
}
```

+++

<ins> les types des nombres à virgule flottante</ins>

il y a trois types: `float`, `double` et `long double`

chaque type fournit au moins autant de stockage que ceux qui le précèdent  
`float` $\leq$ `double` $\leq$ `long double`

```c++
int main () {
  float nb_float = 17.5;
  double nb_double = 199.56;
  long double nb_long_double = 67.98;
  return 0;
}
```

+++

À vous de jouer, dans le fichier `sizeof.cpp` rajoutez l'affichage de la taille en octets de `17.5`, `nb_float`, `nb_double`, et `nb_long_double`.

+++ {"tags": ["level_intermediate"]}

## spécifier les types des nombres

+++ {"tags": ["level_intermediate"]}

Pour les `int`.

Disons rapidement qu'un *littéral intégral* est une entier positif ou nul comme `12` ou `0`. On peut leur attribuer un signe pour en faire des entiers négatif comme `-12`.

Il est possible de spécifier non seulement le signe mais aussi le type d'un littéral intégral.

En effet, par défaut `12` est un `int`. Si on voulait qu'il soit un `unsigned int` on dirait `12u` ou `12U`.

Ainsi si le suffixe contient un `U`, alors le littéral aura obligatoirement un type non signé. Donc `-12U` est `12U`.

On en profite pour vous dire de faire attention quand vous utilisez des expressions franchement difficiles à interpréter comme `int i = -12U;` (si c'est pour vous amuer avec le compilateur ca passe si c'est dans un *vrai*  code c'est franchement dangereux). La conversion employée est dite *implementation-defined* i.e. c'est le compilateur qui décidera quoi faire $\Rightarrow$ votre code n'est pas portable (puisqu'un autre compilateur peut décider de faire le contraire).

Il est aussi possible de spécifier les types `long`, ` et `long long` sur un littéral intégral en plus du fait qu'il soit `unsigned` ou `signed` (qui est le défaut).
 
Ainsi `12l` est un entier de type `signed long int`. `12llu` un entier de type `unsigned long long int`.

+++ {"tags": ["level_intermediate"]}

Pour les nombres à virgule flottante, le défaut est `double` donc vous pouvez indiquer `float` par un `f` et `long double` par un `l` appliqué à un nombre à virgule flottante sinon il en fera un entier.

+++ {"tags": ["level_intermediate"]}

## Valeurs maximales et minimales des types arithmétiques

+++ {"tags": ["level_intermediate"]}

Il existe une classe dont des méthodes vous permettent de connaître les valeurs minimales `min` et maximales `max` des types arithmétiques.

Il faut pour cela inclure le fichier d'entête `limits` où est définie la classe `std::numeric_limits` qui comporte les deux méthodes `min` et `max` qui nous intéressent.

Alors cette classe est un peu particulière puisque vous devez lui préciser le type dont vous recherchez les bornes (par exemple `int`). Vous l'indiquez comme cela: `std::numeric_limits<int>` et donc suivant le type indiqué, les résultats obtenus seront différents par exemple `std::numeric_limits<long long int>`.

Cette classe est ce qu'on appelle une classe **template** ou encore un patron ou encore un modèle de classe. Elle va prendre ici un type en argument.

On vous donne un exemple, pour savoir la borne inférieure du type `int`, je dois appeler la méthode `min()` de la classe `std::numeric_limits <int>` donc:

```c++
std::numeric_limits <int>::min()
```


À vous de jouer. Affichez les limites numériques des types: `int`, `unsigned int`, `long long int`, `float`, `double`, `long double`. Quelles valeurs trouvez vous ? Affichez les limites numériques pour le type `char`. Que constatez-vous ? Hint: il vous rend le plus petit et le plus grand `char` qui ne sont pas des `int` mais des `char` qui ne sont peut être pas des caractères visibles...

+++

## `void`

+++

### *slides*

+++

<ins> le type `void`</ins>

`void` est un type incomplet qui a un ensemble de valeurs vide

il est utilisé
*  comme type de retour pour les fonctions qui ne retournent pas de valeur
* (facultatif)  comme unique paramètre pour les fonctions qui ne prennent pas d'argument

```c++
void foo (void) {}
void bar () {}
int main () {
   foo();
   bar();
   return 0;
}
```

*  comme type de base des pointeurs vers des objets dont le type est inconnu (mais on y reviendra)

+++

## les pointeurs

+++

### *slides*

+++

<ins>Qu'est-ce qu'un pointeur ?</ins>

un pointeur est l'adresse d'un objet en mémoire

pour un type `T`, `T*` est le pointeur vers un objet de type `T`

ainsi `int*` est le type adresse d'un objet de type `int`

l'implémentation des pointeurs est directement liée au mécanisme d'adressage de la machine

on est très *proche de la machine*: on touche dans sa mémoire (mais dans le segment de notre process uniquement)

on voit des exemples juste après ... il nous manque encore des notions

+++

<ins>Accédons à l'adresse mémoire d'un objet</ins>

dans une expression:
* `&` est l'opérateur qui retourne l'adresse d'un objet en mémoire

si `i` est une variable, `&i` est l'adresse de la zone à laquelle le contenu de la variable a été stoqué


On donne un exemple
* on inclut les `iostream` pour afficher
* on crée une variable de type entier `i`
* on affiche `&i` c'est-à-dire l'adresse de `i`

>```c++
>// dans le fichier pointeurs.cpp
>#include<iostream>
>int main () {
  // i est une variable de type entier
  int i = 78;
  // &i est l'adresse où est stoqué i en mémoire
  // on la met dans un objet de nom pi et de type int*
  int *pi = &i; 
  // On affiche cette adresse. Que constatez vous ?
  std::cout << pi << std::endl;
  return 0;
}
```

>```bash
>$ g++ pointeurs.cpp
$./a.out
0x7ffdfe5d7724
```

oui `c++` vous parle hexadécimal

+++

<ins>Accédons à l'objet qui se trouve à une adresse</ins>

dans une expression:
* `*` est l'opérateur qui, appliqué à un pointeur, retourne l'objet à l'adresse indiquée

si `pi` est une adresse, `*pi` est l'objet qui se trouve à cette adresse  
(attention: le `*` n'est pas le même que dans `int *pi = &i`)


>```c++
>// dans le fichier pointeurs.cpp
>#include<iostream>
int main () {
  int i = 78;
  int *pi = &i; // pi est l'adresse de i (pi pointe sur i)
  // Maintenant on va accèder à l'objet pointé par pi (avec *pi)
  // puis on lui ajoute 1 et on affiche i
  // Que constatez-vous ?
  *pi = *pi + 1;
  std::cout << i << std::endl;
  return 0;
}
```

>```bash
>$ g++ pointeurs.cpp
$./a.out
79
```

oui vous avez bien modifié l'objet `i` en le manipulant à travers son adresse `pi`

+++

C'est maintenant qu'on s'amuse ...

Voyez-vous le problème potentiel ? Oui ? Non ? vous allez essayer de le voir avec l'exemple suivant.

À vous de jouer. Dans le fichier `probleme.cpp` définissez une variable `pb` de type adresse d'un `double` et soit vous ne l'initialisez à rien soit vous l'initialisez à `nullptr` mais vous ne donnez aucune adresse valide lors de son initialisation ! Affichez `pb` que voyez-vous ? Affichez maintenant l'objet pointé par `pb`. Que constatez vous ?

Alors ca dépend ... au mieux vous avez une erreur de segmentation et au pire rien ne se passe mais votre code est complètement *infecté* parce qu'il n'y a pas d'objet alloué où vous allez ! Cette zone n'est pas construite pas initialisée donc ce programme est une catastrophe !

+++

```c++
$ g++ probleme.cpp
$ ./a.out
/bin/bash: line 1: 23559 Segmentation fault (core dumped) ./a.out
Compilation exited abnormally with code 139 at Tue Oct  6 18:25:19
```

+++

<ins>`nullptr`</ins>

notons déjà qu'à partir de `c++11` il existe le pointeur null qui est nommé `nullptr`

c'est un pointeur qui ne pointe pas vers un objet

`nullptr` peut être assigné à n'importe quel type de pointeur  
mais uniquement à un pointeur


il n'y a qu'un seul `nullptr` partagé par tous les types de pointeurs  
(et non un pointeur null pour chaque type de pointeur)


avant `nullptr`, zéro `0` devait être utilisé comme initialisation pour le pointeur null

>```c++
>int main () {
   int *pi = nullptr;
   double *pd = 0;
   bool *pb; // pas initialisé !
   return 0;
}
```

+++

## le type des caractères

+++

### *slides*

+++

<ins>le type des caractères `char`</ins>

un caractère en `c++` se met entre deux guillemets simples comme dans `'a'`

un caractère classique `ascii` est un objet de type `char`

prenons un exemple,
* affichons le caractère `'a'`
* puis une tabulation qui est le caractère `ascii` `'\t'`
* puis un `'b'`
* puis le caractère de saut de ligne `'\n'`  
notons qu'il est équivalent à `std::endl`

```c++
# include <iostream>
int main () {
   std::cout << 'a' << '\t' << 'b' << '\n';
   return 0;
}
```

Sur combien d'octets pensez-vous d'un caractère `ascii` est codé ? Vous pouvez le savoir en utilisant la fonction `sizeof` dans `sizeof(char)` ou `sizeof('a')`

+++ {"tags": ["level_intermediate"]}

<ins> les caractères non ascii en `c++`</ins>

les caractères peuvent être précédés de `u`, `U` (pour unicode)

un caractère commençant par la lettre:
* `u` est de type `char16_t`
* `U` est de type `char32_t`
* `L` (pour large) est de type `wchar_t`

ces types de caractères sont conçus pour traiter les caractères non-ascii  
les objets de ces types sont codés sur plusieurs octets (2 pour `char16_t` et 4 pour `char32_t`)  
donc le jeu de caractères qu'on peut coder est plus étendu que l'ascii 

```c++
#include<iostream>
int main () {
   std::cout << u'a' << U'b' << L'c';
return 0;
}
```

+++

<ins>Que dit la norme C ++ sur les types de caractères</ins>

*le type char doit être suffisamment grand pour stocker les caractères ansii*

donc un octet suffit pour encoder tous les caractères ascii

pour les `unsigned char` les valeurs iront de $0$ à $2^n-1$ avec donc $n$ qui vaut $8$ (1 octet) de $0$ à $255$

pour les `signed char` les valeurs iront de $−2^{n−1}$ à $2^{n−1}-1$  avec $n$ qui vaut $8$ de $-128$ à $127$

et si on ne précise rien ? c'est à dire si on met simplement `char` ? dans quel intervalle seront les valeurs ?

et bien on ne le sait pas: c'est *implementation defined*

c'est défini par l'implémentation si un caractère est signé ou non

il n'est donc pas sûr du tout de supposer que `char` peut contenir plus de 127 caractères

ainsi que pensez-vous du code suivant ?

>```c++
>#include <iostream>
>int main () {
   char c = 128;
   std::cout << c << std::endl;
   return 0;
}
```
    
et bien d'un compilateur à un autre son comportement peut changer: on dit que ce code n'est **pas portable** (et ce n'est pas bien)

dans le cas ou vos `char` sont signés, `c++` va faire un `overflow` et vous allez obtenir `0` à la place de `128`

+++ {"tags": ["level_intermediate"]}

Pour cet exercice, nous allons nous amuser avec les caractères ascii. Prenons le caractère `'G'` et affichons sa représentation `ascii`. Bien sûr si on tente d'afficher le caractère, on va obtenir la lettre `G`.

Pour afficher son code `ascii`, il faut demander à `c++` la représentation entière de `'G'`. Pour cela, il faut que `c++` considère le caractère comme un entier (qui sera son code `ascii`) et non comme un `char`. Par exemple, on va mettre notre `'G'` dans une variable de type `short` ou `int`.

```
#include<iostream>
int main () {
   char G = 'G';
   std::cout << G;
   short Gs = G;
   std::cout << Gs;
   return 0;
}
```

```bash
$ g++ le-G.cpp
$ ./a.out
71
```

Quel est le code `ascii` de `'G'` ?


Maintenant, si on regarde la table des caractères `ascii` avec ses `128` caractères vont de $0$ à $127$. 

<img src='media/US-ASCII_code_chart.png' width=600>

Comment feriez-vous une fonction qui déduise d'une lettre, la lettre majuscule correspondante ? C'est à dire une fonction qui prend en argument un `char` et qui renvoie un `char` qui est la lettre argument en majuscule; et qui, si ce n'est pas une lettre ou si c'est déjà une majuscule, rend le caractère passé en argument.

Sachant que les `char` se manipulent comme des entiers ainsi `'a'+1` donnera le code `ascii` de `'b'` donc `98` si on le considère (dans le contexte) comme un entier ou `'b'` si on le considère comme un `char`. Vous ne me croyez pas ? Essayez de faire `char i = 'a' + 1;` et d'afficher `i` et de faire `short j = 'a' + 1;` et d'afficher`j`. Qu'obtenez-vous ?

Donc pour la fonction qui met son argument en majuscule, combien faut-il ajouter à `'a'` pour passer à `'A'` ? Oui il faut ajouter `'A' - 'a'` (parce que `'a' + ('A' - 'a') == 'A'`) et ce sera pareil pour les autres caractères. Et comment tester que la lettre passée en argument est bien une minuscule ? Elle sera supérieure ou égale à un `a` et inférieure ou égale à un `'z'`.

+++ {"tags": ["level_intermediate"]}

## informations annexes

+++ {"tags": ["level_intermediate"]}

### représentation de types  entiers à `n` bits dans un ordinateur

+++ {"tags": ["level_intermediate"]}

La représentation des types numériques est bornée en `c++` (pas comme en `Python` où un entier peut être aussi grand qu'on le veut (mais c'est quand même borné sur la taille de la mémoire).

Un type entier est représenté à l'aide d'un nombre donné de bits `n` et les versions signées et non signées du même type entier sont codées sur le même nombre d'octets.

+++ {"tags": ["level_intermediate"]}

Pour un entier signé sur `n` bits,  un bit est réservé pour le signe (par exemple le bit le plus à gauche) qui vaudra:  
`0` pour les nombres positifs  
`1` pour les nombres négatifs
   
La représentation des nombres entiers positifs est très simple et directe:  
(on prend comme exemple `n = 4` bits avec le premier bit à `0` pour les positifs):  
`0000` == `0`  
`0001` == `1`  
`0010` == `2` ...  
`0111` == `7`

ok c'est facile

+++ {"tags": ["level_intermediate"]}

À vous de jouer, dans un fichier `sizeof.cpp` affichez la taille en octets des valeurs `x`, `17.5`, `double`, `'a'`, `true`, `12llu`.

+++ {"tags": ["level_intermediate"]}

Mais comment faire pour représenter les nombres entiers négatifs dans l'ordinateur ?

Essayons de changer simplement le signe des nombres positifs...

*1000 == 0  
1001 == −1  
1010 == −2...  
1111 == −7*  heu mais c'est pas top  du tout !!

Alors d'abord nous avons deux zéros *0000 == 0* et *1000 == 0*. On va dire que ce n'est pas *trop* important...

Mais plus problématique essayons de faire une adition: *0001 + 1001 = 1010* soit *1 + −1 = −2* ... oui l'ajout habituel ne fonctionne plus du tout ! on est plutôt mal partis comme cela ...

+++ {"tags": ["level_intermediate"]}

Mais alors ! Qu'est-ce qui fonctionne ?

Et bien dire que `0001 + 1111 == 0000` (`1 + −1 = 0`) nous plaît beaucoup !

Cela s'appelle *le complément à deux* par rapport, ici, à $2^4$ (4 bits - 10000).

Il a été suggéré en 1945 par John von Neumann pour l'ordinateur ENIAC.

+++ {"tags": ["level_intermediate"]}

Le complément à deux de $i$ par rapport à $2^n$ (voir wikipedia) est $2^n - i$ c'est à dire $-i$.

Notez que $2^n$ est tronqué à $0$ sur $n$ bits (on enlève ce qui dépasse par la gauche donc `10000` est tronqué à `0000` donc `0`) et le tour est joué !

Faisons le pour $i = 0010 = 2$. Le complément à deux est $2^4 = 10000 = 0010 + 1110$ avec $10000$ tronqué à $0000$ donc $-i$ est $-2$ est $1110$

C'est-à-dire que, pour inverser le signe, vous prenez le complément à deux. Sur $n$ bits, vous pouvez représenter des valeurs de $−2^{n−1}$ à $2^{n-1}-1$)


|int| binaire| 2 comp.|  |
|---|--     -|-     --|---|
|0   | 0000   | 0000
|1   |  0001  | 1111  | (10000 - 0001)|
|2   |  0010  | 1110  | (10000 - 0010)|
|...|....|....|....|....|... ...|
|6 | 0110 | 1010 | (10000 - 0110) |
|7 | 0111 | 1001 | (10000 - 0111) |
|-8| 1000  | 10000 | (10000 - 1000) |
|-7| 1001  | 0111 | (10000 - 1001) |
|...| ...|...|...|...|
|-3 | 1101 |  0011 | (10000 - 1101) |
|-2 | 1110 |  0010 | (10000 - 1110) |
|-1 | 1111 |  0001 | (10000 - 1111  |

Donc on a, pour un entier signé de 4 bits:
* `0111 + 0001 = 1000` soit `7 + 1 = -8`
* `1000 + 1111 = 0111` soit `-8 + -1 = 7`
