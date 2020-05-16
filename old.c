#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node{
  unsigned char visited;

  unsigned int nVizinhos;
  struct node **vizinhos;
} node_t;

void buildGraph();
unsigned int getNodeIndex(int a, int r);
int fordFulkerson();
int bfs(node_t *uSource, node_t *uSink, node_t **path);

void allocateNeighbor(node_t *graph, int index);
void checkIfReallocNeeded(node_t in, node_t out, int index);

unsigned int numAvenidas, numRuas, numSupermercados, numCidadaos;
unsigned int graphSize, pathPtr;
node_t *graphIn, *graphOut, uSourceIn, uSourceOut, uSinkIn, uSinkOut;

int main(){
  int i, j, k, x, y, tmpNo;

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
    tmpNo = getNodeIndex(x, y);

    checkIfReallocNeeded(uSinkIn, uSinkOut, tmpNo);

    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++] = &graphOut[tmpNo];
    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++] = &uSinkIn;
    uSinkIn.vizinhos[uSinkIn.nVizinhos++] = &uSinkOut;
    uSinkOut.vizinhos[uSinkOut.nVizinhos++] = &graphIn[tmpNo];
	}

  /* Criar source universal */
  for(i = 0; i < numCidadaos; i++){
		scanf("%d %d", &x, &y);
    tmpNo = getNodeIndex(x, y);

    checkIfReallocNeeded(uSourceIn, uSourceOut, tmpNo);

    uSourceOut.vizinhos[uSourceIn.nVizinhos++] = &uSourceOut;
    uSourceOut.vizinhos[uSourceOut.nVizinhos++] = &graphIn[tmpNo];
    graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++] = &graphOut[tmpNo];
    graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++] = &uSourceIn;
	}

  printf("%d\n", fordFulkerson());

  return 0;
}

void allocateNeighbor(node_t *graph, int index){
    if(graph[index].vizinhos == NULL)
        graph[index].vizinhos = (node_t **) calloc(32, sizeof(node_t *));
}

void checkIfReallocNeeded(node_t in, node_t out, int index) {
    if(in.nVizinhos % 32 == 0)
        in.vizinhos = (node_t **) realloc(in.vizinhos, sizeof(node_t*)*(in.nVizinhos + 32));
    if(out.nVizinhos % 32 == 0)
        out.vizinhos = (node_t **) realloc(out.vizinhos, sizeof(node_t*)*(out.nVizinhos + 32));
    if(graphIn[index].nVizinhos % 32 == 0)
        graphIn[index].vizinhos = (node_t **) realloc(graphIn[index].vizinhos, sizeof(node_t*)*(graphIn[index].nVizinhos + 32));
    if(graphOut[index].nVizinhos % 32 == 0)
        graphOut[index].vizinhos = (node_t **) realloc(graphOut[index].vizinhos, sizeof(node_t*)*(graphOut[index].nVizinhos + 32));
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
          allocateNeighbor(graphIn,tmpNo);
          allocateNeighbor(graphOut,tmpNo);
          allocateNeighbor(graphIn,tmpVizinho);
          allocateNeighbor(graphOut,tmpVizinho);

          /*  Ai -> Ao -> Bi -> Bo --|
            ^____________________________________|
                                                  */
          /*printf("NO: %d; VIXINHO: %d\n", tmpNo, tmpVizinho);*/
          graphIn[tmpNo].vizinhos[graphIn[tmpNo].nVizinhos++] = &graphOut[tmpNo];
          graphOut[tmpNo].vizinhos[graphOut[tmpNo].nVizinhos++] = &graphIn[tmpVizinho];
          graphIn[tmpVizinho].vizinhos[graphIn[tmpVizinho].nVizinhos++] = &graphOut[tmpVizinho];
          graphOut[tmpVizinho].vizinhos[graphOut[tmpVizinho].nVizinhos++] = &graphIn[tmpNo];
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
	node_t *path[graphSize*2+4];
  node_t *node_i;
  pathPtr = 0;

	while(bfs(&uSourceIn, &uSinkOut, path)){
		int pathFlow = 100000;

    printf("1)\n");

		for(node_i = &uSinkOut; node_i != &uSourceIn; node_i = path[pathPtr]){
      pathPtr--;
      printf("%d\n", pathPtr);
      printf("2)\n");
			pathFlow = 1;
		}

		maxFlow = maxFlow + pathFlow;
    pathPtr = 0;
	}

	return maxFlow;
}

int bfs(node_t *uSource, node_t *uSink, node_t **path){
	node_t *queue[graphSize*2+4];
	int qptr = 0, i;
	memset(queue, 0, sizeof(queue));

  for(i = 0; i < graphSize; i++){
    graphIn[i].visited = 0;
    graphOut[i].visited = 0;
  }

  uSourceIn.visited = 0;
  uSourceOut.visited = 0;
  uSinkIn.visited = 0;
  uSinkOut.visited = 0;


  printf("BFS1)\n");

	queue[qptr] = &uSourceIn;
	qptr++;
	uSourceIn.visited = 1;

  printf("BFS2)\n");

	while(qptr != 0){
    printf("BFSW)\n");
		node_t *k = queue[qptr-1];
		queue[qptr-1] = NULL;
		qptr--;

		for(i = 0; i < k -> nVizinhos; i++){
      printf("BFSWF)\n");
			if(k->vizinhos[i]->visited == 0){
        printf("BFSWFI)\n");
				queue[qptr] = k->vizinhos[i];
        printf("BFSWFI2)\n");
				qptr++;
				path[pathPtr] = k->vizinhos[i];
        pathPtr++;
        printf("BFSWFI3)\n");
				k->vizinhos[i]->visited = 1;
        printf("BFSWFI4)\n");
			}
		}
	}

	return uSinkOut.visited;
}
