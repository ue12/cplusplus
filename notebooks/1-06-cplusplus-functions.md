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

# les fonctions


## prototype des fonctions (arguments et type de retour)

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">arguments et types de retour des fonctions</ins>

<br>

une fonction est introduite par son **prototype** qui indique
* tout d'abord son **type de retour** i.e. le type des éléments que la fonction retourne
* puis le **nom** de la fonction
* puis la **liste des arguments** de la fonction
* puis vient le **corps** de la fonction entre `{` et `}`

<br>
 
donnons comme exemple la fonction qui calcule le maximum de deux entiers

```c++
bool max (int a, int b) {
  if (a < b)
    return b;
  else
    return a;
}

int main () {
  max(12, 45);
  return 0;
}
```

<br>

si la fonction de retourne pas d'élément, son type de retour doit être `void`

<br>

si la fonction ne prend pas d'arguments
* soit la liste des arguments est vide
* soit al liste des arguments contient `void`

```c++
void foo () {
   // la fonction ne fait rien
}
void bar (void) {
   // la fonction ne fait rien non plus
}
int main () {
  foo();
  bar();
  return 0;
}
```
</div>
<!-- #endregion -->

À vous de jouer. Dans un fichier `foo-int.cpp`. Implémentez une fonction `foo` qui prend un `int` en argument, qui ne renvoie rien et qui incrémente de `1` cet `int` dans le corps de la fonction. Implémentez une fonction `main` qui crée une variable `j` de type `int` avec une valeur initiale que vous choisissez, qui appelle la fonction `foo` avec cette variable en argument et qui après l'appel de la fonction affiche la valeur de `j`. Que constatez-vous ? 


## passage des arguments à une fonction


Nous allons expliquer ici quelque chose de très important à savoir la manière dont les arguments passés à une fonction sont traités.

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title">les arguments des fonctions sont passés par copie</ins>


<br>

prenons le programme suivant où un `int` est passé en argument à une fonction `foo`

<br>

```c++
void foo (int i) { // stack 2
   i = i + 1;      // stack 3
}
int main () {
  int j = 17; // stack 1
  foo(j);     // stack 4
 // j stays unchanged
 return 0;
}
```
<br>

montrons l'état de la pile d'exécution aux 4 endroits du code précédent:

```bash
   stack 1   # on est sur la première ligne du main
 --------    # un j qui vaut 17 a été mis dans la pile   
 |      | 
 |      | 
 |      |<- haut de pile 
 |------|
 | j=17 | 
 -------- 
```

<br>

```bash
    stack 2    # foo(j) vient d'être appelée
 --------      # l'argument de la fonction i a été empilé 
 |      |<-    # i avec la valeur de j donc 17 est dans la pile
 |------|      
 | i=17 |      # i n'a plus aucun lien avec j
 |------|
 | j=17 |
 --------
```

<br>

```bash
    stack 3
 --------     # i a été augmenté de 1
 |      |<- 
 |------|  
 | i=18 |  
 |------|
 | j=17 |
 --------
```

<br>

```bash
    stack 4
 --------     # on est sorti de la fonction foo
 |      |     # le compteur de haut de pile
 |------|     # est redescendu
 |      |<- 
 |------|
 | j=17 |
 --------
```

nous constatons que `j` n'a pas été modifié par ce code  
sa valeur a simplement servi à initialiser `i`  
et le `i` qui a été augmenté de `1` n'existe plus
</div>
<!-- #endregion -->

À vous de jouer. Dans un fichier `foo-pint.cpp`. Implémentez une fonction `foo` qui prend l'adresse d'un `int` en argument, qui ne renvoie rien et qui incrémente l'objet, *pointé* par cette adresse, de `1` dans le corps de la fonction. Implémentez une fonction `main` qui crée une variable `j` de type `int` avec une valeur initiale que vous choisissez, qui appelle la fonction `foo` avec l'adresse de cette variable en argument et qui après l'appel de la fonction affiche la valeur de `j`. Que constatez-vous ? 

Rappel sur les adresses (qui désignent des zones en mémoire).  
L'adresse d'un objet est l'endroit où il est stocké en mémoire
* l'opérateur `&` appliqué à un objet, retourne son adresse
* l'opérateur `*` appliqué à une adresse, retourne l'objet à cette adresse

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title"> passons l'adresse d'un argument </ins>

<br>

ou encore passons un argument par adresse (par pointeur)

<br>

vous vous rappelez ? l'adresse d'un objet est l'endroit où il est stocké en mémoire
* l'opérateur `&` appliqué à un objet, retourne son adresse
* l'opérateur `*` appliqué à une adresse, retourne l'objet à cette adresse

<br>
prenons l'exemple d'ue fonction `foo` où l'adresse d'un entier est passé à une fonction


```c++
void foo (int* pi) { // stack 2
   *pi = *pi + 1;    // stack 3
}
int main () {
   int j = 17; // stack 1
   foo(&j);    // stack 4
   // j has changed !
   return 0;
}
```
      
<br>

montrons ce qui se passe dans la pile d'exécution


```bash
      stack 1       # on est sur la première ligne du main
     ---------      # un j qui vaut 17 a été mis dans la pile   
     |       | 
     |       |<- haut de pile 
     |-------|
     | j=17  | 
     --------- 
```

<br>

```bash
      stack 2       # &j est addr = l'adresse de j
     ---------      # on appelle foo avec cette adresse en argument 
     |       |<-    # pi est empilé sur la pile
     |-------|      # c'est un pointeur il a besoin de plus de place qu'un entier
     |       |       # la valeur de pi est l'adresse de j donc addr
     |pi=addr|----    # c'est comme si pi "pointait" sur la case qui contient j
     |-------|   |
addr | j=17  |<---
     ---------
```
<br>

```bash
      stack 3        # *pi est l'objet à l'adresse pi
     ---------       # avec *pi on "va" où pointe pi 
     |       |<-     # on arrive dans la case j
     |-------|       # on prend sa valeur 17
     |       |
     |pi=addr|----     # on lui ajoute 1 donc 18
     |-------|   |     # et on met cette valeur dans la case *pi
addr | j=18  |<---     # donc on met 18 dans la case j
     ---------
```
<br>

```bash
      stack 4       # la fonction foo se termine
     ---------      # le haut de pile redescend 
     |       |      # pi n'existe plus
     |-------|         
     |       |<-       
     |-------|         
addr | j=18  |     
     ---------
```
<!-- #endregion -->

### exercice la fonction `swap` (échange)

<!-- #region -->
implémenter une fonction `swap` qui échange le contenu de deux variables:
   - on l'appelle en lui passant les deux variables
   - et quand on sort de la fonction le contenu de ces deux variables a été échangé
   - demandez-vous quel doit être le type des arguments

```c++
// in file swap.cpp
#include<iostream>
void swap (/* your code here */ x, /* your code here */ y) {
    // your code here
}
int main () {
   int a = 12;
   int b = 81;
   std::cout << a << ' ' << b << std::endl;
   // affiche 12 81 
   swap ( /* your code here */, /* your code here */ );
   std::cout << a << ' ' << b << std::endl;
   // affiche 81 12
}```

```bash
$ g++ swap.cpp -o swap
$ swap
12 81
81 12
```
<!-- #endregion -->

## retour des fonctions


<div class="framed-cell">
<ins class="underlined-title"> adresse de retour de fonctions</ins>

à chaque fois qu'une fonction est appelée, une copie de ses arguments est créée dans la pile d'exécution

<br>

lors de l'exécution de la fonction, si vous y définissez des variables locales, elles sont aussi mises dans la pile d'exécution

<br>

et quand la fonction termine ? 
* toutes ses variables locales et ses arguments sont "libérées" de la pile
* le compteur de haut pile redescend

<br>

la place libérée dans la pile pourra être réutilisée dès le retour de la fonction

<!-- #region -->
<div class="framed-cell">
    <ins class="underlined-title"> Que se passe-t-il dans cet horrrrrrible code ? </ins>
    
Dans le code suivant, que pensez-vous de la valeur de `k` ?

```c++
// dans le fichier segfault.cpp
int* foo () {
    int j = 17;
    return &j;  // stack 2
}
int main () {
    int* pi = nullptr;  // stack 1
    pi = foo();  // stack 3
    int k = *pi;  // stack 4
    return 0;
}
```

<br>

alors que fait ce code ?
* la première ligne du `main` alloue, dans la pile d'execution, la place pour stocker l'adresse d'un `int` et lui met la valeur de l'adresse nulle `nullptr`
* à l'appel de la fonction `foo` la variable `j` de type `int` et de valeur initiale `17` est mise dans la pile d'exécution, puis son adresse est retournée par la fonction `&j`
* lorsque la fonction se termine, cette adresse est stockée dans `pi` mais puisque la fonction `foo` est terminée, les variables locales ont été enlevées de la pile... donc l'adresse que vous retournez n'est pas **valide**

ou encore:

```bash
       stack 1      # on est sur la première ligne du main
     ----------     # un pi qui vaut nullptr (ici 0) a été mis dans la pile   
     |        |<- haut de pile 
     |--------|
     |        |
     |  pi=0  | 
     ---------- 
```

<br>

```bash
       stack 2       
     ----------      # la fonction foo est appelée
     |        |<-    # un j de type int et de valeur 17 est mis dans la pile
     |--------|      # son adresse est retournée par la fonction
addr | j=17   |   
     |--------|   
     |        |
     | pi=0   |
     ----------
```
<br>

```bash
       stack 3        
     ----------       # l'adresse retournée est stockée dans pi
addr |        |<-     # la fonction foo est terminée
     |--------|       # donc les variables locales de foo sont libérées
     |        |       # donc la valeur dans pi n'est PAS VALIDE
     | pi=addr|
     ----------
```

<br>

```bash
      stack 4        
     ----------       
     |        |<-    # un int k est mis sur la pile d'exécution 
     |--------|      # il est initialisé avec l'objet à l'adresse pi
     | k=???? |      # mais cette adresse n'est pas valide
     |--------|      # un segmentation-fault est déclenchée
     |        |       
     | pi=addr|
     ----------
```
<br>

on compile `segfault.cpp`

```bash
$ g++ segfault.cpp
segfault.cpp: In function ‘int* foo()’:
segfault.cpp:2:9: warning: address of local variable ‘j’ returned [-Wreturn-local-addr]
     int j = 17;
```

déjà à la compilation votre compilateur peut éventuellement vous avertir (`warning`) donc
* première règle: **toujours bien lire les message d'`erreur` et les `warnings`**

<br>

on exécute

```bash
$ a.out
Segmentation fault (core dumped)
$ 
```

* à la compilation le process accède une adresse qui est invalide
* vous allez dans un segment de mémoire qui n'est pas *à vous*
* votre système d'exploitation **tue brutalement** votre process
* espérons que votre programme ne devait pas conduire un véhicule autonome...

</div>
<!-- #endregion -->

## surcharge des noms de fonctions


<div class="framed-cell">
<ins class="underlined-title"> les noms des fonctions peuvent être surchargés (overloading)</ins>

<br>

on peut définir plusieurs fonctions avec le même nom

<br>

bien sûr `c++` doit savoir laquelle appeler !

<br>

donc leur utilisation doit pouvoir être discriminée
* soit par le nombre des arguments
* soit par le type des arguments
* mais jamais par le type de retour
    
<br>

>```c++
>// dans la fonction prints.cpp
>#include<iostream>
void print (int)         { std::cout << "print (int)\n"; }
void print (char)        { std::cout << "print (char)\n"; }
void print (const char*) { std::cout << "print (const char*)\n"; }
int main () {
  print(12);
  print('a');
  print("coucou");
  return 0;
}
```

>```bash
>$ g++ prints.cpp
>$ a.out
print (int)
print (char)
print (const char*)
```

</div>


## arguments par défaut des fonctions

<!-- #region -->
<div class="framed-cell">
<ins class="underlined-title"> les arguments par défaut des fonctions </ins>


<br>

les fonctions peuvent avoir des argments par défauts

<br>

```c++
void foo (int i=1, int j=2, int k=3) {}
int main () {
                   //  i    j    k
  foo();           //  1    2    3
  foo(10);         // 10    2    3
  foo(10, 20);     // 10   20    3
  foo(10, 20, 30); // 10   20   30
}
```

<br>

ainsi vous avez là 4 manière d'appeler cette fonction
</div>
<!-- #endregion -->
