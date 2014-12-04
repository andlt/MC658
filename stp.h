
#include <stdlib.h>
#include <glpk.h>
#include <math.h>

typedef struct
{
   int n, t;
   double **costs;
   int *terminals;
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

double solve(const STPInstance *instance, EdgeList *solution);


