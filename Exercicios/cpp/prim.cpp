/* Algoritmo de Prim para encontrar uma Árvrore Geradora Mínima (MST) */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <list>

typedef struct
{
	int n;
	double **adj;	
} graph;

void print_matrix (int n, double **matrix)
{
	int i = 0, j = 0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << "a" << i << j <<": " << matrix[i][j] << "  ";
		std::cout << "\n";
	}

}

int main (int argc, char **argv)
{
	int n = 0;
	graph *G;

	std::cout << "Insira um valor:\n";

	std::cin >> n;

	double **m_adj = new double*;
	for (int i = 0; i < n; i++)
		m_adj[i] = new double[n];

	G = (graph *) malloc (sizeof (graph));
	G->n = n;
	G->adj = m_adj;
	
	print_matrix(G->n, G->adj);

	return 0;
}

void mst_prim (graph G)
{
	
}
