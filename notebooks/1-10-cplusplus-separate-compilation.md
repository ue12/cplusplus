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

# séparation d'un programme en plusieurs fichiers

+++

Le but de ce notebook est de séparer un code en fichiers d'implémentation `.cpp` et fichiers d'entête `.h` et d'en faire la compilation séparée `g++ -c` afin d'avoir des fichiers en code objet `.o`.

**warning**
* le code donné ci-dessous est un exemple: vous devez si vous l'avez, utiliser votre propre code.
* on considère que les expressions sont bien formées donc nous n'avons mis aucun rattrapage d'erreurs  
cela viendra plus tard lorsque nous apprendrons à utiliser les exceptions `c++`  
ainsi, si une chaîne de caractère commence par un chiffre, on considérera qu'elle ne va contenir que des chiffres et aussi la fonction `is_operand` ne teste-t-elle que le premier caractère de la chaîne de chiffres 
* ce code `c++` peut naturellement être amélioré (par exemple en mettant certaines fonctions `inline` ou en protégeant les `.h` contre l'inclusion multiple)  
mais nous verrons ces notions un peu plus tard

+++

## exemple de code

+++

<div class="framed-cell">
<ins class="underlined-title">exemple du code de la calculette en notation polonaise inversée</ins>
    
Jusqu'à présent nous avons écrit nos programmes dans un seul fichier `main.cpp`. L'idée de ce notebook est de montrer comment séparer intelligemment un programme en plusieurs fichiers d'implémentation (les `.cpp`) et de déclaration (les `.h`). Afin de rendre possible la réutilisation du code et donc nous allons faire de la compilation séparée i.e. chaque `.cpp` deviendra un `.o`.

En annexe nous donnons un exemple de code de la calculette afin que les élèves n'ayant pas leur propre version puissent faire l'exercice de séparation, mais naturellement il est préférable de travailler sur son propre code. 

Pourquoi séparer un programme ? Prenons l'exemple de notre calculette en notation polonaise inversée où toutes les fonctions ont été toutes mises dans un fichier `rpn_calc.cpp`. Nous y remarquons trois parties très différentes:
* la pile
* la calculette
* le fichier qui contient la fonction `main` et qui teste la calculette

Les trois fonctions qui définissent la pile `init_stack`, `pop`, `push`, `print_stack` et `delete_stack` ne sont pas utilisables dans d'autres programmes, pourtant cette pile pourrait être utilise dans d'autres contextes que celui de la calculette

De même la calculette définie par les fonctions `is_operator`, `is_operande` et `rpn_eval` pourrait être utilisée dans un autre contexte

La fonction `main` sert ici simplement à tester notre calculette

+++

## séparation du code

+++

<div class="framed-cell">
<ins class="underlined-title">on sépare notre programme de la calculette en plusieurs fichiers</ins>
    
nous allons séparer le code de notre calculette en polonaise inversée en plusieurs fichiers

<br>

le code des fonctions de la `stack` est mis dans le fichier `stack.cpp`  
les fonctions que nous voulons exporter vers l'extérieur sont déclarées dans le fichier `stack.h`

<br>

le code de la calculette est mis dans le fichier `rpn_calc.cpp`  
les fonctions que nous voulons exporter vers l'extérieur sont déclarées dans le fichier `rpn_calc.h`  
comme la calculette utilise les fonctions de la pile, elle doit importer naturellement le fichier `stack.h`

<br>

en effet toute fonction doit être déclarée (ou bien sûr définie) avant de pouvoir être utilisée  
puisque `c++` doit vérifier que notre appel de la fonction correspond bien à sa déclaration
    
<br>

enfin le `main` est mis dans le fichier de test `test.cpp`, les fichiers d'entête qui nous intéressent y sont importés

<br>

le code donné ci-dessous est un exemple, vous devez mettre votre propre code à la place  
ce code peut naturellement être amélioré (par exemple en mettant certaines fonctions `inline` ou en protégeant les `.h` contre l'inclusion multiple)  
mais nous verrons ces notions un peu plus tard 

<br>

Faites-le, avant de regarder l'exemple ci-dessous.

+++

## les fichiers `.h` et `.cpp`

+++

<div class="framed-cell">
<ins class="underlined-title">stack.h</ins>
    
```c++
// un exemple de fichier stack.h
// mettez votre propre version !
#include <iostream>
void push (int stack [], int* top, int token);
int pop (int stack [], int* top);
int* init_stack (int n);
void delete_stack (int stack []);
```

+++

<div class="framed-cell">
<ins class="underlined-title">stack.cpp</ins>

```c++
// un exemple de fichier stack.cpp
// mettez votre propre version !
#include <iostream>

void push (int stack [], int* top, int token) {
  stack[top] = token;
  *top = *top + 1;
}

int pop (int stack [], int* top) {
  *top = *top - 1;
  return stack[top];
}

int* init_stack (int n) {
  return new int[n];
}

void delete_stack (int stack []) {
  delete [] stack;
}

void print_stack (int stack []) {
  // votre code ici
}
```

+++

<div class="framed-cell">
<ins class="underlined-title">rpn_calc.h</ins>

```c++
// fichier rpn_calc.h
// mettez votre propre version !

int rpn_eval (int n, char* expr []);
```
</div>

+++

<div class="framed-cell">
<ins class="underlined-title">rpn_calc.cpp</ins>

```c++
// fichier rpn_calc.cpp
// mettez votre propre version !
#include<iostream>
#include"stack.h"
bool is_operand (char* token) {
  return ((token[0] >= '0') and (token[0] <= '9'));
}
bool is_unary_minus (char* token) {
  return token[0] == '!';
}
bool is_plus (char* token) {
  return token[0] == '+';
}
bool is_minus (char* token) {
  return token[0] == '-';
}
bool is_mult (char* token) {
  return token[0] == 'x';
}
bool is_div (char* token) {
  return token[0] == '/';
}
bool is_operator (char* token) {
  return (is_unary_minus(token)
          or is_plus(token)
          or is_minus(token)
          or is_mult(token)
          or is_div(token));
}
int rpn_eval (int n, char* expr []) {
  int* stack = init_stack(n);
  int top = 0;
  for (int i = 0; i < n; i++) {
    char* token = expr[i];
    if (is_operand(token))
      push(stack, &top, atoi(token));
    else if (is_operator(token)) {
      if (is_unary_minus(token)) {
        int val = pop(stack, &top);
        push(stack, &top, -val);
      } else {
        int val2 = pop(stack, &top);
        int val1 = pop(stack, &top);
        if (is_plus(token))
          push(stack, &top, val1+val2);
        else if (is_minus(token))
          push(stack, &top, val1-val2);
        else if (is_mult(token))
          push(stack, &top, val1*val2);
        else if (is_div(token))
          push(stack, &top, val1/val2);
      }
    }
  }
  int result = stack[0];
  delete_stack(stack);
  return result;
}
```
</div>

+++

<div class="framed-cell">
<ins class="underlined-title">test.cpp</ins>

```c++
// fichier test.cpp
// mettez votre propre version !
#include<iostream>
#include"rpn_calc.h"

int main (int argc, char* argv []) {
  std::cout << rpn_eval(argc-1, argv+1) << std::endl;
  return 0;
}
```
</div>

+++

## compilation séparée

+++

<div class="framed-cell">
<ins class="underlined-title">compilation séparée</ins>
  
Nous allons maintenant compiler les fichiers séparément c'est à dire transformer chaque `.cpp` en son fichier `.o` correspondant puis produire un exécutable en linkant ensemble les `.o`:

<br>

```bash
g++ -c rpn_calc.cpp
g++ -c stack.cpp
g++ -c test.cpp
g++ -o calc rpn_calc.o stack.o test.o
```

<br>

ce qui revient à faire:

```bash
$ g++ -o calc rpn_calc.cpp stack.cpp test.cpp 
```


</div>

+++

## modification de fichiers et recompilation

+++

Nos fichiers ont été compilés séparément dans des `.o` dont on a fait l'édition de lien, notre exécutable fonctionne ... tout va bien pour le moment

<br>

Supposons maintenant que je corrige une erreur dans la fonction `pop` du fichier `stack.cpp`, quels fichiers dois-je recompiler ? 

<br>

supposons maintenant que j'implémente la fonction `print_stack`
* je me rends compte qu'il faut lui passer en argument, le nombre des éléments empilés (qui est donc indiqué pour nous par `top`)
* je dois donc modifier le prototype de la fonction `print_stack(int stack[])` par  `void print (int stack[], int&)`

Quels sont tous les fichiers qui seront touchés par cette modification ?

Je les modifie et pour créer un nouvel exécutable ... je ne dois surtout pas oublier de recompiler les fichiers modifiés avant de refaire l'édition de lien sinon 

sinon mon exécutable est mal formé et fera des erreurs d'exécution (si ils n'en a pas fait au moment de l'édition de lien) ...

<br>

et si je modifie le fichier `test.cpp`, quels fichiers dois-je recompiler ? et si je modifie le code de la fonction `rpn_eval` ou son nombre d'arguments ... Quels fichiers dois-je recompiler ? 

<br>

vous avez compris, faire cela *à-la-main* sera fastidieux et source d'erreur ...
et surtout **je ne dois pas en oublier !** sinon je vais avoir des exécutables `mal formés`


<br>


d'un autre côté, c'est quelque chose qui pourrait se faire automatiquement sachant
* les dates de modification des fichiers
* les dépendances entre les fichiers

<br>

Prenons l'exemple du fichier `rpn_calc.cpp` qui inclut le fichier `stack.h`:
* donc dès que je vais modifier quelque chose dans `stack.h`, il faudra recompiler `rpn_calc.cpp`
* Comment est-ce que je sais que le fichier `stack.h` a été touché plus récemment que ``rpn_calc.cpp` ?
* et bien oui en comparant leurs dates de modification...

<br>

non vous n'allez pas devoir refaire cela à-la-main, dans les années 1970 la commande `make` a été inventée pour cela ... et elle est toujours utilisée malgré son utilisation de la tabulation comme séparateur ...

<br>

une introduction à la commande `make` est faite dans un prochain notebook
</div>

+++

# annexe

+++

## code de la calculette en notation polonaise inversée

+++

un exemple du code de la calculette en notation polonaise inversée  
avec toutes les fonctions dans un même fichier  
(soyez toujours critique vis à vis d'un code que vous récupérez il peut contenir des erreurs...)

```c++
#include<iostream>
#include<cstdlib>

void push (int stack [], int* top, int token) {
  stack[*top] = token; 
  *top = *top + 1;
}
int pop (int stack [], int* top) {
  *top = *top - 1;
  return stack[*top];
}
int* init_stack (int n) {
  return new int[n];
}
void delete_stack (int stack []) {
  delete [] stack;
}
void print_stack (int stack []) {
  // votre code ici
}
bool is_operand (char* token) {
  return ((token[0] >= '0') and (token[0] <= '9'));
}
bool is_unary_minus (char* token) {
  return token[0] == '!';
}
bool is_plus (char* token) {
  return token[0] == '+';
}
bool is_minus (char* token) {
  return token[0] == '-';
}
bool is_mult (char* token) {
  return token[0] == 'x';
}
bool is_div (char* token) {
  return token[0] == '/';
}
bool is_operator (char* token) {
  return (is_unary_minus(token)
          or is_plus(token)
          or is_minus(token)
          or is_mult(token)
          or is_div(token));
}
int rpn_eval (int n, char* expr []) {
  int* stack = init_stack(n);
  int top = 0;
  for (int i = 0; i < n; i++) {
    char* token = expr[i];
    if (is_operand(token))
      push(stack, &top, atoi(token));
    else if (is_operator(token)) {
      if (is_unary_minus(token)) {
        int val = pop(stack, &top);
        push(stack, &top, -val);
      } else {
        int val2 = pop(stack, &top);
        int val1 = pop(stack, &top);
        if (is_plus(token))
          push(stack, &top, val1+val2);
        else if (is_minus(token))
          push(stack, &top, val1-val2);
        else if (is_mult(token))
          push(stack, &top, val1*val2);
        else if (is_div(token))
          push(stack, &top, val1/val2);
      }
    }
  }
  int result = stack[0];
  delete_stack(stack);
  return result;
}
int main (int argc, char* argv []) {
  std::cout << rpn_eval(argc-1, argv+1);
  return 0;
}


```
