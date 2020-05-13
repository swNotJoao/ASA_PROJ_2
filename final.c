#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getNodeIndex(int a, int r);
void addLinks(char **matriz);
int fordFulkerson(char **matriz, int source, int sink);
int bfs(char **matriz, int uSource, int uSink, char *path);

int numAvenidas, numRuas, numSupermercados, numCidadaos;
int uSource, uSink;

int i, j, k, ignoreThis;
int x,y;
char **matriz;

int main(){
	ignoreThis = scanf("%d %d", &numAvenidas, &numRuas);
	uSource = numAvenidas*numRuas*2;
	uSink = numAvenidas*numRuas*2 + 1;

	/* Construir matriz e suas ligacoes */
	matriz = (char **) calloc(numRuas*numAvenidas*2 + 4, sizeof(char*));
	for(i = 0; i < numAvenidas*numRuas*2 + 4; i++){
		matriz[i] = (char *) calloc(numAvenidas*numRuas*2 + 4, sizeof(char));
	}
	addLinks(matriz);

	ignoreThis = scanf("%d %d", &numSupermercados, &numCidadaos);

	/* Criar sink universal */
	for(i = 0; i < numSupermercados; i++){
		ignoreThis = scanf("%d %d", &x, &y);

		matriz[uSink][uSink+2]++;
		/*matriz[uSink+2][uSink]++;*/

		matriz[uSink+2][getNodeIndex(x,y)] = 1;
		/*matriz[getNodeIndex(x,y)][uSink+2] = 1;*/

		matriz[getNodeIndex(x,y)][getNodeIndex(x,y)+numRuas*numAvenidas] = 1;
		/*matriz[getNodeIndex(x,y) +numRuas*numAvenidas][getNodeIndex(x,y)] = 1;*/

		matriz[getNodeIndex(x,y) +numRuas*numAvenidas][uSink] = 1;
		/*matriz[uSink][getNodeIndex(x,y) +numRuas*numAvenidas] = 1;*/
	}

	/* Criar source universal */
	for(i = 0; i < numCidadaos; i++){
    	ignoreThis = scanf("%d %d", &x, &y);

			matriz[uSource][uSource+2]++;
			/*matriz[uSource+2][uSource]++;*/

			matriz[uSource+2][getNodeIndex(x,y)] = 1;
			/*matriz[getNodeIndex(x,y)][uSource+2] = 1;*/

			matriz[getNodeIndex(x,y)][getNodeIndex(x,y)+numRuas*numAvenidas] = 1;
			/*matriz[getNodeIndex(x,y) +numRuas*numAvenidas][getNodeIndex(x,y)] = 1;*/

			matriz[getNodeIndex(x,y) +numRuas*numAvenidas][uSource] = 1;
			/*matriz[uSource][getNodeIndex(x,y) +numRuas*numAvenidas] = 1;*/
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
	return (r-1)*numAvenidas + a - 1;
}

void addLinks(char **matriz){
	/* Colocar a 1 ligacoes por avenidas e ruas */
	/*int offsets[] = {-1, numAvenidas, 1, -numAvenidas};*/
	int offsets[][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
	int i, j, size = numAvenidas * numRuas, tmpVizinho, tmpNo;

	for(i = 1; i <= numAvenidas; i++){
		for(j = 1; j <= numRuas; j++){
			for(k = 0; k < 4; k++){
				tmpNo = getNodeIndex(i, j);
				if(i + offsets[k][0] <= 0 || j + offsets[k][1] <= 0 || i + offsets[k][0] > numAvenidas || j + offsets[k][1] > numRuas)
					continue;
				tmpVizinho = getNodeIndex(i + offsets[k][0], j + offsets[k][1]);
				if(tmpVizinho >= 0 && tmpVizinho <= numAvenidas * numRuas - 1){
					/*printf("i:%d j:%d; iv:%d jv:%d\n", i, j, offsets[k][0], offsets[k][1]);*/
					/*printf("NO: %d; Vixinho %d\n", tmpNo, tmpVizinho);*/

					matriz[tmpNo][tmpNo + size] = 1;
					/*matriz[tmpNo+size][i] = 1;*/

					matriz[tmpNo+size][tmpVizinho] = 1;
					/*matriz[tmpVizinho][i+size] = 1;*/

					matriz[tmpVizinho][tmpVizinho+size] = 1;
					/*matriz[tmpVizinho+size][tmpVizinho] = 1;*/

					matriz[tmpVizinho+size][tmpNo] = 1;
					/*matriz[i][i+offsets[j]+size] = 1;*/
				}
			}
		}
	}
}

int fordFulkerson(char **matriz, int source, int sink){
	int maxFlow = 0;
	char path[numAvenidas*numRuas*2 + 4];

	while(bfs(matriz, source, sink, path)){
		int pathFlow = 100000;

		for(i = uSink; i != uSource; i = path[i]){
			k = path[i];
			if(matriz[k][i] < pathFlow){
				pathFlow = matriz[k][i];
			}
		}

		for(i = uSink; i != uSource; i = path[i]){
      k = path[i];
      matriz[k][i] = matriz[k][i] - pathFlow;
			matriz[i][k] = matriz[i][k] + pathFlow;
    }

		maxFlow = maxFlow + pathFlow;
	}

	return maxFlow;
}

int bfs(char **matriz, int uSource, int uSink, char *path){
	char visited[numAvenidas*numRuas*2 + 4];
	char queue[numAvenidas*numRuas*2 + 4];
	int qptr = 0;
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

		for(i = 0; i < numAvenidas*numRuas*2 + 4; i++){
			if(visited[i] == 0 && matriz[k][i] > 0){
				queue[qptr] = i;
				qptr++;
				path[i] = k;
				visited[i] = 1;
			}
		}
	}

	return visited[uSink];
}
