---
jupyter:
  jupytext:
    formats: ipynb,md
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
      jupytext_version: 1.6.0
  kernelspec:
    display_name: 'Python 3.7.4 64-bit (''notebook'': conda)'
    language: python
    name: python37464bitnotebookcondaf2a876b6f16b4058adafaa8bc5b33803
---

## Cython et les classes


Maintenant que nous avons vu à quel point Cython peut nous permettre d'enrichir nos codes Python nous allons nous pencher sur la problématique de la POO. En effet la tendance quand on fait du code C++ c'est de faire des classes dans tous les sens. 


La question qui se pose alors est comment peut-on faire pour utiliser ces classes faites en C++ dans Python. Nous avons bien réussi à utiliser dans Python la fonction `compute_pi_c` nous devrions bien pouvoir faire la même chose avec des classes !! 


La réponse est oui évidemment on peut faire ça !! Et c'est ce que nous allons voir de suite ! 


### La classe Stack 


Pour repartir sur des bases connues je vous propose de reprendre le code de la classe `IntStack` faite et refaite au premier semestre. 

```python
!echo "--------- Stack.h ---------"
!cat code/cython/cpp_stack/src/Stack.h 
!echo "--------- Stack.cpp ---------"
!cat code/cython/cpp_stack/src/Stack.cpp 
```

<!-- #region -->
Et du coup ce que l'on veut faire c'est avoir la possibilité dans Python de faire : 

```python 
from stackcpp import CppIntStack 

s = CppIntStack(10)
s.push(1)
s.push(2)
s.push(3) 
print( s.pop())
```
<!-- #endregion -->

Pour faire ça c'est très simple il nous suffit de faire la couche d'interface qui va bien en Cython, un peu comme pour `compute_pi_c` du notebook précédent. 


Pour cela on commence par faire la couche d'interface qui permet à Cython de savoir qu'il existe une classe IntStack et qu'elle dispose d'un certain nombre de méthodes. C'est en quelque sorte une traduction pour Cython du fichier d'en-tête C++. 

```
from libcpp cimport bool 
from libcpp.vector cimport vector 

cdef extern from "Stack.h": 
    cdef cppclass IntStack:
        IntStack(const int&)
        int pop()
        void push(const int&)
        bool isEmpty()
        vector[int] getStorage()
```


**Important** : il n'est pas nécessaire dans le fichier Cython de remettre l'intégralité des méthodes définies dans le fichier `.hpp`. Il faut uniquement celle que l'on souhaite utiliser dans le Cython donc le Python. Typiquement une bonne dichotomie est de se dire que dans le Cython on fait l'interface avec les fonctions `public` uniquement. 


Ensuite vient la partie encapsulation de la classe C++ dans une classe Python. Cela se fait de la manière suivante : 

```
cdef class CppIntStack:
    cdef IntStack* _stack

    def __cinit__(self, int sz):
        self._stack = new IntStack(sz)

    def __dealloc__(self):
        del self._stack

    def push(self, int x):
        self._stack.push(x)

    def pop(self):
        return self._stack.pop()

    def isEmpty(self):
        return self._stack.isEmpty()

    def getStorage(self):
        return self._stack.getStorage()
```


La définition de cette classe d'encapsulation ressemble beaucoup à une classe Python classique, si ce n'est les quelques subtilités suivantes : 

* On ajoute un attribut de classe `_stack` de type `IntStack*`, il va s'agir du pointeur vers notre objet encapsulé. 
* On a une méthode `__cinit__`, à ne pas confondre avec `__init__`. Qui est en quelque sorte notre constructeur de classe. C'est dans le `__cinit__` que l'on vient créer l'instance de `IntStack`
* On a une méthode `__dealloc__`, qui joue le rôle de destructeur de la classe. C'est içi que l'on doit impérativement détruire l'instance de `IntStack` associée à notre classe. Sinon cela mêne à une fuite mémoire. 




<!-- #region -->
Ensuite la compilation de l'ensemble se fait simplement en utilisant un fichier setup.py de la forme suivante : 

```python 
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",]}

extensions = [
    Extension("stackcpp", sources=["src/stackcpp.pyx", "src/Stack.cpp"], **ext_options),
]

setup(
    ext_modules=cythonize(extensions)
)
```

<!-- #endregion -->

```python
!cd code/cython/cpp_stack ; python setup.py build_ext --inplace 
```

```python
import sys
import pathlib as pl 
sys.path.append(str(pl.Path(".").absolute() / "code" / "cython" / "cpp_stack") )
```

```python
import stackcpp 
```

```python
s = stackcpp.CppIntStack(10)
s.push(1)
s.push(2)
s.push(3)
print(s.pop())
```

```python

```

```python

```

### Une classe Stack générique 


Nous avons vu au premier semestre que faire juste une classe `IntStack` c'est un peu limité. Et que pour faire un truc plus sympa le C++ offre un mécanisme magique, les **templates**. Par exemple, une implémentation de la classe `Stack` générique est la suivante : 

```python
!cat code/cython/cpp_stack/src/StackGeneric.hpp
```

La subtilité qui apparait alors pour faire l'interface Cython entre cette classe template et Python est comment gérer l'aspect template. Pour cela malheureusement Cython n'offre pas forcément de solution miracle... 


La première étape est de définir l'inteface de la manière suivante : 

```
from libcpp cimport bool 
from libcpp.vector cimport vector 

cdef extern from "StackGeneric.hpp": 
    cdef cppclass StackGeneric[T]:
        StackGeneric(const int&)
        T pop()
        void push(const T&)
        bool isEmpty()
        vector[T] getStorage()
```


On voit alors apparaitre la syntaxe utilisant les `[T]` en place des `<T>` pour les templates. De même on voit l'utilisation du mot clé `cppclass` qui permet de définir en Cython des classes c++. 


Une fois cette couche d'interface réalisée nous pouvons utiliser dans Cython notre classe C++. Pour la rendre accessible à Python il nous faut maintenant faire l'encapsulation de cette dernière dans une classe Python. 


Et ces à ce niveaux que la dure réalité de la vie nous rattrape... Python n'étant pas typé, il n'y a pas de concept équivalent à celui de template ... Du coup il va nous falloir faire un wrapper par type souhaité pour `T` ...


Par exemple pour `T=int` on obtient quelque chose du genre : 

```
cdef class CppStackGenericInt:
    cdef StackGeneric[int]* _stack

    def __cinit__(self, int sz):
        self._stack = new StackGeneric[int](sz)

    def __dealloc__(self):
        del self._stack

    def push(self, int x):
        self._stack.push(x)

    def pop(self):
        return self._stack.pop()

    def isEmpty(self):
        return self._stack.isEmpty()

    def getStorage(self):
        return self._stack.getStorage()
```


Tandis que pour `T=double` on obtient : 

```
cdef class CppStackGenericDouble:
    cdef StackGeneric[double]* _stack

    def __cinit__(self, int sz):
        self._stack = new StackGeneric[double](sz)

    def __dealloc__(self):
        del self._stack

    def push(self, double x):
        self._stack.push(x)

    def pop(self):
        return self._stack.pop()

    def isEmpty(self):
        return self._stack.isEmpty()

    def getStorage(self):
        return self._stack.getStorage()
```


Je vous l'accorde on a perdu en élégance !! Mais c'est le prix à payer pour pouvoir utiliser dans Python langage sans typage, des fonctionnalités fortement liée au concept de typage.... Mais en tout cas ça permet de faire le job est d'offrir à Python les fonctionnalité C++. 


**Limitation :** avec cet aspect template on voit apparaitre une limitation de Cython qui est qu'il ne supporte pas les template variadic. Cela peut s'avérer quelque peu génant d'autant plus que les templates variadic sont de plus en plus utilisé dans le C++ moderne. 


### Une architecture un peu plus complexe 


Nous venons donc de voir comment via Cython rendre  une classe C++ utilisable dans le monde Python !! 

C'est cool mais il faut bien se rendre compte que dans tous code C++ qui se respecte on a une zoologie de classe, qui se partagent des pointeurs communs. 




Bien évidemment Cython permet de faire ça. Nous allons en effet pouvoir instancier des classes dans le monde Python jouer sur qui possède quel pointeur dans le monde Python. 


Mais il va falloir se remonter les manches un peu !! 


**Voir notebook/code/cython/multi_class**

```python

```
