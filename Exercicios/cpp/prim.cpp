/* Algoritmo de Prim para encontrar uma Árvrore Geradora Mínima (MST) */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <list>

#define INF 9999

typedef struct
{
	int n;
	double **adj;	
} graph;

typedef struct
{
	int v;
	int dist;
} node;	

bool node_comp (node no1, node no2)
{
	return (no1.dist > no2.dist);
}

double w (graph* G, int i, int j)
{
	return G->adj[i][j];
}

void mst_prim (graph *G, int root)
{
	int *dist = NULL;
	int *parent = NULL;
	int i = 0, j = 0;
	std::vector<node> fila;
	node no_temp;
	node k;

	dist = new int[G->n];
	parent = new int[G->n];
	
	for (i = 0; i < G->n; i++)
	{
		dist[i] = INF;
		parent[i] = -1;
	}

	dist[root] = 0;
	
	for (i = 0; i < G->n; i++)
	{	
		no_temp.v = i;
		no_temp.dist = dist[i];
		fila.push_back(no_temp);

		std::push_heap(fila.begin(), fila.end(), node_comp);
	}

	while (fila.size() > 0)
	{
		no_temp = fila.front();
		std::pop_heap(fila.begin(), fila.end(), node_comp);
		fila.pop_back();

		for (i = 0; i < G->n; i++)
		{
			if (i != no_temp.v)
			{
				//  se o nó i está na fila
				//  e w(no_temp, i) < dist[i]
				//     parent[i] = no_temp
				//     dist[i] = w(no_temp, i)
			}
		}
	}

}

void load_graph (graph *G)
{
	int i = 0, j = 0;
	
	for (i = 0; i < G->n; i++)
	{
		for (j = 0; j < G->n; j++)
		{
			G->adj[i][j] = i*(G->n) + j;
		} 
	}
}

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
	
	load_graph(G);
	//print_matrix(G->n, G->adj);

	mst_prim(G, 0);

	return 0;
}


