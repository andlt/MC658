
/*
   Implemente o algoritmo aproximativo, descrito no enunciado, na função solve.

   Esta função recebe uma instância do STP cujos dados são armazenados na estrutura de dados
   definida pelo tipo STPInstance (veja o arquivo stp.h).
   O atribuito n desta estrutura corresponde à quantidade de vértice e o atributo t à quantidade de terminais.
   O custo da aresta entre os vértices i e j é armazenado na linha i e coluna j da matriz instance->costs,
   ou seja, em instance->costs[i][j] onde 0 <= i, j <= n-1.
   Já o vetor terminals contém t posições e armazena os índices dos vértices terminais.
   
   Além da instância, esta função também recebe uma estrutura de dados EdgeList
   que deve armazenar a lista das arestas da solução encontrada.
   Esta estrutura já foi inicializada com uma lista de arestas vazia no arquivo main.c
   através da chamada da função createEmptyEdgeList().
   Para adicionar uma aresta basta executar a função addEdgeToList passando
   como parâmetro a lista (solution) e os dois vértices da aresta que será adicionada.
   Exemplo de inserção da aresta (0,1) em solution: addEdgeToList(solution, 0, 1).
      
   O valor retornado pela função deve ser o custo da solução obtida.
  
   ATENÇÃO: Não altere os dados da instância pois a mesma é utilizada no programa principal!
*/

#include "stp.h"
#include <vector>

#define INF 99999

typedef struct
{
	int v1;
	int v2;
	double w;
} edge;	

EdgeList* createEmptyEdgeList()
{
	EdgeList *list = (EdgeList*)malloc(sizeof(EdgeList));
	list->edges = NULL;
	list->n = 0;
	return list;
}

void addEdgeToList(EdgeList *list, int v1, int v2)
{
	EdgeListNode *newEdge = (EdgeListNode*)malloc(sizeof(EdgeListNode));
	newEdge->next = list->edges;
	newEdge->v1 = v1;
	newEdge->v2 = v2;
	list->edges = newEdge;
	list->n++;
}

void floydWarshall (const STPInstance *instance, double * minPath, int * predecessor)
{
/* Guarda no apontador minPath um vetor com n^2 posições, onde minPath[i*n+j] é a distância do caminho mínimo entre os vértices i e j vértices do grafo recebido em STPInstance. Guarda em predecessor o primeiro elemento do caminho mínimo entre i e j. */

	int i = 0;
	int j = 0;
	int k = 0;
	int n = instance->n;

	minPath = new double[n*n];
	predecessor = new int[n*n];

	/* Liberar os apontadores se estiverem sendo usados */

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			minPath[i*n+j] = instance->costs[i][j];
			predecessor[i*n+j] = j;
		}
	}

	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (minPath[i*n+j] > minPath[i*n+k] + minPath[k*n+j])
				{
					minPath[i*n+j] = minPath[i*n+k] + minPath[k*n+j];
					predecessor[i*n+j] = predecessor[i*n+k];
				}
			}
		}
	}
}

/*void vertexQueueInit(vector<vertex> vertexQueue, double *dist)
{
	
}*/

double extractMinEdgeQueue (const STPInstance *instance, int *queueV1, int *queueV2, double *queueVal, bool *queueOnOff, int queueSize, int *edgeV1, int *edgeV2)
{
/* */

	int i = 0;
	int minIndex = -1;
	double minVal = INF;

	for (i = 0; i < queueSize; i++)
	{
		if (queueOnOff[i] == true)
		{
			if (queueVal[i] < minVal)
			{
				minIndex = i;
				minVal = queueVal[i];
			}
		}
	}

	if (minIndex != -1)
	{
		*edgeV1 = queueV1[minIndex];
		*edgeV2 = queueV2[minIndex];	
		queueOnOff[minIndex] = false;
	}
	else
	{
		edgeV1 = NULL;
		edgeV2 = NULL;
	}
	return minVal;

};

void getNextVertex (const STPInstance *instance, bool *vertexInTree, int queueSize, int *father, int *son)//, int *vertex1, int *vertex2)
{
/* */

	int i = 0;
	int j = 0;
	//int minIndex = -1;
	int minCost = INF;
	//int nextVertex = -1;
	int currentFather = -1;
	int currentSon = -1;

	father = NULL;
	son = NULL;

	for (i = 0; i < queueSize; i++)
	{
		if (vertexInTree[i] == true)
		{
			for (j = 0; j < queueSize; j++)
			{
				if (vertexInTree[j] == false)
				{
					if (instance->costs[i][j] < minCost)
					{
						currentFather = instance->terminals[i];
						currentSon = j;
						minCost = instance->costs[i][j];
					}
				}
			}
		}
	}

	*father = currentFather;
	*son = currentSon;
};

int * prim (const STPInstance *instance, double *dist)
{
/* */	

	int i = 0;
	//int j = 0;
	//int k = 0;
	int n = instance->n;
	int t = instance->t;
	int * fathers = NULL;
	//int nextVertex = -1; //int * edgeV1 = NULL;
	//int * edgeV2 = NULL;
	int father = -1;//int * queueV1 = NULL;
	int son = -1;//int * queueV2 = NULL;
	//int * queueVal = NULL; //double * queueVal = NULL;
	bool * vertexInTree = NULL;

	fathers = new int[n];
	//queueV1 = new int[t*t];
	//queueV2 = new int[t*t];
	//queueVal = new int[t*t]; //queueVal = new double[t*t];
	vertexInTree = new bool[t];//*t];

	for (i = 0; i < n; i++)
	{
		fathers[i] = -1;
	}

	/* Inicializa a fila de prioridade */
	//for (i = 0; i < t; i++)
	//{
		/*for (j = 0; j < t; j++)
		{
			queueV1[i*t+j] = i;
			queueV2[i*t+j] = j;
			queueVal[i*t+j] = instance->costs[i][j];
			queueOnOff[i*t+j] = true;
		}*/
		//queueVal[i*t+j] = i;
	//}

	

	getNextVertex(instance, vertexInTree, t, &father, &son);//extractMinEdgeQueue(instance, queueV1, queueV2, queueVal, queueOnOff, t*t, edgeV1, edgeV2);
	while (father != -1 && son != -1) //while (edgeV1 != NULL && edgeV2 != NULL)
	{
		fathers[son] = father;
		vertexInTree[son] = true;
		//extractMin(instance, queueV1, queueV2, queueVal, queueOnOff, t*t, edgeV1, edgeV2);
	}

	return fathers;
}

double solve (const STPInstance *instance, EdgeList *solution)
{
	double solutionCost = 0;
	double * minPath = NULL; // minPath[i*n+j] = valor do caminho mínimo entre os vértices i e j
	int * predecessor = NULL; // predecessor[i*n+j] = próximo vértice no caminho mínimo entre i e j
	int * fathers = NULL;

	fathers = new int[instance->n];

	/* Algoritmo de Floyd-Warshal */

	floydWarshall(instance, minPath, predecessor);
		
	/* Algoritmo de Prim */

	fathers = prim(instance);

	/* Algoritmo de Prim */

	return solutionCost;
}


