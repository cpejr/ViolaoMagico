-

### O Violão Mágico:

O violão mágico consiste em um equipamento que possibilita que pessoas que têm problemas de mobilidade no braço direito possam tocar violão. O equipamento possui alguns componentes, como por exemplo Display, motores, etc. Através de um computador, o usuário insere no Cartão SD um arquivo .txt com algumas informações que correspondem aos motores que devem ser acionados correspondentes a cada corda, velocidade da música, subdivisões, etc.

### Princípios de funcionamento:

- Cada motor está acoplado a uma corda, para tocar essa corda, basta que o motor gire. O sentido do giro depende da posição atual do eixo.

- Inicialmente, assim que o violão for ligado, aparecerá no display um menu de configurações com três opções: Músicas, Afinar e Resetar motores.

- Caso o usuário deseje tocar uma música ele deverá selecionar a opções músicas e em seguida aparecerá no display o nome de todos os arquivos do cartão SD.

- Caso o usuário queira resetar os motores ou afinar alguma corda, ele deve selecionar a opção desejada e em seguida a corda que a operação deverá ser feita.

- Uma vez encontrada a música desejada, o usuário deverá selecioná-la e em seguida clicar em tocar. Feito isso, o cartão SD lerá a primeira batida e a função ‘parseFile’ será chamada, passando esse arquivo como parâmetro.

- Caso nada seja feito, a primeira batida será tocada infinitamente.

- Se o usuário desejar parar a batida, ele deverá apertar o botão “Selecionar”.

- Caso se queira mudar de batida, em qualquer instante da música, ele deverá apertar o botão de cima ou de baixo . Uma vez feito isso, o violão irá apenas terminar de tocar a batida atual e irá para a próxima.
