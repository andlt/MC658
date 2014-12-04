# MC658 - Projeto e Análise de Algoritmos III
# Professor: Cid C. de Souza 
# PED: Lucas Oliveira (autor do modelo abaixo)
# 2º semestre de 2014
# 
# modelo GLPK do problema da  árvore de  Steiner
# para rodar (gravando arquivo .lp): glpsol --model labs-prp.mod --data <instancias> --output a --wcpxlp a.lp

param n, integer, >= 3;

set V := 0..(n-1);

set E, within V cross V;

set T, within V;

param c{(i,j) in E};

param t0;

set T2 := setof{v in T: v != t0} v;

var x{(i,j) in E}, binary;

var y{t in T2, (i,j) in E}, >= 0;

minimize total: sum{(i,j) in E} c[i,j] * x[i,j];

s.t. flow1{t in T2}: sum{j in V: j != t0} y[t,j,t0] - sum{j in V: j != t0} y[t,t0,j] = -1;

s.t. flow2{t in T2}: sum{j in V: j != t} y[t,j,t] - sum{j in V: j != t} y[t,t,j] = 1;

s.t. flow3{t in T2, i in V: i != t0 and i != t}: sum{j in V: i != j} y[t,j,i] - sum{j in V: i != j} y[t,i,j] = 0;

s.t. arc{t in T2, i in V, j in V: i != j}: x[i,j] >= y[t,i,j];

solve;

printf "Tree:\n";
printf{(i,j) in E: x[i,j] > 0} "       %3d       %3d       %8g     %8g\n", i, j, c[i,j], x[i,j];
printf "Optimal tree has cost %f\n", total;

end;
