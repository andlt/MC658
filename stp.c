
/*
   Implemente o modelo do problema da árvore de Steiner (STP), descrito no enunciado,
   na função solve utilizando as rotinas do GLPK.

   Esta função recebe uma instância do STP cujos dados são armazenados na estrutura de dados
   definida pelo tipo STPInstance (veja o arquivo stp.h).
   O atribuito n desta estrutura corresponde à quantidade de vértice e o atributo t à quantidade de terminais.
   O custo da aresta entre os vértices i e j é armazenado na linha (i-1) e coluna (j-1) da matriz instance->costs,
   ou seja, em instance->costs[i-1][j-1] onde 0 <= i, j <= n-1.
   Já o vetor terminals contém t posições e armazena os índices dos vértices terminais.
   
   Além da instância, esta função também recebe uma estrutura de dados EdgeList
   que deve armazenar a lista das arestas que estão na solução ótima computada.
   Esta estrutura já foi inicializada com uma lista de arestas vazia no arquivo main.c
   através da chamada da função createEmptyEdgeList().
   Para adicionar uma aresta basta executar a função addEdgeToList passando
   como parâmetro a lista (solution) e os dois vértices da aresta que será adicionada.
   Exemplo de inserção da aresta (0,1) em solution: addEdgeToList(solution, 0, 1).
      
   O valor retornado pela função deve ser o custo ótimo da solução obtida.
  
   ATENÇÃO: Não altere os dados da instância pois a mesma é utilizada no programa principal!
*/

#include "stp.h"
#include <stdio.h>

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

double solve(const STPInstance *instance, EdgeList *solution)
{
	/* Declare as variáveis auxiliares */
	double solutionCost = 0;
	int n = instance->n;
	int t = instance->t;
	int constraintNumber = 1;
	int i = 0, j = 0, k = 0;	// contadores
	int *constraintIndices = NULL;
	double *constraintCoefficients = NULL;

	constraintIndices = (int*)malloc((n*n+1)*sizeof(int));
	constraintCoefficients = (double*)malloc((n*n+1)*sizeof(double));

	/* Crie um problem inicialmente vazio no GLPK */
	glp_prob *model = glp_create_prob();

	/* Ajuste o sentido da função objetivo */
	glp_set_obj_dir(model, GLP_MIN);
	   
	/* Adicione as colunas (variáveis) */

	// Para este problema são necessárias E variáveis x e T2*E variáveis y
	// Mapeamento: variáveis x[i,j] utilizando a fórmula (i-1)*n + j
	// Mapeamento: variáveis y[t,i,j] utilizando a fórmula n*n-1 + ((k-1)*n +i-1)*n + j
	// Total: n*n + n*n*n variáveis
	glp_add_cols(model, n*n + t*n*n);

	/* Ajuste o tipo, limitantes e coeficientes da função objetivo das variáveis */
	
        // Preparação das variáveis x
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; j++)
		{
			// Define o tipo da variável como sendo binário
     			glp_set_col_kind(model, (i-1)*n + j, GLP_BV);

			// Define o limitante inferior (0) e superior (1) da variável
         		glp_set_col_bnds(model, (i-1)*n + j, GLP_DB, 0.0, 1.0);

			// Define o coeficiente da variável na função objetivo
			glp_set_obj_coef(model, (i-1)*n + j, instance->costs[i-1][j-1]);
		}
	}

	// Preparação das variáveis y
	for (k = 1; k <= t; k++)
	{
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				// Define o tipo da variável como sendo contínua
	     			glp_set_col_kind(model, n*n-1 + ((k-1)*n +i-1)*n + j, GLP_BV);

				// Define o limitante inferior (0) da variável
				glp_set_col_bnds(model, n*n-1 + ((k-1)*n +i-1)*n + j, GLP_LO, 0.0, 777);

				// Define o coeficiente da variável na função objetivo
				glp_set_obj_coef(model, n*n-1 + ((k-1)*n +i-1)*n + j, 0);
			}
		}
	}
	   
	/* Crie as restrições */

	// Cria no modelo 2n restrições (linhas) nulas (com os coeficientes e limitantes zerados)
	glp_add_rows(model, 2*t);

	// Restrição que estamos modificando:
	constraintNumber = 1;

	// Preenchimento das restrições flow1:
	// flow1{t in T2}: sum{j in V: j != t0} y[t,j,t0] - sum{j in V: j != t0} y[t,t0,j] = -1;

	for (k = 1; k < t; k++)
	{
		// Define o valor exato (RHS) da restrição      
		glp_set_row_bnds(model, constraintNumber, GLP_FX, -1.0, -1.0);

		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				// Ajusta o índice da variável que será informado à rotina do GLPK
				constraintIndices[(i-1)*n + j] = n*n-1 + ((k-1)*n +i-1)*n + j;

				// Ajusta o coeficiente da variável cujo índice foi definido na linha anterior para ser informado ao GLPK
				constraintCoefficients[(i-1)*n + j] = 1;
			}
		}

		// Passa ao GLPK a restrição que acabou de ser definida nos vetores constraintIndices e constraintCoefficients
		glp_set_mat_row(model, constraintNumber, n*n, constraintIndices, constraintCoefficients);

		// atualiza o indice da próxima restrição a ser inserida
		constraintNumber++;
	}

	// Preenchimento das restrições flow2:
	// flow2{t in T2}: sum{j in V: j != t} y[t,j,t] - sum{j in V: j != t} y[t,t,j] = 1;

	// Preenchimento das restrições flow3:
	// flow3{t in T2, i in V: i != t0 and i != t}: sum{j in V: i != j} y[t,j,i] - sum{j in V: i != j} y[t,i,j] = 0;

	// Preenchimento das restrições arc:
	// arc{t in T2, i in V, j in V: i != j}: x[i,j] -y[t,i,j] >= 0;

	for (k = 1; k <= t; k++)
	{
		for (i = 1; 1 <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				if (j != i)
				{
					// Define o limite inferior (RHS)  da restrição      
      					glp_set_row_bnds(model, constraintNumber, GLP_LO, 0, 777);
					
				}
			}
		}
	}
	   
	/* Chame o resolvedor */

	// Define os parâmetros que serão passados ao resolvedor   
	glp_iocp param;
	glp_init_iocp(&param);

	// Ativa o presolver
	param.presolve = GLP_ON;

	// Resolve o modelo
	int status = glp_intopt(model, &param);

	// Verifica se houve algum erro durante a otimização
	if (status)
	{
		printf("Ocorreu um erro durante o processo de otimizacao.\n");
	}
	else
	{
		// Verifica se o método encontrou uma solução
      
     		status = glp_mip_status(model);

	      	if ((status == GLP_OPT) || (status == GLP_FEAS))
	      	{
		 	// Imprime a solução encontrada
		 
			if (status == GLP_OPT)
				printf("Solucao otima encontrada!\n");
		 	else
				printf("A solucao encontrada pode nao ser otima!\n");
	      
		 	printf("Custo da solucao: %f\n", glp_mip_obj_val(model));
			solutionCost = glp_mip_obj_val(model);

		 	for (i = 1; i <= n; i++)
		 	{
		   		 for (j = 1; j <= n; j++)
		      		 	printf("%f ", glp_mip_col_val(model, (i-1)*n + j));
		 
		    		printf("\n");
		 	}
		}     
		else
	     	{
			printf("Nenhuma solucao foi encontrada!\n");
	      	}
	}

	/* Leitura da solução encontrada pelo GLPK */
		
	return solutionCost;
}


