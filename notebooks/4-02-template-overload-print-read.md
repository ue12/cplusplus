---
jupyter:
  jupytext:
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
      jupytext_version: 1.7.1
  kernelspec:
    display_name: Python 3
    language: python
    name: python3
---

Continuons nos template


Tout d'abord, quelque chose qui n'est pas propre aux templates, c'est la définition de la fonction qui permet l'affichage d'un objet sur un flux de sortie, comme par exemple la sortie standard std::cout ou la sortie d'erreur std::cerr ...

<!-- #region -->
```c++
// dans le fichier main.cpp
#include <iostream>
int main (int argc, char* argv[]) {
    std::cout << "Le nom de mon exécutable est " << argv[0] << std::endl; 
}
```
```
$ g++ main.cpp
Le nom de mon exécutable est ./a.out
```
<!-- #endregion -->

Vous remarquez que quand on écrit un objet, d'un type de base comme une chaîne de caractère ou un entier, on utilise un flux de sortie (ici std::cout) dans lequel on redirige avec << un objet d'un de ces types.


Pour les types de base de c++, il sait très bien faire. Certains d'entre vous on même remarqué que si on lui donne un tableau d'entiers il nous affiche l'adresse du tableau et si on lui donne un tableau de char, il nous affiche le contenu du tableau et pas l'adresse. Ce serait vraiment trop nul si quand je disais de faire *std::cout << "Hello !";* c++ m'affichait *0x7ffd34b6c260* c'est ce qu'il ferait si il n'avait pas traité les tableaux de caractères d'une manière spéciale !

<!-- #region -->
```c++
// dans le fichier main.cpp
#include <iostream>
int main (int argc, char* argv []) {
  int itab [] = {1, 2, 3, 4};
  char ctab []  = {'h', 'e', 'l', 'l', 'o'};
  std::cout << ctab << " " << itab << std::endl; 
}
```
```
$ g++ main.cpp
hello 0x7ffc8d24cce0
```
<!-- #endregion -->

et pour nos types à nous, il n'essaie même pas de faire quoi que ce soit ! regardez ce qu'il fait quand on tente de lui faire afficher un objet d'un type X (il nous injurie copieusement)

<!-- #region -->
```c++
// in file main.cpp
#include <iostream>
class X {};
int main (int argc, char* argv []) {
  X x;
  std::cout << x << std::endl; 
}

```
```
$ g++ main.cpp
main.cpp: In function ‘int main(int, char**)’:
main.cpp:5:13: error: no match for ‘operator<<’ (operand types are ‘std::ostream {aka std::basic_ostream<char>}’ and ‘X’)
   std::cout << x << std::endl;
   ~~~~~~~~~~^~~~
In file included from /usr/include/c++/7/iostream:39:0,
                 from main.cpp:1:
/usr/include/c++/7/ostream:108:7: note: candidate: std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::basic_ostream<_CharT, _Traits>::__ostream_type& (*)(std::basic_ostream<_CharT, _Traits>::__ostream_type&)) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>]
       operator<<(__ostream_type& (*__pf)(__ostream_type&))
.../...
```
<!-- #endregion -->

Que nous dit-il ? et bien: *no match for ‘operator<<’ (operand types are ‘std::ostream’ and ‘X’)* et des tas de trucs bizarres après...

En fait il n'a pas trouvé la fonction qui définit l'operateur $<<$ sur les objets de votre type *X*. Donc comment c++ reconnaît-il la fonction qu'il va utiliser pour afficher des objets de votre type X ? Il va chercher la fonction qui redéfinit *operator$<<$* en prenant comme première opérande un stream de sortie (*std::ostream*) et comme second argument un objet de type *X*. Super on y va !

<!-- #region -->
```c++
// dans le fichier main.cpp
#include <iostream>

class X {};

std::ostream& operator<< (std::ostream& os, const X& x) {
  os << "Je suis un objet de type X !";
  return os;
}

int main (int argc, char* argv []) {
  X x;
  std::cout << x << std::endl; 
}
```
```
$ g++ main.cpp -o main
$ ./main
Je suis un objet de type X !
```
<!-- #endregion -->

Alors là vous remarquez plusieurs choses, déjà nous avons passé notre objet x de type X par une référence sur un objet X constant. Toujours les mêmes raisons: avec *const* vous évitez de modifier par inadvertance, dans la fonction qui l'affiche, l'objet passé en argument (et croyez nous ca n'arrive pas qu'aux débutants) et avec le passage par référence (&) ? et bien vous éviter de recopier l'objet alors que vous vouliez simplement l'afficher. Que ceux qui pensent que x est tout petit et que ca ne pose pas de problèmes, rajoutent dans X une matrice de long long double de taille $1.000.000 \times 1.000.000$, affichent un objet de type X, et on en reparle...


Et puis vous remarquez que la fonction retourne le flux dans lequel elle écrit. Quelle en est la raison ? Et bien c'est pour chaîner les affichages. En effet faire *os << "hello" << 12* revient, si on le décompose, à appeler *operator<<(operator<<(os, "hello"), 12)*. Nous vous le montrons:

<!-- #region -->
```c++
// in file main.cpp
#include <iostream>
int main () {
  std::cout << "hello" << " world\n";
  std::operator<<( std::operator<<(std::cout, "hello"), " world\n");
}
```
```
$ g++ main.cpp -o main
$ ./main
hello world
hello world
```
<!-- #endregion -->

Oui ces deux lignes font la même chose ! vous préférez la seconde ? nous aussi.

Encore quelque chose avant de passer aux templates. Vous remarquez que l'operator$<<$ que nous avons défini sur les X est une fonction globale: ce n'est pas une fonction membre de la classe X. Il est habituel que ces fonctions d'affichage désirent utiliser des données privées (private) à la classe, que se passe-t-il ?

<!-- #region -->
```c++
// in file main.cpp
#include <iostream>

class X {
  int element;
public:
  X (int e) : element(e) {}
};

std::ostream& operator<< (std::ostream& os, const X& x) {
  os << "Je suis un objet de type X et de valeur " << x.element;
  return os;
}
int main (int argc, char* argv []) {
  X x (12);
  std::cout << x << std::endl; 
}
```
```
$ g++ main.cpp
main.cpp: In function ‘std::ostream& operator<<(std::ostream&, const X&)’:
main.cpp:10:54: error: ‘int X::element’ is private within this context
   os << "Je suis un objet de type X et de valeur " << x.element;
                                                         ^~~~~~~
main.cpp:4:7: note: declared private here
   int element;
       ^~~~~~~
Compilation exited abnormally with code 1 at Thu Apr 23 22:01:28
```
<!-- #endregion -->

c++ refuse très clairement (il est vrai que les messages d'erreur sont très parlants en c++ - sauf le ';' oublié à la fin de la déclaration d'une classe). Donc c++ refuse qu'une fonction globale accède au champ privé d'une classe qui ne veut pas. Il n'est pas envisageable de mettre ces membres en public pour arranger la fonction d'affichage. Il n'est pas du tout dans le style de c++ de déclarer des fonctions pour accéder aux champs privés (à-la get/set) sauf si c'est une exigence dans le design de cette classe. Mais alors comment allons nous faire ? oui bien sûr la classe va déclarer cette fonction friend. Donc comme toutes les fonctions friend choisies par une classe, elle pourra accéder aux champs privés (private) et protégés (protected) de la classe. 

<!-- #region -->
```c++
#include <iostream>

class X {
  friend std::ostream& operator<< (std::ostream&, const X&);
  int element;
public:
  X (int e) : element(e) {}
};

std::ostream& operator<< (std::ostream& os, const X& x) {
  os << "Je suis un objet de type X de valeur " << x.element;
  return os;
}
int main (int argc, char* argv []) {
  X x (12);
  std::cout << x << std::endl; 
}
```
```
$ g++ main.cpp -o main
$ ./main
Je suis un objet de type X de valeur 12
```
<!-- #endregion -->

Passons maintenant à la même chose pour des classes templates. Dans le premier cas, la fonction d'affichage n'accède pas à un membre privé et n'a pas besoin de déclaration friend

<!-- #region -->
```c++
// in file main.cpp
#include <iostream>
template <typename T>
class X {
  T element;
public:
  X (T e) : element(e) {}
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const X<T>& x) {
  os << "Je suis un objet de type X";
  return os;
}
int main (int argc, char* argv []) {
  X<int> x (12);
  std::cout << x << std::endl; 
}
```
```
$ g++ main.cpp -o main
$ ./main
hello world
hello world
```
<!-- #endregion -->

On remarque que la fonction est template sur le type T dont a besoin la classe X.


Quand *element* est privé à la classe X, on doit déclarer la redéfinition de operator$<<$ en friend. Alors là nous vous l'accordons, c'est un peu compliqué syntaxiquement. Déjà la fonction operator$<<$ est une fonction template dans une classe template. Et en plus il faut éviter de réutiliser T, ca tombe bien y'a d'autres lettres disponibles.

<!-- #region -->
```c++

#include <iostream>

template <typename T>
class X {
  template <typename U> friend std::ostream& operator<< (std::ostream&, const X<U>&);
  T element;
public:
  X (T e) : element(e) {}
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const X<T>& x) {
  os << "Je suis un objet de type X de valeur " << x.element;
  return os;
}
int main (int argc, char* argv []) {
  X<int> x (12);
  std::cout << x << std::endl; 
}
```
<!-- #endregion -->

C'est maintenant à vous de jouer, remplacer la fonction print de votre classe template Stack par la surcharge de l'operator$<<$ en la mettant friend.


Sautons maintenant du coq à l'âne (traduction en anglais: *jump from one subject to another*) et parlons de quelque chose de complètement différent.


Il n'y a pas que les types qu'une classe peut prendre en argument, il y a aussi des arguments classique, comme des int: 

<!-- #region -->
```c++
template <typename T, int SIZE>
class X {
  T tab [SIZE];
public:
};

int main (int argc, char* argv []) {
  X<int, 10> x;
}
```
<!-- #endregion -->

Alors là nous voyons votre oeil s'éclairer ... oui tous les objets de type X sont composé d'un tableau de SIZE objets de type T... ca ne vous donne pas une idée pour votre classe template ? Exactement vos piles peuvent être template non seulement sur le type des objets mais aussi sur la taille de la pile ! mais alors plus besoin de new ? non ! ni de delete ? non plus ! mais qu'en est-t-il du constructeur de copie et de l'affectation ? ... on ne peut pas gagner sur tous les tableaux c'est le cas de dire, ces deux fonctions ne s'appliqueront qu'aux piles ayant exactement le même T et le même SIZE.


Reprennez votre pile template et mettez la taille en template. Vous n'avez plus besoin de garder la taille de la pile: vous l'avez dans le template...


Et maintenant encore une petite chose, il peut arriver que certains types ne soient pas compatibles pour les templates que vous écrivez. Ça a été le cas par exemple pour la fonction my_add sur le type des c-style string i.e. *const char\** sur lesquels le + n'existait pas. C'est une erreur de compilation, mais tant que la fonction n'est pas utilisée sur ce type et bien on ne s'en rend pas compte ... donc  les templates c'est un peu délicat à débugger. Et si, quand vous testez une classe template sur un type donné, il faut définir toutes les fonctions pour s'assurer qu'elles soient bien ok (syntaxiquement) pour ce type ...


Voici une petite version rapide de votre pile .

<!-- #region -->
```c++
#include <iostream>

template <typename T, int SIZE>
class SmallStack {
  T tab [SIZE];
  int top=0;
public:
  void push (T e) {
    tab[top++] = e;
  }
  T pop () {
    return tab[--top];
  }
  void print () const {
    for (int i=0 ; i<top; ++i)
      std::cout << tab[i] << ' ';
    std::cout << std::endl;
  }
};

int main (int argc, char* argv []) {
  SmallStack<int, 10> st;
  st.push(1);
  st.push(2);
  st.push(3);
  st.push(4);
  st.print();
  std::cout << "pop " << st.pop() << std::endl;
  st.print();
  std::cout << "pop " << st.pop() << std::endl;
  st.print();
  std::cout << "pop " << st.pop() << std::endl;
  st.print();
}
```
```
$ g++ main.cpp -o main
$ ./main
1 2 3 4 
pop 4
1 2 3 
pop 3
1 2 
pop 2
1 
```
<!-- #endregion -->

Maintenant je vous donne le code suivant, je vais instancier ma pile template sur un type X. Voyez vous les problèmes ? 

<!-- #region -->
```c++
// in file main.cpp
#include <iostream>

// même code de SmallStack que précédement
template <typename T, int SIZE>
class SmallStack {
  T tab [SIZE];
  int top=0;
public:
  void push (T e) {
    tab[top++] = e;
  }
  T pop () {
    return tab[--top];
  }
  void print () const {
    for (int i=0 ; i<top; ++i)
      std::cout << tab[i] << ' ';
    std::cout << std::endl;
  }
};

// une nouvelle classe
class X {
  int element;
public:
  X (int e) : element(e) {}
};


// une fonction qui ajoute un élément à une pile de 10 objets de type X
void foo (SmallStack<X, 10>* st) {
  X x(12);
  st->push(x);
}
// et là rien de spécial
int main () {
    return 0;
}
```
```
$ g++ main.cpp -o main
$ ./main
```
<!-- #endregion -->

Je ne fais rien dans ce code donc c++ ne génèrera aucune fonction templates, mais pourtant j'aimerai bien savoir si l'instanciation de toutes les fonctions membres vont fonctionner pour ma classe template ... et bien je peux le faire, c++ me propose un mécanisme pour forcer l'instanciation, je n'ai qu'à dire:

<!-- #region -->
```c++
template class SmallStack<X, 10>;
```
<!-- #endregion -->

Si vous rajoutez cette ligne après la définition de la classe X et que vous compiliez votre programme vous allez obtenir une erreur:
```
g++ main.cpp
main.cpp: In instantiation of ‘void SmallStack<T, SIZE>::print() const [with T = X; int SIZE = 10]’:
main.cpp:27:16:   required from here
main.cpp:16:17: error: no match for ‘operator<<’ (operand types are ‘std::ostream {aka std::basic_ostream<char>}’ and ‘const X’)
       std::cout << tab[i] << ' ';
       ~~~~~~~~~~^~~~~~~
In file included from /usr/include/c++/7/iostream:39:0,
                 from c0.cpp:1:
/usr/include/c++/7/ostream:108:7: note: candidate: std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(std::basic_ostream<_CharT, _Traits>::__ostream_type& (*)(std::basic_ostream<_CharT, _Traits>::__ostream_type&)) [with _CharT = char; _Traits = std::char_traits<char>; std::basic_ostream<_CharT, _Traits>::__ostream_type = std::basic_ostream<char>]
.../...
```


Ah mais oui, le type X n'a pas redéfini l'operator$<<$  qui est appelé dans la fonction print de la classe SmallStack ! est-ce tout ? non regardez la fonction foo, il ne vous paraît pas bizarre son argument ? surtout sachant que les objets SmallStack contiennent un tableau à-la-c de SIZE objets de type T ... réflechissez et demandez nous si vous ne voyez pas.


END
