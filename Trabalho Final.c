#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM 100

typedef struct {
    int id;
    char nome[TAM];
    char telefone[20];
    char email[TAM];
} Contatos;

Contatos lista[TAM];
int totalContatos = 0;
int proximoId = 1;

void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void limparTela() {
    system("cls");
}

void carregarContatos() {
    FILE *arquivo = fopen("contatos.csv", "r");
    totalContatos = 0;

    if(arquivo == NULL) {
        return;
    }

    while(totalContatos < TAM &&
           fscanf(arquivo, "%d,%[^,],%[^,],%[^\n]\n",
                  &lista[totalContatos].id,
                  lista[totalContatos].nome,
                  lista[totalContatos].telefone,
                  lista[totalContatos].email) == 4)
    {
        totalContatos++;
    }

    fclose(arquivo);
}

void salvarContatos() {
    FILE *arquivo = fopen("contatos.csv", "w");
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }
    for(int i = 0; i < totalContatos; i++) {
        fprintf(arquivo, "%d,%s,%s,%s\n",
                lista[i].id,
                lista[i].nome,
                lista[i].telefone,
                lista[i].email);
    }
    fclose(arquivo);
}

void atualizarProximoId() {
    if(totalContatos == 0) {
        proximoId = 1;
    }else{
        int maiorId = lista[0].id;
        for(int i = 1; i < totalContatos; i++) {
            if(lista[i].id > maiorId) {
                maiorId = lista[i].id;
            }
        }
        proximoId = maiorId + 1;
    }
}

void menu() {
    printf("========= Contatos (%d) =========\n", totalContatos);
    printf("1 - Inserir contato.\n");
    printf("2 - Listar contatos.\n");
    printf("3 - Buscar contato.\n");
    printf("4 - Editar contato.\n");
    printf("5 - Excluir contato.\n");
    printf("6 - Salvar e Sair.\n");
    printf("\n");
    printf("    Escolha uma opcao: ");
}

void cadastrarContato() {
    limparTela();
    if(totalContatos == TAM) {
        printf("Lista de contatos cheia!!!\n");
        printf("Pressione ENTER para voltar...");
        getchar();
        return;
    }

    printf("Entre com o nome: ");
    fgets(lista[totalContatos].nome, TAM, stdin);
    lista[totalContatos].nome[strcspn(lista[totalContatos].nome, "\n")] = '\0';

    printf("Entre com o numero: ");
    fgets(lista[totalContatos].telefone, 20, stdin);
    lista[totalContatos].telefone[strcspn(lista[totalContatos].telefone, "\n")] = '\0';

    printf("Entre com o email: ");
    fgets(lista[totalContatos].email, TAM, stdin);
    lista[totalContatos].email[strcspn(lista[totalContatos].email, "\n")] = '\0';

    lista[totalContatos].id = proximoId;
    proximoId++;
    totalContatos++;

    printf("\nCadastrado com sucesso!\n");
    getchar();
}

void listarContatos() {
    limparTela();
    if(totalContatos == 0) {
        printf("A lista de contatos esta vazia! Cadastre algum contato.\n");
        printf("\nPressione Enter pra voltar ao menu...");
        getchar();
        return;
    }
    printf("===== Lista de contatos =====\n");
    for(int i = 0; i < totalContatos; i++) {
        printf("Contato numero %d:\n", i+1);
        printf("ID: %d\n", lista[i].id);
        printf("Nome: %s\n", lista[i].nome);
        printf("Numero: %s\n", lista[i].telefone);
        printf("E-mail: %s\n", lista[i].email);
        printf("--------------------------\n");
    }
    getchar();
}
void buscarContato() {
    limparTela();
    char busca[TAM];
    int count = 0;
    printf("Entre com o nome ou numero ou email: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';
    int tamBusca = strlen(busca);
    int EhNumero = (busca[0] >= '0' && busca[0] <= '9');
    for(int i = 0; i < totalContatos; i++) {
        int bate = 0;
        if(EhNumero) {
            if(strncmp(lista[i].telefone, busca, tamBusca) == 0) {
                bate = 1;
            }
        } else {
            if(strncmp(lista[i].nome, busca, tamBusca) == 0 ||
               strncmp(lista[i].email, busca, tamBusca) == 0) {
                bate = 1;
            }
        }
        if(bate==1) {
        count++;
        printf("--------Contato encontrado-------\n");
        printf("ID: %d\n", lista[i].id);
        printf("Nome: %s\n", lista[i].nome);
        printf("Telefone: %s\n", lista[i].telefone);
        printf("Email: %s\n", lista[i].email);
        printf("--------------------------\n");
    }
    }
    if(count == 0) {
        printf("Nenhum contato encontrado!!!\n");
    }
    printf("Pressione ENTER para voltar...");
    getchar();
}
void editarContato() {
    limparTela();
    if(totalContatos == 0) {
        printf("Lista vazia!\n");
        getchar();
        return;
    }
    char busca[TAM];
    printf("Entre com o nome ou numero ou email do contato a ser editado: ");
    fgets(busca, sizeof(busca), stdin); 
    busca[strcspn(busca, "\n")] = '\0';
    printf("--------Contatos encontradas-------\n");
    int encontrado = 0;
    for(int i = 0; i < totalContatos; i++) {
        if(strstr(lista[i].nome, busca) != NULL ||
            strstr(lista[i].telefone, busca) != NULL ||
            strstr(lista[i].email, busca) != NULL) {
            encontrado = 1;
            printf("ID: %d\n", lista[i].id);
            printf("Nome: %s\n", lista[i].nome);
            printf("Telefone: %s\n", lista[i].telefone);
            printf("Email: %s\n", lista[i].email);
            printf("--------------------------\n");
            printf("Pressione ENTER para continuar...");
            getchar();
        }
    }
    if(encontrado==0) {
        printf("Nenhum contato encontrado com os dados fornecidos!\n");
        printf("Pressione ENTER para voltar...");
        getchar();
        return;
    }

    int idDigitado = 0, indice = -1;
    char novoNome[TAM], novoEmail[TAM], novoTelefone[20];
    limparTela();
    printf("Entre com o ID do contato a ser editado: ");
    scanf("%d", &idDigitado);
    if(idDigitado == 0) {
        printf("Operacao cancelada!\n");
        return;
    }
    limparBuffer();
    for(int i = 0; i < totalContatos; i++) {
        if(lista[i].id == idDigitado) {
            indice = i;
            break;
        }
    }
    if (indice != -1) {
        int op;
        do {
        limparTela();
        printf("Editando contato ID %d\n", lista[indice].id);
        printf("1 - Editar nome (Atual: %s)\n", lista[indice].nome);
        printf("2 - Editar telefone (Atual: %s)\n", lista[indice].telefone);
        printf("3 - Editar email (Atual: %s)\n", lista[indice].email);
        printf("4 - Voltar ao menu principal e salvar\n");   
        printf("Escolha uma opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch(op) {
            case 1:
                printf("Entre com o novo nome: ");
                fgets(novoNome, TAM, stdin);
                novoNome[strcspn(novoNome, "\n")] = '\0';
                strcpy(lista[indice].nome, novoNome);
                printf("Nome atualizado\nPressione ENTER para continuar...");
                getchar();
                break;
            case 2:
                printf("Entre com o novo telefone: ");  
                fgets(novoTelefone, 20, stdin);
                novoTelefone[strcspn(novoTelefone, "\n")] = '\0';
                strcpy(lista[indice].telefone, novoTelefone);
                printf("Telefone atualizado\nPressione ENTER para continuar...");
                getchar();
                break;
            case 3:
                printf("Entre com o novo email: ");
                fgets(novoEmail, TAM, stdin);
                novoEmail[strcspn(novoEmail, "\n")] = '\0';
                strcpy(lista[indice].email, novoEmail);
                printf("Email atualizado\nPressione ENTER para continuar...");
                getchar();
                break;
        }
    } while(op != 4);
    printf("Contato atualizado com sucesso!\n");
    salvarContatos();
        
    } else {
        printf("Contato com ID %d nao encontrado!\n", idDigitado);
        printf("Pressione ENTER para voltar...");
        getchar();
        return;
    }
}
void excluirContato() {

}
int main() {
    carregarContatos();
    atualizarProximoId();
    int op;

    do {
        limparTela();
        menu();

        if (scanf("%d", &op) == 1) {
             limparBuffer();
        } else {
             limparBuffer();
             op = 0;
        }

        switch(op) {
            case 1:
                cadastrarContato();
                break;
            case 2:
                listarContatos();
                break;
            case 3:
                buscarContato();
                break;
            case 4:
                editarContato();
                break;
            case 5:
                excluirContato();
                break;
            case 6:
                salvarContatos();
                break;
            default:
                printf("Opcao invalida!\n");
                printf("Pressione ENTER para voltar...");
                getchar();
        }
    } while(op != 6);

    return 0;
}