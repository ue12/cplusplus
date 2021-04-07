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

## Utilisation de Cython pour l'accélération de code Python 


Dans ce premier notebook nous allons voir l'usage de base de Cython qui est comment passer un bout de code Python un peu lent en du code compilé plus rapide. 


Pour cela considérons l'exemple, maintenant traditionnel du calcul de $\pi$. Pour ceux qui ne se souviendraient plus de comment on calcul $\pi$ voici ci dessous la formule magique. 

$$ \pi = \int_{0}^{1} \frac{4}{1+x^2} $$






Le calcul de $\pi$ peut donc se faire en Python de la manière suivante : 

```python
def compute_pi( nbpoint ):
    s = 0
    l = 1./nbpoint
    for i in range(nbpoint):
        x = l * ( i + 0.5 )
        s += l * ( 4. / (1. + x**2 ) )
    return s
```

```python
import time
def performance(f, args, repeat=10):
    start = time.perf_counter()
    for _ in range(repeat):
        f(*args)
    stop = time.perf_counter()
    print(f"{f.__module__}::{f.__name__} mean time ({repeat} run): {(stop-start)/repeat} s")
```

```python
repeat = 10
performance(compute_pi, (1000000,) , repeat)
```

La question qui se pose alors est comment accélérer significativement ce bout de code ? Déjà pour accélérer un code il faut savoir quel est la partie qui coince et qui dégrade les performances. Dans le cas présent c'est facile vu que le code fait 7 lignes... 


Le facteur limitant ici c'est tout simplement la boucle `for`. Comme on l'entend souvent, Python pour des boucles `for` avec beaucoup d'itérations c'est pas ce qu'il y a de mieux.


### Première solution --- on se fatigue pas trop ! 


La première solution que nous allons mettre en place pour accélérer le calcul de $\pi$, repose sur l'usage bête et méchant de Cython en tant que traducteur d'un code Python vers du C/C++ utilisable dans Python.  


Pour cela il suffit d'écrire la fonction `compute_pi` à l'identique dans un fichier ayant pour extension `.pyx`. 

```python
!cat code/cython/pi_computer/src/pimodule1.pyx 
```

Pour le moment rien d'extraordinaire. La subtilité se situe dans le fait qu'il faut maintenant utiliser la machinerie Cython pour transformer le fichier `pyx` en une librairie dynamique pouvant être utilisée par Python comme un module. 

<!-- #region -->
Mais comment compiler avec Cython ? Pour cela il faut écrire un fichier setup.py contenant le code suivant :  

```python 
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",]}

extensions = [
    Extension("pimodule1", sources=["src/pimodule1.pyx"], **ext_options),
]

setup(
    ext_modules=cythonize(extensions)
)
```
<!-- #endregion -->

<!-- #region -->
Une fois ce fichier écrit la compilation se fait simplement en tapant la commande : 

```bash 
$ python setup.py build_ext --inplace 
```
<!-- #endregion -->

```python
# On ajoute dans le PATH le dossier contenant la lib générée
import sys 
import pathlib as pl 
sys.path.append(str(pl.Path(".").absolute() / "code" / "cython" / "pi_computer") )

```

```python
import pimodule1
```

```python
performance(compute_pi, (1000000,) , repeat)
performance(pimodule1.compute_pi, (1000000,) , repeat)
```

On observe donc que l'on a un gain non négligeable avec un minimum d'effort ! 


Mais du coup est-ce qu'on ne peut pas faire mieux ? La réponse est évidemment oui on peut faire encore mieux. 


### Seconde solution --- on fait un petit effort 


Pour faire mieux c'est très simple en fait. Il faut faire une partie du travail pour Cython. Rappelons le une différence notable entre le Python et le C++ est l'aspect fortement typé du langage C++ par rapport à Python qui n'a pas de typage. Du coup quand on donne un bout de code Python à Cython ce dernier est obligé de deviné le type des variables qu'il manipule puisqu'en Python on ne spécifie pas ces types. 

Deux solutions serait alors envisageables pour Cython : 

* Il essaye de deviner ce que vous avez en tête, ce qui permettrait de faire du code vraiment optimisé mais risquerai aussi à faire du code plein de bugs. 
* Il utilise des types très génériques et donc pas du tout performant en terme d'allocation mémoire. 


Du coup la solution la plus évidente, qui au passage est également la plus optimale, est de dire à cython le type des variables. Cela se fait en utilisant le mot clé `cdef` et la syntaxe est la suivante : 

```
cdef type_c nom_variable 
```


Par exemple si on reprend le code de la fonction calculant pi on peut faire quelque chose du genre : 

```python
!cat code/cython/pi_computer/src/pimodule2.pyx
```

On voit donc que la modification faites est minime cependant cela va tout changer....

```python
!cd code/cython/pi_computer ; python setup.py build_ext --inplace
```

```python
import pimodule2 
```

```python
performance(compute_pi, (1000000,) , repeat)
performance(pimodule1.compute_pi, (1000000,) , repeat)
performance(pimodule2.compute_pi, (1000000,) , repeat)
```

![](https://media.giphy.com/media/sjDV6YTbw8tig/giphy.gif)


Comme quoi faire un petit effort ça vaut vraiment le coup ici !!! 


Si on veut vraiment comprendre ce qui se passe, pas le choix il faut aller comparer les fichiers `.cpp` générés par Cython (attention ces fichiers font très très peur ... ) ! 

<!-- #region -->
Mais par exemple, si on regarde ce qui se passe dans la boucle, le code généré à partir du code Cython naïf est le suivant : 

```c++
__pyx_t_1 = __Pyx_PyFloat_AddObjC(__pyx_v_i, __pyx_float_0_5, 0.5, 0, 0); if (unlikely(!__pyx_t_1)) __PYX_ERR(0, 6, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_1);
    __pyx_t_5 = PyNumber_Multiply(__pyx_v_l, __pyx_t_1); if (unlikely(!__pyx_t_5)) __PYX_ERR(0, 6, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    __Pyx_XDECREF_SET(__pyx_v_x, __pyx_t_5);
    __pyx_t_5 = 0;
    __pyx_t_5 = PyNumber_Power(__pyx_v_x, __pyx_int_2, Py_None); if (unlikely(!__pyx_t_5)) __PYX_ERR(0, 7, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_5);
    __pyx_t_1 = __Pyx_PyFloat_AddCObj(__pyx_float_1_, __pyx_t_5, 1., 0, 0); if (unlikely(!__pyx_t_1)) __PYX_ERR(0, 7, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_1);
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    __pyx_t_5 = __Pyx_PyFloat_DivideCObj(__pyx_float_4_, __pyx_t_1, 4., 0, 1); if (unlikely(!__pyx_t_5)) __PYX_ERR(0, 7, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    __pyx_t_1 = PyNumber_Multiply(__pyx_v_l, __pyx_t_5); if (unlikely(!__pyx_t_1)) __PYX_ERR(0, 7, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_1);
    __Pyx_DECREF(__pyx_t_5); __pyx_t_5 = 0;
    __pyx_t_5 = PyNumber_InPlaceAdd(__pyx_v_s, __pyx_t_1); if (unlikely(!__pyx_t_5)) __PYX_ERR(0, 7, __pyx_L1_error)
    __Pyx_GOTREF(__pyx_t_5);
    __Pyx_DECREF(__pyx_t_1); __pyx_t_1 = 0;
    __Pyx_DECREF_SET(__pyx_v_s, __pyx_t_5);
    __pyx_t_5 = 0;

```
<!-- #endregion -->

<!-- #region -->
Tandis que le code généré par Cython à partir de notre fichier `.pyx` lorsque l'on fait un petit effort est le suivant : 

```c++
__pyx_v_i = __pyx_t_5;
__pyx_v_x = (__pyx_v_l * (__pyx_v_i + 0.5));
__pyx_t_2 = (1. + pow(__pyx_v_x, 2.0));
if (unlikely(__pyx_t_2 == 0)) {
   PyErr_SetString(PyExc_ZeroDivisionError, "float division");
   __PYX_ERR(0, 9, __pyx_L1_error)
}
__pyx_v_s = (__pyx_v_s + (__pyx_v_l * (4. / __pyx_t_2)));

```
<!-- #endregion -->

On peut alors observer plein de choses : 

* Le second code est beaucoup plus court. Trivial oui mais qui dit code plus court dit code qui fait moins de choses donc qui va plus vite 
* Dans la version naive, on remarque plein de `Pyx_GOTREF`, `Pyx_DECREF` cela signifie que le code utilise des Py_Object partout et donc passe son temps à allouer/désallouer de la mémoire. 


### Troisième solution --- on fait du C++ directement 


Il existe une troisième solution pour optimiser notre calcul de $\pi$. Cette solution utilise la capacité de Cython d'utiliser du code c++ directement et le rendre accessible à l'univers Python. 


Pour cela commençons par écrire la fonction `compute_pi` en c++. Une solution possible est la suivante : 

```python
!echo "----- pi_core.h ------ "
!cat code/cython/pi_computer/src/pi_core.h
!echo "----- pi_core.cpp ------ "
!cat code/cython/pi_computer/src/pi_core.cpp
```

Bon la question qui se pose alors est comment faire communiquer notre fonction c++ et Python. Pour cela il nous faut écrire une couche d'interface utilisant un fichier `.pyx`. 

```python
!cat code/cython/pi_computer/src/pimodule3.pyx
```

Pas très compliqué vous en conviendrez !! 

On voit apparaitre la syntaxe `extern from` qui permet de faire exister dans le monde Cython des fonctions du monde C++. Il nous suffit alors ensuite d'encapsuler notre fonction `compute_pi_c` dans la fonction Cython `compute_pi`. Dans cet exemple l'encapsulation est triviale car ne nécessite aucun travail sur les arguments. Nous verrons dans les notebook suivant que ce n'est pas toujours le cas. 

<!-- #region -->
Ensuite il suffit de compiler le module. Tout se passe encore une fois dans le fichier `setup.py`. 

```python 
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",]}

extensions = [
    Extension("pimodule3", sources=["src/pimodule3.pyx", "src/pi_core.cpp"], **ext_options)
]

setup(
    ext_modules=cythonize(extensions)
)
```
<!-- #endregion -->

```python
import pimodule3
```

```python
performance(compute_pi, (1000000,) , repeat)
performance(pimodule1.compute_pi, (1000000,) , repeat)
performance(pimodule2.compute_pi, (1000000,) , repeat)
performance(pimodule3.compute_pi, (1000000,) , repeat)
```

Nous venons donc de voir qu'avec un effort modéré nous pouvons significativement augmenté les performances d'un code Python soit en utilisant Cython comme traducteur Python -> C++ soit en utilisant Cython pour faire la couche d'interface entre une fonctionnalité C++ et le monde Python. 

```python

```
