#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const unsigned ARRAY_SIZE = 1024;
static const unsigned NUMBER_OF_TRIALS = 10000000;

/* On va declarer des tableaux en statique pour essayer de jouer simplement avec les caches */

int main(int argc, char *argv[]) {
  const double coeff = log(2);

  double * in1 = malloc(sizeof(double) * ARRAY_SIZE);
  double * in2 = malloc(sizeof(double) * ARRAY_SIZE);
  double * in3 = malloc(sizeof(double) * ARRAY_SIZE);
  double * acc = malloc(sizeof(double) * ARRAY_SIZE);
  unsigned int * index = malloc(sizeof(unsigned) * ARRAY_SIZE);


  /* des tableaux de donnees arbitraires */
  for (unsigned i=0; i < ARRAY_SIZE; i++) {
    in1[i] = i*1.0e-9; in2[i] = i*0.5e-9;
    index[i] = (2*i+5)%ARRAY_SIZE;
  }
  /* Plein de runs de la boucle a vectoriser */
  for (unsigned t=0; t < NUMBER_OF_TRIALS; t++) {
    for (unsigned i=0; i < ARRAY_SIZE; i++) {
      acc[i] += coeff*in1[i] + in2[i]; /* MACC de base */
      /* variante de traitement a tester */
      // acc[i] = acc[i-1] + coeff*in1[i] + in2[i];
      /* variante de traitement a tester */
      //acc[i] += coeff*in1[i] + in2[index[i]];
      /* variante de traitement a tester */
      //acc[i%2] += coeff*in1[i%3] + in2[i];
      /* variante de traitement a tester */
      //acc[i%2] += coeff*in1[i] + log(in2[i]);

    }
  }
  /* évite la suppression d'operations jug"es inutiles par le compilateur */
  __attribute__((unused)) volatile double store = acc[argc];

  free(index);
  free(acc);
  free(in3);
  free(in2);
  free(in1);
  return 0;
}
