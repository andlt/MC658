# modelo GLPK do problema da CPFL (ver slides)
# para exemplos ver: /usr/share/doc/glpk-utils/examples
# para ver manual do mathprog, ver: /usr/share/doc/glpk-doc/gmpl.pdf
# para rodar: glpsol --model minas.mod --data <arquivo de cenario> --output minas.opt --wcpxlp minas.lp 
# (Nota: o comando cima pode ser executado executando o arquivo "roda" neste diretorio)

/* declaracoes de conjuntos e parametros */

set MINAS;
set SUBPRODUTOS;
set NOCIVOS;

/* param z; */ 

param tabela{i in MINAS, j in SUBPRODUTOS};

param limites{j in NOCIVOS};

/* ===> variaveis: proporcao do carvao de cada mina na mistura */
var x{i in MINAS}, >= 0;

/* ===> funcao objetivo */
maximize energia: sum{i in MINAS, j in SUBPRODUTOS diff NOCIVOS } tabela[i,j]*x[i];

/* ===> restricoes: limita a producao de enxofre e de poeira da mistura */
s.t. restricaoambiental{j in NOCIVOS}: 
   sum{i in MINAS} tabela[i,j]*x[i] <= limites[j];

/* ===> restricao: limita a proporcao de cada tipo de carvao na mistura */
s.t. proporcao: sum{i in MINAS} x[i] = 1.0;

/* ===> resolve */
solve;

/* ===> impressao de resultados */

/* mostra valor ótimo */
printf: "\n* Energia Maxima = %.2f \n", sum{i in MINAS, j in SUBPRODUTOS diff NOCIVOS } tabela[i,j]*x[i] > "saida.txt";

/* mostra proporcao de cada tipo de carvao */
printf: "\n* Proporcao dos tipos de carvao: \n" >> "saida.txt";
printf{i in MINAS}: "%12s %10.2f\n",i, x[i] >> "saida.txt";

/* mostra producao de cada subproduto nocivo */
printf: "\n* Produtos nocivos gerados:\n" >> "saida.txt";
printf{j in NOCIVOS}: "%12s %10.2f\n",j,sum{i in MINAS} tabela[i,j]*x[i] >> "saida.txt";

printf: "\n That's all folks !\n" >> "saida.txt";

end;

  