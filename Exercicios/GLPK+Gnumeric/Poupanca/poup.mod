# Problema da poupança
#
# Este problema encontra a combinação ideal de fundos de investimento no qual 
# deve-se investir dadas algumas regras (ver slide 4 de pl-intro)

set F;
/* fundos de investimento */

param ra{f in F};
/* rendimento anual do fundo i em porcentagem */

param pm{f in F};
/* período de maturação do fundo j. 0 = menos de 10 anos, k = 10 ou mais anos */

param c{f in F};
/* classificação do fundo k 0 = bom ou aceitável, 1 = muito bom o excelente */

param val;
/* valor a ser investido */

param prop_unic;
/* proporção máxima que pode ser investida em um único fundo */

param prop_10;
/* proporção mínima que precisa ser investida em fundos com pelo menos 10 anos de maturação */

param prop_clas;
/* porporção mínima que precisa ser investida em fundos com classificação muito boa ou melhor */

var x{f in F} >= 0;
/* valor investido em cada fundo */

maximize rend: sum{f in F} x[f] * ra[f];

s.t. x{f in F}: x[f] <= val * prop_unic
/* No máximo prop_unic por cento do valor pode ser investido em um único fundo */

s.t. sum{f in F} x[f] * c[f] >= val // * prop_10
/* Pelo menos prop_10 por cento precisa ser investido em fundos com 10 anos ou mais */

s.t. sum{f in F} x[f] * c[f] >= val // * prop_clas
/* Pelo menos prop_clas por cento precisa ser investido em fundos com classificação muito boa ou superior */

s.t. sum{f in F} x[f] = val
/* todo o valor deve ser investido */

solve;

end;
