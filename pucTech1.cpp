// Do list Puc-Tech

#include <stdio.h>
#include <string.h>

#define MAX_TAREFAS 100
#define MAX_TEXTO   200
#define ARQUIVO "tarefas.txt"

typedef struct {
    int  id;
    char texto[MAX_TEXTO];
    int  concluida; /* 0 = pendente, 1 = concluida */
} Tarefa;

/* remove '\n' do final de uma string */
void remover_newline(char *s) {
    int i = 0;
    while (s[i] != '\0' && s[i] != '\n') i++;
    s[i] = '\0';
}

/* converte string para int sem atoi/stdlib */
int str_para_int(const char *s) {
    int resultado = 0;
    int i = 0;
    while (s[i] >= '0' && s[i] <= '9') {
        resultado = resultado * 10 + (s[i] - '0');
        i++;
    }
    return resultado;
}

/* -- persistencia ------------------------------------------- */

void salvar(Tarefa tarefas[], int total) {
    FILE *f = fopen(ARQUIVO, "w");
    if (!f) { printf("Erro ao abrir arquivo para escrita.\n"); return; }
    fprintf(f, "%d\n", total);
    for (int i = 0; i < total; i++)
        fprintf(f, "%d|%d|%s\n", tarefas[i].id, tarefas[i].concluida, tarefas[i].texto);
    fclose(f);
}

int carregar(Tarefa tarefas[]) {
    FILE *f = fopen(ARQUIVO, "r");
    if (!f) return 0; /* arquivo ainda nao existe */
    int total = 0;
    fscanf(f, "%d\n", &total);
    for (int i = 0; i < total; i++) {
        fscanf(f, "%d|%d|%[^\n]\n",
               &tarefas[i].id,
               &tarefas[i].concluida,
               tarefas[i].texto);
    }
    fclose(f);
    return total;
}

/* -- operacoes ---------------------------------------------- */

void listar(Tarefa tarefas[], int total) {
    printf("\n+------------------------------------------+\n");
    printf("|          LISTA DE TAREFAS                |\n");
    printf("+------------------------------------------+\n");
    if (total == 0) {
        printf("|  Nenhuma tarefa cadastrada.              |\n");
    } else {
        for (int i = 0; i < total; i++) {
            char status = tarefas[i].concluida ? 'X' : ' ';
            printf("| [%c] #%-3d %-33s|\n",
                   status, tarefas[i].id, tarefas[i].texto);
        }
    }
    printf("+------------------------------------------+\n");
}

void adicionar(Tarefa tarefas[], int *total) {
    if (*total >= MAX_TAREFAS) {
        printf("Limite de %d tarefas atingido!\n", MAX_TAREFAS);
        return;
    }
    printf("Digite a tarefa: ");
    getchar(); /* limpa '\n' que sobrou no buffer */
    fgets(tarefas[*total].texto, MAX_TEXTO, stdin);
    remover_newline(tarefas[*total].texto);

    if (strlen(tarefas[*total].texto) == 0) {
        printf("Tarefa vazia, operacao cancelada.\n");
        return;
    }

    /* id = maior id existente + 1 */
    int maior = 0;
    for (int i = 0; i < *total; i++)
        if (tarefas[i].id > maior) maior = tarefas[i].id;

    tarefas[*total].id        = maior + 1;
    tarefas[*total].concluida = 0;
    (*total)++;
    salvar(tarefas, *total);
    printf("Tarefa adicionada!\n");
}

void remover(Tarefa tarefas[], int *total) {
    char buf[16];
    printf("ID da tarefa a remover: ");
    scanf("%15s", buf);
    int id = str_para_int(buf);

    int pos = -1;
    for (int i = 0; i < *total; i++)
        if (tarefas[i].id == id) { pos = i; break; }

    if (pos == -1) { printf("ID nao encontrado.\n"); return; }

    for (int i = pos; i < *total - 1; i++)
        tarefas[i] = tarefas[i + 1];

    (*total)--;
    salvar(tarefas, *total);
    printf("Tarefa #%d removida!\n", id);
}

void concluir(Tarefa tarefas[], int total) {
    char buf[16];
    printf("ID da tarefa a concluir: ");
    scanf("%15s", buf);
    int id = str_para_int(buf);

    for (int i = 0; i < total; i++) {
        if (tarefas[i].id == id) {
            tarefas[i].concluida = 1;
            salvar(tarefas, total);
            printf("Tarefa #%d marcada como concluida!\n", id);
            return;
        }
    }
    printf("ID nao encontrado.\n");
}

/* -- menu --------------------------------------------------- */

void menu() {
    printf("\n  1. Listar tarefas\n");
    printf("  2. Adicionar tarefa\n");
    printf("  3. Remover tarefa\n");
    printf("  4. Marcar como concluida\n");
    printf("  0. Sair\n");
    printf("\nEscolha: ");
}

/* -- main --------------------------------------------------- */

int main(void) {
    Tarefa tarefas[MAX_TAREFAS];
    int total = carregar(tarefas);
    char buf[16];
    int opcao;

    printf("\n=== TODO LIST ===\n");
    printf("Arquivo: %s\n", ARQUIVO);

    do {
        menu();
        scanf("%15s", buf);
        opcao = str_para_int(buf);

        switch (opcao) {
            case 1: listar(tarefas, total);       break;
            case 2: adicionar(tarefas, &total);   break;
            case 3: remover(tarefas, &total);     break;
            case 4: concluir(tarefas, total);     break;
            case 0: printf("Ate logo!\n");        break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}
