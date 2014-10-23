
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>

#define INF 9999

typedef struct
{
   int n;
   double **costs;
} TSPInstance;

typedef struct tree_node
{
	struct tree_node *sons;		// lista de nós filhos
	struct tree_node *brother;	// lista de nós irmãos

	int added_v;			// vértice adicionado ao caminho neste nó
	//int first_v;			// primeiro vértice do caminho parcial
	//int last_v;			// último vértice do caminho parcial

	double z;			// distância acumulada entre os vértices que já estão no caminho
	double lower_lim;		// limitante inferior desta árvore

	int level;			// número de vértices fixados nesta solução
	int *partialSolution;		// caminho parcial escolhido até este nó
} bb_node;

typedef struct
{
	int v;
	int dist;
} node;	

double solve(const TSPInstance *instance, int *bestSolution);





