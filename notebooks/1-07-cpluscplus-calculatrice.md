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

#  Calculatrice notation polonaise inversée 


## A l'issue de la séance 


L'objectif de cet exercice est qu'à l'issue de la séance vous ayez un programme c++ vous permettant d'évaluer une expression mathématique fournie en notation polonaise inversée. 


## Pré-requis 

La notation polonaise inversée (**R**everse **P**olish **N**otation) est une notation mathématique dans laquelle les opérateurs suivent leurs opérandes. 

Par exemple l'expression : 

$$1+2*(3-1)$$ 

se traduit en notation polonaise inversée de la manière suivante : 

$$ 1\, 2\, 3\, 1\, -\, *\, +$$


L'évaluation d'une expression en RPN peut se faire en suivant l'algorithme suivant : 

```
for each token in expression:                                                         
  if token is an operator:                                                            
    operand_2 <- pop from the stack                                                   
    operand_1 <- pop from the stack                                                   
    result <- evaluate token with operand_1 and operand_2                             
    push result back onto the stack                                                   
  else if token is an operand:                                                        
    push token onto the stack                                                         
  endif                                                                               
endfor                                                                                
result <- pop from the stack 
```

<!-- #region -->
## Travail à faire 

L'objectif du TP est donc que vous fassiez un programme C++ permettant de fournir comme argument une expression en notation polonaise 

```bash 
$ ./rpn_cpp 3 5 ! x
-15
```

Pour cela nous vous fournissons le squelette de code suivant : 

```c++
int main(int argc, char* argv[]){

// your code here
}
```

Enjoy !!
<!-- #endregion -->

<!-- #region -->
## Quelques indications complémentaires


* Le symbole de négation (le moins unaire) est symbolisé par `!` pour éviter la confusion avec le symbole `-`
* La multiplication est symbolisée par `x` pour éviter les problèmes d'interprétation du `*` dans le bash
* Pour travailler avec les stack écrire des fonctions opérants sur la stack fournie en argument
* Ecrire une fonction vérifiant qu'un `char` est un entier positif
* Ecrire une fonction vérifiant qu'un `char` est un operateur. 
<!-- #endregion -->



```python

```
