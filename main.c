#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "proj1blib.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese");
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
        printf("\n===SISTEMA DE GERENCIAMENTO DE TAREFAS===\n");
        printf("[1] Adicionar nova Tarefa\n");
        printf("[2] Modificar uma Tarefa\n");
        printf("[3] Concluir Tarefa\n");
        printf("[4] Lista de Tarefas Pendentes\n");
        printf("[5] Lista de Tarefas Concluídas\n");
        printf("[6] Lista de Tarefas Concluidas com e sem Atrasos\n");
        printf("[7] Remover tarefa\n");
        printf("[8] Sair do Programa\n\n");
        printf(">> ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: {
                Tarefa * nova_tarefa = criaTarefa(&codigo_atual, data_atual);
                cadastraNovaTarefa(filas, &pendentes, nova_tarefa);
                break;
            }
            case 2:
                printf("Digite o codigo da tarefa que deseja editar: ");
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
                printf("\nLista de Tarefas Pendentes:\n");
                imprimeLista(pendentes);
                break;
            case 5:
                printf("\nLista de Tarefas Concluidas:\n");
                imprimeLista(concluidas);
                break;
            case 6:
                printf("\nTarefas Concluidas com Atraso:\n");
                listarTarefasConcluidas(concluidas, true);

                printf("\nTarefas Concluidas sem Atraso:\n");
                listarTarefasConcluidas(concluidas, false);
                break;
            case 7:
                printf("Digite o codigo da tarefa que deseja remover: ");
                int codigo_remocao;
                scanf("%d", &codigo_remocao);
                deletaTarefa(codigo_remocao, &pendentes, filas);
                printf("Tarefa com codigo %d removida com sucesso.\n", codigo_remocao);
                break;
            case 8:
                printf("\nSaindo do programa...\n");
                liberarLista(pendentes);
                liberarArrayDeFilas(filas, 3);
                return 0;
            default:
                printf("Opção invalida. Tente novamente.\n");
        }
    }

    return 0;
}
