
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct
{
   int n, t;
   double **costs;
   int *terminals;
   int *isTerminal;
} STPInstance;

typedef struct EdgeListNode
{
   int v1, v2;
   struct EdgeListNode *next;
} EdgeListNode;

typedef struct
{
   int n;
   EdgeListNode *edges;
} EdgeList;

EdgeList* createEmptyEdgeList();

void addEdgeToList(EdgeList *list, int v1, int v2);

EdgeList* duplicateEdgeList(EdgeList *list);

void copyEdgeListRemovingAndAdding(EdgeList *destinationList, EdgeList *sourceList, EdgeList *removeList, EdgeList *addList);

void swapEdgeLists(EdgeList *list, EdgeList *list2);

void clearEdgeList(EdgeList *list);

double shortestPath(const STPInstance *instance, int *flag, EdgeList *path, int skip);	

double constructSolution(const STPInstance *instance, EdgeList *solution, int random);

double localSearch(const STPInstance *instance, EdgeList *solution, double solutionCost);

double solve(const STPInstance *instance, EdgeList *solution, int executeLocalSearch);


