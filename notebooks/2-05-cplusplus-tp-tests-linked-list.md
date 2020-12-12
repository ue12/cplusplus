# une idée pour les tests de la liste doublement chaînée


Quand un programme est écrit et qu'il compile cela ne vous assure pas qu'il fonctionne correctement pour autant. Vous devez le tester, nous allons donner ici une petite idée des tests que nous pourrions faire pour la liste chaînée.


## les fonctions de la liste


Voici les fonctions demandées dans le TP de base où on a rajouté 3 fonctions utiles pour les tests.

On donne ensuite des idées d'autres fonctions à rajouter plus ou moins simples

**Les fonctionnalités demandées dans le TP de base:**
- créer une liste vide  `IntList::IntList ()`
- ajouter un élément en début de liste `IntList::add_front (int)`
- ajouter un élément en fin de liste `IntList::add_back (int)`
- afficher la liste `void IntList::print ()`
- enlever l'élément de début de liste `void IntList::remove_front ()`
- enlever l'élément de fin de liste `void IntList::remove_back ()`
- enlever la première instance d'un élément `void IntList::remove (int)`
- déterminer si la liste contient un élément `bool IntList::search (int)`

**Les 3 fonctions à rajouter**
- donner la taille de la liste `int IntList::size ()`
- tester si la liste est vide `bool IntList::is_empty ()`
- renvoyer l'élément à une position donnée `int IntList::get_element_at (int)`

**D'autres fonctionnalités:**
- vider la liste `void IntList::empty ()`
- enlever toutes les instances d'un élément `void IntList::remove_all (int)`

- créer une liste avec un élément `IntList::IntList (int)`
- créer une liste à partir d'un tableau  (à-la-c) d'éléments `IntList::IntList (int*)`

- renvoyer la position d'un élément dans la liste (le premier trouvé) `int IntList::position (int)`
- renvoyer l'élément à une position donnée `int IntList::get_element_at (int)`
- ajouter un élément à une position donnée `void IntList::add_element_at (int, int)`
- enlever l'élément à une position donnée `void IntList::remove_element_at (int, int)`


<!-- #region -->
```c++
class IntList {
public:
  IntList ();
  ~IntList ();
  bool is_empty ();
  int size ();
  int get_element_at (int i);
  void add_front (int e);
  void add_back (int e);
  void remove_front ();    
  void remove_back();
  void remove (int e);
  bool search (int e);
  void print ();
private:
  /* vos attributs ici */
};

```
<!-- #endregion -->

## les fonctions de tests


Nous ne listons pas toutes les fonctions de test mais donnons une idée des premières fonctions de certains cas, à vous de les compléter et à vous de rajouter des cas de tests.


Concernant les tests, pour certains programmes il s'agira de tester une seule sortie: si vous avez implémenté une fonction qui tri un tableau, les fonctions de test vont consister à appeler la fonction de tri sur différents tableaux, en n'oubliant pas le tableau vide et le tableau à un élément, et vous vérifiez qu'ils sont triés.

Dans d'autres programmes, vous avez une plus grande diversité de cas à tester, comme ce sera la cas pour la liste chaînée.

Il est préférabe d'écrire de petites fonctions de test bien ciblées abordant chacune une notion. N'écrivez pas de looongues fonctions de tests qui font des tas de cas, en cas d'erreur, vous allez perdre du temps à comprendre où ca se passe.

Il faut comprendre que l'exhaustivité des tests n'est pas possible. Vous ne pourrez jamais générer et tester tous les cas possibles mais votre couverture de tests doit être la plus complète possible.

Il en faut pas oublier les cas exceptionnels, comme par exemple dépiler une pile vide, inverser une matrice non inversible ou les cas des structures vides, la liste vide, la pile vide.

Quand on modifie un code, il est important de s'assurer qu'on n'a pas compromis le code existant qui fonctionnait (i.e. passait les tests) auparavant. C'est pour cela que vous devez écrire soigneusement vos fonctions de test, que vous devez les garder et repasser ces tests lors de modifications ou d'extension de votre code.

On va donner ci-dessous des exemples simples de ce que pourraient être des tests sur la liste doublement chaînée. 


On va faire un fichier de test d'entête `test-intlist.h` et un fichier de tests d'implémentation `test-intlist.cpp` qui contiendra la fonction `main`.


## le fichier d'entête des tests `test-intlist.h`


alors oui il se peut que ces fonctions de tests comportent des erreurs ... n'hésitez pas à les corriger !


### Les includes


ilne faut pas oublier d'inclure dans votre fichier de test votre fichier d'entête de la liste et les librairies que vous utilisez comme les `iostream` ou encore `cstdlib` pour la fonction `rand`...

<!-- #region -->
```c++
#include "intlist.h"
#include <cstdlib>
```
<!-- #endregion -->

### on teste la construction

<!-- #region -->
```c++
// test de la construction
inline void test_default_constructor_1 () {
  std::cout << "test default constructor on empty list";
  IntList list;
  std::cout << std::boolalpha <<  " *** " << ((list.size() == 0) and (list.is_empty())) << std::endl;
}
// ...
```
<!-- #endregion -->

### on teste la fonction `add_front`

<!-- #region -->
```c++
inline void test_add_front_1 () {
  std::cout << "test add front one element with value";
  IntList list;
  int e1 = rand();
  list.add_front(e1);
  std::cout << std::boolalpha << " *** " <<  (list.get_element_at(0) == e1) << std::endl;
}

inline void test_add_front_2 () {
  std::cout << "test add front n elements with values";
  IntList list;
  int n = rand()%10;
  int tab [n];
  for (int i = 0; i < n; i++) {
    tab[i] = rand();
    list.add_front(tab[i]);
  }
  for (int i = n-1; i >= 0; i--) {
    if (tab[(n-1)-i] != list.get_element_at(i)) {
      std::cout << " *** " << "bad element value" << std::endl;
      return;
    }
  }
  std::cout << " *** " << "true" << std::endl;
}
// ...  
```
<!-- #endregion -->

### on teste la fonction `add_back`

<!-- #region -->
```c++
inline void test_add_back_1 () {
  std::cout << "test add back one element with value";
  IntList list;
  int e1 = rand();
  list.add_back(e1);
  std::cout << std::boolalpha << " *** " <<  (list.get_element_at(0) == e1) << std::endl;
}

inline void test_add_back_2 () {
  std::cout << "test add back n elements with values";
  IntList list;
  int n = rand()%10;
  int tab [n];
  for (int i = 0; i < n; i++) {
    tab[i] = rand();
    list.add_back(tab[i]);
  }
  for (int i = 0; i < n; i++) { 
    if (tab[i] != list.get_element_at(i)) {
      std::cout << " *** " << "bad element value";
      return;
    }
  }
  std::cout << " *** " << " true" << std::endl;
}
// ...
```
<!-- #endregion -->

###  on teste la taille de la liste

<!-- #region -->
```c++
// test size
inline void test_size_1 () {
  std::cout << "test size of empty list";
  IntList list;
  std::cout << std::boolalpha << " *** " <<  (list.size() == 0) << std::endl;
}

// test add_front
inline void test_size_2 () {
  std::cout << "test size one element";
  IntList list;
  int e = rand();
  list.add_front(e);
  std::cout << std::boolalpha << " *** " <<  ((list.size() == 1) and (not list.is_empty())) << std::endl;
}

inline void test_size_3 () {
  std::cout << "test size of list with n elements";
  IntList list;
  int n = rand()%100;
  for (int i = 0; i < n; i++) {
    int e = rand();
    list.add_front(e);
  }
  std::cout << std::boolalpha << " *** " <<  (list.size() == n) << std::endl;
}
// ...
```
<!-- #endregion -->

### il reste des tas de tests à faire ...


## le fichier d'implémentation tests `test-intlist.cpp`

<!-- #region -->
```c++
#include "test-intlist.h"

int main () {
  test_default_constructor_1 ();
  test_add_front_1 ();
  test_add_front_2 ();
  test_add_back_1 ();
  test_add_back_2 ();
  test_size_1 ();
  test_size_2 ();           
  return 0;
} 
```
<!-- #endregion -->

## la compilation et l'exécution


on vérifie que le programme passe ces premiers tests ...

<!-- #region -->
```bash
$ g++ test-intlist.cpp -o test-intlist
$ ./test-intlist
test default constructor on empty list *** true
test add front one element with value *** true
test add front n elements with values *** true
test add back one element with value *** true
test add back n elements with values ***  true
test size of empty list *** true
test size one element *** true
$
```
<!-- #endregion -->
