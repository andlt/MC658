
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
#include <list>

#define MAX_ITERATIONS 500

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

void removeEdgeFromList (const STPInstance *instance, EdgeList *list, int v1, int v2)
{
/* Remove da lista list a aresta (v1, v2) */

	EdgeListNode *aux = NULL;
	EdgeListNode *cur = NULL;

	if (list != NULL)
	{
		if (list->edges != NULL)
		{
			if ((list->edges->v1 == v1 && list->edges->v2 == v2)
				|| (list->edges->v1 == v2 && list->edges->v2 == v1))
			{	
				aux = list->edges;
				list->edges = list->edges->next;
			}
			else
			{
				for (cur = list->edges; cur->next != NULL; cur = cur->next)
				{
					if ((cur->next->v1 == v1 && cur->next->v2 == v2)
						|| (cur->next->v1 == v2 && cur->next->v2 == v1))
					{	
						aux = cur->next;
						cur->next = cur->next->next;
					}
				}
				if ((cur->v1 == v1 && cur->v2 == v2) ||
					(cur->v1 == v2 && cur->v2 == v1))
				{
				}
			}
		}
	}

	free(aux);
}

void removeEdgeFromList2 (const STPInstance *instance, EdgeList *list, int v1, int v2)
{
/* Remove da lista list a aresta (v1, v2) */

	EdgeList *auxList = createEmptyEdgeList();
	
	addEdgeToList(auxList, v1, v2);

	copyEdgeListRemovingAndAdding(createEmptyEdgeList(), list, auxList, createEmptyEdgeList());

	free(auxList);
}

int vertexDegree (const STPInstance *instancem, EdgeList *graph, int v)
{
/* Retorna o grau do vértice v */

	int degree = 0;
	EdgeListNode *auxEdge = NULL;

	for (auxEdge = graph->edges; auxEdge != NULL; auxEdge = auxEdge->next)
	{
		if(auxEdge->v1 == v || auxEdge->v2 == v) //adjacentes a v
		{
			degree++;
		}
	}

	return degree;

}

double localSearch(const STPInstance *instance, EdgeList *solution, double solutionCost)
{
/*  Utiliza o algoritmo GRASP sugerido para realizar uma busca local a partir 
    da solução solution (S*) de custo solutionCost (l*) */

	/* Contadores */
	int i = 0; 		
	EdgeListNode *auxEdge = NULL;		

	/* Variáveis Auxiliares */
	int v1 = -1;	// u			
	int v2 = -1;	// v
	int auxVertex = -1;	// w
	
	/* Grafos */
	EdgeList *currentSolution = NULL;	// S
	double currentSolutionCost = INFINITY;	// l
	EdgeList *newSolution = NULL;		// S'
	double newSolutionCost = INFINITY;	// l'
	EdgeList *connectionPath = NULL;	// R
	double connectionPathCost = INFINITY;
	EdgeList *minPath = NULL;		// P
	double minPathCost = INFINITY;
	
	/* Listas */	
	int *flag = NULL;			// flag		
	EdgeList *edgesList = NULL;		// Q
	std::list<int> vertexList; 		// Q'

	/* Passo1: Seja S* a solução recebida na entrada e l* o seu custo. */

	do
	{
		/* Passo2: Faça S=S* e l=l*. */
		currentSolution = duplicateEdgeList(solution);
		currentSolutionCost = solutionCost;

		/* Crie uma lista Q com as arestas em S. */
		edgesList = duplicateEdgeList(currentSolution);

		do
		{
		 	/* Passo3: Retire uma aresta (u,v) de Q. */
			v1 = edgesList->edges->v1;
			v2 = edgesList->edges->v2;
			removeEdgeFromList(instance, edgesList, v1, v2);

		 	/* Passo4: Crie um  vetor flag[]  onde a i-ésima  posição recebe 0 se o vértice i está em S ou -1 caso contrário. */
			flag = new int[instance->n];
			for (i = 0; i < instance->n; i++)
			{
				flag[i] = -1;
			}
			for (auxEdge = currentSolution->edges; auxEdge != NULL; auxEdge = auxEdge->next)
			{
				flag[auxEdge->v1] = 0;
				flag[auxEdge->v2] = 0;
			}

		 	/* Passo5: Faça R = {(u,v)}. */
			if(connectionPath != NULL)
				free(connectionPath);
			connectionPath = createEmptyEdgeList();
			addEdgeToList(connectionPath, v1, v2);

		 	/* Passo6: Faça flag[v] = 1. */
			flag[v2] = 1;	

		 	/* Passo7: Crie  uma  lista Q' com  todos os  vértices  adjacentes a v, exceto u. */
			for (auxEdge = currentSolution->edges; auxEdge != NULL; auxEdge = auxEdge->next)
			{
				if(auxEdge->v1 != v1 && auxEdge->v2 != v1) // u não está no par?
				{
					if(auxEdge->v1 == v2) // v está no par?
					{
						vertexList.push_back(v2); //adiciona o outro vértice à lista
					}
					else if (auxEdge->v2 == v2) // v está no par?
					{
						vertexList.push_back(v1); //adiciona o outro vértice à lista
					}
				}
			}

		 	/* Passo8: Retire um vértice w de Q'. */
			do
			{
				if(!vertexList.empty())
				{
					auxVertex = vertexList.front();
					vertexList.pop_front();
				}

		 	/* Passo9: Faça flag[w] = 1. */
				flag[auxVertex] = 1;

		 	/* Passo10: Insira em Q' todos os vértices adjacentes a w cuja flag é 0. */
				for (auxEdge = currentSolution->edges; auxEdge != NULL; auxEdge = auxEdge->next)
				{
					if (auxEdge->v1 == auxVertex) // w está no par ?
					{
						if (flag[auxEdge->v2] == 0) // flag 0 ?
						{
							vertexList.push_back(auxEdge->v2); // adiciona o outro vértice
						}
					}
					else if (auxEdge->v2 == auxVertex) // w está no par ?
					{
						if (flag[auxEdge->v1] == 0) // flag 0 ?
						{
							vertexList.push_back(auxEdge->v1); // adiciona o outro vértice
						}
					}
				}
	
		 	/* Passo11: Se Q' não estiver vazia, volte ao passo 8. */
			} while(!vertexList.empty());

		  	/* Passo12: Faça w = u. */
			auxVertex = v1;

		 	/* Passo13: Se w é um terminal ou possui  grau diferente de  dois em S, vá para o passo 16. */
			while (!instance->isTerminal[auxVertex] && 
				vertexDegree(instance, currentSolution, auxVertex) != 2)
			{

		 	/* Passo14: Faça flag[w] = -1. */
				flag[auxVertex] = -1;	

		 	/* Passo15: Seja w' o único vértice  adjacente a w que  possui  flag 0. */ 
				for (i = 0; i < instance->n; i++)
				{
					if (flag[i] == 0)
					{	
						/* Acrescente (w,w') em R, faça w = w' e volte ao passo 13.*/
						addEdgeToList(connectionPath, auxVertex, i);
						auxVertex = i;
						break;
					}
				}
			}

		 	/* Passo16: Faça w = v. */
			auxVertex = v2;

		 	/* Passo17: Se w é um  terminal ou  possui grau diferente de dois em S, vá para o passo 20. */
			while (!instance->isTerminal[auxVertex] && 
				vertexDegree(instance, currentSolution, auxVertex) != 2)
			{

		 	/* Passo18: Faça flag[w] = -1. */
				flag[auxVertex] = -1;	

		 	/* Passo19: Seja w' o único vértice  adjacente a w que  possui  flag 1. */ 
				for (i = 0; i < instance->n; i++)
				{
					if (flag[i] == 1)
					{	
						/* Acrescente (w,w') em R, faça w = w' e volte ao passo 13.*/
						addEdgeToList(connectionPath, auxVertex, i);
						auxVertex = i;
						break;
					}
				}
			}
		 
		 	/* Passo20: Execute  o algoritmo de caminhos  mínimo informando o vetor
		     	   flag para  encontrar  o caminho de custo  mínimo P que liga
		     	   as duas subárvores obtidas ao remover de S as arestas em R. */
			minPath = createEmptyEdgeList();
			minPathCost = shortestPath(instance, flag, minPath, 0);

		 	/* Passo21: Se o  custo de P */
			connectionPathCost = 0;
			for (auxEdge = connectionPath->edges; auxEdge != NULL; auxEdge = auxEdge->next)
			{
				connectionPathCost += instance->costs[auxEdge->v1][auxEdge->v2];
			}

			/* for menor que o custo  total  das arestas em R, */
			if (minPathCost < connectionPathCost)
			{
				/* então  faça uma cópia das  arestas em S que não estão
				   em  R e acrescente nesta  cópia as arestas de P.   */
				newSolution = createEmptyEdgeList();
				copyEdgeListRemovingAndAdding(newSolution, currentSolution, connectionPath, minPath);
			}

			/* Seja S' a solução  obtida  a  partir  desta  cópia e l'  o seu custo. */
			if(newSolution != NULL)
			{
				newSolutionCost = 0;
				for (auxEdge = newSolution->edges; auxEdge != NULL; auxEdge = auxEdge->next)
				{
					newSolutionCost += instance->costs[auxEdge->v1][auxEdge->v2];
				}		
			}

			/* Se l' < l*, faça S*=S' e l*=l'. */
			if (newSolutionCost < solutionCost)
			{
				solution = newSolution;
				solutionCost = newSolutionCost;
			}
	  
	 	/* Passo22: Se Q não estiver vazia, volte ao passo 3. */
		}while (edgesList->edges != NULL);

	 	/* Passo23: Se   l* < l,   volte   ao   passo   2. */ 
	}while (newSolutionCost < solutionCost);

	/* C aso   contrário, retorne (S*,l*). */
	return newSolutionCost;
}

double solve(const STPInstance *instance, EdgeList *solution, int executeLocalSearch)

{
	int i = 0; // contadores
	double solutionCost = INFINITY;
	EdgeList *newSolution = NULL;
	double newSolutionCost = INFINITY;

	solutionCost = constructSolution(instance, solution, 1);

	// Para cada iteração
	for (i = 0; i < MAX_ITERATIONS; i++) // 500 iterações
	{
		// Construa uma solução de forma aleatória
		// (utilize a função pronta constructSolution passando o parâmetro random igual a 1).
		// Não se esqueça que a lista (solution) informada para esta função deve estar alocada e vazia!
		newSolution = createEmptyEdgeList();	
		newSolutionCost = constructSolution(instance, newSolution, 1);
		
		// Se o parâmetro executeLocalSearch for igual a 1,
		// execute a busca local na solução encontrada no passo anterior.
		if (executeLocalSearch == 1)
		{
			localSearch(instance, newSolution, newSolutionCost);
		}
		
		// Verifique e atualize, caso necessário, se a nova solução obtida é
		// melhor que a melhor solução encontrada até o momento.
		// Dica: utilize a função swapEdgeList para colocar em solution
		// as arestas da nova melhor solução encontrada.
		if (newSolutionCost < solutionCost)
		{
			swapEdgeLists(newSolution, solution);
			solutionCost = newSolutionCost;
		}

		// Liberar a memória da solução antiga
		clearEdgeList(newSolution);
		newSolutionCost = INFINITY;
	}
		
	return solutionCost;
}


