
/* MC658 Projeto e Análise de Algoritmos III 
   1o semestre de  2014s1 
   Prof. Cid C. de Souza (IC-UNICAMP) 
   PED: Lucas Oliveira 
   
   Exemplo de uso da biblioteca GLPK com o C/C++.
   
   Neste exemplo vamos criar um modelo e resolvê-lo através das rotinas do GLPK.
   O problema dado como exemplo será o seguinte.
   Seja M uma matriz quadrada com n linhas e colunas.
   Cada célula desta matriz possui um item.
   O item na linha i e coluna j possui custo c[i,j] e peso w[i,j].
   O objetivo é escolher quais itens levar de tal forma que,
   a soma dos pesos dos itens que foram levados de uma mesma linha i não ultrapasse u[i],
   e a soma dos pesos dos itens que foram levados de uma mesma coluna j seja pelo menos l[j].
   Isto deve ser feito minimizando a soma dos custos do itens.
   Seja x[i,j] uma variável binária que assume valor 1 apenas quando decidimos levar o item
   que está na linha i e coluna j, o modelo que representa o problema é:
   
   minimizar sum{i,j in 1..n} c[i,j]*x[i,j]
   sujeito a:
   sum{j in 1..n} w[i,j]*x[i,j] <= u[i] para i in 1..n
   sum{i in 1..n} w[i,j]*x[i,j] >= l[i] para j in 1..n   
   x[i,j] in {0,1} para i,j in 1..n

   Como tarefa para se preparar para o lab 3, modifique o código para incluir
   as restrições adicionais descritas abaixo.

   - Devem ser levados exatamente dois itens da diagonal principal e secundária:
   
   sum{d in 1..n} x[d,d] = 2
   sum{d in 1..n} x[n-d+1,d] = 2
   
   - As células (i,j) da k-ésima camada da matriz são aquelas onde k <= i,j <= n-k+1 e
   ao menos uma das igualdades i = k, j = k, n-i+1 = k, n-j+1 = k é verdadeira, para 1 <= k <= piso(n/2).
   Vamos impor que a soma dos custos dos itens levados de uma mesma camada
   não deve ultrapassar quatro vezes e meia o custo médio da camada.
   Seja C[k] o conjunto com as células da k-ésima camada, esta restrição é definida da seguinte forma:
   
   sum[(i,j) in C[k]] c[i,j]*x[i,j] <= 4.5 * ( sum[(i,j) in C[k]] c[i,j] ) / | C[k] | para k in 1..piso(n/2)
  
   Por exemplo, esta restrição para a instância vista abaixo terá as seguintes desigualdades:
   
   Para k = 1:
     c[1,1]*x[1,1] + c[1,2]*x[1,2] + c[1,3]*x[1,3] + c[1,4]*x[1,4] + c[1,5]*x[1,5] +
     c[2,5]*x[2,5] + c[3,5]*x[3,5] + c[4,5]*x[4,5] + c[5,5]*x[5,5] +
     c[5,4]*x[5,4] + c[5,3]*x[5,3] + c[5,2]*x[5,2] + c[5,1]*x[5,1] +
     c[4,1]*x[4,1] + c[3,1]*x[3,1] + c[2,1]*x[2,1] <=
     4.5 * (
           c[1,1] + c[1,2] + c[1,3] + c[1,4] + c[1,5] +
           c[2,5] + c[3,5] + c[4,5] + c[5,5] +
           c[5,4] + c[5,3] + c[5,2] + c[5,1] +
           c[4,1] + c[3,1] + c[2,1]
         ) / 16; 


   Para k = 2:
     c[2,2]*x[2,2] + c[2,3]*x[2,3] + c[2,4]*x[2,4] +
     c[3,4]*x[3,4] + c[4,4]*x[4,4] + 
     c[4,3]*x[4,3] + c[4,2]*x[4,2] +
     c[3,2]*x[3,2] <=
     4.5 * (
           c[2,2] + c[2,3] + c[2,4] +
           c[3,4] + c[4,4] + 
           c[4,3] + c[4,2] +
           c[3,2]
         ) / 8;   
        
   Mesmo conhecendo as restrições específicas para esta instância,
   sugiro que você implemente uma rotina que gere estas restrições para qualquer valor de n.
   
   Por fim, para compilar execute o comando : gcc itenssquare.c -o itenssquare -lglpk -lm    
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glpk.h>

// Instância de teste
// Custo ótimo 14.5 (sem as restrições adicionais)
// Custo ótimo 16.2 (considerando mais as restrições das diagonais)
// Custo ótimo 17.5 (considerando mais as restrições das diagonais e custo médio)

// Quantidade de itens em cada linha e coluna
int n = 5;

// Custo de cada item
double c[5][5] = { {2.0, 0.8, 2.1, 2.0, 2.7},
                   {0.3, 2.1, 1.7, 2.3, 0.5},
                   {0.2, 0.3, 2.4, 3.0, 1.4},
                   {1.0, 3.5, 1.0, 4.0, 2.3},
                   {1.3, 1.0, 2.3, 0.5, 0.5} };
                        
// Peso de cada item                        
double w[5][5] = { {1.0, 0.5, 1.3, 0.7, 1.5},
                   {2.1, 2.4, 1.1, 0.5, 0.7},
                   {1.0, 1.3, 0.5, 2.0, 2.3},
                   {0.2, 2.0, 1.0, 1.6, 1.2},
                   {1.5, 0.3, 1.0, 3.0, 3.3} };

// Limite superior de cada linha                        
double u[5] = {3.1, 2.5, 2.7, 3.9, 2.2};

// Limite inferior de cada coluna
double l[5] = {1.2, 0.7, 2.0, 3.1, 1.2};

int main()
{
   // Variáveis auxiliares
   int i, j, constraintNumber, *constraintIndices;
   double *constraintCoefficients;

   // Aloca os vetores utilizados para criar as restrições do problema
   // ***********************************************************************************************
   // ATENÇÃO ===> É importante dizer que estes vetores serão utilizados da posição 1 em diante
   //              Ou seja, no GLPK você aloca uma posição a mais e descarta a posição 0 dos vetores.
   // ***********************************************************************************************
   constraintIndices = (int*)malloc((n+1)*sizeof(int));
   constraintCoefficients = (double*)malloc((n+1)*sizeof(double));

   // Cria um modelo com nenhuma variável e nenhuma restrição
   glp_prob *model = glp_create_prob();

   // Define o sentido da otimização que, para este problema, é minimização
   glp_set_obj_dir(model, GLP_MIN);

   // Cria as variáveis (colunas) no modelo
   // Para este problema são necessárias n*n variáveis
   // Estas n*n variáveis são definidas pelo GLPK através dos indices que vão de 1 até n*n (x1, x2, ..., x(n*n))
   // Portanto, neste momento é importante determinar qual variável no GLPK (índice) representará qual variável x[i,j]
   // Para tanto, fazemos o mapeamento das variáveis x[i,j] utilizando a fórmula (i-1)*n + j
   // Isto é, a variável x[i,j] será representada pela variável de índice (i-1)*n + j no modelo do GLPK
   // Note que é imprescindível que cada índice (variável do GLPK) seja associado a no máximo uma variável x[i,j]
   // Caso contrário, uma variável do GLPK pode representar duas variáveis x[i,j] diferentes que assumem valores distintos nas soluções ótimas
   // Neste caso, o modelo estará incorreto
   glp_add_cols(model, n*n);
   
   // Ajuste dos tipos, limitantes e coeficientes da função objetivo das variáveis do modelo
   
   for (i = 1; i <= n; i++)
      for (j = 1; j <= n; j++)
      {
         // Define o tipo da variável como sendo binária (em outros modelos poderia ser contínua (GLP_CV) ou inteira (GLP_IV))
         glp_set_col_kind(model, (i-1)*n + j, GLP_BV);
         
         // Define o limitante inferior (0) e superior (1) da variável
         // Consultem no manual as outras forma para definir apenas o limitante inferior ou superior
         glp_set_col_bnds(model, (i-1)*n + j, GLP_DB, 0.0, 1.0);

         // Define o coeficiente da variável na função objetivo
         glp_set_obj_coef(model, (i-1)*n + j, c[i-1][j-1]);
      }
      
   // Cria no modelo 2n restrições (linhas) nulas (com os coeficientes e limitantes zerados)
   // Ou seja, neste momento é criada uma matriz de zeros que correspondem aos coeficientes das restrições do modelo
   // O próximo passo será modificar esta matriz de tal forma que ela represente as restrições do problema descrito
   glp_add_rows(model, 2*n);

   // Esta variável define qual das restrições (qual linha da matriz de coeficientes) estamos modificando
   // ************************************************************************************************************
   // ATENÇÃO: perceba que as restrições (linhas), assim como as variáveis (colunas), são indexadas a partir de 1.
   // ************************************************************************************************************
   constraintNumber = 1;
   
   // Preenchimento das restrições limitando a soma das linhas:  
   // sum{j in 1..n} w[i,j]*x[i,j] <= u[i] para i in 1..n

   for (i = 1; i <= n; i++)
   {
      // Define o limite superior (RHS) da restrição      
      glp_set_row_bnds(model, constraintNumber, GLP_UP, 0.0, u[i-1]);

      for (j = 1; j <= n; j++)
      {
         // Ajusta o índice da variável que será informado à rotina do GLPK
         constraintIndices[j] = (i-1)*n + j;
         
         // Ajusta o coeficiente da variável cujo índice foi definido na linha anterior para ser informado ao GLPK
         // ******************************************************************************************************
         // ATENÇÃO: perceba que na matriz w os índices e colunas são indexados a partir de ZERO !
         // ******************************************************************************************************
         constraintCoefficients[j] = w[i-1][j-1];
      }

      // Passa ao GLPK a restrição que acabou de ser definida nos vetores constraintIndices e constraintCoefficients
      glp_set_mat_row(model, constraintNumber, n, constraintIndices, constraintCoefficients);
      
      // atualiza o indice da próxima restrição a ser inserida
      constraintNumber++;
   }
 
   // Preenchimento das restrições limitando a soma das colunas: 
   //    sum{i in 1..n} w[i,j]*x[i,j] >= l[i] para j in 1..n   
   for (j = 1; j <= n; j++)
   {
      // Define o limite inferior (RHS)  da restrição      
      glp_set_row_bnds(model, constraintNumber, GLP_LO, l[j-1], 0.0);

      for (i = 1; i <= n; i++)
      {
         // Ajusta o índice da variável que será informado a rotina do GLPK
         constraintIndices[i] = (i-1)*n + j;
         
         // Ajusta o coeficiente da variável cujo índice foi definido na linha anterior para ser informado ao GLPK
         constraintCoefficients[i] = w[i-1][j-1];
      }

      // Passa ao GLPK a restrição que acabou de ser definida nos vetores constraintIndices e constraintCoefficients
      glp_set_mat_row(model, constraintNumber, n, constraintIndices, constraintCoefficients);
      
      // atualiza o indice da próxima restrição a ser inserida
      constraintNumber++;
   } 
 
   // Define os parâmetros que serão passados ao resolvedor   
   glp_iocp param;
   glp_init_iocp(&param);
   
   // Ativa o presolver
   param.presolve = GLP_ON;

   // Resolve o modelo
   int status = glp_intopt(model, &param);

   // Verifica se houve algum erro durante a otimização
   if (status)
   {
      printf("Ocorreu um erro durante o processo de otimizacao.\n");
   }
   else
   {
      // Verifica se o método encontrou uma solução
      
      status = glp_mip_status(model);
      
      if ((status == GLP_OPT) || (status == GLP_FEAS))
      {
         // Imprime a solução encontrada
         
         if (status == GLP_OPT)
            printf("Solucao otima encontrada!\n");
         else
            printf("A solucao encontrada pode nao ser otima!\n");
      
         printf("Custo da solucao: %f\n", glp_mip_obj_val(model));

         for (i = 1; i <= n; i++)
         {
            for (j = 1; j <= n; j++)
               printf("%f ", glp_mip_col_val(model, (i-1)*n + j));
         
            printf("\n");
         }
      }
      else
      {
         printf("Nenhuma solucao foi encontrada!\n");
      }
   }


   // Desaloca os vetores
   free(constraintIndices);
   free(constraintCoefficients);

   return 0;
}


