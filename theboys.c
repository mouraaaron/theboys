// programa principal do projeto "The Boys - 2024/2"
// Autor: Aaron Moura, GRR 20255804

// seus #includes vão aqui
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fila.h"
#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"
#include "lef.h"
#include "fprio.h" 


// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  srand(time(NULL));
  struct Mundo *M = cria_mundo();
  if(!M)
  {
    fprintf(stderr, "Erro ao criar o mundo\n");
    exit(1);
  }


  incia_lef(M);

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

