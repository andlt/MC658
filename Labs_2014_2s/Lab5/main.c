/*
  Utilize este modelo para simplificar seu trabalho. Não é necessário
  modificar a função main(). São fornecidas a leitura das instâncias,
  a medição do tempo e a saída dos resultados no formato especificado.
  Apenas implemente o algoritmo pedido no enunciado na função
  especificada no arquivo stp.c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stp.h"

typedef struct IntListNode
{
   int i;
   struct IntListNode *next;
} IntListNode;

STPInstance* readInstance(const char *filename)
{
   char c;
   int i, j, v1, v2;
   double cost;
   FILE *fp;

   STPInstance *instance = (STPInstance*)malloc(sizeof(STPInstance));

   fp = fopen(filename, "r");

   if(!fp)
   {
      fprintf(stdout, "Nao foi possivel abrir o arquivo de entrada.\n");
      exit(0);
   }
   
   for (i = 0; i < 19; i++)
      fgetc(fp);
   
   fscanf(fp, "%d", &(instance->n));

   for (i = 0; i < 19; i++)
      fgetc(fp);
   
   instance->isTerminal = (int*)malloc(instance->n*sizeof(int));
   instance->costs = (double**)malloc(instance->n*sizeof(double*));

   for (i = 0; i < instance->n; i++)
   {
      instance->isTerminal[i] = 0;      
      instance->costs[i] = (double*)malloc(instance->n*sizeof(double));
      
      for (j = 0; j < instance->n; j++)
         instance->costs[i][j] = 0.0;
   }
     
   for (i = 0; i < (instance->n*(instance->n-1)); i++)
   {
      fscanf(fp, "%d %d %lf", &v1, &v2, &cost);
      instance->costs[v1][v2] = cost;
   }

   instance->t = 0;
   instance->terminals = (int*)malloc(instance->n*sizeof(int));
      
   for (i = 0; i < 11; i++)
      fgetc(fp);
   
   fscanf(fp, "%c", &c);
   
   while (c != ';')
   {
      fscanf(fp, "%d", &v1);
      instance->isTerminal[v1] = 1;
      instance->terminals[instance->t++] = v1;
      fscanf(fp, "%c", &c);
   }

   instance->terminals = (int*)realloc(instance->terminals, instance->t*sizeof(int));
   
   fclose(fp);

   return instance;
}

void verifyAndPrintSolution(const STPInstance *instance, EdgeList *solution, double solutionCost)
{
   double cost;
   int i, n, m;
   IntListNode **adjacency;
   EdgeListNode *edgesList;

   if (solution->edges == NULL)
   {
      printf("Incorrect solution: edges list empty!\n");
      return;
   }

   adjacency = (IntListNode**)malloc(instance->n*sizeof(IntListNode*));

   for (i = 0; i < instance->n; i++)
      adjacency[i] = NULL;

   n = 0;
   m = 0;
   cost = 0.0;
   
   printf("Solution edges:\n");
   
   edgesList = solution->edges;
   
   while (edgesList != NULL)
   {
      printf("%d %d\n", edgesList->v1, edgesList->v2);
   
      if (adjacency[edgesList->v1] == NULL)
         n++;
         
      IntListNode *newIntListNode = (IntListNode*)malloc(sizeof(IntListNode));
      newIntListNode->i = edgesList->v2;
      newIntListNode->next = adjacency[edgesList->v1];
      adjacency[edgesList->v1] = newIntListNode;

      if (adjacency[edgesList->v2] == NULL)
         n++;

      newIntListNode = (IntListNode*)malloc(sizeof(IntListNode));
      newIntListNode->i = edgesList->v1;
      newIntListNode->next = adjacency[edgesList->v2];
      adjacency[edgesList->v2] = newIntListNode;
      
      cost += instance->costs[ edgesList->v1 ][ edgesList->v2 ];
      
      edgesList = edgesList->next;
      m++;
   }
   
   int uncoveredTerminal = -1;

   for (i = 0; i < instance->t; i++)
      if (adjacency[instance->terminals[i]] == NULL)
      {
         uncoveredTerminal = instance->terminals[i];
         break;
      }

   IntListNode *list = (IntListNode*)malloc(sizeof(IntListNode));
   list->i = solution->edges->v1;
   list->next = NULL;
   
   while (list != NULL)
   {
      int v = list->i;
      IntListNode *tmp = list;
      list = list->next;
      free(tmp);

      tmp = adjacency[v];
      
      if (tmp != NULL)
      {
          while (tmp->next != NULL)
             tmp = tmp->next;
		     
          tmp->next = list;
          list = adjacency[v];
          adjacency[v] = NULL;
      }
   }
   
   int disconnected = 0;
   
   for (i = 0; i < instance->n; i++)
      if (adjacency[i] != NULL)
      {
         disconnected = 1;
         
         while (adjacency[i] != NULL)
         {
            IntListNode *tmp = adjacency[i];
            adjacency[i] = adjacency[i]->next;
            free(tmp);
         }
      }

   free(adjacency);
   
   if (uncoveredTerminal != -1)
   {
      printf("Incorrect solution: terminal %d is not in solution!\n", uncoveredTerminal);
      return;  
   }
   
   if (disconnected)
   {
      printf("Incorrect solution: it is disconnected!\n");
      return;
   }
   
   if (n-1 != m)
   {
      printf("Incorrect solution: there is at least one cycle!\n");
      return;
   }

   if (fabs(cost - solutionCost) > 1e-2)
   {
      printf("Incorrect solution: wrong cost! (%.2f != %.2f)\n", solutionCost, cost);
      return;
   }

   printf("\nSolution cost: %.2f\n", cost);
}

int main(int argc, char **argv)
{
   STPInstance *instance;
   EdgeList *solution;
   double solutionCost;
   int i;

   if(argc != 2)
   {
      printf("Uso: %s <instancia>\n", argv[0]);
      return 0;
   }
   
   srand(time(NULL));

   instance = readInstance(argv[1]);

   solution = createEmptyEdgeList();

   printf("\n=================================================\n");

   printf("Deterministic Greedy Construction\n");

   clock_t beginning = clock();

   solutionCost = constructSolution(instance, solution, 0);

   printf("\n");

   verifyAndPrintSolution(instance, solution, solutionCost);

   printf("Running time: %.2f\n\n", ((clock() - beginning) / (float)CLOCKS_PER_SEC));

   printf("=================================================\n");

   printf("Random Greedy Construction Without Local Search\n");

   clearEdgeList(solution); 
   
   beginning = clock();

   solutionCost = solve(instance, solution, 0);

   printf("\n");

   verifyAndPrintSolution(instance, solution, solutionCost);

   printf("Running time: %.2f\n\n", ((clock() - beginning) / (float)CLOCKS_PER_SEC));
      
   printf("=================================================\n");
   
   printf("Random Greedy Construction With Local Search\n");   
   
   clearEdgeList(solution); 

   beginning = clock();

   solutionCost = solve(instance, solution, 1);

   printf("\n");

   verifyAndPrintSolution(instance, solution, solutionCost);

   printf("Running time: %.2f\n\n", ((clock() - beginning) / (float)CLOCKS_PER_SEC));

   printf("=================================================\n");

   for (i = 0; i < instance->n; i++)
      free(instance->costs[i]);
   
   free(instance->costs);
   free(instance->terminals);
   free(instance->isTerminal);
   free(instance);
   
   clearEdgeList(solution);   
   free(solution);

   return 0;
}


