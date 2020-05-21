#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getNodeIndex(int a, int r);

int ignoreThis;
int numAvenidas, numRuas, numSupermercados, numCidadaos;

int main(){
	ignoreThis = scanf("%d %d", &numAvenidas, &numRuas);

	ignoreThis = scanf("%d %d", &numSupermercados, &numCidadaos);

	/* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		ignoreThis = scanf("%d %d", &x, &y);

	}

	/* Criar source universal */
	for(i = 0; i < numCidadaos; i++){
    	ignoreThis = scanf("%d %d", &x, &y);

  }

	return 0;
}

int getNodeIndex(int a, int r){
	/*
	6  3  0

	7  4  1

	8  5  2
	*/
	return (r-1)*numAvenidas + a - 1;
}
