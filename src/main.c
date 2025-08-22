#include "Menus.c"
#include "eventos.c"
#include "users.c"
#include "utilities.c"
#include "HeapSort.c"
#include "buscas.c"
#include "carrinho.c"
#include "IntercalacaoOtima.c"
#include "selecaoSubstituicao.c"
#include "Hash.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>



int main()
{
    srand(time(NULL));

    FILE *arq_eventos, *arq_users, *arq_carrinho, *arq_ingressos, *arq_hash, *arq_overflow;
    FILE *log, *log_b;
    User *usuarioLogado = NULL;

    if ((arq_eventos = fopen("eventos.dat", "wb+")) == NULL){
        ERROR("erro ao abrir arquivo eventos.dat");
        exit(1);
    }

    if ((arq_users = fopen("users.dat", "wb+")) == NULL){
        ERROR("Erro ao abrir arquivo users.dat");
        exit(1);
    }

    if ((arq_carrinho = fopen("carrinhos.dat", "wb+")) == NULL){
        ERROR("Erro ao abrir arquivo carrinhos.dat");
        exit(1);
    }

    if ((arq_ingressos = fopen("ingressos.dat", "wb+")) == NULL){
        ERROR("Erro ao abrir arquivo ingressos.dat");
        exit(1);
    }

    if ((log_b = fopen("log.txt", "a")) == NULL){
        ERROR("Erro ao abrir arquivo log.txt");
        exit(1);
    }

    if ((log = fopen("log_ordenacao.txt", "a")) == NULL){
        ERROR("Erro ao abrir arquivo log_ordenacao.txt");
        exit(1);                    
    }

    criarTabelaHash(HASH_FILE);
    if ((arq_hash = fopen(HASH_FILE, "rb+")) == NULL) {
        ERROR("Erro ao abrir arquivo hash.");
        exit(1);
    }
    if ((arq_overflow = fopen(OVERFLOW_FILE, "rb+")) == NULL) { // wb+ para criar se não existir
        arq_overflow = fopen(OVERFLOW_FILE, "wb+");
    }

    int tamEv = 20, tamUs = 110;
    int memDisponivel = 10;// Memória disponível para o reservatório
    int comp = 0, tro = 0;//comparações e trocas

    criarBaseEventos(arq_eventos, tamEv);
    criarBaseUsuarios(arq_users, tamUs);

    rewind(arq_users);
    User *tempUser;
    while((tempUser = lerUsuario(arq_users)) != NULL) {
        inserirUsuario_Hash(arq_hash, arq_overflow, tempUser);
        free(tempUser);
    }

    int opcao;
    do{
        limpar_tela_ansi();
        printf("--------------------------- MENU ---------------------------\n");
        printf("1 - Login/Logout\n");
        printf("2 - Fazer Busca e/ou Hash\n");
        printf("3 - Ordenar\n");
        printf("4 - Menu Eventos\n");
        printf("5 - Carrinho\n");
        printf("6 - Ingressos\n");
        printf("0 - sair\n");
        printf("-------------------------- SAIDA ---------------------------\n");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                usuarioLogado = login_logout(arq_users, usuarioLogado);
                break;
            case 2:
                rewind(arq_users);
                menuBusca(arq_eventos, arq_users, arq_hash, arq_overflow, log, comp, tro);
                break;
            case 3:
                menuOrdenacao(arq_eventos, arq_users, log, memDisponivel, comp, tro);
                break;
            case 4:
                menuEventos(usuarioLogado, arq_eventos, arq_carrinho, log);
                break;
            case 5:
                menuCarrinho(arq_carrinho, arq_ingressos, usuarioLogado);
                break;
            case 6:
                menuIngressos(arq_ingressos, usuarioLogado);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausarTela();
                break;
        }
    } while (opcao != 0);
        
}
