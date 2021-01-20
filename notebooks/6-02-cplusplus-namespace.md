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

# Namespace


## Des namespace dans quel but ? 

<!-- #region -->
Le c++ a introduit par rapport au c historique, beaucoup de chose me direz vous certe mais dans le lot il y a les namespace, ou espace de nom en bon français. Le principe est de permettre un cloisonnement des fonctionnalité pour : (i) une meilleure organisation du code ; (ii) prévenir les collisions de noms. 

C'est ce que fait une classe déjà me direz vous !! Oui mais non en fait. Tout d'abord une classe a besoin d'être instanciée alors qu'un espace de nom non. De plus une classe est fermée dans le sens où elle est défini dans un fichier et si dans un autre fichier on veut ajouter une nouvelle méthode on ne peut pas. Alors qu'un *namespace* lui peut-être enrichie de toute part. 

Pour une analogie qui vous parlera forcément vous pouvez considérer les modules Python. Lorsque que l'on veut utilider des DataFrame de `pandas` par exemple la premier étape est d'importer le module `pandas`

```python
import pandas 
```

`pandas` devient alors l'équivalent de notre namespace. Et donc pour utiliser un élément du module `pandas` on doit précéder cet élément du `namespace`. 

```python
df = pandas.DataFrame()
```

Et bien les *namespace* C++ vont jouer exactement le même rôle.
<!-- #endregion -->

## Définir et utiliser un namespace 


<!-- #region -->
La définition d'un *namespace* se fait à l'aide du mot clé `namespace`, jusque là facile. Par exemple pour définir une fonction dans un *namespace* la syntaxe est la suivante : 

```c++

namespace mylib{ // Debut du namespace
void info(const std::string& msg){
    std::cout << msg << std::endl;
}

void warning(const std::string& msg){
    std::cout << "[WARNING] " << msg << std::endl;
}
 
} // end namespace mylib 
```

On a ainsi définit deux fonctions dans le **namespace mylib**. Pour utiliser ces deux fonctions dans un programme il faudra alors spécifier le namespace. Par exemple : 

```c++
#include "myheader.h"

int main(){

    mylib::info("Un message bidon");
    mylib::warning("Un warning bidon lui aussi");

    return 0;
}
```

<!-- #endregion -->

<!-- #region -->
Et là vous vous dites que ça ressemble vachement à tous les `std::` qu'on se trimbale depuis le début du cours C++ en fait !!!! Et bien oui en effet et c'est normal car `std` n'est qu'un namespace, celui de la librairie standard c++. 

Et donc certains d'entre vous l'auront tout de suite deviné, ou pas. Cela veut donc dire que vous pouvez faire des choses du genre : 

```c++
#include "myheader.h"

using namespace mylib;

int main(){

    info("Un message bidon");
    warning("Un warning toujours bidon");

    return 0;
}
```
<!-- #endregion -->

Oui en effet vous pouvez le faire, mais attention... Au même titre que faire `from numpy import *` en Python est répréhensible par la loi, faire `using namespace toto` en c++ est passible de la peine capitale. Bon peut-être pas en vrai, je vais juste vous dire que c'est mal et vous faire une longue tirade sur les besoins d'élégance et de style en programmation. 

<!-- #region -->
L'avantage des *namespace* comme nous l'avons dit précédemment est que l'on peut les définir en plusieurs étapes. Par exemple on peut commencer un namespace dans un premier fichier

```c++
// Fichier message.h
#include <iostream>

namespace mylib {
    void info(const std::string& msg){
        std::cout << msg << std::endl;
    }

    void warning(const std::string& msg){
        std::cout << "[WARNING] " << msg << std::endl;
    }

}//end namespace
```

Et continuer à enrichir notre fichier dans un second namespace par exemple 

```c++
// Fichier calcul.h

namespace mylib {

    double sum(const std::vector<double>& v){
        double s = 0.;
        for( auto x: v){
            s += x;
        }
        return s;
    }

    double mean(const std::vector<double>& v){
        return sum(v) / v.size();
    }

} // end namespace
```

Et on peut ensuite inclure nos deux fichiers dans notre fichier `main.cpp` et alors les fonctions seront toutes dans le namespace `mylib`. 

```c++
#include <vector>

#include "message.h"
#include "calcul.h"

int main(){
    std::vector<double> v {1,2,3,4};
    double s = mylib::sum(v);
    double m = mylib::mean(v);

    mylib::info("Tout marche");

    return 0;
}
```

<!-- #endregion -->

## Namespace trucs et astuces



### Namespace imbriqués


<!-- #region -->
En Python on trouve régulièrement des choses du genre : 

```python
import scipy.integrate as sci
```

C'est à dire l'importation d'un sous-module, donc un module dans un module. Et bien en C++ c'est pareil on peut définir des namespace dans des namespace. Quel intérêt me direz vous ? Et bien c'est surtout pour cloisonner le code et ranger les choses. Par exemple si l'on reprend l'exemple précédent des deux fonctions "calcul" et des deux fonctions "affichage" dans le namespace `mylib`. On pourrait très bien répartir ces fonctions dans deux sous-namespace afin de mieux organiser le code. Cela donnerait alors :
<!-- #endregion -->

<!-- #region -->
```c++
// Fichier message.h

namespace mylib{
    namespace message{

        void info(const std::string& msg){
            std::cout << msg << std::endl;
        }

        void warning(const std::string& msg){
            std::cout << "[WARNING] " << msg << std::endl;
        }
    } // end namespace message 
} // end namespace mylib 
```

et pour le fichier calcul 

```c++
// Fichier calcul.h 
namespace mylib {

    namespace calcul{
        double sum(const std::vector<double>& v){
            double s = 0.;
            for( auto x: v){
                s += x;
            }   
            return s;
        }

        double mean(const std::vector<double>& v){
            return sum(v) / v.size();
        }
    } // end namespace calcul 
} // end namespace mylib 
```

A l'usage cela donnerai alors 

```c++
// Fichier main.cpp
#include <vector>

#include "message.h"
#include "calcul.h"

int main(){
    std::vector<double> v {1,2,3,4};
    double s = mylib::calcul::sum(v);
    double m = mylib::calcul::mean(v);

    mylib::message::info("Tout marche");

    return 0;
}
```
<!-- #endregion -->

### Renommer un namespase à l'usage 


<!-- #region -->
Parfois il arrive que les namespace soient à rallonge et que l'on ait un peu la flemme de mettre le nom complet. Encore une fois l'analogie parfaite c'est Python. Nous avons tous l'habitude, je dirai même le reflexe inscrit dans nos gêne, de faire 

```python
import numpy as np
```

En effet personne n'a le courage d'utiliser le nom `numpy` dans son intégralité dans ses programmes Python. Et bien en c++ on peut faire l'équivalent. La syntaxe repose encore sur le mot clé `namespace` et est la suivante : 

```c++
namespace calc=mylib::calcul;
```
<!-- #endregion -->

<!-- #region -->
Par exemple si l'on reprend l'exemple précédent, on peut simplifier le fichier `main.cpp` de la manière suivante : 

```c++
// Fichier main.cpp

namespace msg=mylib::message;
namespace calc=mylib::calcul;

int main(){

    std::vector<double> v {1,2,3,4};
    msg::info("Un message bidon");
    double m = calc::mean(v);

    return 0;
}
```
<!-- #endregion -->

```python

```
