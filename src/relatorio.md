
# Relatorio


## Objetivo:

O meu shell implementa a execução interativa e por meio de batch, aceita mais de um comando por linha de comando, contem estilos de execução, contem suporte para pipes, contem redirecionamento de entrada e saida, contem suporte para mudança de diretorios e contem execução em background, suportando o comando fg.

## Checklist do enunciado

* [Modo interatico e de batch] OK
* [Mais de um comando executado por linha de comando] OK
* [Estilos de execução] OK
* [Pipe] Ok
* [Redirecionamento] OK
* [cd] OK
* [Execução em background] Ok

__Observações__

## Como Reproduzir

### Compilar:

```
make clean
make
```

### Executar:

```
./shell 
```
ou
```
./shell comandos
```

# Arquitetura e decisões tecnicas

## Arquivos:

* main.c: Arquivo principal do projeto, que contem a função main, que é executada.

* shell.c/.h: Arquivos que contem as funções de execução do shell.

* utilities.c/.h: Arquivos que contem funções de multiplos usos.

## Invariantes e decisões:

Decidi separar os arquivos do meu projeto dessa forma, pois em primeira instancia fiz tudo em um arquivo apenas, porem, a organização do projeto começou a ficar muito confusa devido a mistura de funções.

# Linha do tempo da semana:

### Dia 2

#### implementação do Input:

##### Estrategia 1 input de array estatico:

Usar um array estatico para armazenar o comando.

![imagem de exemplo do meu codigo com getline](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_155004.png)

__Tentativa 1__

- Oque tentei: Um input com mais de 128 caracteres

- Input:0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

- Oque aconteceu: o input foi repartido em partes pois o array não teve capacidade de comporta-lo e o resto foi lido no proximo while.

![imagem de exemplo do teste com array estatico](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_162240.png)

##### Estrategia 2 input de array dinamico:

Ideia inicial de ler um input em um array estatico de ate 128 caracteres era limitante.  
Ate que com alguma pesquisa descobri que a função getline faz o realloc automatico  
do array dinamico. 

![imagem de exemplo do meu codigo com getline](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_123613.png)

#### implementação do parser do comando:

##### Estrategia 1 strtok para separar os argumentos:

Com o uso da função strtok() separei os comandos dos argumentos.  
Um detalhe que pode passar desapercebido é que a função adiciona  
um elemento a mais a lista de argumentos com valor de NULL.  

![imagem de exemplo do meu codigo com strtok](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_154353.png)

#### implementação da execução dos comandos 

##### Estrategia 1 fork com execvp:

If e else no começo da função para identificar se é um comando builtin.  
O fork para criar uma copia do processo pai.  
O execvp para executar o comando no terminal.

![imagem de exemplo do meu codigo com fork e execvp](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_155537.png)

![imagem de exemplo do meu resultado do dia 2](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_160957.png)

### Dia 3

#### Implementação dos multiplos comandos (`;`)

##### Estrategia 1 refatoramento do SliceInput

Com a função SliceInput ja existente que fatiava o comando de acordo com os espaços  
fiz algumas alteraçãoes no seu funcionamento e criei uma função generica `SliceStr()`.
Essa função por sua vez foi usada para separar os comandos.

![imagem de exemplo do meu codigo do sliceStr](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_172717.png)

#### Implementação do Pipe (`|`)

##### Estratégia 1 redirecionar o stdin/stdout com pipe() e dup2()

Caso o comando atual não for o último do pipeline, o pai cria um novo pipe de comunicação usando `pipe()` se duplica o com `fork()`.
* O filho usa `dup2()` para substituir a sua saída padrão pela entrada do pipe (`fileDescriptions[1]`).
* O pai armazena a referência de leitura desse pipe (`fileDescriptions[0]`) e passa para o próximo `fork()`, para que o próximo filho leia dali em vez do `stdin` padrão.

![imagem de exemplo do meu ExecutePipeline](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_164311.png)

![imagem de exemplo do resultado do dia 3](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_174446.png)

### Dia 4

#### Estrategia 1 Implementação dos redirecionamento de output

Criei uma nove função que executa logo depois de dividir os comandos para checar se a algum tipo de redirecionamento, assim mudando temporariamente o stdin e stdout do pai, que continuara seu fluxo naturalmente.

![imagem de exemplo do resultado do dia 3](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_183321.png)

__Tentativa 1__

- Oque tentei: dar echo para um arquivo teste "teste.txt"

- Input: echo teste > teste.txt  

- Oque aconteceu: Por ter dado espaços depois do nome do arquivo ele criou um arquivo com espaços no nome.

![imagem de exemplo do resultado da redirecionamento de output](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_184016.png)

#### Estrategia 2 Implementação dos redirecionamento com Trim

Depois do erro decidi, limpar nome do arquivo andes de redirecionar.

![imagem de exemplo do resultado da redirecionamento de output](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_184159.png)

__Tentativa 1__

- Oque eu tentei: dar echo para um arquivo teste "teste.txt"
 
- Input: echo teste > teste.txt

- Oque aconteceu: a primeira letra do nome ficou repetida

![imagem de exemplo do meu codigo do meu trim não funcional](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_184604.png)

#### Estrategia 3 Refatoramento do Trim

Repensei a logica do meu trim e cheguei em uma função funcional

![imagem de exemplo do codigo do meu trim funcional](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_185005.png)

![imagem de exemplo do meu trim funcionando](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_185318.png)

### Dia 5

#### Estrategia 1 implementação do redirecionamento de output com append

Para fazer o output com append adicionei um verificar na função de analisar se redirecionamento de output para checar se é append

![imagem de exemplo da minha função de redirecionamento com append](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_190102.png)

#### Estrategia 1 implementação do redirecionamento do input

Para fazer a checagem do redirecionamento de input eu apenas copiei a função do output, removendo a seção de checagem de append.

![imagem com a função de checagem de redirecionamento de input](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_190319.png)

#### Estrategia 1 implementação da execução em background

Criando uma função para checar a string do comando e retornar se é em background ou não, eu posso executar uma tabela que armazenara os processos em execução.

![imagem com a função de checagem de background](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_192218.png)

#### Estrategia 1 implementando a tabela de processos em background

Com algumas funções para adicioanr e remover processos, eu posso usar a tabela para criar varios processos em background.

![imagem com as funções de manipulação da tabela](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_192607.png)

![imagem com a continuação das funções da tabela](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_195146.png)

__Tentativa 1__

- O que eu tentei: rodar alguns comandos como sleep em background

- Input: 
```
sleep 10 &

sleep 1 &
```

Oque aconteceu: Os avisos que os processos foram concluidos so aparecem depois do usuario dar algum input

![imagem com a continuação das funções da tabela](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_195431.png)

#### Estrategia 2 Checando processos concluidos com sigaction

Como o sistema operacional lança para o processo pai um aviso quando um processo filho terminal  
eu posso usar esse sinal para avisar na tela o termino do processo

![imagem com o codigo do handlesigchld](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_200823.png)

![imagem com o codigo handle sendo implementado na main](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_200920.png)

__Tentativa 1__

- O que eu tentei: rodar um commando sleep em background

- Input: ``` sleep 1 & ```

- Oque aconteceu, o comando rodou em background mas assim que ele terminou começou a aparecer muitos avisos na tela como se o comando se multiplica-se.

![imagem com o erro de duplicação de comando](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_200334.png)

#### Estrategia 3 Limpando o getline depois dele ter sido interrompido

Depois do getline ser interrompido é preciso limpar os seus erros para que ele retorne a receber inputs.

![imagem com o codigo do GetInput atualizado](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_202756.png)

![imagem com o codigo funcionando](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_202702.png)

### Dia 6

#### Estrategia 1 Implementando a funcionalidade por prompt de arquivo

Para pegar os inputs de um arquivo apenas fiz verificações se um arquivo for incluido e fiz uma iteração entre todas as linhas do arquivo

![imagem com o codigo do prompt por arquivo](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_203945.png)

![imagem com o codigo do prompt por arquivo funcionando](https://github.com/VitorcSA/Shell/raw/main/media/images/Captura_de_tela_20260307_204026.png)
