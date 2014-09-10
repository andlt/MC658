#include <stdio.h>

#define LADO 8

int main ()
{
	int i = 0, j = 0;
	int tab[LADO][LADO];

	zera_matriz(tab, LADO, LADO);

	insere_rainha(tab, 0);

	for (i = 0; i<LADO; i++) {
		for (j = 0; j<LADO; j++){
			printf("%d ", tab[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int teste (int (*tab)[LADO], int lin, int col)
{
	/* Testa se não existem rainhas na coluna ou diagonal de tab[lin][col] */

	int i = 0;
	
	for(i = 0; i < LADO && i != lin; i++){
		if(tab[i][col] != 0){
			return 1;
		}
	}

}

int insere_rainha (int (*tab)[LADO], int lin)
{
	/* Insere uma rainha na linha lin, e testa se a nova rainha pode ser 
	atacada por uma das rainhas já existentes. Retorna 0 se a operação for 
	um sucesso (programa termina) e 1 caso as condições não possam ser 
	atingidas a partir daquele ponto */

	int i = 0;

	if (lin == LADO){ //chegamos ao fim do tabuleiro
		return 0;
	} 

	for (i = 0; i < LADO; i++){
		tab[lin][i] = 1; //coloca a rainha
		if(teste(tab, lin, i) == 0){	//se n"ao pode ser atacada
			return insere_rainha (tab, lin+1);
		}
	}

	return 1;

}

int zera_matriz (int (*matriz)[LADO], int lin, int col)
{
	int i = 0, j = 0;

	for (i = 0; i<lin; i++) {
		for (j = 0; j<col; j++){
			matriz[i][j] = 0;
		}
	}
}
