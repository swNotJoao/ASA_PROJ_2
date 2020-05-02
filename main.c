#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getNodeIndex(int a, int r);
void addLinks(int **matriz);
int fordFulkerson(int **matriz, int source, int sink);
int bfs(int **matrizResidual, int uSource, int uSink, int *path);

int numAvenidas, numRuas, numSupermercados, numCidadaos;
int uSource, uSink;

int i, j, k;
int x,y;
int **matriz;

int main(){
	scanf("%d %d", &numAvenidas, &numRuas);
	uSource = numAvenidas*numRuas;
	uSink = numAvenidas*numRuas + 1;

	/* Construir matriz e suas ligacoes */
	matriz = (int **) calloc(numRuas*numAvenidas + 2, sizeof(int*));
	for(i = 0; i < numAvenidas*numRuas + 2; i++){
		matriz[i] = (int *) calloc(numAvenidas*numRuas + 2, sizeof(int));
	}
	addLinks(matriz);

	scanf("%d %d", &numSupermercados, &numCidadaos);

	/* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		scanf("%d %d", &x, &y);
		matriz[uSink][getNodeIndex(x,y)] = 1;
		matriz[getNodeIndex(x, y)][uSink] = 1;
	}

	/* Criar source universal */
	for(i = 0; i < numCidadaos; i++){
                scanf("%d %d", &x, &y);
                matriz[uSource][getNodeIndex(x,y)] = 1;
                matriz[getNodeIndex(x, y)][uSource] = 1;
        }

	printf("%d\n", fordFulkerson(matriz, uSource, uSink));

	return 0;
}

int getNodeIndex(int a, int r){
	/*
	6  3  0

	7  4  1

	8  5  2
	*/
	return (r-1)*3 + a - 1;
}

void addLinks(int **matriz){
	/* Colocar a 1 ligacoes por avenidas e ruas */
	int offsets[] = {-1, -3, 1, 3};
	int i, j;

	for(i = 0; i < numAvenidas * numRuas; i++){
		for(j = 0; j < 4; j++){
			if(i + offsets[j] >= 1 && i + offsets[j] <= numAvenidas * numRuas){
				matriz[i][i + offsets[j]] = 1;
				matriz[i + offsets[j]][i] = 1;
			}
		}
	}
}

int fordFulkerson(int **matriz, int source, int sink){
	int maxFlow = 0;
	int **matrizResidual;
	int path[numAvenidas*numRuas + 2];

	matrizResidual = (int **) calloc(numRuas*numAvenidas + 2, sizeof(int*));
        for(i = 0; i < numAvenidas*numRuas + 2; i++){
                matrizResidual[i] = (int *) calloc(numAvenidas*numRuas + 2, sizeof(int));
        }

	for(i = 0; i < numAvenidas*numRuas + 2; i++)
                for(j = 0; j < numAvenidas*numRuas + 2; j++)
                        matrizResidual[i][j] = matriz[i][j];

	while(bfs(matrizResidual, source, sink, path)){
		int pathFlow = 100000;

		for(i = uSink; i != uSource; i = path[i]){
			k = path[i];
			if(matrizResidual[k][i] < pathFlow){
				pathFlow = matrizResidual[k][i];
			}
		}

		for(i = uSink; i != uSource; i = path[i]){
                        k = path[i];
                        matrizResidual[k][i] = matrizResidual[k][i] - pathFlow;
			matrizResidual[i][k] = matrizResidual[i][k] + pathFlow;
                }

		maxFlow = maxFlow + pathFlow;
	}

	return maxFlow;
}

int bfs(int **matrizResidual, int uSource, int uSink, int *path){
	int visited[numAvenidas*numRuas + 2];
	int queue[numAvenidas*numRuas + 2], qptr = 0;
	memset(visited, 0, sizeof(visited));
	memset(queue, -1, sizeof(queue));

	queue[qptr] = uSource;
	qptr++;
	visited[uSource] = 1;
	path[uSource] = -1;

	while(qptr != 0){
		int k = queue[qptr-1];
		queue[qptr-1] = -1;
		qptr--;

		for(i = 0; i < numAvenidas*numRuas +2; i++){
			if(visited[i] == 0 && matrizResidual[k][i] > 0){
				queue[qptr] = i;
				qptr++;
				path[i] = k;
				visited[i] = 1;
			}
		}
	}

	return visited[uSink];
}
