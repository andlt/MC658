#include <stdio.h>
#include <stdlib.h>

int main ()
{
	void* mat = NULL;
	mat = malloc(sizeof(double) * 5000000 * 5000000);
	return 0;
}
