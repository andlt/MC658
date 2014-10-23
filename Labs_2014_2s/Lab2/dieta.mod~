
/* Lab 2 MC658 Projeto e Análise de Algoritmos III 
   2o semestre de 2014 
   Prof. Cid C. de Souza (IC-UNICAMP) 
   PED: Lucas Oliveira 
   Para rodar dar o comando : glpsol --model <arquivo>.mod --data <arquivo de cenario> 
   
   Nome do aluno: André Luís de Lannoy C Tavares
   RA...........: 116125

*/

/* ===================================== */
/* declarações de conjuntos e parâmetros */
/* ===================================== */

set A; /* tipos de alimentos */
set N; /* nutrientes */
set D; /* dias */

param e{n in N, d in D}, >= 0; /* quantidade mínima do nutriente n necessária no dia d */
param f{n in N, d in D}, >= 0; /* quantidade máxima do nutriente n possível no dia d */

param r{a in A, n in N}, >= 0; /* quantidade do nutriente n fornecida pela porção do alimento a */

param c{a in A}, >= 0; /* custo da porção do alimento a */

param q{a in A}, >= 0; /* quantidade de gramas da porção do alimento a */

param v{a in A}, >= 0; /* volume da porção do alimento a ocupado no refrigerador */

param k >= 0; /* consumo máximo diário em gramas */

param t >= 0; /* volume disponível no refrigerador */

/* ======================================== */
/* Implemente o seu modelo daqui para baixo */
/* ======================================== */

/* --------- */
/* variáveis */
/* --------- */

var x{a in A, d in D} >= 0;

/* --------------- */
/* função objetivo */
/* --------------- */

minimize custo: sum{a in A, d in D} c[a] * x[a,d];

/* ---------- */
/* restrições */
/* ---------- */

/* Respeito aos valores mínimos da dieta */

s.t. min_val{n in N, d in D}: e[n, d] <= sum{a in A} x[a, d] * r[a, n];

/* Respeito aos valores máximos da dieta */

s.t. max_val{n in N, d in D}: f[n, d] >= sum{a in A} x[a, d] * r[a, n];

/* Dieta balanceada */

s.t. bal{a in A}: sum{d in D} x[a, d] / card(D) >= sum{a2 in A, d in D} x[a2, d] / (2*card(A)*card(D));

/* Consumo máximo diário */

s.t. max_cons{d in D}: sum{a in A} x[a, d] * q[a] <= k;

/* Restrição de volume diário no refrigerador */

s.t. max_vol{d in D}: sum{a in A} x[a, d] * v[a] <= t;


/* =========================================== */
/* comando para resolver o modelo : NÃO MUDE ! */
/* =========================================== */
solve;

/* =================================== */
/* comando de impressão do valor ótimo */
/* =================================== */
printf "Custo total: %.3f\n\n", custo;

end;


