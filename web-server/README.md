# Servidor Web Multi-Cliente

Este projeto consiste em um servidor web capaz de lidar com múltiplos clientes simultaneamente, implementando quatro estratégias diferentes para manipulação concorrente: interativo, por fork, por select e thread com fila de tarefas.

## Como Executar


### Compilação e Execução

Clone o repositório e navegue até o diretório do projeto:

```bash
git clone https://github.com/seu-usuario/servidor-multi-cliente.git
cd servidor-multi-cliente
```

Execute o comando `make` para compilar o código:

```bash
make
```

Agora, você pode executar o servidor, especificando a porta e a estratégia desejada:

```bash
./server <porta> <estrategia>
```

Substitua `<porta>` pelo número da porta desejado e `<estrategia>` pela estratégia desejada. As estratégias disponíveis são:

- `interativo` (1): Manipulação interativa de clientes.
- `fork` (2): Manipulação por criação de processos filho (fork).
- `select` (3): Manipulação usando a função select para entrada/saída.
- `thread` (4): Manipulação por meio de threads com uma fila de tarefas.

## Exemplos

### Executar servidor na porta 8080 com a estratégia de fork:

```bash
./server 8080 2
```

### Executar servidor na porta 8000 com a estratégia de thread:

```bash
./server 8000 4
```

## Licença

Este projeto está licenciado sob a licença MIT - consulte o arquivo [LICENSE](LICENSE) para obter detalhes.
```

Certifique-se de personalizar as instruções e os detalhes de execução de acordo com a estrutura específica do seu projeto e as escolhas de implementação.
