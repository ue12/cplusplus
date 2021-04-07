---
jupyter:
  jupytext:
    notebook_metadata_filter: rise
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
      jupytext_version: 1.6.0
  kernelspec:
    display_name: Python 3
    language: python
    name: python3
  rise:
    autolaunch: true
    scroll: true
---

<!-- #region slideshow={"slide_type": "slide"} -->
# Utilisation du C++ dans Python 
<!-- #endregion -->

```python
%%HTML
<link rel="stylesheet" type="text/css" href="rise.css">
```

<!-- #region slideshow={"slide_type": "subslide"} -->
Vous êtes maintenant des spécialistes du C++ et êtes capables de résoudre tous les problèmes qui se présenterent à vous à coup de `std::vector`, `double *`, `class` et `g++`. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Cependant dans un certain nombre de cas se lancer bille en tête dans la réalisation d'un programme complet en c++ peut s'apparenter à prendre le marteau pour écraser une mouche. Vous pouvez donc partir sur une solution Python facile à mettre en place. Mais alors comment faire quand le programme que l'on fait en Python a besoin d'un petit boost en terme de performance ? 
<!-- #endregion -->

La solution est simple !! Il suffit d'appeler un bout de C++ là où on a besoin que ca aille vite. 

**Remarque :** il ne suffit pas d'appeler du C++ pour que ça accélère les choses. Car pour rappel un mauvais code C++ peut aller moins vite qu'un bon code Python ! 


Mais donc oui c'est possible de rendre accessible à Python du code C++. Il existe même plusieurs manières de faire ça : 

* Utilisation directe de CPython 
* Utilisation de Swig 
* Utilisation Pybind11 
* Utilisation de Cython 


Dans le cadre de ce cours nous n'allons nous intéresser qu'à la solution faisant appel à Cython. 

<!-- #region slideshow={"slide_type": "slide"} -->
## Cython
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Cython pour C-Extension for Python est un langage de programmation et un compilateur destiné à l'écriture de module Python optimisés. La syntaxe de Cython est très similaire à celle de Python avec en plus un support d'éléments venant du C/C++. 

Les objectifs de Cython sont : 

* Permettre la génération à partir de code Python d'un module écrit en CPython optimisé 
* Écrire des interfaces entre Python et C++
<!-- #endregion -->

<!-- #region -->
Pour la suite du cours il vous faut donc installer Cython sur vos machines si vous ne l'avez pas déjà. 

Pour vérifier si vous avez ou non Cython d'installer dans un terminal tapez la commande suivante : 

```bash 
$ which cython 
```
Si la réponse est de la forme : 

```
/usr/bin/which: no cython in ... 
```

Pas de chance vous n'avez pas Cython. Pour l'installer il suffit alors de faire 

```bash 
$ pip install cython 
```





<!-- #endregion -->

```python

```
