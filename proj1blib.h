#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void printaDiv() {
    printf("\n");
    for (int i = 0; i < 50; i++) {
        printf("=");
    }
    printf("\n");
}

//* TAREFA
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

//Retorna true se a data comparada for mais antiga que a data base
bool dataMenor(Data data_comparada, Data data_base) {
    if (data_comparada.ano < data_base.ano) return true;
    else if (data_comparada.mes < data_base.mes) return true;
    else if (data_comparada.dia < data_base.dia) return true;
    return false;
}

typedef struct{
    int codigo;
    char nome[30];
    char projeto[30];
    Data inicio;
    Data termino;
    int status;
    int prioridade;
    bool concluida_com_atraso;
} Tarefa;

//*NO
typedef struct No{
    Tarefa * tarefa;
    struct No * proximo_no;
} No;

//*LISTA
typedef No Lista;

//Cria uma nova lista sendo nula
Lista * criaLista() {
    return NULL;
}

//Se a lista estiver vazia, retorna true
bool vaziaLista(Lista * lista) {
    if (lista == NULL) return true;
    return false;
}

//Cria um novo nó com uma dada tarefa e o insere na posição ordenada pela data de término
void insereLista(Lista ** lista, Tarefa * tarefa) {
    No * novo_no = (No *) malloc(sizeof(No));
    novo_no->tarefa = tarefa;
    novo_no->proximo_no = NULL;
    Lista * no_atual = *lista;

    //Insere no início
    if (vaziaLista(no_atual) || (tarefa->prioridade < no_atual->tarefa->prioridade) ||
       (tarefa->prioridade == no_atual->tarefa->prioridade && dataMenor(tarefa->termino, no_atual->tarefa->termino))) {
        novo_no->proximo_no = no_atual;
        (*lista) = novo_no;
    }
    //Insere da posição 1 pra frente
    else {
        while(no_atual->proximo_no != NULL && (tarefa->prioridade > no_atual->proximo_no->tarefa->prioridade ||
                                               (tarefa->prioridade == no_atual->proximo_no->tarefa->prioridade &&
                                                dataMenor(tarefa->termino, no_atual->proximo_no->tarefa->termino)))) {
            no_atual = no_atual->proximo_no;
        }

        novo_no->proximo_no = no_atual->proximo_no;
        no_atual->proximo_no = novo_no;
    }
}

//printa todos os detalhes da tarefa
void printaTarefa(Tarefa tarefa) {
    printaDiv();
    printf("TAREFA \"%s\":\n\n", tarefa.nome);
    printf("Projeto: \"%s\";\n", tarefa.projeto);
    printf("Data de Inicio: %02d/%02d/%04d;\n", tarefa.inicio.dia, tarefa.inicio.mes, tarefa.inicio.ano);
    printf("Data de Termino: %02d/%02d/%04d;\n", tarefa.termino.dia, tarefa.termino.mes, tarefa.termino.ano);
    printf("Status: %d;\n", tarefa.status);
    printf("Prioridade: %d;\n", tarefa.prioridade);
    printf("Codigo: %d;", tarefa.codigo);
    printaDiv();
}

//Printa todos os códigos da lista seguindo a ordem
void printaLista(Lista * lista) {
    if (!vaziaLista(lista)) {
        while (!vaziaLista(lista)) {
            printaTarefa(*(lista->tarefa));
            lista = lista->proximo_no;
        }
    }
    else printf("\nEsta lista está vazia.\n");
}

//*FILA
typedef struct {
    No * inicio;
    No * fim;
} Fila;

typedef struct {
    Fila *filas[3]; // Três filas para cada prioridade
    int tamanhoFilas[3]; // Tamanho atual de cada fila
} ListaPendencias;

//Cria uma nova fila com o início e o fim nulos
Fila * criaFila() {
    Fila * nova_fila = (Fila *) malloc(sizeof(Fila));
    nova_fila->fim = NULL;
    nova_fila->inicio = NULL;
    return nova_fila;
}

//Se a fila estiver vazia, retorna true
bool vaziaFila(Fila * fila) {
    if (fila->inicio == NULL) return true;
    return false;
}

//Cria um novo nó com uma dada tarefa e o insere na fila
void insereFila(Fila * fila, Tarefa * tarefa) {
    No * novo_no = (No *) malloc(sizeof(No));
    novo_no->tarefa = tarefa;

    if (vaziaFila(fila)) {
        fila->fim = novo_no;
        fila->inicio = novo_no;
        novo_no->proximo_no = NULL;
    } else {
        fila->fim->proximo_no = novo_no;
        fila->fim = novo_no;
        fila->fim->proximo_no = NULL;
    }
}

void printaFila(Fila * fila) {
    No * inicio = fila->inicio;
    if (!vaziaFila(fila)) {
        while (inicio != NULL) {
            printaTarefa(*(inicio->tarefa));
            inicio = inicio->proximo_no;
        }
    }
    else printf("\nEsta fila está vazia.\n");
}

// Função para verificar o status da tarefa
int verificaStatusTarefa(Data termino, int hoje[]) {
    int contaDiasT = termino.dia + (termino.mes * 30) + (termino.ano * 365);
    int contaDiasH = hoje[0] + hoje[1] * 30 + hoje[2] * 365;

    if (contaDiasT > contaDiasH) {
        return 1; // Tarefa atrasada
    } else if (contaDiasH < contaDiasT) {
        return 0; // Tarefa em dia
    } else {
        return -1; // Tarefa pendente
    }
}


// Função para marcar uma tarefa como pendente e movê-la para a lista de tarefas pendentes
void marcarTarefaComoPendente(int codigo, Fila *filas[], Lista **pendentes) {
    for (int i = 0; i < 3; i++) {
        No *no_atual = filas[i]->inicio;
        No *no_anterior = NULL;

        while (no_atual != NULL) {
            if (no_atual->tarefa->codigo == codigo) {
                // Encontrou a tarefa com o código
                no_atual->tarefa->status = -1; // Define o status como pendente
                // Adicione a lógica para mover a tarefa para a lista de tarefas pendentes aqui
                insereLista(pendentes, no_atual->tarefa);

                // Remova a tarefa da fila de tarefas
                if (no_anterior == NULL) {
                    // A tarefa a ser removida é a primeira da fila
                    filas[i]->inicio = no_atual->proximo_no;
                } else {
                    // A tarefa a ser removida não é a primeira da fila
                    no_anterior->proximo_no = no_atual->proximo_no;
                }
                free(no_atual); // Libere a memória do nó removido

                return;
            }

            no_anterior = no_atual;
            no_atual = no_atual->proximo_no;
        }
    }
    // Se chegou aqui, a tarefa não foi encontrada
    printf("Tarefa com código %d não encontrada.\n", codigo);
}


Tarefa * criaTarefa(int * codigo_atual, int hoje[]) {

    Tarefa * nova_tarefa = (Tarefa *) malloc(sizeof(Tarefa));

    printaDiv();
    printf("AREA DE CRIACAO DE TAREFA\n\n");

    //NOME
    printf("Digite o nome da tarefa: ");
    fflush(stdin);
    fgets(nova_tarefa->nome, 30, stdin);
    int nome_i = 0;
    while (nova_tarefa->nome[nome_i] != '\n') nome_i++;
    nova_tarefa->nome[nome_i] = '\0';

    //PROJETO
    printf("\nDigite o nome do projeto: ");
    fflush(stdin);
    fgets(nova_tarefa->projeto, 30, stdin);
    int projeto_i = 0;
    while (nova_tarefa->projeto[projeto_i] != '\n') projeto_i++;
    nova_tarefa->projeto[projeto_i] = '\0';

    //DATA INICIO
    printf("\nData de inicio: %d/%d/%d", hoje[0], hoje[1], hoje[2]);
    nova_tarefa->inicio.dia = hoje[0];
    nova_tarefa->inicio.mes = hoje[1];
    nova_tarefa->inicio.ano = hoje[2];

    //DATA TERMINO
    printf("\nDigite a data de termino");
    printf("\nDia: "); fflush(stdin); scanf("%d", &nova_tarefa->termino.dia);
    printf("Mes: "); fflush(stdin); scanf("%d", &nova_tarefa->termino.mes);
    printf("Ano: "); fflush(stdin); scanf("%d", &nova_tarefa->termino.ano);

    //STATUS
    nova_tarefa->status = -1;

    //PRIORIDADE
    printf("\nDigite a prioridade da tarefa ([1] => Alta; [2] => Media; [3] => Baixa)\n> ");
    fflush(stdin);
    scanf("%d", &nova_tarefa->prioridade);
    getchar();

    //CODIGO
    nova_tarefa->codigo = *codigo_atual;
    printf("\nO codigo da tarefa eh: %d", *codigo_atual);
    printaDiv();

    (*codigo_atual)++;

    return nova_tarefa;
}

//Recebe uma tarefa e insere em uma das filas ou na lista de pendentes
void cadastraNovaTarefa(Fila * filas[], Lista ** pendentes, Tarefa * tarefa) {
    if (tarefa->status == -1) {
        insereLista(pendentes, tarefa);
    }
    else {
        insereFila(filas[tarefa->prioridade - 1], tarefa);
    }
}

//retorna a tarefa com o código passado
Tarefa * getTarefa(int codigo, Lista * pendentes, Fila * filas[]) {
    while (pendentes != NULL) {
        if (pendentes->tarefa->codigo == codigo) return pendentes->tarefa;
        pendentes = pendentes->proximo_no;
    }

    for (int i = 0; i < 3; i++) {
        No * no_atual = filas[i]->inicio;
        while (no_atual != NULL) {
            if (no_atual->tarefa->codigo == codigo) return no_atual->tarefa;
            no_atual = no_atual->proximo_no;
        }
    }

    return NULL; // Default
}

//Recebe o código de uma tarefa e altera as suas informações
void editaTarefa(int codigo, Fila * filas[], Lista * pendentes) {
    Tarefa * tarefa = getTarefa(codigo, pendentes, filas);

    time_t segundos;
    time(&segundos);
    struct tm *data_hora_atual = localtime(&segundos);
    int hoje[3] = {data_hora_atual->tm_mday, data_hora_atual->tm_mon + 1, data_hora_atual->tm_year + 1900}; // Data atual (dia, mês, ano)

    printaDiv();
    printf("AREA DE EDICAO DE TAREFA\n\n");

    printf("Para editar, digite:\n[0] Nome\n[1] Projeto;\n[2] Data de inicio;\n[3] Data de termino;\n[4] Prioridade;\n> ");
    int escolha;
    fflush(stdin);
    scanf("%d", &escolha);

    switch (escolha) {
        case 0:
            printf("\n Novo nome: ");
            fflush(stdin);
            fgets(tarefa->nome, 30, stdin);
            break;
        case 1:
            printf("\n Novo projeto: ");
            fflush(stdin);
            fgets(tarefa->projeto, 30, stdin);
            break;
        case 2:
            printf("Nova data de inicio:");
            printf("\nDia: "); fflush(stdin); scanf("%d", &tarefa->inicio.dia);
            printf("Mes: "); fflush(stdin); scanf("%d", &tarefa->inicio.mes);
            printf("Ano: "); fflush(stdin); scanf("%d", &tarefa->inicio.ano);
            break;
        case 3:
            printf("Nova data de termino:");
            printf("\nDia: "); fflush(stdin); scanf("%d", &tarefa->termino.dia);
            printf("Mes: "); fflush(stdin); scanf("%d", &tarefa->termino.mes);
            printf("Ano: "); fflush(stdin); scanf("%d", &tarefa->termino.ano);
            tarefa->status = verificaStatusTarefa(tarefa->termino, hoje);
            break;
        case 4:
            printf("Nova prioridade: ");
            fflush(stdin);
            scanf("%d", &tarefa->prioridade);
            break;
        default:
            printf("Escolha invalida");
    }

    printaDiv();
}

void concluirTarefa(int codigo, Fila * filas[], Lista ** pendentes, Lista ** concluidas, int data_atual[]) {
    // Procura a tarefa nas filas de prioridade
    for (int i = 0; i < 3; i++) {
        No * no_atual = filas[i]->inicio;
        No * no_anterior = NULL;

        while (no_atual != NULL) {
            if (no_atual->tarefa->codigo == codigo) {
                // Remove a tarefa da fila
                if (no_anterior == NULL) {
                    filas[i]->inicio = no_atual->proximo_no;
                } else {
                    no_anterior->proximo_no = no_atual->proximo_no;
                }

                // Atualiza a data de término para a data atual
                no_atual->tarefa->termino.dia = data_atual[0];
                no_atual->tarefa->termino.mes = data_atual[1];
                no_atual->tarefa->termino.ano = data_atual[2];

                // Calcula se a tarefa foi concluída com atraso ou não
                bool concluida_com_atraso = dataMenor(no_atual->tarefa->termino, no_atual->tarefa->inicio);

                // Adiciona a informação de conclusão com atraso à tarefa
                no_atual->tarefa->concluida_com_atraso = concluida_com_atraso;

                // Adiciona a tarefa à lista de tarefas concluídas
                insereLista(concluidas, no_atual->tarefa);

                // Libera a memória do nó
                free(no_atual);

                printf("Tarefa concluída com sucesso!\n");
                return;
            }

            no_anterior = no_atual;
            no_atual = no_atual->proximo_no;
        }
    }

    // Procura a tarefa na lista de tarefas pendentes
    No * no_atual = *pendentes;
    No * no_anterior = NULL;

    while (no_atual != NULL) {
        if (no_atual->tarefa->codigo == codigo) {
            // Remove a tarefa da lista de pendentes
            if (no_anterior == NULL) {
                *pendentes = no_atual->proximo_no;
            } else {
                no_anterior->proximo_no = no_atual->proximo_no;
            }

            // Calcula se a tarefa foi concluída com atraso ou não
            bool concluida_com_atraso = dataMenor(no_atual->tarefa->termino, no_atual->tarefa->inicio);

            // Adiciona a informação de conclusão com atraso à tarefa
            no_atual->tarefa->concluida_com_atraso = concluida_com_atraso;

            // Atualiza a data de término para a data atual
            no_atual->tarefa->termino.dia = data_atual[0];
            no_atual->tarefa->termino.mes = data_atual[1];
            no_atual->tarefa->termino.ano = data_atual[2];

            // Adiciona a tarefa à lista de tarefas concluídas
            insereLista(concluidas, no_atual->tarefa);

            // Libera a memória do nó
            free(no_atual);

            printf("Tarefa concluída com sucesso!\n");
            return;
        }

        no_anterior = no_atual;
        no_atual = no_atual->proximo_no;
    }

    // Se não encontrou a tarefa, informa que não foi encontrada
    printf("Tarefa não encontrada.\n");
}


// Função para listar as tarefas concluídas com base no parâmetro 'com_atraso'
void listarTarefasConcluidas(Lista *concluidas, bool com_atraso) {
    Lista *no_atual = concluidas;

    while (no_atual != NULL) {
        // Verifica se a tarefa foi concluída com atraso ou sem atraso
        if (no_atual->tarefa->concluida_com_atraso == com_atraso) {
            printaTarefa(*(no_atual->tarefa)); // Assumindo que você tenha uma função printaTarefa
        }
        no_atual = no_atual->proximo_no;
    }
}

No * desenfila(Fila * fila) {
    if (!vaziaFila(fila)) {
        No * no_retirado = fila->inicio;
        fila->inicio = fila->inicio->proximo_no;
        return no_retirado;
    }

    return NULL;
}

No * retiraTarefa(int codigo, Lista ** pendentes, Fila * filas[]) {
    Tarefa * tarefa = getTarefa(codigo, *pendentes, filas);
    No * no_excluido;

    //Procura na lista de pendentes
    if (tarefa->status == -1) {

        if ((*pendentes)->tarefa == tarefa) {
            no_excluido = *pendentes;
            (*pendentes) = (*pendentes)->proximo_no;
        }

        else {
            Lista * pendentes_aux = *pendentes;

            while(pendentes_aux->proximo_no->tarefa != tarefa && pendentes_aux->proximo_no != NULL) {
                pendentes_aux = pendentes_aux->proximo_no;
            }

            no_excluido = pendentes_aux->proximo_no;
            pendentes_aux->proximo_no = pendentes_aux->proximo_no->proximo_no;
        }
    }

    else {
        Fila * fila = filas[tarefa->prioridade - 1];

        if (fila->inicio->tarefa == tarefa) {
            no_excluido = fila->inicio;
            fila->inicio = fila->inicio->proximo_no;
        }

        else {
            No * no_aux;

            bool achou_tarefa = false;
            while (!achou_tarefa) {
                no_aux = desenfila(fila);
                if (no_aux->tarefa->codigo == codigo) {
                    no_excluido = no_aux;
                    achou_tarefa = true;
                } else {
                    insereFila(fila, no_aux->tarefa);
                    free(no_aux);
                }
            }
        }
    }

    return no_excluido;
}

void deletaTarefa(int codigo, Lista ** pendentes, Fila * filas[]) {
    No * no_liberado = retiraTarefa(codigo, pendentes, filas);
    free(no_liberado->tarefa);
    free(no_liberado);
}

void liberarLista(Lista * lista) {
    while (lista != NULL) {
        Lista * proximo = lista->proximo_no; // Salva o próximo nó antes de liberar o atual
        free(lista->tarefa); // Libera a memória da tarefa dentro do nó
        free(lista); // Libera a memória do nó atual
        lista = proximo; // Move para o próximo nó
    }
}

void liberarFila(Fila * fila) {
    while (!vaziaFila(fila)) {
        No * primeiro = fila->inicio; // Obtém o primeiro nó da fila
        fila->inicio = fila->inicio->proximo_no; // Atualiza o início da fila
        free(primeiro->tarefa); // Libera a memória da tarefa dentro do nó
        free(primeiro); // Libera a memória do nó
    }
    free(fila); // Libera a memória da estrutura da fila
}

void liberarArrayDeFilas(Fila * vetorDeFilas[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        liberarFila(vetorDeFilas[i]);
        vetorDeFilas[i] = NULL; // Defina o ponteiro da fila como NULL após liberar
    }
}
