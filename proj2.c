#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void buildGraph();
int getNodeIndex(int a, int r);

typedef struct node node_t;
typedef struct node{
  char visited;
  int nVizinhos;
  node_t **vizinhos;
} node_t;

unsigned int numAvenidas, numRuas, numSupermercados, numCidadaos;
node_t *graph, uSource, uSink;

int main(){
  int graphSize = 2;
  int i, j, k, x, y;

  scanf("%d %d", &numAvenidas, &numRuas);
  graphSize += numAvenidas * numRuas;
  scanf("%d %d", &numSupermercados, &numCidadaos);
  graphSize += numSupermercados + numCidadaos;

  graph = (node_t *) calloc(graphSize, sizeof(node_t));
  buildGraph();

  /* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		scanf("%d %d", &x, &y);

	}

  return 0;
}

void buildGraph(){
  int offsets[] = {-1, -3, 1, 3};
  int i, j, size = numAvenidas * numRuas;

  if(uSource.vizinhos == NULL)
    uSource.vizinhos = (node_t *) calloc(32, sizeof(node_t));
  if(uSink.vizinhos == NULL)
    uSink.vizinhos = (node_t *) calloc(32, sizeof(node_t));

  for(i = 0; i < numAvenidas * numRuas; i++){
		for(j = 0; j < 4; j++){
			if(i + offsets[j] >= 0 && i + offsets[j] <= numAvenidas * numRuas - 1){
				if(graph[i].vizinhos == NULL)
          graph[i].vizinhos = (node_t *) calloc(32, sizeof(node_t));

        if(graph[i].nVizinhos % 32 == 0)
          graph[i].vizinhos = realloc(graph[i].vizinhos, sizeof(node_t*)*(graph[i].nVizinhos + 32));

        graph[i].vizinhos[graph[i].nVizinhos++] = graph[i + offsets[j]];
			}
		}
	}
}

int getNodeIndex(int a, int r){
	/*
	6  3  0

	7  4  1

	8  5  2
	*/
	return (r-1)*numAvenidas + a - 1;
}
