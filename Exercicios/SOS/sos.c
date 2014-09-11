/* Programa para resolver o Problema da Soma de Conujuntos (SOS) através de um 
  algoritmo de backtracking */

#include <stdio.h>
#include <stdlib.h>

void quicksort (int *v, int n)
{
/* Ordena o vetor v de n digitos utilizando o algoritmo quicksort */
}

void sos (int *sol, int cur_sum, int k, int sum_left, int *w, int W, int n)
{
/* Função limitante do Backtracking. Recebe a solução em contrução (sol), o somatório de itens atualmente inseridos no conjunto (cur_sum), a próxima dimensão da tupla a ser fixada (k) e o somatório de itens que ainda podem ser adicionados (sum_left) */

	int i = 0;

	/* Considera as soluções com o k-ésimo item */

	//printf("\n\nRecursão: k: %d\n", k);

	sol[k] = 1;
	//printf("cur_sum: %d, w[%d]: %d, W: %d\n", cur_sum, k, w[k], W);

	if (cur_sum + w[k] == W)
	{
		printf("\nSolução econtrada:\n");
		for (i = 0; i < n; i++)
		{
			if(sol[i] == 1)
				printf("%d ", w[i]);
		}
		printf("\n\n");
	}
	else
	{
		//printf("\nk: %d, n-1: %d\n", k, n-1);
		if(k < n-1)
		{
			//printf("cur_sum: %d + w[%d]: %d + w[%d]: %d <= W: %d\n?", cur_sum, k, w[k], k+1, w[k+1], W);
			if (cur_sum + w[k] + w[k+1] <= W)
			{
				sos(sol, cur_sum + w[k], k+1, sum_left - w[k], w, W, n);
			}
		}
	}

	/* Considera as soluções sem o k-ésimo item */

	sol[k] = 0;

	if(k < n-1)
	{
		if ((cur_sum + sum_left - w[k] >= W) && (cur_sum + w[k+1] <= W))
		{
			sos(sol, cur_sum, k+1, sum_left - w[k], w, W, n);
		}
	}
}

int main ()
{
	int n = 0;
	int W = 0;
	int *w = NULL;
	int *sol = NULL;
	int i = 0;
	int total_sum = 0;

	printf("\nEntre com o tamanho n do conjunto:\n");
	scanf("%d", &n);

	printf("\nEntre com os %d valores:\n", n);
	w = malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
	{
		scanf("%d", &w[i]);
	}

	printf("\nEntre com o valor W alvo:\n");
	scanf("%d", &W);

	sol = malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
	{
		sol[i] = 0;
		total_sum = total_sum + w[i];
	}	

	//ordenar w do menor para o maior

	sos(sol, 0, 0, total_sum, w, W, n);

	return 0;
}


