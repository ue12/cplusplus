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
---

<div class="licence">
<span>Licence CC BY-NC-ND</span>
<div style="display:grid">
    <span>UE12</span>
</div>
<div style="display:grid">
    <span><img src="media/ensmp-25-alpha.png" /></span>
</div>
</div>

+++

Dans ce cours sur le langage c++, nous avons choisi de présenter des notebooks jupyter (le code écrit entre ```c++ ``` se coupe et se colle facilememt dans des fichiers de texte même si du code c++ ne peut pas être interprété dans le notebook - il existe des notebook pour c++ comme xeux/cling mais ils ne nous ont pas satisfaits).

Chaque notebook introduit une thématique par des slides (dans une sous-section *slides*) qui sont suivies par des explications (dans une sous-section *notes*).

Le cours petu se faire de manière *classique*: l'enseignant expiquant les slides. Il pourra ensuite être repris en autonomie par les élèves.

+++

# Introduction rapide

+++

## le langage `c++`

+++

### *slides*

+++ {"cell_style": "split"}

<ins>Le langage c++</ins>

* généraliste
   - contraintes de temps
   - gestion fine de la mémoire

* fortement typé (voir code à côté)

* compilé

* orienté objet

* haut niveau d'abstraction

* nornalisé ISO

* librairie standard

+++ {"cell_style": "split"}

>```c++
>// fichier first.cpp
>// les entrées/sorties
>#include <iostream> 
// on my computer
// type int is 32 bits
int max = 4294967295;
int plus_un (int a) {
    return a + 1;
}
```
>```c++
>int main () {
  std::cout << plus_un(max)
            << std::endl;
  return 0;
}
```

+++

### *notes*

+++

`c++` est un langage de programmation **généraliste** c'est à dire qu'il n'est pas dédié à un domaine d'application spécifique. Il est tout particulièrement employé lorsque vous avez des contraintes en temps d'exécution dans vos programmes, puisqu'il permet une gestion fine de la mémoire.


Il est **fortement typé**. Tout objet en mémoire est naturellement typés mais vous devez aussi donner un type aux variables, aux arguments des fontions à leur type de retour (`return`) ... Tout identificateur avant son utilisation dans une expresion doit être typé.

`c++` est un langage **compilé** contrairement, par exemple, à `Python` qui est interprété ou à `Java` qui a une machine virtuelle la `JVM` (Java virtual machine) qui exécute des programmes `Java` compilés sous forme d'un codage (bytecode) de beaucoup plus bas niveau que `Java` mais qui reste encore indépendant d'un langage-machine spécifique.

```bash
$ g++ mon-super-programme.cpp -o foo 
$ ./foo
```

Le `c++` est un langage **orienté objet**. Vous allez pouvoir définir de nouveaux types de données exprimés par des structures formées par d'autres types de donnnées. Un type pour gérer les `date` sera, par exemple, composée de trois entités pour le jour, le mois et l'année.

`c++` est un langage de **haut niveau**, par sa POO (programmation orienté objet), mais aussi par sa notion de `templates` qui sont des modèles de classes, les classes ne seront réellement créées que lors de la compilation.

`c++` est un langage **nornalisé ISO** *International Standard ISO/IEC 14882:2017(E) – Programming Language C++* voir  https://isocpp.org/about

`c++` comporte une vaste **librairie standard** `std` (containeurs, algorithmes, entrées/sorties...) qui fait partie de la norme du langage. Il existe naturellement aussi des libraries extérieures à la norme, comme par exemple celles développées par `boost` https://www.boost.org/ qui parfois sont inclues dans la norme.

+++

## la partie du `c++` que vous allez voir

+++

### *slides*

+++

<ins>la partie du c++ que vous allez voir</ins>

* le langage reste complexe


* de nombreuses notions (mémoire)


* un langage à différents niveaux d'abstration:

   - des mécanismes de base (le `c` sous jacent, les exceptions, les classes, la dérivation...)
   
   - des mécanismes de haut niveau (les templates, les conteneurs de la `stl` - gestion automatique de la mémoire ...)
   
   - une forte évolution du langage c++11, c++14, c++17, c++20...

+++

   
   
en 9 séances de cours:

* le langage de base de `c++11`

* les templates simples

* quelques types et fonctionnalités de la `stl`

+++

### *notes*

+++

Ce langage reste complexe. Son apprentissage nécessite la compréhension de pas mal de notions dont, par exemple, la manière dont est organisée la mémoire lorsqu'un programme s'exécute.

Il est possible de programmer en `c++` en restant à un haut niveau d'abstration et, par exemple, en évitant de gérer la mémoire (en utilisant les conteneurs standards, les smart-pointers, auto...). Mais pour nous, vous enseigner `c++` consiste à vous en faire comprendre les mécanismes de base de son fonctionnement aussi vous allez mettre les mains dans le cambouis, lors de ces 9 séances de cours, qui ne suffisent de toutes manières pas pour aborder les constructions plus abstraites. Vous verrez ces constructions par vous même.

+++

## une très forte évolution du langage depuis les 30 dernières années

+++

Créé dans les années 80, au début comme une extension de programmation orientée objets au langage `c`  encore appelé `c with classes`, ce langage évolue depuis, vers toujours plus d'abstraction (smart/unique pointer, auto/for-auto, templates variadiques...). Il est un `modern c++`. Voir https://en.cppreference.com/w/cpp/language/history

+++

<img src="media/iso-c++-timeline.png" width=500>

+++

## sa relation avec `c`

+++

Le langage `c` a été conçu par Dennis Ritchie chez Bell Labs dans les années 1970 pour programmer le système d'exploitation `UNIX`. Le premier tutoriel `c` a été co-écrit par Brian Kernighan en 1978. Si vous aimez l'archéologie le texte original est là https://archive.org/details/TheCProgrammingLanguageFirstEdition).

Un comité de normalisation du `c` a été  initié en 1983. Lnorme ANSI-C a été achevée en 1989 *(C89) ISO/CEI 9899: 1999*  
(ISO=Organisation internationale de normalisation)  
(CEI=Commission internationale d'électrotechnique)

Le langage C est toujours vivant. Sa dernière norme `ANSI-C` remonte à 2018, pour une simple `bugfix release`. Il est à l'origine ou a influencé de nombreux autres langages, dont le  `c++`.

Dans ce cours, il n'y aura pas de présentation du langage `c` lui-même nous vous montrerons les bases  de `c` apparaissant dans le langage `c++`.
  
`c++` se fonde sur le langage de programmation `c` et la librairie standard de `c` est devenue un sous-ensemble de la librairie standard de `c++`.

Le langage `c` continue à exister de manière séparée de `c++`.

+++

## la compilation et l'exécution d'un programme

+++

### *slides*

+++ {"cell_style": "split"}

<ins>édition, compilation, édition de liens et exécution d'un programme `c++`</ins>

(on y reviendra plus en détail mais on en donne une petite idée)
* l'édition des programmes

* la compilation  
erreurs lexicales et  syntaxiques


* l'édition de liens (libraries)  
*undefined*, *multiple defined*


* l'exécution  
*segmentation-fault*

+++ {"cell_style": "split"}

```bash



$ code first.cpp         # edition
$ g++ -c first.cpp       # compilation
$ ls
first.cpp first.o
# edition de liens
$ g++ first.o -o my-first-prg

$ ./my-first-prg
0 # oh ! it is not 4294967296
```

+++

### *notes*

+++

Après avoir écrit (edité) le programme sous un éditeur de texte, vous allez passer à la phase de compilation du programme puis à son exécution. C'est à dire que le programme deviendra un processus que vous système d'exploitation exécutera sur le processeur de votre ordinateur.

Vous aurez deux types d'erreurs. Les premières sont des erreurs qui apparaissent au moment de la compilation. C'est à dire que vous aurez pas bien obéi à la syntaxe ou au lexique de `c++` et celui-là refusera de faire de votre programme un exécutable. Pour corriger ces erreurs là il y a un excellent moyen: *lire le message qu'écrit le compilateur* et/ou lire la documentation sur Internet (*RTFM*) ... et demander de l'aide si vous ne comprennez pas les messages d'erreur.

Les secondes erreurs sont les plus vicieuses: ce sont celles qui apparaissent au moment de l'exécution de votre programme. En fait non, elles apparaissent surtout au moment où vous faites une démonstration de votre programme à quelqu'un et oui surtout quand cette personne est un chef, un client ... Et la pire de toutes les erreurs est la fameuse `segmentation fault` là vous avez fait quelque chose qui a fortemen déplu à `c++` et il a tué l'exécution de votre programme (en espérant que ce n'était pas le pilote automatique d'un véhicule autonome...).

La programmation est quelque chose de relativement laborieux (surtout dans des langages de programmation compliqués comme `c++`), naturellement plus on code meilleur on est. Mais surtout: plus vous passerez de temps à comprendre (par vous même) *Pourquoi ce ... de programme ne compile pas ?* ou (surtout) *Pourquoi ce ... de programme plante à l'exécution ?* ... plus vous gagnerez en expertise sachant qu'on ne vous y reprendra pas deux fois de faire la même erreurs ... enfin plutôt pas à trois fois ou quatre...

+++

## les fichiers qui contiennent vos programmes

+++

### *slides*

+++

<ins>les fichiers qui contiennent vos programmes</ins>

des fichiers de texte avec des extensions pour être reconnus par les autres logiciels

des fichiers d'implémentation `.cpp`, `.cc`, `.cxx` ...

des fichiers d'entête (de définitions de types, de déclatation de fonctions et de variables) `.h`

+++

### *notes*

+++

Vos programmes vont être écrits dans des fichiers de texte auxquels vous allez donner une extension afin qu'ils soient reconnus comme des programmes `c++`, par exemple `.cpp` ou `.cxx` ou `.cc` ou `c++` mais pas `.c` qui est réservé au langage `c`.

Pour les écrire, vous allez utiliser l'éditeur vs-code auquel vous ajouterez l'extension `c++` pour bénéficier de tout un tas d'aide à l'édition.

+++ {"tags": []}

## les caractères qu'on trouve dans les programme `c++`

+++ {"tags": []}

### *slides*

+++ {"tags": []}

<ins> les caractères qu'on trouve dans les programme c++ </ins>

une restriction sur les caractères utilisables dans vos programmes:   
* le caractère espace
* plusieurs caractères de contrôle *non imprimables* (tabulations, backspace, delete, return ...)
* les 91 caractères suivants:  
```c++
a b c d e f g h i j k l m n o p q r s t u v w x y z
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
0 1 2 3 4 5 6 7 8 9
_ {} [] # () <> \%:; . ? * + - / ^ & | ~! =, \ "'
```

utilisation d'un codage `ascii` sur 7 bits (`b7 b6 b5 - b4 b3 b2 b1` genre `J` est le code `100 1010` ou 4A` ou `4a` )


préférez ne pas utiliser d'accents dans les identificateurs pour des problèmes de portage de code

+++ {"tags": ["level_intermediate"]}

C'est bien connu, les ordinateurs préfèrent les nombres surtout quand ils sont codés en binaire ou en hexadécimal. En ce qui nous concerne, nous aimons bien les lettres et, clairement, nous ne sommes pas disposés à parler à nos ordis en binaire ou en hexadécimal, éventuellement, si on doit faire un effort, on accepte de leur parler en anglais ! Oui parce que des langages de programmation en francais ben y'en a plus.

+++ {"tags": ["level_intermediate"]}

En informatique il va donc souvent être question de codes. Par exemple, les fichiers texte de nos programmes `c++` (nous parlons de tout ce qui est en dehors de chaînes de caractères entre `"` et `"`) vont utiliser un jeu de `96` carractères, qui s'appelle l'US-ASCII (American Standard Code for Information Interchange) aka UTF8-1:

Dans le code ascii on trouve: 
* le caractère espace
* plusieurs caractères de contrôle *non imprimables* (tabulations, backspace, delete, return ...)
* les 91 caractères suivants:
```c++
a b c d e f g h i j k l m n o p q r s t u v w x y z
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
0 1 2 3 4 5 6 7 8 9
_ {} [] # () <> \%:; . ? * + - / ^ & | ~! =, \ "'
```

+++ {"tags": ["level_intermediate"]}

Ce code est (naturellement) fondé sur l'alphabet anglais, qui comporte nombre réduit de caractères (pas d'accents...), et dont l'encodage tient sur 7 bits:
* il comprend donc 128 (i.e. $2^7$ = 128) codes  
de `0` à `127` en binaire, `0` à `7F` en hexadécimal  
http://www.iana.org/assignments/character-sets/character-sets.xhtml
   

Sur ce vieux tableau (dessiné en 1972) vous voyez le codage ascii sur les `7` bits `b7 b6 b5 - b4 b3 b2 b1` avec
   - `b7 b6 b5` qui vont de `000` à `111` (en binaire) ou de `0` à `7` en décimal et hexadécimal
   - `b4 b3 b2 b1` qui vont de `0000` à `1111` (en binaire) ou de `0` à `F` en hexadécimal
   - le code ascii va du caractère `00` (`NUL` rien) au caractère `7F` `DEL` ou `delete` 
   - on voit que le chiffre `1` est de code `31` et que `A` est de code `41`

<img src="media/US-ASCII_code_chart.png" width=600>

+++ {"tags": ["level_intermediate"]}

Attention, les chaînes de caractères sont plus complexes à gérer (nous y reviendrons si nous en avons le temps).

+++

## le lexique et la grammaire du langage `c++`

+++

### *slides*

+++

<ins>le lexique et la grammaire du langage `c++`</ins>
* des unités lexicales comme les keywords `if`, `int`, `true`  
mais aussi les chiffres, les lettres, les identificateurs...
```
digit       [0-9]        // one digit from 0 to 9
letter      [A-Z_a-z]
identifier  ({letter}({letter}|{digit})*) // | is or * is 0 or several times
```
* des règles syntaxiques 
```
selection_statement:
      IF '(' condition ')' statement
    | IF '(' condition ')' statement ELSE statement
    | SWITCH '(' condition ')' statement
    ```

+++

Que trouve-t-on dans un langage de programmation ? Naturellement un langage  de programmation va avoir (comme les langages naturels) des **unités lexicales**, par exemple `if` est le nom de l'instruction de comparaison, il va aussi avoir une **syntaxe** pour écrire des programmes en organisant ces unités lexicales entre elles.


Les unités lexicales c ++ sont: des identificateurs (y compris les mots-clés), des nombres, des caractères, des chaînes de caractères, des opérateurs (comme `+`), des caractères de ponctuations (`;`).

Voici quelques uns des mots clés du langage `c++` y'en a d'autres (`and`, `or`, , `not`...) et il s'en crée de nouveaux avec l'évolution de la norme:

|  |   |   |   |   |
|:--|:---|:---|:---|:---|
| alignas | alignof | asm | auto | bool |
| break | case | catch | char | char16_t |
| char32_t | class | const | const_cast | constexpr |
| continue | decltype | default | delete | do |
| double | dynamic_cast | else | enum | explicit |
| export | extern | false | float | for |
| friend | goto | if | inline | int |
| long | mutable | namespace | new | noexcept |
| nullptr | operator | private | protected | public |
| register | reinterpret_cast | return | short | signed |
| sizeof | static | static_assert | static_cast | struct |
| switch | template | this | thread_local | throw |
| true | try | typedef | typeid | typename |
| union | unsigned | using | virtual | void |
| volatile | wchar_t | while | ...|... |

+++ {"tags": ["level_intermediate"]}

Les unités lexicales sont décrites par des expressions régulières comme cela:

```c++
ws          [ \f\v\t]    // 4 white spaces: ' ', le saut de page, vertical and horizontal tabs 
digit       [0-9]        // one digit from 0 to 9
hex         [0-9A-Fa-f]  // one digit from 0 to 9, or from A to F or a o f
letter      [A-Z_a-z]    // one letter from A to Z, or from a to z or '_'
identifier  ({letter}({letter}|{digit})*)            // | is or
number      (\.?{digit}({digit}|{letter}|[eE][-+]|\.)*) // * is 0 or several times
                                                        // ? is o or one time
    ...   
```

+++ {"tags": ["level_intermediate"]}

Les règles syntaxiques (grammaires) des langages de programmation sont écrites dans des formats dédiés, comme le format EBNF (Extended Backus-Naur Form). Ces formats sont ensuite exploités par des outils d'analyse syntaxique (ou *parser* en anglais). Cette analyse est, avec l'analyse lexicale (ou *lexer*), réalisée dans les toutes premières phases de compilation.

+++ {"tags": ["level_intermediate"]}

Les règles syntaxiques sont décrites comme cela:

```bash
%keyword IF SWITCH ELSE WHILE DO FOR ...
%%
selection_statement:
      IF '(' condition ')' statement
    | IF '(' condition ')' statement ELSE statement
    | SWITCH '(' condition ')' statement
      
iteration_statement:
      WHILE '(' condition ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' for_init_statement condition.opt ';' expression.opt ')' statement

built_in_type_specifier:
   CHAR | WCHAR_T | BOOL | SHORT | INT | LONG | SIGNED | UNSIGNED | FLOAT | DOUBLE | VOID
   

```

Cette syntaxe décrit les constructions syntaxiques qui sont autorisées dans un langage. Remarquez que vous voyez aussi la liste des noms des types prédéfinis (*built in*) de c++ comme `int`, `char`, `bool`...

Inventer un langage va consister à en inventer le lexique, la grammaire ainsi que le sens (ce qu'ils font) des constructions de votre langage. Le lexique et la syntaxe seront écrits comme indiqué ci-dessus et les sémantiques seront données par des morceaux de codes suivant les expressions syntaxiques.
