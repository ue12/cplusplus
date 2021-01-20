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
