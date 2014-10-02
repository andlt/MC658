# Problema da Termoelétrica
#
# Este problema encontra quanto de cada tipo de carvão deve ser utilizado por uma termoelétrica para maximizar a geração de energia sem estourar os limites de enxofre e poeira.

set M;
/* Minas produtoras de carvão */

set SP
/* Subprodutos das minas */

set N
/* Subprodutos nocivos */

/*param e {m in M};*/
/* quantidade de enxofre produzida por cada tipo de carvão (ppm) */

param e_max;
/* quantidade máxima de enxofre que pode ser gerada */

/*param p {m in M};*/
/* quantidade de poeira produzida por cada tipo de carvão (kg) */

param p_max;
/* quantidade máxima de poeira que pode ser gerada */

/*param v {m in M};*/
/* quantidade de vapor de água (energia) produzida por cada tipo de carvão (kg) */

param tabela {m in M, sp in SP};

var x {m in M};
/* quantidade utilizada de cada tipo de carvão */

maximize energia: sum {m in M} v[m] * x[m];
/* energia total gerada */

s.t. enxofre: sum {m in M} e[m] * x[m] <= e_max;
/* quantidade de enxofre gerada abaixo do total permitido */

s.t. poeira: sum {m in M} p[m] * x[m] <= p_max;
/* quantidade de poeira gerada abaixo do total permitido */

solve;

end;

