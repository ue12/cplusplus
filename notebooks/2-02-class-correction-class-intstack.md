---
jupytext:
  formats: md:myst
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

# correction de la première version de la classe `IntStack`

+++

<div class = "framed-cell">
<ins class = "underlined-title">une première version de la classe `IntStack`</ins>

<br>

le fichier d'entête `intstack.h` qui définit la classe pour la pile d'entiers

```c++
// dans le fichier intstack.h
#include <iostream>
class IntStack {
public:
  IntStack (int s): tab (new int [size]), size(s), top (0)  {
  }
  void push (int e) {
    tab[top] = e;
    top = top+1;
  }
  int pop () {
    top = top-1;
    return tab[top];
  }
  void print () {
    std :: cout << "[";
    for (int i=0; i<top; i++) {
      std::cout << tab[i] << ' ';
    }
    std::cout << '[' << std::endl;
  }
  void delete_stack () {
    delete [] tab;
  }
private:
  int size;
  int top;
  int* tab;
};
```

<br>

on teste notre classe pour la pile d'entiers

```c++
#include <iostream
void test () {
  IntStack st (12);
  st.print(); // [[

  st.push(81);
  st.print(); // [81 [

  st.push(52);
  st.print(); // [81 52 [

  st.push(36);
  st.print(); // [81 52 36 [

  std :: cout << st.pop () << std::endl; // 36
  st.print (); // [81 52 [
}

int main () {
  test();
  return 0;
}
```

<br>

notons dans ce code quelque chose de très **important**:

* dans la liste d'intialisation nous faisons les initialisations des 3 attributs des objets de type `IntStack` piles à savoir `size`, `top` et `tab`
* ici la taille de `tab` pour le `new[]` est prise comme étant `size` et non `s`
* ainsi pour que ce code fonctionne correctement, il faut que `size` ait été initialisé **avant** que `tab` ne le soit !

<br>

Qu'est-ce qui nous garantit que `size` sera initialisé **avant** `tab` ?


* ce n'est **pas** l'ordre des initialisations dans la liste d'initialisation du constructeur  
i.e. ce n'est **pas** l'ordre `tab (new int [size]), size(s), top (0)`


* c'est l'ordre dans lequel vous avez déclaré les attributs dans votre classe `IntStack`  
```c++
private:
  int size;
  int top;
  int* tab;
```

donc dans ce code ce sera dans l'ordre `size`, `top` et `tab`

</div>
