
# Informações:

Esse shell foi programado, executado e testado em Linux;


# Arquitetura

## include/

* pasta que contem todos os arquivos headers do projeto.

### Arquivos contidos:

1. utilities.h
> *_Contem a declaração das funções utilitarias que não tem envolvimento direto com o projeto, podendo ser ultilizados em situações adiversas_*

2. shell.h
> *_Contem a declaração das funções que são primordiais para a execução do projeto_*

## src/

* pasta que contem todos os arquivos de implementação do projeto.

### Arquivos contidos:

1. main.c
> *_Contem a implementação da função main do projeto_*


2. shell.c
> *_Contem a implementação de todas as funções primordiais para a execução do shell_*

3. utilities.c
> *_Contem a implementação de funções que podem ser ultilizadas em situações gerais_*

# Implementação

## Como compilar?

* Para compilar o shell, basta digitar:
~~~
make
~~~

## Como Executar?

* Para fazer a execução do shell, basta compila-lo e digitar:
~~~
./shell
~~~

