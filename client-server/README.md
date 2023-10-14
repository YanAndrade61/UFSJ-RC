**Blackjack em C**

Este programa implementa um jogo de blackjack em C, utilizando o protocolo cliente-servidor. O jogo permite que seja inicializada uma sala com **n** jogadores, e cada cliente pode conectar-se a ela. Ao fim da partida, os clientes que quiserem podem continuar a jogar, o servidor fecha quando não houver mais nenhum jogador.

**Regras**

* Os jogadores começam com duas cartas, sendo que os números valem os números e as figuras K,J,Q valem 10, e A vale 1.
* No turno do jogador, esse pode comprar **n** cartas, sendo que seu turno acaba quando opta por parar ou estoura o limite de 21.
* Ao final de todos os turnos, o servidor apresenta o placar, e informa aos clientes se eles ganharam ou perderam. Como existem possibilidades de empate, como dois jogadores com 21, mais de um pode ganhar.

**Como jogar**

Para jogar, siga estas etapas:

1. Inicie o servidor executando o comando `./server <numero de jogadores>`.
2. Conecte-se ao servidor executando o comando, caso esteja no servidor use **127.0.0.1**: `./client <endereço IP do servidor>`.
3. Espere até todos jogadores conectarem-se.
4. Continuar comprando cartas até decidir parar ou estourar o limite de 21.
5. Ao fim, aparecerá uma mensagem indicando se ganhou, perdeu ou empatou.
6. É possível verificar o placar final no servidor.

**Compilar**

Para compilar o programa, execute o makefile: `make`
