
/*
   Implemente o GRASP na função solve e a busca local na função localSearch.

   A função solve recebe uma instância do STP cujos dados são armazenados na estrutura de dados
   definida pelo tipo STPInstance (veja o arquivo stp.h).
   O atribuito n desta estrutura corresponde à quantidade de vértice e o atributo t à quantidade de terminais.
   O custo da aresta entre os vértices i e j é armazenado na linha i e coluna j da matriz instance->costs,
   ou seja, em instance->costs[i][j] onde 0 <= i, j <= n-1.
   Já o vetor terminals contém t posições e armazena os índices dos vértices terminais.
   ***** ATENÇÃO !!! Neste laboratório foi acrescentado na estrutura da instância o vetor isTerminal.
   ***** A i-ésima posição deste vetor possui valor 1 se o vértice i é um terminal e 0 caso contrário.
   ***** O propósito deste vetor é facilitar a sua implementação.
   
   A função solve também recebe uma estrutura de dados EdgeList
   que deve armazenar a lista das arestas da melhor solução encontrada.
   Esta estrutura já foi inicializada com uma lista de arestas vazia no arquivo main.c
   através da chamada da função createEmptyEdgeList().
   Para adicionar uma aresta basta executar a função addEdgeToList passando
   como parâmetro a lista (solution) e os dois vértices da aresta que será adicionada.
   Exemplo de inserção da aresta (0,1) em solution: addEdgeToList(solution, 0, 1).
   
   O parâmetro executeLocalSearch da função solve determina se a busca local será executada pelo GRASP.
   Caso o valor deste parâmetro seja 1, a busca local deverá sempre ser executada.
   E se o valor for diferente de 1, a busca nunca deverá ser executada.
      
   O valor retornado pela função solve deve ser o custo da solução obtida.
   
   A função localSearch recebe parâmetros análogos aos da função solve.
   A diferença é que esta função não possui o parâmetro executeLocalSearch e o custo da solução
   armazenada em solution deve ser informado atráves do parâmetro solutionCost.
   
   No final da execução desta função, a melhor solução obtida deve estar armazenada em solution
   e o seu custo deve ser retornado pela função.
   
   A fim de facilitar a sua implementação fornecemos as seguintes funções prontas:
   
   - EdgeList* createEmptyEdgeList()
     (Cria uma lista vazia.)
   
   - void addEdgeToList(EdgeList *list, int v1, int v2)
     (Insere a aresta (v1, v2) na lista list.)
     
   - EdgeList* duplicateEdgeList(EdgeList *list)
     (Cria e retorna uma nova lista contendo uma cópia das arestas que estão em list.)
   
   - void copyEdgeListRemovingAndAdding(EdgeList *destinationList, EdgeList *sourceList, EdgeList *removeList, EdgeList *addList)
     (Recebe uma lista destinationList e inclui nela as arestas que estão em addList
      e as arestas que estão em sourceList e não estão em removeList.)

   - void swapEdgeLists(EdgeList *list, EdgeList *list2)
     (Faz com que a list receba as arestas de list2 e vice-versa.)

   - void clearEdgeList(EdgeList *list)
     (Remove todas as arestas de list.)
   
   - double shortestPath(const STPInstance *instance, int *flag, EdgeList *path, int skip)
     (Encontra um caminho de custo mínimo entre os vértices origem e destino.
      A i-ésima posição do vetor flag deve receber o valor 0 se o vértice i é uma origem,
      o valor 1 se i é um destino e demais valores se i não é uma origem e nem um destino.
      O caminho de custo mínimo encontrado será armazenado na lista path.
      *** A lista path informada neste parâmetro deve já estar alocada e vazia!
      *** O parâmetro skip é utilizado pelo algoritmo de construção randômico.
      *** Na busca local você deve passar o valor 0 neste parâmetro.
      O retorno desta função é o custo do caminho encontrado.)
   
   - double constructSolution(const STPInstance *instance, EdgeList *solution, int random)
     (Constrói uma solução para a instância.
      A solução encontrada é colocada em solution.
      *** A lista solution informada neste parâmetro deve já estar alocada e vazia!
      *** O parâmetro random define se a construção será aleatória ou determinística.
      *** No GRASP você deve passar o valor 1 neste parâmetro.
      O retorno desta função é o custo da solução obtida.)

   ***** ATENÇÃO:  Não altere nenhuma destas funções pois algumas delas estão sendo utilizadas no programa principal!
   ***** ATENÇÃO2: Também não altere os dados da instância pois a mesma é utilizada no programa principal!
*/

#include "stp.h"

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

EdgeList* duplicateEdgeList(EdgeList *list)
{
	EdgeList *newList = createEmptyEdgeList();
	EdgeListNode *listEdges = list->edges;
	
	while (listEdges != NULL)
	{
		EdgeListNode *newEdge = (EdgeListNode*)malloc(sizeof(EdgeListNode));
		newEdge->v1 = listEdges->v1;
		newEdge->v2 = listEdges->v2;
		newEdge->next = newList->edges;
		newList->edges = newEdge;
		
		listEdges = listEdges->next;
	}
	
	newList->n = list->n;
	
	return newList;
}

void copyEdgeListRemovingAndAdding(EdgeList *destinationList, EdgeList *sourceList, EdgeList *removeList, EdgeList *addList)
{
	EdgeListNode *sourceListEdges = sourceList->edges;

	while (sourceListEdges != NULL)
	{
		EdgeListNode *removeListEdges = removeList->edges;
		
		while (removeListEdges != NULL)
		{
			if ( ((removeListEdges->v1 == sourceListEdges->v1) && (removeListEdges->v2 == sourceListEdges->v2)) ||
			     ((removeListEdges->v1 == sourceListEdges->v2) && (removeListEdges->v2 == sourceListEdges->v1)) )
			     break;
		
			removeListEdges = removeListEdges->next;
		}
		
		if (removeListEdges == NULL)
		{
			EdgeListNode *newEdge = (EdgeListNode*)malloc(sizeof(EdgeListNode));
			newEdge->v1 = sourceListEdges->v1;
			newEdge->v2 = sourceListEdges->v2;
			newEdge->next = destinationList->edges;
			destinationList->edges = newEdge;
			destinationList->n++;
		}
				
		sourceListEdges = sourceListEdges->next;
	}
	
	EdgeListNode *addListEdges = addList->edges;

	while (addListEdges != NULL)
	{
		EdgeListNode *newEdge = (EdgeListNode*)malloc(sizeof(EdgeListNode));
		newEdge->v1 = addListEdges->v1;
		newEdge->v2 = addListEdges->v2;
		newEdge->next = destinationList->edges;
		destinationList->edges = newEdge;
		
		addListEdges = addListEdges->next;
	}
	
	destinationList->n += addList->n;
}

void swapEdgeLists(EdgeList *list, EdgeList *list2)
{
	EdgeListNode *tmp = list->edges;	
	list->edges = list2->edges;
	list2->edges = tmp;

	int n = list->n;
	list->n = list2->n;
	list2->n = n;
}

void clearEdgeList(EdgeList *list)
{
	list->n = 0;
	
	while (list->edges != NULL)
	{
		EdgeListNode *tmp = list->edges;
		list->edges = list->edges->next;
		free(tmp);
	}
}

double shortestPath(const STPInstance *instance, int *flag, EdgeList *path, int skip)
{
	double *dist = (double*)malloc(instance->n*sizeof(double));
	int *pred = (int*)malloc(instance->n*sizeof(int));
	int *active = (int*)malloc(instance->n*sizeof(int));
	int *queue = (int*)malloc(instance->n*sizeof(int));
	int queueSize = instance->n;
	int i, min, minPos;
	
	for (i = 0; i < instance->n; i++)
	{
		dist[i] = ((flag[i] != 0) ? DBL_MAX : 0);
		pred[i] = -1;
		active[i] = 1;
		queue[i] = i;
	}
	
	while (1)
	{
		minPos = 0;
		
		for (i = 1; i < queueSize; i++)
			if (dist[queue[minPos]] > dist[queue[i]])
				minPos = i;
				
		min = queue[minPos];
		queue[minPos] = queue[--queueSize];
		active[min] = 0;
		
		if ((flag[min] == 1) && ((skip--) == 0))
			break;
			
		for (i = 0; i < instance->n; i++)
			if ((active[i]) && (dist[i] > dist[min] + instance->costs[i][min]))
			{
				dist[i] = dist[min] + instance->costs[i][min];
				pred[i] = min;
			}
	}
	
	double cost = dist[min];

	while (pred[min] != -1)
	{
		addEdgeToList(path, min, pred[min]);
		min = pred[min];
	}	
	
	free(dist);
	free(pred);
	free(queue);
	free(active);
	
	return cost;
}

double constructSolution(const STPInstance *instance, EdgeList *solution, int random)
{
	double solutionCost = 0;
	int i, skip;

	int *flag = (int*)malloc(instance->n*sizeof(int));
	int t = instance->t-1;
	
	for (i = 0; i < instance->n; i++)
		flag[i] = -1;
	
	for (i = 1; i < instance->t; i++)
		flag[ instance->terminals[i] ] = 1;
	
	flag[ instance->terminals[0] ] = 0;
	
	EdgeList *path = createEmptyEdgeList();
	
	while (t > 0)
	{
		if (random)
		{
			skip = rand() % 5;
			if (skip > t-1) skip = t-1;
		}
		else
			skip = 0;
			
		solutionCost += shortestPath(instance, flag, path, skip);
		
		while (path->edges != NULL)
		{
			addEdgeToList(solution, path->edges->v1, path->edges->v2);
						
			if (flag[path->edges->v1] == 1) t--;
			if (flag[path->edges->v2] == 1) t--;

			flag[path->edges->v1] = 0;
			flag[path->edges->v2] = 0;
			
			EdgeListNode *tmp = path->edges; 
			path->edges = path->edges->next;
			free(tmp);
			path->n--;
		}
	}
	
	free(flag);
	free(path);
		
	return solutionCost;
}

typedef struct IntListNode
{
   int i;
   struct IntListNode *next;
} IntListNode;

double localSearch(const STPInstance *instance, EdgeList *solution, double solutionCost)
{

	// Implemente aqui a busca local.
	
	// Ao utilizar a função shortestPath aqui, lembre-se que o parâmetro skip deve ser 0.
	
	return solutionCost;
}

double solve(const STPInstance *instance, EdgeList *solution, int executeLocalSearch)
{
	double solutionCost = 0;
	
	// Para cada iteração
	
		// Construa uma solução de forma aleatória
		// (utilize a função pronta constructSolution passando o parâmetro random igual a 1).
		// Não se esqueça que a lista (solution) informada para esta função deve estar alocada e vazia!
		
		// Se o parâmetro executeLocalSearch for igual a 1,
		// execute a busca local na solução encontrada no passo anterior.
		
		// Verifique e atualize, caso necessário, se a nova solução obtida é
		// melhor que a melhor solução encontrada até o momento.
		// Dica: utilize a função swapEdgeList para colocar em solution
		// as arestas da nova melhor solução encontrada.
		
	return solutionCost;
}


