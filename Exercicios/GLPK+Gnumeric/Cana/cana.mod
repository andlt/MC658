# modelo GLPK do problema de transporte da cana (slides MC658)
# para exemplos ver: /usr/share/doc/glpk-utils/examples
# para ver manual do mathprog, ver: /usr/share/doc/glpk-doc/gmpl.pdf
# para rodar: glpsol --model cana.mod --data <arquivo de cenario> --output cana.opt --wcpxlp cana.lp 
# (Nota: o comando cima pode ser executado executando o arquivo "roda" neste diretorio)

/* declaracoes de conjuntos e parametros */

param f, integer, >= 1; /* quantidade de fazendas */
param u, integer, >=1;  /* quantidade de usinas */

set FAZENDAS := {1..f}; /* conjunto de indices de fazendas */
set USINAS :={1..u};    /* conjunto de indices de usinas */

set ESTRADAS, within FAZENDAS cross USINAS;

param distancia{(i,j) in ESTRADAS};

param producaoFazenda{i in FAZENDAS};

param capacidadeUsina{j in USINAS};

/* ===> variaveis: quantidade transportada entre as fazendas e as usinas.  */
var x{(i,j) in ESTRADAS}, >= 0;

/* ===> funcao objetivo */
minimize custo: 
     sum{(i,j) in ESTRADAS} distancia[i,j]*x[i,j];

/* ===> restricoes: limita a producao escoada das fazendas */
s.t. escoamento{i in FAZENDAS}: 
     sum{j in USINAS} x[i,j] <= producaoFazenda[i];

/* restricao: usinas trabalham na sua capacidade maxima ou processam o que foi produzido */
s.t. escoamentoXprocessamento: 
     sum{(i,j) in ESTRADAS} x[i,j] = min(sum{k in FAZENDAS}producaoFazenda[k],sum{l in USINAS} capacidadeUsina[l]);

/* ===> restricao: limita o processamento das usinas */
s.t. processa{j in USINAS}: 
     sum{(i,j) in ESTRADAS} x[i,j] <= capacidadeUsina[j];

/* ===> resolve */
solve;

/* ===> impressao de resultados */

/* mostra valor ótimo */
printf: "\n* Custo = %.2f \n", sum{(i,j) in ESTRADAS} distancia[i,j]*x[i,j] > "saida.txt";

/* mostra valor transportado entre cada par (fazenda, usina)  */
printf: "\n* Valores transportados: \n" >> "saida.txt";
printf{(i,j) in ESTRADAS}: "x[%d,%d]=%.2f\n",i,j,x[i,j] >> "saida.txt";

/* folga do escoamento em relacao a producao nas fazendas */
printf: "\n*Folga do escoamento das fazendas\n" >> "saida.txt";
printf{i in FAZENDAS}: "Fazenda %d: %10.2f\n",i,producaoFazenda[i]-sum{(i,j) in ESTRADAS}x[i,j] >> "saida.txt";

/* folga no processamento em relacao a capacidade das usinas */
printf: "\n*Folga no processamento das usinas\n" >> "saida.txt";
printf{j in USINAS}: "Usina %d: %10.2f\n",j,capacidadeUsina[j]-sum{(i,j) in ESTRADAS}x[i,j] >> "saida.txt";

printf: "\n That's all folks !\n" >> "saida.txt";

/* impressao dos dados de entrada para simples conferencia */
printf{i in FAZENDAS}: "producaoFazenda[%d]=%.2f\n",i,producaoFazenda[i] > "safe.txt";
printf{j in USINAS}: "capacidadeUsina[%d]=%.2f\n",j,capacidadeUsina[j] >> "safe.txt";
printf{(i,j) in ESTRADAS}: "distancia[%d,%d]=%.2f\n",i,j,distancia[i,j] >> "safe.txt";

end;

  