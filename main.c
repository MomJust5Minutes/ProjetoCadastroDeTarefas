#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "proj1blib.h"

int main() {
    int codigo_atual = 1;

    time_t segundos;
    time(&segundos);
    struct tm *data_hora_atual = localtime(&segundos);
    int data_atual[3] = {data_hora_atual->tm_mday, data_hora_atual->tm_mon + 1, data_hora_atual->tm_year + 1900}; // Data atual (dia, mês, ano)
    printf("Data atual: %02d/%02d/%04d\n", data_atual[0], data_atual[1], data_atual[2]);

    Lista * concluidas = criaLista();
    Lista * pendentes = criaLista();
    Fila * filas[3]; // Filas para cada prioridade
    for (int i = 0; i < 3; i++) {
        filas[i] = criaFila();
    }

    int escolha;
    while (1) {
        printf("\nSISTEMA DE GERENCIAMENTO DE TAREFAS\n");
        printf("[1] Adicionar nova Tarefa\n");
        printf("[2] Modificar uma Tarefa\n");
        printf("[3] Concluir Tarefa\n");
        printf("[4] Atualização do Status da Tarefa\n");
        printf("[5] Lista de Tarefas Pendentes\n");
        printf("[6] Lista de Tarefas Concluídas\n");
        printf("[7] Lista de Tarefas Concluidas com e sem Atrasos\n");
        printf("[8] Sair do Programa\n");
        printf("> ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: {
                Tarefa * nova_tarefa = criaTarefa(&codigo_atual, data_atual);
                cadastraNovaTarefa(filas, &pendentes, nova_tarefa);
                break;
            }
            case 2:
                printf("Digite o código da tarefa que deseja editar: ");
                int codigo_edicao;
                scanf("%d", &codigo_edicao);
                editaTarefa(codigo_edicao, filas, pendentes);
                break;
            case 3:
                printf("Digite o codigo da tarefa que deseja marcar como concluida: ");
                int codigo_conclusao;
                scanf("%d", &codigo_conclusao);
                concluirTarefa(codigo_conclusao, filas, &pendentes, &concluidas, data_atual);
                break;
            case 4:
                // Implemente a lógica para atualização do status da tarefa aqui
                break;
            case 5:
                printf("\nLista de Tarefas Pendentes:\n");
                printaLista(pendentes);
                break;
            case 6:
                printf("\nLista de Tarefas Concluidas:\n");
                printaLista(concluidas);
                break;
            case 7:
                // Implemente a lógica para listar tarefas concluídas com e sem atrasos aqui
                break;
            case 8:
                printf("\nSaindo do programa...\n");
                liberarLista(pendentes);
                liberarArrayDeFilas(filas, 3);
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
