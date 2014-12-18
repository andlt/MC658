
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
#include <stdio.h>

#define INF 99999
#define ROOT_CODE -9 // código para raízes da terminalMST. Deve ser <= -2

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
/* Guarda no apontador minPath um vetor com n^2 posições, onde minPath[i*n+j] é a distância do caminho mínimo entre os vértices i e j vértices do grafo recebido em STPInstance, e predecessor[i*n+j] é o primeiro elemento do caminho mínimo entre i e j. */

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

void terminalToVertex (const STPInstance *instance, int *index)
{
/* Traduz o índice index de terminal (1 a t) para vértice (1 a n) */

	*index = instance->terminals[*index];
}

void getNextFatherSon (const STPInstance *instance, bool *terminalInTree, int *father, int *son)
{
/* Encontra o par father (terminal presente na terminalMST) e son (terminal ausente) com menor custo mínimo */
	
	int i = 0;
	int j = 0;
	int vertexI = -1;
	int vertexJ = -1;
	const int t = instance->t;
	int currentFather = -1;
	int currentSon = -1;
	double minCost = INF;

	/* para cada par de terminais com um na MST e outro fora */
	for (i = 0; i < t; i++)
	{
		if (terminalInTree[i] == true)
		{
			for (j = 0; j < t; j++)
			{
				if (terminalInTree[j] == false)
				{
					/* converte os índices de terminal para vértice */
					vertexI = instance->terminals[i];
					vertexJ = instance->terminals[j];
					/* se o caminho mínimo entre eles for o melhor até agora */
					if (instance->costs[vertexI][vertexJ] < minCost)
					{
						/* guarda o par */
						currentFather = vertexI;
						currentSon = vertexJ;
					}
				}
			}
		}
	}

	/* seta father e son para o melhor par encontrado */
	*father = currentFather;
	*son = currentSon;
}

void addNextTerminalMST (const STPInstance *instance, int *terminalMST, int *terminalMSTroot, bool *terminalInTree)
{
/* Adiciona na Árvore Geradora Mínima de terminais o terminal fora dela com menor caminho mínimo para um terminal que já está na MST */

	int father = -1; // índice (de 1 a n) do terminal na MST com menor caminho mínimo para um fora dela
	int son = -1; // índice (de 1 a n) do terminal fora da MST com menor caminho mínimo para um nela

	/* Encontra a próxima aresta a ser adicionada à MST (par father e son) */

	getNextFatherSon(instance, terminalInTree, &father, &son);

	/* Traduz o índice de father e son de terminais (1 a t) para vértices (1 a n) */
	//terminalToVertex(instance, &father);
	//terminalToVertex(instance, &son);

	terminalMST[son] = father;
}

void setTerminalMST (const STPInstance *instance, int *terminalMST, int *terminalMSTroot)
{
/* Obtem a Árvore Geradora Mínima do grafo formado pelos terminais, abstraindo os vértices entre eles (consideramos o o peso da aresta entre eles como o peso do caminho mínimo entre eles*/

	int i = 0;
	const int n = instance->n;
	const int t = instance->t;
	int usedTerminals = 0; // número de terminais adicionados à MST
	bool *terminalInTree = new bool[t]; // terminalInTree[i] = true se o terminal [i] está na árvore
	
	for (i = 0; i < t; i++)
	{
		terminalInTree[i] = false; /* Nenhum terminal está na MST */
	}

	terminalMST = new int[n]; // terminalMST[i] = -9 se i é a raíz da MST, -1 se i não é terminal e x se x é o terminal pai de i na MST 
	for (i = 0; i < n; i++)
	{
		terminalMST[i] = -1; /* Nenhum vértice possui pais na MST */
	}

	/* Criamos a raíz e colocamos na MST */
	*terminalMSTroot = instance->terminals[0]; // tomamos o primeiro terminal como raíz
	terminalInTree[0] = true;
	terminalMST[*terminalMSTroot] = ROOT_CODE;
	usedTerminals++;

	/* Enquanto todos os terminais não estiverem na raíz, adiciona o terminal mais próximo de um elemento da raíz a ela */
	while (usedTerminals < instance->t)
	{	
		addNextTerminalMST(instance, terminalMST, terminalMSTroot, terminalInTree);
		usedTerminals++;
	}
}

void fillSolutionWithPath (const STPInstance *instance, EdgeList *solution, double *minPath, int *predecessor, int *terminalMST, int father, int son)
{
/* Adiciona os vértices que compõem o caminho mínimo de father até son na solução */

	int vertex = -1;
	const int n = instance->n;

	for (vertex = son; vertex != father; vertex = predecessor[vertex*n+father])
	{
		//
	}
}

void fillSolutionWithPaths (const STPInstance *instance, EdgeList *solution, double *minPath, int *predecessor, int *terminalMST)
{
/* Adiciona os vértices que compõem o caminho mínimo de father até son na solução */

	int i = 0;
	const int n = instance->n;

	/* Para cada vértice da MST */
	for (i = 0; i < n; i++)
	{
		/* Se é um terminal e não é a raíz */
		if (terminalMST[i] != -1 &&terminalMST[i] != ROOT_CODE)
		{
			/* Adiciona as arestas que compõem aquele caminho à solução */
			fillSolutionWithPath (instance, solution, minPath, predecessor, terminalMST, i, terminalMST[i]);
		}
	}
}

double solve (const STPInstance *instance, EdgeList *solution)
{
	double solutionCost = 0;
	double * minPath = NULL; // minPath[i*n+j] = valor do caminho mínimo entre os vértices i e j
	int * predecessor = NULL; // predecessor[i*n+j] = próximo vértice no caminho mínimo entre i e j
	int * terminalMST = NULL; // terminalMST[i] = ROOT_CODE se i é a raíz da MST, -1 se i não é terminal e x se x é o terminal pai de i na MST 
	int terminalMSTroot = -1;

	/* Calcula o caminho mínimo entre cada par de vértices */

	floydWarshall(instance, minPath, predecessor);
		
	/* Obtem a Árvore Geradora Mínima do grafo formado pelos terminais, abstraindo os vértices entre eles (consideramos o o peso da aresta entre eles como o peso do caminho mínimo entre eles*/

	setTerminalMST(instance, terminalMST, &terminalMSTroot);

	/* Para cada vértice em terminalMST, adiciona o caminho correspondente à solução */

	fillSolutionWithPaths(instance, solution, minPath, predecessor, terminalMST);

	return solutionCost;
}


