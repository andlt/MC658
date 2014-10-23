#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

typedef struct
{
	double lim_inf;
}No;

bool compara_no (No no1, No no2)
{
	return (no1.lim_inf > no2.lim_inf);
}

int main (int argc, char **argv)
{
	std::vector<No> heap;
	No no_temp;
	
	for (int i = 9; i >= 0; i--)
	{
		no_temp.lim_inf = i;
		heap.push_back(no_temp);
		
		std::push_heap(heap.begin(), heap.end(), compara_no);
	}
	
	for (int i = 0; i < 10; i++)
	{
		std::cout << heap.front().lim_inf << ' ';
		printf("%lf\n", heap.front().lim_inf);
		
		std::pop_heap(heap.begin(), heap.end(), compara_no);
		
		heap.pop_back();
	}
	
	std::cout << std::endl;

	return 0;
}
