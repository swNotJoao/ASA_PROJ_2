#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void buildGraph();
unsigned int getNodeIndex(int a, int r);

typedef struct node{
  unsigned char visited;

  unsigned int nVizinhos;
  struct node **vizinhos;
} node_t;

unsigned int numAvenidas, numRuas, numSupermercados, numCidadaos;
unsigned int graphSize;
node_t *graphIn, *graphOut, uSourceIn, uSourceOut, uSinkIn, uSinkOut;

int main(){
  int i, j, k, x, y;

  scanf("%d %d", &numAvenidas, &numRuas);
  graphSize += numAvenidas * numRuas;
  scanf("%d %d", &numSupermercados, &numCidadaos);
  graphSize += numSupermercados + numCidadaos;

  graphIn = (node_t *) calloc(graphSize, sizeof(node_t));
  graphOut = (node_t *) calloc(graphSize, sizeof(node_t));
  buildGraph();

  /* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		scanf("%d %d", &x, &y);

	}

  /*Will need this*/
  /*if(graph[i].nVizinhos % 32 == 0)
    graph[i].vizinhos = realloc(graph[i].vizinhos, sizeof(node_t*)*(graph[i].nVizinhos + 32));*/

  return 0;
}

void buildGraph(){
  int offsets[][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  int i, j, k, size = numAvenidas * numRuas, tmpVizinho, tmpNo;

  if(uSourceIn.vizinhos == NULL)
    uSourceIn.vizinhos = (node_t **) calloc(32, sizeof(node_t *));
  if(uSinkIn.vizinhos == NULL)
    uSinkIn.vizinhos = (node_t **) calloc(32, sizeof(node_t *));
  if(uSourceOut.vizinhos == NULL)
    uSourceOut.vizinhos = (node_t **) calloc(32, sizeof(node_t *));
  if(uSinkOut.vizinhos == NULL)
    uSinkOut.vizinhos = (node_t **) calloc(32, sizeof(node_t *));

	for(i = 1; i <= numAvenidas; i++){
		for(j = 1; j <= numRuas; j++){
			for(k = 0; k < 4; k++){
				tmpNo = getNodeIndex(i, j);
				if(i + offsets[k][0] <= 0 || j + offsets[k][1] <= 0 || i + offsets[k][0] > numAvenidas || j + offsets[k][1] > numRuas)
					continue;
				tmpVizinho = getNodeIndex(i + offsets[k][0], j + offsets[k][1]);
				if(tmpVizinho >= 0 && tmpVizinho <= numAvenidas * numRuas - 1){
          if(graphIn[tmpNo].vizinhos == NULL)
            graphIn[tmpNo].vizinhos = (node_t **) calloc(32, sizeof(node_t *));
          if(graphOut[tmpNo].vizinhos == NULL)
            graphOut[tmpNo].vizinhos = (node_t **) calloc(32, sizeof(node_t *));

          /*  Ai ->(L1) Ao ->(L2) Bi ->(L3) Bo --|
            ^____________________________________|
                              L4                  */
          printf("NO: %d; VIXINHO: %d\n", tmpNo, tmpVizinho);
          graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++] = &graphOut[tmpNo];
          /*FALTA ADICIONAR LIGACOES L2, L3 E L4*
          L2
          L3
          L4
          (Como na linha 74) -> graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++] = &graphOut[tmpNo];
          */


        }
      }
    }
  }
}

unsigned int getNodeIndex(int a, int r){
	/*
	6  3  0

 	7  4  1

	8  5  2
	*/
	return (r-1)*numAvenidas + a - 1;
}
