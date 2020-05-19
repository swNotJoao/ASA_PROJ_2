#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node;
struct vizinho;

typedef struct vizinho{
  unsigned int flow;
  unsigned int rflow;
  struct node *dest;

}vizinho_t;

typedef struct node{
  unsigned int visited;
  int id;

  unsigned int nVizinhos;
  struct vizinho *vizinhos;
}node_t;

void buildGraph();
unsigned int getNodeIndex(int a, int r);
int fordFulkerson();
int dfs(node_t *node);
void dfs_visit(node_t *node);

void allocateNeighbor(node_t *graph, int index);
void checkIfReallocNeeded(node_t in, node_t out, int index);

int numAvenidas, numRuas, numSupermercados, numCidadaos;
int graphSize;
int ignoreThis;
node_t *graphIn, *graphOut, uSourceIn, uSourceOut, uSinkIn, uSinkOut;
vizinho_t **path;
int pathPtr = 0;
int nVisisited = 0;

int main(){
  int i,x, y, tmpNo;

  ignoreThis = scanf("%d %d", &numAvenidas, &numRuas);
  graphSize += numAvenidas * numRuas;
  ignoreThis = scanf("%d %d", &numSupermercados, &numCidadaos);
  graphSize += numSupermercados + numCidadaos;

  graphIn = (node_t *) calloc(graphSize, sizeof(node_t));
  graphOut = (node_t *) calloc(graphSize, sizeof(node_t));
  buildGraph();

  /* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		ignoreThis = scanf("%d %d", &x, &y);
    tmpNo = getNodeIndex(x, y);

    checkIfReallocNeeded(uSinkIn, uSinkOut, tmpNo);

    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos].dest = &graphOut[tmpNo];
    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++].flow = 1;
    graphIn[tmpNo].id = tmpNo;

    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos].dest = &uSinkIn;
    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++].flow = 1;
    graphOut[tmpNo].id = -tmpNo;

    uSinkIn.vizinhos[uSinkIn.nVizinhos].dest = &uSinkOut;
    uSinkIn.vizinhos[uSinkIn.nVizinhos++].flow++;
    uSinkIn.id = -99;

    uSinkOut.vizinhos[uSinkOut.nVizinhos].dest = &graphIn[tmpNo];
    uSinkOut.vizinhos[uSinkOut.nVizinhos++].flow = 1;
    uSinkOut.id = -98;
	}

  /* Criar source universal */
  for(i = 0; i < numCidadaos; i++){
		ignoreThis = scanf("%d %d", &x, &y);
    tmpNo = getNodeIndex(x, y);

    checkIfReallocNeeded(uSourceIn, uSourceOut, tmpNo);

    uSourceIn.vizinhos[uSourceIn.nVizinhos].dest = &uSourceOut;
    uSourceIn.vizinhos[uSourceIn.nVizinhos++].flow++;
    uSourceIn.id = -89;

    uSourceOut.vizinhos[uSourceOut.nVizinhos].dest = &graphIn[tmpNo];
    uSourceOut.vizinhos[uSourceOut.nVizinhos++].flow = 1;
    uSourceOut.id = -88;

    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos].dest = &graphOut[tmpNo];
    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++].flow = 1;
    graphIn[tmpNo].id = tmpNo;

    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos].dest = &uSourceIn;
    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++].flow = 1;
    graphOut[tmpNo].id = -tmpNo;
	}

  printf("%d\n", fordFulkerson());

  return 0;
}

void allocateNeighbor(node_t *graph, int index){
    if(graph[index].vizinhos == NULL)
        graph[index].vizinhos = (vizinho_t *) calloc(32, sizeof(vizinho_t));
}

void checkIfReallocNeeded(node_t in, node_t out, int index) {
    if(in.nVizinhos % 32 == 0)
        in.vizinhos = (vizinho_t *) realloc(in.vizinhos, sizeof(vizinho_t)*(in.nVizinhos + 32));
    if(out.nVizinhos % 32 == 0)
        out.vizinhos = (vizinho_t *) realloc(out.vizinhos, sizeof(vizinho_t)*(out.nVizinhos + 32));
    if(graphIn[index].nVizinhos % 32 == 0)
        graphIn[index].vizinhos = (vizinho_t *) realloc(graphIn[index].vizinhos, sizeof(vizinho_t)*(graphIn[index].nVizinhos + 32));
    if(graphOut[index].nVizinhos % 32 == 0)
        graphOut[index].vizinhos = (vizinho_t *) realloc(graphOut[index].vizinhos, sizeof(vizinho_t)*(graphOut[index].nVizinhos + 32));
}

void buildGraph(){
  int offsets[][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  int i, j, k, tmpVizinho, tmpNo;

  if(uSourceIn.vizinhos == NULL)
    uSourceIn.vizinhos = (vizinho_t *) calloc(32, sizeof(vizinho_t));
  if(uSinkIn.vizinhos == NULL)
    uSinkIn.vizinhos = (vizinho_t *) calloc(32, sizeof(vizinho_t));
  if(uSourceOut.vizinhos == NULL)
    uSourceOut.vizinhos = (vizinho_t *) calloc(32, sizeof(vizinho_t));
  if(uSinkOut.vizinhos == NULL)
    uSinkOut.vizinhos = (vizinho_t *) calloc(32, sizeof(vizinho_t));

	for(i = 1; i <= numAvenidas; i++){
		for(j = 1; j <= numRuas; j++){
			for(k = 0; k < 4; k++){
				tmpNo = getNodeIndex(i, j);

				if(i + offsets[k][0] <= 0 || j + offsets[k][1] <= 0 || i + offsets[k][0] > numAvenidas || j + offsets[k][1] > numRuas)
					continue;

				tmpVizinho = getNodeIndex(i + offsets[k][0], j + offsets[k][1]);
				if(tmpVizinho >= 0 && tmpVizinho <= numAvenidas * numRuas - 1){
          allocateNeighbor(graphIn,tmpNo);
          allocateNeighbor(graphOut,tmpNo);
          allocateNeighbor(graphIn,tmpVizinho);
          allocateNeighbor(graphOut,tmpVizinho);

          /*  Ai -> Ao -> Bi -> Bo --|
            ^________________________|
                                                  */
          /*printf("NO: %d; VIXINHO: %d\n", tmpNo, tmpVizinho);*/
          graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos].dest = &graphOut[tmpNo];
          graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++].flow = 1;
          graphIn[tmpNo].id = tmpNo;


          graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos].dest = &graphIn[tmpVizinho];
          graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++].flow = 1;
          graphOut[tmpNo].id = -tmpNo;

          graphIn[tmpVizinho].vizinhos[graphIn[tmpVizinho].nVizinhos].dest = &graphOut[tmpVizinho];
          graphIn[tmpVizinho].vizinhos[graphIn[tmpVizinho].nVizinhos++].flow = 1;
          graphIn[tmpVizinho].id = tmpVizinho;

          graphOut[tmpVizinho].vizinhos[graphOut[tmpVizinho].nVizinhos].dest = &graphIn[tmpNo];
          graphOut[tmpVizinho].vizinhos[graphOut[tmpVizinho].nVizinhos++].flow = 1;
          graphOut[tmpVizinho].id = -tmpVizinho;
          /*gcc -O3 -ansi -Wall $file -lm*/


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

int fordFulkerson(){
	int maxFlow = 0;
  path = (vizinho_t **) calloc(graphSize*2, sizeof(vizinho_t*));

  /*printf("FORD FULKERSON\n");*/

	while(dfs(&uSourceIn)){
    /*printf("\n");*/
		maxFlow++;
	}

	return maxFlow;
}

int dfs(node_t *node){
  int i;
  for(i = 0; i < graphSize; i++){
    graphIn[i].visited = 0;
    graphOut[i].visited = 0;
  }

  uSourceIn.visited = 0;
  uSourceOut.visited = 0;
  uSinkIn.visited = 0;
  uSinkOut.visited = 0;

  nVisisited = 0;
  pathPtr = 0;
  dfs_visit(node);

  for(i = 0; i<pathPtr; i++){
    /*printf("%d\n", path[i]->dest->id);*/
    path[i]->flow--;
  }

  if(uSinkOut.visited)
    return 1;
  return 0;
}

void dfs_visit(node_t *node){
  int i;
  /*int flag = 0;*/
  node->visited = 1;

  for(i = 0; i < node->nVizinhos; i++){
    /*printf("NO: %d | VIZINHO: %d\n", node->id, node->vizinhos[i].dest->id);*/
    if(node->vizinhos[i].dest->visited == 0 && node->vizinhos[i].flow > 0){
      path[pathPtr] = &(node->vizinhos[i]);
      pathPtr++;
      dfs_visit(node->vizinhos[i].dest);
      /*flag = 1;*/
      /*printf("%d|", node->vizinhos[i].dest->id);*/
      }
    }
}
