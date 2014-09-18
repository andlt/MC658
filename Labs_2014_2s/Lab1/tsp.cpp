
/*
   Implemente na função solve o algoritmo branch-and-bound pedido no enunciado.

   Esta função recebe uma instância do TSP cujos dados são armazenados na estrutura de dados
   definida pelo tipo TSPInstance (veja o arquivo tsp.h).
   O atribuito n desta estrutura corresponde a quantidade de cidades.
   O custo da viagem entre as cidades i e j é armazenado na linha i e coluna j da matriz instance->costs,
   ou seja, em instance->costs[i][j] onde 0 <= i, j <= n-1.
   
   Além da instância, esta função também recebe um vetor de n números inteiros
   onde deve ser armazenada a melhor solução (circuito hamiltoniano) computada.
   Você pode assumir que o circuito comece em um cidade qualquer.
   O valor na i-ésima posição do vetor bestSolution deve ser o índice da i-ésima cidade
   visitada no circuito encontrado.
   
   O valor retornado pela função deve ser o custo da solução obtida.

   ATENÇÃO: Não altere os dados da instância pois a mesma é utilizada no programa principal!
*/

#include "tsp.h"

double get_z (const TSPInstance *instance, const int *partialSolution, int m)
{
/* Retorna a distância acumulada na solução parcial partialSolution */
	
	int i = 0;
	double z = 0;

	for (i = 0; i < m-1; i++)
	{
		z = z + instance->costs[ partialSolution[i] ][ partialSolution[i+1] ];
	}	

	return z;
}

int get_closest (const TSPInstance *instance, const int *partialSolution, int m, int s)
{
/* Retorna o vértice mais próximo de s que ainda não está no circuito */

	int i = 0;
	

	for (i = 0; i < m; i++)
	{
		return 0;
	}
}

double add_u_cost (const TSPInstance *instance, const int *partialSolution, int m)
{
/* Retorna o custo de conectar o primeiro vértice do circuito ao vértice mais próximo dele que não faz parte do circuito. */

	int u = -1;
	int v1 = partialSolution[0];

	u = get_closest(instance, partialSolution, m, v1);

	return instance->costs[u][v1];
}

double add_w_cost (const TSPInstance *instance, const int *partialSolution, int m)
{
/* Retorna o custo de conectar o último vértice do circuito ao vértice mais próximo dele que não faz parte do circuito. */

	int w = -1;
	int vm = partialSolution[m-1];

	w = get_closest(instance, partialSolution, m, vm);

	return instance->costs[vm][w];
}

double calculateLowerBound(const TSPInstance *instance, const int *partialSolution, int m)
{
	float lb = 0;

	// Calcule Z(S) onde S está armazenada em partialSolution e m corresponde à quantidade de vértices em partialSolution
	lb = lb + get_z(instance, partialSolution, m);
	
	// Calcule c(u,v1) e c(vm,w)
	lb = lb + add_u_cost(instance, partialSolution, m);
	lb = lb + add_w_cost(instance, partialSolution, m);
	
	// Calcule AGM(V-S)
	
	// Retorne Z(S) + c(u,v1) + c(vm,w) + AGM(V-S)
	return 0;
}

tree_node *tree_init (const TSPInstance *instance, int s)
{
/*Inicializa a árvore começando o caminho pelo vértice 0 */

	tree_node *root = NULL;
	
	root = new tree_node;

	root->added_v = s;
	root->first_v = s;
	root->last_v = s;

	root->z = 0;
	root-> level = 1;

	root->sons = malloc(sizeof(tree_node)*instance->n);

	root->partialSolution = new vector

}

double solve(const TSPInstance *instance, int *bestSolution)
{
	double bestSolutionCost = DBL_MAX;
	tree_node *root;

	/* Implemente aqui o algoritmo branch-and-bound */
	
	// Escolha um vértice qualquer e crie o nó raiz que representará o circuito parcial contendo apenas o vértice escolhido
	root = tree_init(0);	

	// Adicione este nó no heap de nós
	
	// Enquanto o heap não estiver vazio
	
		// Retire do heap um nó cujo limitante inferior é mínimo
		// *** Note que os heaps implementados pelo C e C++ são heap de máximo
		// *** Você deve tomar cuidado com a função de comparação dos nó no heap
		// *** para poder obter o nó de menor limitante
		
		// Gerar os filhos do nó escolhido e calcular seus limitantes inferiores
			
			// Verificar se o nó deve ser amadurecido com base no limitante inferior e superior (poda por limitante)
			
			// Verificar se o nó representa uma solução completa
			
				// Caso represente, verificar se esta é melhor que a melhor solução encontrada até o momento
		
			// Se não houver poda por limitante e a solução não é completa, adicione o nó no heap
		
      
	return bestSolutionCost;
}



