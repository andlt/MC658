
/*
   Implemente a heurística de caminhos mínimos, descrita no enunciado, na função solve.

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

#include <iostream>
#include <fstream>


#define INF 9999
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

int getMinDistIndex (const STPInstance *instance, bool *q, int *dist)
{
	/* Retorna o índice do vértice i de q (q[i] == true) com menor valor dist[i] */

	int minDist = INF;
	int minDistIndex = -1;
	int i = 0;				// contador
	int n = instance->n;			// facilitar a leitura

	for (i = 0; i < n; i++)
	{
		if(q[i] == true)
		{
			if(dist[i] < minDist)
			{
				minDistIndex = i;
				minDist = dist[i];
			}
		}
	}

	return minDistIndex;
}

int * minPath (const STPInstance *instance, bool *vertexInSol, int *end)
{
	/* Caminhos Mínimo Entre Múltiplas Fontes e Destinos
	Retorna um vetor contendo a sequência de vértices no caminho mínimo entre 
	um dos vértices i de instance que já está na solução (vertexInSol[i] = 
	true) e um vértice de instance que não está. */

	int *dist = NULL;			// distância de cada vértice aos vértices da érvore
	int *pred = NULL;			// pred[i] = j => j é o precedessor de i no caminho mínimo
	bool *q = NULL;				// q[i] = true => i está no conjunto auxiliar de vértices q
	int minDistIndex = -1;			// index do vetor de dist mínima
	int i = 0;				// contadores
	int n = instance->n;			// facilitar a leitura

	/* Seta os valores de dist, pred e q */

	dist = new int [n];
	pred = new int [n];
	q = new bool [n];

	for (i = 0; i < n; i++)
	{
		dist[i] = INF;			// distância inicial infinita
		pred[i] = -1;			// sem predecessor
		q[i] = true;			// todos estão em q
	}


	for (i = 0; i < n; i++)
	{
		if (vertexInSol[i] == true)
		{
			dist[i] = 0;		// distância para a solução de quem está na solução é 0
		}
	}

	/* Retiramos de q o vértice com menor valor de dist */

	minDistIndex = getMinDistIndex(instance, q, dist);
	q[minDistIndex] = false;
	*end = minDistIndex;					// recalcula o vértice final

	/* Enquanto end for umm vértice que já está na solução, recalculamos 
	todas as distâncias à arvore */ 

	while(vertexInSol[minDistIndex] == true)
	{
		for (i = 0; i < n; i++)
		{
			if (q[i] == true)
			{
				if (dist[i] > dist[minDistIndex] + instance->costs[minDistIndex][i])
				{
					dist[i] = dist[minDistIndex] + instance->costs[minDistIndex][i];
					pred[i] = minDistIndex;		// predecessor de i é minDistIndex
					*end = i;			// i é o útlimo vértice
				}
			}	
		}

		minDistIndex = getMinDistIndex(instance, q, dist);	// calcula o próximo minDistIndex
		q[minDistIndex] = false;
		*end = minDistIndex;					// recalcula o vértice final
	}
	
	return pred;
}

bool missingTerminal (const STPInstance *instance, bool *vertexInSol)
{
	/* Retorna true se um dos terminais de instance não estiver em vertexInSol */

	int i = 0;				// contadores
	int n = instance->n;			// facilitar a leitura
	int t = instance->t;
	int *terminals = instance->terminals;

	for (i = 0; i < t; i++)
	{
		if (vertexInSol[terminals[i]] == false)
		{
			return true;
		}
	}

	return false;
}

double solve(const STPInstance *instance, EdgeList *solution)
{
	double solutionCost = 0;
	bool *vertexInSol = NULL;		// vertexInSol[i] == 1 se o vértice i faz parte da solução, 0 se não
	int *pred = NULL;
	int end = -1;
	int i = 0;				// contadores
	int n = instance->n;			// facilitar a leitura
	int t = instance->t;
	int *terminals = instance->terminals;

	/* Inicializa os valores */

	vertexInSol = new bool [n];
	pred = new int [n];

	for (i = 0; i < n; i++)
	{
		vertexInSol[i] = false;
	}

	/* Passo1: Faça  S = {},  U={t0}  onde t0 é um  terminal qualquer em T. */

	vertexInSol[terminals[0]] = true;	// adiciona o primeiro vértice de T a U

	while(missingTerminal(instance, vertexInSol))
	{
	 	/* Passo2: Encontre um caminho de menor custo que liga um vértice em U
		  	a um vértice em T \ U. Seja V' e E' o  conjunto  vértices e
		  	arestas  deste  caminho,  respectivamente.  Faça S = S ∪ E',
		  	U = U ∪ V'. */

		pred = minPath(instance, vertexInSol, &end);	// devolve um caminho que termina fora da árvore, no vértice end

		while (pred[end] != -1)
		{
			addEdgeToList(solution, end, pred[end]);	// insere a aresta
			solutionCost = solutionCost + instance->costs[end][pred[end]];		//atualiza o custo
			vertexInSol[end] = true;			// insere os vértices
			vertexInSol[pred[end]] = true;
			end = pred[end];				// anda no vetor de predecessores
		}

		/* Passo3: Se |U ∩ T|=|T|, retorne S. Caso contrário, volte ao passo 2. */
	}
		
	return solutionCost;
}


