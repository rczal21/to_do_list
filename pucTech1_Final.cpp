//To Do list Puc-Tech versão final 

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

void remover_newline (char *s);
void salvar(Tarefa tarefas[], int total);
int carregar(Tarefa tarefas[]);
void listar(Tarefa tarefas[], int total);
void adicionar(Tarefa tarefas[], int *total);
void remover(Tarefa tarefas[], int *total);
void concluir(Tarefa tarefas[], int total);
void menu();

int main()
{
	Tarefa tarefas[MAX_TAREFAS];
	int total = carregar(tarefas);
    int opcao;

    printf("\n=== TO DO LIST ===\n");
    printf("Arquivo: %s\n", ARQUIVO);

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionar(tarefas, &total);   break;
            case 2: remover(tarefas, &total);     break;
            case 3: listar(tarefas, total);       break;            
            case 4: concluir(tarefas, total);     break;
            case 0: printf("Ate logo!\n");        break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}

void menu(){
	printf("\n 1. Adicionar tarefa\n");
	printf("\n 2. Remover tarefa\n");
	printf("\n 3. Listar tarefa\n");
	printf("\n 4. Marcar como conluida\n");
	printf("\n 0. Sair do programa\n");
	printf("\n Escolha: ");
}

void concluir(Tarefa tarefas[], int total) {
	int id;
    printf("ID da tarefa a concluir: ");
    scanf("%d", &id);

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

void remover(Tarefa tarefas[], int *total) {
	int id;
    printf("ID da tarefa a remover: ");
    scanf("%d", &id);

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

void remover_newline(char *s) {
    int i = 0;
    while (s[i] != '\0' && s[i] != '\n') i++;
    s[i] = '\0';
}

