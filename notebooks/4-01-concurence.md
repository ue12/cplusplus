---
jupyter:
  jupytext:
    text_representation:
      extension: .md
      format_name: markdown
      format_version: '1.2'
      jupytext_version: 1.6.0
  kernelspec:
    display_name: Python 3
    language: python
    name: python3
---

<!-- #region slideshow={"slide_type": "slide"} -->
# Programmation concurrente en C++
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Dans le cours **Programmes Coopérants** vous avez vu les bases de la programmation concurente en Python par la création de process ou de threads. Vous avez également vu comment il est possible de faire du calcul parallèle et donc exploiter les architectures des super calculateurs modernes via le protocole MPI. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Dans ce cours nous allons reprendre une partie de ce qui a été abordé mais en nous focalisant sur l'utilisation du C++. Plus précisément nous allons voir comment faire de la programmation concurrente en C++ via deux aspects : (i) l'utilisation du multithread ; (ii) l'utilisation de modèle de programmation asynchrone. 

Alors si des puristes lisent ce notebook, ils pourraient être tentés de dire que le multi-thread ce n'est pas forcément du concurrent mais du parallèle et que de l'asynchrone ce n'est pas du concurrent... C'est une question de point de vue. Plus précisément ce que l'on va voir aujourd'hui c'est comment faire un programme qui va devoir effectuer un grand nombre de tâches (une tâche étant une série d'instructions) et les effectuer le plus rapidement possible en tirant au maximum parti des ressources du matériel à disposition. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "slide"} -->
## Rappel sur les *lambda* fonctions
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Pour commencer faisons juste un petit rappel sur les fonctions anonymes ou *lambda* fonctions. En effet lorsque l'on fait de la programmation concurrente en C++ les fonctions anonymes s'avèrent être extrêmement pratiques pour se simplifier la vie. Nous allons donc rapidement faire un tour d'horizon de leurs définitions et utilisations. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
La syntaxe générale des fonctions anonymes est la suivante : 

```c++
[capture]( params ) -> ret { body }
```

* `params` représente la liste des paramètres d'entrée de votre fonction, donc une suite de paramètres nommés et typés, comme dans une fonction classique
* `ret` est le type de retour de votre fonction anonyme, vous pouvez ne pas le préciser il est alors automatiquement déduit si le mot clé `return est présent dans `body` sinon il est considéré comme `void`
* `body` est le corps de votre fonction
* `capture` est une liste de variables existant dans le scope de déclaration de la fonction anonyme et devant être transmises au scope interne à la fonction.

Vous pouvez si vous le souhaitez stocker votre fonction anonyme dans une variable (pour l'utiliser ensuite) de la manière suivante :

```c++
auto f = [capture](params) -> ret { body };
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Regardons tout de suite un exemple: 

```c++
#include <iostream>
#include <string>

int main () {

    auto f = [](const std::string& msg){
        std::cout << msg << "\n";
    };

    f("coucou");

    return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->

Pour expliquer cette histoire de `capture` regardons l'exemple suivant : 

```c++
double a=2.1;
auto f = [](){ std::cout << "a = " << a << "\n" ; };
// Compilation Error 'a' is not captured 
```

En effet ce code ne compile pas car `a` est bien défini mais n'est pas accessible depuis l'intérieur de la lambda fonction. Une solution me direz vous est alors de passer `a` comme argument d'entrée de la fonction. Oui c'est vrai. Mais on peut également utiliser la partie `capture` de la lambda fonction pour capturer `a` dans le scope de la fonction. Cela donne : 

```c++

double a=2.1;
auto f = [a](){ std::cout << "a = " << a << "\n" ; };
```

En revanche avec cette approche, la variable `a` est en lecture seule au sein de la fonction anonyme. C'est-à-dire que l'on ne peut pas modifier la variable `a` dans la fonction. Si vous souhaitez pouvoir modifier la valeur de `a` il faut la capturer par référence. 

```c++

double a=2.1;
auto f = [&a](){ 
    std::cout << "a = " << a << "\n" ; 
    a = 1024.0;
    };
```

<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
*Remarque* il existe une syntaxe particulière au niveau de la capture permettant de capturer toutes les variables présentes dans le scope de définition de la fonction pour les injecter dans le scope interne de la fonction. Il s'agit de :

* `[=]` qui capture toutes les variables par copie *read-only*
* `[&]` qui capture toutes les variables par références, permettant ainsi de les modifier au sein de la fonction. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
*Remarque 2* bien évidemment ces notations qui ont l'air très pratiques d'utilisation sont plutôt à éviter car je suis sûr que vous en conviendrez ce n'est pas très propre comme approche. Ca manque de classe !
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "slide"} -->
## Programmation multi-thread
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Après ce bref rappel au sujet des fonctions anonymes nous allons pouvoir entrer dans le vif du sujet. Nous allons donc commencer par voir comment faire de la programmation multi-thread en C++. 

Pour rappel le modèle de programmation multi-thread a pour principe d'exploiter au maximum l'architecture multi-cœurs des processeurs récents. Pour cela le programme principal va créer des threads qui vont s'exécuter de manière concurrente sur les différents cœurs de votre processeur. Pour ceux qui auraient oublié ce qu'est un thread, également appelé processus léger, c'est un ensemble d’instructions machine regroupées au sein d'une pile d'exécution partageant sa mémoire avec le processus l'ayant créé. Formulé autrement des threads sont des blocs d'instructions C++ partageant entre eux la même mémoire. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Depuis la norme 2011 du C++, C++11, l'utilisation des threads est fortement simplifiée. Il vous suffit d'inclure le header file correspondant 

```c++
#include <thread>
```

L'élément de base est la classe `std::thread` qui va nous permettre de créer un thread pour une fonction donnée.  
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Considérons tout de suite l'exemple suivant d'une fonction `main` créant un `thread` chargé d'afficher dix fois le même message.

```c++
#include <thread>
#include <iostream>

int main(){
    std::thread t ([]()->void {
        for( int i=0; i<10; i++ ){
            std::cout << "Hello World from thread" << i << std::endl;
        }
    });

    std::cout << "Hello World from the main function " << std::endl;

    t.join();

    return 0;
}
```

```c++
$ g++ main.cpp -lpthread
```

On constate alors à l'exécution que le thread et le programme principal vivent chacun leur vie de leur côté. Bien évidemment il est possible de créer autant de threads qu'on le souhaite dans un programme.  
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
```c++
#include <thread>
#include <iostream>
#include <vector>

int main(){
    const int n_threads {10};

    std::vector<std::thread> _threads;

    for( int i=0; i<n_threads; i++){
        _threads.push_back(std::thread([](const int& tid)->void {
                            for( int i=0; i<10; i++ ){
                                 std::cout << "Hello World from thread "<< tid << std::endl;
                            }
            }, i) // i est la position du thread dans le vecteur
        );
    }

    std::cout << "Hello World from the main function " << std::endl;

    for(std::thread& t: _threads){
        t.join();
    }

    return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
En exécutant ce code plusieurs fois sur votre ordinateur portable vous allez alors voir apparaître quelque chose de potentiellement étrange... De temps quelques lignes de la sortie sont entremêlées entre elles. Par exemple 

```
Hello World from thread 5Hello World from thread 7
Hello World from thread Hello World from thread 
Hello World from thread Hello World from thread 5
8
Hello World from thread 8
7
```

Bizarre non ? Un avis sur la question ? C'est un effet de ce que l'on appelle un `race condition` ! Le principe est simple, nous avons deux threads qui accèdent simultanément à la même variable partagée et la modifie. C'est le risque lorsque l'on fait du multithread. Pour prévenir cela il est nécessaire de mettre en place des mécanismes de verrouillage.
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
### Utilisations des mutex
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Un mutex, *Mutual Exclusion*, est un objet utilisé en programmation concurrente pour éviter que différents threads n'accèdent simultanément à des ressources partagées. Pour utiliser un `mutex` en C++ il vous suffit d'inclure le header file correspondant :

```c++
#include <mutex>
```

L'objet de base est `std::mutex`, oui ça manque d'originalité tout ça je sais. Cet objet est extrêmement simple car il ne possède que deux méthodes `lock()` et `unlock()`. Comme le nom le laisse imaginer ces deux méthodes ont respectivement pour but de bloquer  et débloquer les threads. 

Reprenons tout de suite l'exemple précédent. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
```c++
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

std::mutex mtx;

int main(){
  const int n_threads {10};

  std::vector<std::thread> _threads;

  for( int i=0; i<n_threads; i++){
    _threads.push_back(std::thread([](const int& tid)->void {
          for( int i=0; i<10; i++ ){
            mtx.lock();
            std::cout << "Hello World from thread "<< tid << std::endl;
            mtx.unlock();
          }
	}, i)
      );
  }

  std::cout << "Hello World from the main function " << std::endl;

  for(std::thread& t: _threads){
    t.join();
  }

  return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Afin d'assurer qu'il n'y a pas d'accès simultanés à la sortie standard, via le `std::cout`, on encadre donc la ligne `std::cout` par un appel à la méthode `lock()` entraînant ainsi l'arrêt des autres threads et ensuite un appel à `unlock` pour débloquer les threads. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
**Attention** l'utilisation d'un mutex permet en effet de résoudre les problèmes d'accès concurrents aux ressources partagées. En revanche cela a un prix, il s'agit de la performance. En effet le fait de bloquer/débloquer des threads prend un temps, minime certes, mais non nul. Donc si on répète cette opération de nombreuses fois cela va fortement dégrader les performances. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
**Attention** il faut faire très attention lorsque vous utilisez un mutex au fait qu'un `lock` doit toujours être associé à un `unlock` sinon votre programme va se bloquer définitivement. Cela peut s'avérer parfois délicat notamment quand on doit en plus gérer les exceptions. Pour faciliter cela il existe dans la librairie standard, l'objet `std::unique_lock<Mutex>`. Ce dernier permet de faire un `lock` et surtout de déverrouiller le mutex à la sortie du contexte, i.e. sortie de fonction par exemple (l'objet `unique_lock` sera alors détruit et son destructeur libère le lock). Par exemple voici ci-dessous un usage où l'on ne fait pas le `unlock` manuellement :

```c++
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::unique_lock

std::mutex mtx;           // mutex for critical section

void print_block (int n, char c) {
  std::unique_lock<std::mutex> lck (mtx);
  for (int i=0; i<n; ++i) { std::cout << c; }
  std::cout << '\n';
}

int main (){
  std::thread th1 (print_block,50,'*');
  std::thread th2 (print_block,50,'$');

  th1.join();
  th2.join();

  return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Pour illustrer cela considérons par exemple le calcul de $\pi$. Une manière possible pour calculer $\pi$ est d'évaluer numériquement l'intégrale suivante :

$$ \pi = \int_{0}^{1} \frac{4}{1+x^2} $$

Le calcul séquentiel classique peut se faire de la manière suivante : 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
```c++
#include <iostream> 

int main(){

    int nb_point = 100000000;
    double l=1./nb_point;

    double pi=0;
    for( int i=0; i<nb_point; i++){
        double x=l*(i+0.5);
        pi += l*( 4. / (1. + x*x ) );
    }

    std::cout << "PI = " << pi << std::endl;

    return 0;
}
```

Si vous lancez alors ce code et mesurez le temps d'exécution vous obtenez le résultat suivant : 

```bash 
0.84user 0.00system 0:00.85elapsed 98%CPU
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Si l'on fait alors une version multi-thread naïve, c'est à dire avec plein de `lock`/`unlock`. Cela pourrait donner le code suivant : 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;

void pi_thread_worker(const uint& nbpoint, const uint tid, const uint nbthread, double& pi){
    double s = 0.;
    double l = 1./nbpoint;
    int start = tid*(nbpoint/nbthread);
    int stop = (tid+1)*(nbpoint/nbthread);
    if( tid == nbthread-1){
        stop += nbpoint%nbthread;
    }

    double x;
    for( int i=start; i<stop; i++){
        x = l * ( i + 0.5 );
        mtx.lock();
        pi += l * ( 4. / (1 + x*x ) );
        mtx.unlock();   
    }
}

int main(int argc, char* argv []){
    if( argc == 1 ){
        std::cerr << "Specify the number of thread" << std::endl;
        return 1;
    }

    int nb_point = 100000000;
    int nb_thread = atoi(argv[1]);

    double pi=0;
    std::vector<std::thread> threads;
    for( int i=0; i<nb_thread; i++){
        threads.push_back( std::thread( pi_thread_worker, nb_point, i, nb_thread, std::ref(pi) ) );
    }

    for( int i=0; i<nb_thread; i++){
        threads[i].join();
    }

    return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Si on lance alors ce code avec deux threads on obtient le résultat suivant : 

```bash 
11.56user 11.78system 0:11.84elapsed 197%CPU
```

On est donc plus de 10 fois plus lent que la version séquentielle !!!!! C'est normal en même temps, le code précédent c'est du grand n'importe quoi !!! 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
En effet plutôt que de faire les `lock`/`unlock` dans la boucle il est préférable de créer une variable temporaire dans le thread. 

```c++
void pi_thread_worker(const uint& nbpoint, const uint tid, const uint nbthread, double& pi){
    double s = 0.;
    double l = 1./nbpoint;
    int start = tid*(nbpoint/nbthread);
    int stop = (tid+1)*(nbpoint/nbthread);
    if( tid == nbthread-1){
        stop += nbpoint%nbthread;
    }

    double x;
    double tmp=0;
    for( int i=start; i<stop; i++){
        x = l * ( i + 0.5 );
        tmp += l * ( 4. / (1 + x*x ) );
    }
    mtx.lock();
    pi += tmp;
    mtx.unlock():
}
```

Si on relance alors ce code, avec cette légère modification, toujours avec deux threads on obtient le résultat suivant : 
```bash 
0.95user 0.00system 0:00.49elapsed 195%CPU
```

On obtient donc dans ce cas un temps d'exécution divisé d'un facteur presque 2 par rapport à la version séquentielle. Comme quoi le langage ne fait pas tout ! le rigolo derrière le clavier a sa part de responsabilité ... 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
**Exercice** Refaites le calcul de $\pi$ en multi-thread sans aucun mutex ! 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
### Un autre moyen de faire des threads : OpenMP  
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
OpenMP, pour Open Multi-Processing, est un protocole générique permettant de faire du calcul parallèle à mémoire partagée (multi-thread). L'avantage historique de OpenMP est son support universel sur Linux, Mac et Windows (avantage moins marquant maintenant que les threads sont dans la librairie standard C++).  
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
La grosse distinction entre OpenMP et de la programmation en thread comme on vient de le voir est que OpenMP se base sur la déclaration de directive dans le code source, on n'ajoute pas de code ou presque pas. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Par exemple, faisons un programme qui lance plusieurs threads chaque threads traitant une partie des itérations d'une boucle `for`. En utilisant OpenMP ce programme serait : 

```c++
#include <iostream>
#include <omp.h>

int main(){
    #pragma omp parallel for 
    for(int i=0; i<20; i++){
        std::cout << "Element " << i << " dans le thread " << omp_get_thread_num() << std::endl;
    }

}
```

La compilation se fait de la manière suivante : 

```bash 
$ g++ -fopenmp main.cpp -o main
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Nous pouvons alors remarquer plein de choses : 

* Le parallélisme de la boucle s'est fait en une seule ligne à l'aide de `#pragma omp parallel for`
* On ne précise à aucune moment le nombre de threads => suivant vos ordinateurs vous allez obtenir 2,4 ou 8 threads

Rq: le nombre de thread se contrôle facilement via la fonction `omp_set_num_threads(int)` ou bien par la variable d'environnement `OMP_NUM_THREADS`. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
**Notion de synchronisation en OpenMP** 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Dans la programmation à base de threads nous avons vu juste avant que l'on peut être obligé à certains moment de synchroniser des threads ou à minima de vérouiller l'accès à certaines variables pour éviter les problèmes de Race Condition. Dans OpenMP il est également possible prévenir cela en utilisant l'instruction `#pragma omp critical`
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Par exemple, si on reprend le calcul de $\pi$ dans sa version très mal écrite où on fait beaucoup trop de lock avec des directives OpenMP nous pourrions écrire :  

```c++
double pi_computer_bad_critical(const int nb_point, int n_thread ){
  omp_set_num_threads(n_thread);
  double l=1./nb_point;
  double pi=0; 
  #pragma omp parallel 
  {
    double tmp=0;
    #pragma omp for
    for( int i=0; i<nb_point; i++){
      double x = l*(i+0.5);
      #pragma omp critical 
      pi += l*( 4. / (1. + x*x ) );
    }
  }
  return pi;
}

```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Ainsi avec la directive `#pragma omp critical` on déclare que la ligne juste en dessous est critique, i.e. que lorqu'un thread l'exécute il ne faut surtout pas que les autres threads y touchent. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Bien évidemment la bonne version du calcul de $\pi$ est la suivante : 

```c++
double pi_computer_critical(const int nb_point, int n_thread ){

  omp_set_num_threads(n_thread);
  double l=1./nb_point;

  double pi=0;
  
  #pragma omp parallel 
  {
    double tmp=0;
    #pragma omp for
    for( int i=0; i<nb_point; i++){
      double x = l*(i+0.5);
      tmp += l*( 4. / (1. + x*x ) );
    }
    #pragma omp critical 
    pi += tmp;
  }
  return pi;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Il existe en OpenMP une autre notion qui peut s'avérer fortement utile pour les applications type calcul scientifique c'est la notion de reduction. L'intérêt est d'optimiser les opérations cumulées sur une variable. Par exemple dans le cas de $\pi$ il y a la ligne faisant la somme `pi+=l*(4./(1+x*x));` En utilisant une directive de réduction nous pouvons simplifier le code parallèle de la manière suivante : 

```c++
double pi_computer_reduction(const int nb_point, int n_thread ){

  omp_set_num_threads(n_thread);
  double l=1./nb_point;

  double pi=0;
  #pragma omp parallel for reduction(+:pi)
  for( int i=0; i<nb_point; i++){
    double x = l*(i+0.5);
    pi += l*( 4. / (1. + x*x ) );
  }
  return pi;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
**Différences entre OpenMP et Threads**
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
A première vu nous pourrions dire que OpenMP c'est vachement plus simple et qu'il ne faut pas se prendre la tête à utiliser les threads directement. 

Je nuancerai en donnant les élements suivants : 

* OpenMP est à visée calcul scientifique, ultra performant pour les boucles for 
* Dès que l'on veut un peu de finesse dans la gestion de threads, par exemple deux threads qui font deux choses complètement différentes, pas le choix on ne peut plus utiliser OpenMP. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
### Application 

L'objectif est de faire en parallèle (thread ou OpenMP) la fonction faisant le produit d'une matrice NxM par un vecteur M. 


Vous trouverez dans le dépôt git, un fichier `mat_vec_prod_skeleton.cpp` contenant la définition d'une classe `Matrix<T>` et la fonction séquentiel réalisant le produit matrice vecteur. Vous trouverez également une fonction `benchmark` permettant de faire les mesures de performance. 


<!-- #endregion -->

<!-- #region slideshow={"slide_type": "slide"} -->
## Programmation asynchrone
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Nous allons à présent voir une autre approche de programmation concurrente : la programmation asynchrone. Le principe est de permettre l'exécution de tâches, nécessitant un temps non négligeable, en parallèle du fil d'exécution principal. Le modèle asynchrone a commencé à apparaître avec l'émergence des services web, base de données en lignes, ... Le principe est de permettre l'exécution en "tâche de fond" des fonctions consommatrices en temps mais pas en ressources, comme par exemple des requêtes sur Internet ou dans une base de données, l'écriture ou la lecture de fichiers, ...  
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Depuis la norme C++11 le C++ offre tout ce qu'il faut pour faire de la programmation asynchrone facilement. Tout ce dont vous avez besoin se trouve dans le header `future`

```c++
#include <future>
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
L'élément de base est la fonction `std::async` qui prend en argument : 

* Une politique d'exécution qui permet de contrôler le comportement asynchrone 
    * `std::launch::async` exécute la fonction de manière asynchrone dans un thread séparé
    * `std::launch::deferred` exécute la fonction de manière synchrone.
* Une fonction 
* Les arguments de la fonction 

Le prototype de cette fonction `std::async` est le suivant

```c++
template <class Fn, class... Args>
future<typename result_of<Fn(Args...)>::type> async (launch policy, Fn&& fn, Args&&... args);
```

On voit alors que cette fonction `async` nous renvoie un objet de type `std::future<T>`. L'objet `std::future<T>` est templaté par le type de retour de la fonction `fn` que vous donnez à votre `std::async`. Le `std::future` permet une encapsulation du résultat de la fonction asynchrone, permettant ainsi de toujours avoir le résultat à portée de main dans le programme principal.

Pour récupérer la valeur associée au `std::future` il suffit d'appeler la méthode `get()` qui vous renvoie alors la valeur retournée par votre fonction asynchrone. Si la fonction ne s'est pas encore exécutée ou n'est pas terminée au moment du `get()` l'appel à `get()` est bloquant et donc attend la fin de l'exécution de la fonction asynchrone. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Voyons tout de suite un exemple : 

```c++
#include <iostream>
#include <future>

int main(){

  std::future<int> val = std::async(std::launch::async, []()-> int {
      std::cout	<< "Hello from future" << std::endl;
      return 1.;
    });


  std::cout << "Hello World from main" << std::endl;

  int value = val.get();
  std::cout << "Value = " << value << std::endl;
  return 0;

}
```

```bash 
$ g++ main.cpp -o main -lpthread
$ ./main 
Hello World from main
Hello from future
Value = 1
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Pour voir de manière plus concrète l'intérêt de l'asynchrone regardons un exemple un peu plus parlant. Considérons un programme qui doit récupérer des informations dans des bases de données. Chaque requête à une base prends 5 secondes. Le code séquentiel classique pour simuler ce comportement est le suivant : 

```c++ 
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std::chrono;

std::string request_db(std::string req){
  std::this_thread::sleep_for(seconds(5));
  return "DB_" + req;
}


int main(){
  system_clock::time_point start = system_clock::now();

  std::string data1 = request_db("req db1");
  std::string data2 = request_db("req db2");

  auto end = system_clock::now();

  auto diff = duration_cast < std::chrono::seconds > (end - start).count();
  std::cout << "Total Time = " << diff << " Seconds" << std::endl;

  std::string data = data1 + " :: " + data2;
  std::cout << "Data = " << data << std::endl;

  return 0;
}
```

L'exécution de ce code donne alors comme résultat : 

```
Total Time = 10 Seconds
Data = DB_req db1 :: DB_req db2
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Si maintenant on modifie très légèrement le code à coup de `std::async` on peut obtenir le code suivant : 

```c++ 
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>

using namespace std::chrono;

std::string request_db(std::string req){
  std::this_thread::sleep_for(seconds(5));
  return "DB_" + req;
}


int main(){
  system_clock::time_point start = system_clock::now();

  std::future<std::string> data1 = std::async(std::launch::async, request_db, "req db1");
  std::future<std::string> data2 = std::async(std::launch::async, request_db,"req db2");

  std::string data = data1.get() + " :: " + data2.get();

  auto end = system_clock::now();
  auto diff = duration_cast < std::chrono::seconds > (end - start).count();
  std::cout << "Total Time = " << diff << " Seconds" << std::endl;

  std::cout << "Data = " << data << std::endl;

  return 0;
}
```

On obtient alors le résultat suivant : 
```
Total Time = 5 Seconds
Data = DB_req db1 :: DB_req db2
```

Donc comme prévu le temps d'exécution est réduit à 5 secondes car les tâches se font simultanément !!! Wouaaaahhh oui je sais c'est génial. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Pour finir ce premier tour d'horizon de la concurrence en C++ je vous propose de faire un exemple de gestion d'un ensemble de tâches. Il s'agit d'un système où l'on a une queue contenant les différents paramètres d'entrée, par exemple une requête à faire à une base de donnée, et des `workers` viennent piocher dans la queue font la requête et mettent le résultat dans une nouvelle `queue`.
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
Si vous êtes encore bien éveillé vous aurez peut-être remarqué que dans le scénario précédent on a besoin d'une `queue` mais il faut qu'elle soit un peu particulière puisque qu'elle va être partagée entre plusieurs fil d'exécution... Il faut donc que l'on s'occupe des problèmes d'accès concurrents. En d'autre mot quand on va faire un `pop()` par exemple et bien il faut être sur qu'un autre fil d'exécution ne fait pas un `pop()` exactement au même moment. Il nous faut donc une `queue` qui soit *thread-safe*.  Alors dans les cours Python vous avez l'habitude que je vous dise qu'il s'agit d'un langage merveilleux et donc que tout est déjà fait pour vous ... Et bien pas de chance en C++ c'est pas la même chose, néanmoins C++ reste un langage merveilleux pas de doute à ce sujet. Mais on vous prend moins la main donc pas de `queue` thread-safe sur étagère. Mais pas d'inquiétude vous allez voir c'est facile. 
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Alors pour faire notre `queue` asynchrone, que l'on appellera avec beaucoup d'originalité `AsyncQueue` nous allons avoir besoin de trois choses : 

* La queue synchrone classique de la librairie standard `std::queue`
* Un mutex qui nous permettra de verrouiller la queue lorsque l'on opère dessus
* Une variable conditionnelle `std::condition_variable`, il s'agit d'un objet de la librairie standard qui permet de synchroniser les threads et de notifier des threads de certains évènements. 

Le fonctionnement attendu de la `AsyncQueue` est le suivant : lorsqu'un thread fait une action sur la queue (`push`, `pop` ou `empty`), un thread qui voudrait faire une action quelconque sur la queue est bloqué, un thread qui ne touche pas à la queue continue son exécution. 
 
Ci-dessous un exemple d'implémentation de cette `AsyncQueue`, vous remarquerez l'usage de la méthode `wait` dans la méthode `pop` qui, dans le cas où la queue est vide, permet à un thread d'attendre qu'un autre thread fasse un `push` pour retourner un résultat.
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
```c++
#include<iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <thread>

template<typename T>
class AsyncQueue {
private:
    std::queue<T> _queue;
    std::mutex _mtx;
    std::condition_variable _notifier;

public:
    AsyncQueue()=default;
    AsyncQueue(const AsyncQueue&) = delete;
    AsyncQueue& operator=(const AsyncQueue&) = delete;

    bool empty(){
        std::unique_lock<std::mutex> lock(this->_mtx);
        bool ret = this->_queue.empty();
        lock.unlock();
        return ret;
    }

    void push(const T& x){
        std::unique_lock<std::mutex> lock(this->_mtx);
        this->_queue.push(x);
        lock.unlock();
        this->_notifier.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> lock(this->_mtx);
        while(this->_queue.empty()){
            this->_notifier.wait(lock);
        }
        T val = this->_queue.front();
        this->_queue.pop();
        return val;
    }
};
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Une fois que nous avons fait notre `AsyncQueue` nous avons fait le plus dur !! Le reste c'est facile. Alors juste pour le cosmétique nous allons commencer par faire une fonction `async_print` qui va s'assurer que l'on ait pas de chevauchement des lignes à l'affichage. 

```c++

void async_print(std::string x) {     // Thread safe print 
  static std::mutex mutex;
  std::unique_lock<std::mutex> locker(mutex);
  std::cout << x << "\n";
  locker.unlock();
}
```

Ensuite on fait notre fonction `worker` qui prend en entrée la queue contenant les données d'entrées et la queue initialement vide qui va nous permettre de stocker les résultats. Le principe de cette fonction est très simple, tant que la queue des entrées n'est pas vide on fait quelque chose. Le quelque chose en question dans ce cas étant de dormir pendant X secondes avec X le numéro du worker. 

<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
```c++
void worker(AsyncQueue<int>& input,
	    unsigned int id,
	    AsyncQueue<std::string>& output) {
  while( ! input.empty() ){
    auto item = input.pop();
    std::ostringstream tmp;
    tmp << " " << item << " --> C" << id;
    async_print(tmp.str());
    std::this_thread::sleep_for(std::chrono::seconds(id));
    tmp.str("");
    tmp << "       " << item << " done " << "C" << id << " --->  results";
    async_print(tmp.str());
    tmp.str("");
    tmp << "done " << item;
    output.push( tmp.str() );
  }
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
Pour finir nous pouvons alors écrire notre `main` de la manière suivante par exemple.
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "fragment"} -->
```c++
int main()
{
  const int nbWorker {4};
  const int nbInput {14};

  AsyncQueue<int> q;
  AsyncQueue<std::string> results;

  for( int i=0; i<nbInput ; i++){
    q.push( i );
  }

  std::vector<std::future<void> > workers;
  for (int i = 0 ; i < nbWorker ; ++i) {
    std::future<void> w = std::async(std::launch::async, worker, std::ref(q), i + 1, std::ref(results));
    workers.push_back(std::move(w));
  }

  for (auto& w : workers) {
    w.get();
  }

  while(!results.empty()){
    std::cout << results.pop() << std::endl;
  }
  return 0;
}
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
L'exécution de ce code donne alors le résultat suivant : 

```
 1 --> Worker2
 0 --> Worker1
 2 --> Worker3
 3 --> Worker4
       0 done Worker1 --->  results
 4 --> Worker1
       1 done Worker2 --->  results
 5 --> Worker2
       4 done Worker1 --->  results
 6 --> Worker1
       2 done Worker3 --->  results
 7 --> Worker3
       6 done Worker1 --->  results
 8 --> Worker1
       5 done Worker2 --->  results
 9 --> Worker2
       3 done Worker4 --->  results
 10 --> Worker4
       8 done Worker1 --->  results
 11 --> Worker1
       11 done Worker1 --->  results
 12 --> Worker1
       9 done Worker2 --->  results
 13 --> Worker2
       7 done Worker3 --->  results
       12 done Worker1 --->  results
       10 done Worker4 --->  results
       13 done Worker2 --->  results
done 0
done 1
done 4
done 2
done 6
done 5
done 3
done 8
done 11
done 9
done 7
done 12
done 10
done 13
```
<!-- #endregion -->

<!-- #region slideshow={"slide_type": "subslide"} -->
### Application 

Une application simple de ce type d'approche est le web scraping. 
TODO
<!-- #endregion -->

```python

```
