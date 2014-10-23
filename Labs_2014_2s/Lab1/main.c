/*
  Utilize este modelo para simplificar seu trabalho. Não é necessário
  modificar a função main(). São fornecidas a leitura das instâncias,
  a medição do tempo e a saída dos resultados no formato especificado.
  Apenas implemente os algoritmos pedidos no enunciado nas funções
  especificadas no arquivo tsp.c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "tsp.h"

TSPInstance* readInstance(const char *filename)
{
   int i, j, v1, v2;
   double cost;
   FILE *fp;

   TSPInstance *instance = (TSPInstance*)malloc(sizeof(TSPInstance));

   fp = fopen(filename, "r");

   if(!fp)
   {
      printf("Nao foi possivel abrir o arquivo de entrada.\n");
      exit(0);
   }
   
   for (i = 0; i < 19; i++)
      fgetc(fp);
   
   fscanf(fp, "%d", &(instance->n));

   for (i = 0; i < 21; i++)
      fgetc(fp);
   
   instance->costs = (double**)malloc(instance->n*sizeof(double*));

   for (i = 0; i < instance->n; i++)
   {
      instance->costs[i] = (double*)malloc(instance->n*sizeof(double));
      
      for (j = 0; j < instance->n; j++)
         instance->costs[i][j] = 0.0;
   }
     
   for (i = 0; i < (instance->n*(instance->n-1)); i++)
   {
      fscanf(fp, "%d %d %lf", &v1, &v2, &cost);
      instance->costs[v1-1][v2-1] = cost;
   }
   
   fclose(fp);

   return instance;
}

void verifyAndPrintSolution(const TSPInstance *instance, int *solution, double solutionCost)
{
   double cost;
   int i, j, *vertice;

   vertice = (int*)malloc(instance->n*sizeof(int));

   for (i = 0; i < instance->n; i++)
      vertice[i] = 0;

   for (i = 0; i < instance->n; i++)
   {
      if ((solution[i] < 0) || (solution[i] > instance->n-1))
      {
         printf("Incorrect solution: wrong city index!\n");

         for (j = 0; j < instance->n; j++)
            printf("%d ", solution[j]);
         printf("\n");

         free(vertice);
         return;
      }
        
      if (vertice[ solution[i] ] == 0)
         vertice[ solution[i] ] = 1;
      else
      {
         printf("Incorrect solution: city visited twice!\n");
         
         for (j = 0; j < instance->n; j++)
            printf("%d ", solution[j]);
         printf("\n");
         
         free(vertice);
         return;
      }
   }

   cost = instance->costs[ solution[instance->n-1] ][ solution[0] ];
   
   for (i = 0; i < instance->n-1; i++)
      cost += instance->costs[ solution[i] ][ solution[i+1] ]; 
   
   if (fabs(cost - solutionCost) > 1e-2)
      printf("Incorrect solution: wrong cost!\n");
   else
      printf("Solution cost: %.2f\n", cost);      

   printf("Tour:");
   
   for (i = 0; i < instance->n; i++)
      printf(" %d", solution[i]); 
   
   printf("\n");

   free(vertice);
}

int main(int argc, char **argv)
{
   TSPInstance *instance;
   double solutionCost;
   int i, *solution;
   clock_t time;

   if(argc < 2)
   {
      printf("Usage: %s <instance>\n", argv[0]);
      return 0;
   }

   instance = readInstance(argv[1]);

   solution = (int*)malloc(instance->n*sizeof(int));     

   for (i = 0; i < instance->n; i++)
      solution[i] = 0;

   time = clock();

   solutionCost = solve(instance, solution);

   printf("\n");

   verifyAndPrintSolution(instance, solution, solutionCost);

   printf("Running time: %.2f\n\n", ((clock() - time) / (float)CLOCKS_PER_SEC));

   for (i = 0; i < instance->n; i++)
      free(instance->costs[i]);

   free(instance->costs);
   free(instance);
   free(solution);

   return 0;
}


