#include "../include/Menus.h"

#define ERROR(msg) fprintf(stderr, "\033[1;31m%s\033[0m\n", msg);

void menuOrdenacao(FILE *arq_eventos, FILE *arq_users, FILE *log, int memDisponivel, int comp, int tro) {
    int esc, esc2;

    printf("\n=== MENU ORDENACAO ===\n");
    printf("1 - Ordenar Eventos\n");
    printf("2 - Ordenar Usuarios\n");
    printf("Escolha: ");
    scanf("%d", &esc);

    if (esc == 1) {
        printf("\nVoce escolheu ordenar a base de eventos!\n");
        printf("\n1 - HeapSort\n2 - Intercalacao Otima\nEscolha: ");
        scanf("%d", &esc2);
        if (esc2 == 1){
            limpar_tela_ansi();

            printf("Base de eventos desordenada:\n");
            printf("==========================================\n");
            imprimirBaseEvento(arq_eventos);
            printf("==========================================\n\n");
            printf("\n\n");
            printf("------------------------------------\n");
            printf("Agora a base ordenada!\n");
            rewind(arq_eventos);
            int total_eventos = tamanho_arquivoEv(arq_eventos);
            heapSort(arq_eventos, total_eventos, TIPO_Evento, &comp, &tro, log);
            imprimirBaseEvento(arq_eventos);

            pausarTela();   
        }
        else if (esc2 == 2){
            limpar_tela_ansi();
            printf("\nVoce escolheu ordenar eventos por Intercalacao Otima!\n");

            printf("Base de eventos desordenada:\n");
            printf("==========================================\n");
            imprimirBaseEvento(arq_eventos);
            printf("==========================================\n\n");

            // Calcula total de registros
            rewind(arq_eventos);
            int totalRegistros = tamanho_arquivoEv(arq_eventos);
                            
            printf("Iniciando ordenacao por Intercalacao Otima...\n");
            printf("Total de registros: %d\n", totalRegistros);
            printf("Memoria disponivel: %d registros\n\n", memDisponivel);
                            
            // Executa a ordenação
            int resultado = ordenarPorIntercalacaoOtima(arq_eventos, memDisponivel, totalRegistros, log);
                                                
            if (resultado > 0) {
                printf("\nOrdenacao concluida com sucesso!\n");
                printf("Registros processados: %d\n\n", resultado);
                                    
                // Imprime arquivo ordenado
                FILE *arqOrdenado = fopen("eventos_ordenados.dat", "rb");
                if (arqOrdenado) {
                    printf("Base de eventos ordenada:\n");
                    printf("==========================================\n");
                    imprimirBaseEvento(arqOrdenado);
                    printf("==========================================\n");
                    fclose(arqOrdenado);
                } 
                else {
                    printf("Erro ao abrir arquivo ordenado.\n");
                }
            } 
            else {
                printf("Erro na ordenação.\n");
            }
                                
            fclose(log);
        }
        else{
            printf("Opcao invalida");
        } 
    } 
    else if (esc == 2) {
        printf("\nVoce escolheu ordenar a base de Usuarios!\n");
        printf("\n1 - HeapSort\nEscolha: ");
        scanf("%d", &esc2);
        if (esc2 == 1){
            limpar_tela_ansi();

            printf("==========================================\n");
            printf("A base desordenada:\n");
            printf("==========================================\n");
            imprimirBaseUser(arq_users);
            printf("\n\n");
            printf("==========================================\n");
            printf("Agora a base ordenada!\n");
            printf("==========================================\n");
            rewind(arq_users);
            // CORREÇÃO: Certifique-se de usar a função correta para contar usuários
            int total_users = tamanho_arquivoUs(arq_users);
            printf("Debug: Total de usuários encontrados: %d\n", total_users);
            
            // Verifica se há usuários para ordenar
            if (total_users <= 0) {
                printf("Erro: Nenhum usuário encontrado no arquivo!\n");
                pausarTela();
                return;
            }
            heapSort(arq_users, total_users, TIPO_User, &comp, &tro, log);
            rewind(arq_users);
            imprimirBaseUser(arq_users);
            
            pausarTela(); 
        }
        else{
            printf("Opcao invalida");
        } 
    }
}

void menuBusca(FILE *arq_eventos, FILE *arq_users, FILE *arq_hash, FILE *arq_overflow, FILE *log, int comp, int tro) {
    int esc, id;
    User *us;
    Evento *ev;
    User *userEncontrado;

    printf("\n=== MENU BUSCA ===\n");
    printf("1 - Busca Sequencial\n");
    printf("2 - Busca Binaria\n");
    printf("3 - Busca Hash (Usuarios)\n");
    printf("Escolha: ");
    scanf("%d", &esc);

    if (esc == 1) {
        int esc01;
        limpar_tela_ansi();
        printf("----------------- OPCOES -----------------\n");
        printf("Voce deseja usar Busca Sequencial em:\n");
        printf("1 - Eventos\n");
        printf("2 - Usuarios\n");
        printf("Escolha: ");
        scanf("%d", &esc01);

        if (esc01 == 1){
            printf("Voce escolheu Eventos!\n");
            printf("Informe o ID do evento: ");
            scanf("%d", &id);
            rewind(arq_eventos);
            ev = Evento_buscaSequencial_PorId(arq_eventos, id, log);
            if (!ev){
                printf("Id de numero %d nao encontrado.", id);
                printf("Tente novamente");
            }
            imprimirEvento(ev);
            pausarTela();                
        }
        else if (esc01 == 2){
            printf("Voce escolheu Usuarios!\n");
            printf("Informe o ID do usuario: ");
            scanf("%d", &id);
            rewind(arq_users);
            us = User_buscaSequencial_PorId(arq_users, id, log);
            if (!us){
                printf("Id de numero %d nao encontrado.", id);
                printf("Tente novamente");
            }
            imprimirUser(us);
            pausarTela(); 
        }
        else{
            printf("Opcao invalida");
        }
    }
    else if (esc == 2) {
        int esc01;
        limpar_tela_ansi();
        printf("----------------- OPCOES -----------------\n");
        printf("Voce deseja usar Busca Binária em:\n");
        printf("1 - Eventos\n");
        printf("2 - Usuarios\n");
        printf("Escolha: ");
        scanf("%d", &esc01);

        if(esc01 == 1){
            printf("Você escolheu Eventos!\n");
            printf("A base tem que estar ordenada!\nOrdenando...\n");
                                    
            int total_eventos = tamanho_arquivoEv(arq_eventos);
            heapSort(arq_eventos, total_eventos, TIPO_Evento, &comp, &tro, log);
            printf("Ordenado!\nAgora informe o ID do evento: ");
            scanf("%d", &id);

            rewind(arq_eventos);
            ev = Evento_buscaBinaria_PorId(arq_eventos, id, 0, tamanho_arquivoEv(arq_eventos) - 1, log);

            if (!ev){
                printf("Id de numero %d não encontrado.", id);
                printf("Tente novamente");
                pausarTela();
            }
            imprimirEvento(ev);
            pausarTela();
        }
        else if (esc01 == 2){
            printf("Você escolheu Usuarios!\n");
            printf("A base tem que estar ordenada!\nOrdenando...\n");
                                    
            int total_users = tamanho_arquivoUs(arq_users);
            heapSort(arq_users, total_users, TIPO_User, &comp, &tro, log);
            printf("Ordenado!\nAgora informe o ID do evento: ");
            scanf("%d", &id);

            rewind(arq_users);
            us = User_buscaBinaria_PorID(arq_users, id, 0, tamanho_arquivoUs(arq_users) - 1, log);

            if (!us){
                printf("Id de numero %d não encontrado.", id);
                printf("Tente novamente");
                pausarTela();
            }
            imprimirUser(us);
            pausarTela();
        }
        else{
            printf("Opcao invalida");
        }
    }
    else if (esc == 3) {
        int esc01;
        // limpar_tela_ansi();
        printf("----------------- OPCOES HASH -----------------\n");
        printf("1 - Buscar\n");
        printf("2 - Inserir\n");
        printf("3 - Remover\n");
        printf("Escolha: ");
        scanf("%d", &esc01);

        if (esc01 == 1){
            printf("Digite o ID para a busca: ");
            scanf("%d", &id);
            userEncontrado = buscarUsuario_Hash(arq_hash, arq_overflow, id);
            if(userEncontrado != NULL) {
                printf("Usuario encontado: \n");
                imprimirUser(userEncontrado);
                free(userEncontrado);
            }
            else{
                printf("Usuario com ID %d nao encontrado.\n", id);
            }
            pausarTela();
        }
        else if (esc01 == 2){
            printf("Cadastro rapido:\n");
            cadastrarUsuario(arq_users, "NovoUser", "email@teste.com", "123", "(00)00000-0000", "000.000.000-00", 1);
            rewind(arq_users);
            User *ultimo = lerUsuario(arq_users); // pega último inserido
            inserirUsuario_Hash(arq_hash, arq_overflow, ultimo);
            free(ultimo);
            printf("Usuario inserido na Hash!\n");
            pausarTela();
        }
        else if (esc01 == 3){
            printf("Digite o id para a remocao: ");
            scanf("%d", &id);
            userEncontrado = buscarUsuario_Hash(arq_hash, arq_overflow, id);
            if(userEncontrado != NULL) {
                printf("Usuario encontado: \n");
                imprimirUser(userEncontrado);
                removerUsuario_Hash(arq_hash, arq_overflow, id);
                printf("Usuario com id %d removido!", id);
                free(userEncontrado);
            }
            pausarTela();
        }
    }
}

User *login_logout(FILE *arq_users, User *usuarioLogado){
    int esc02;

    printf("OPCOES - LOGIN/LOGOUT\n");
    printf("1 - Login\n2 - Cadastrar\n3 - Logout\n");
    printf("Escolha: ");
    scanf("%d", &esc02);

    if(esc02 == 1){

        if (usuarioLogado){
            printf("\nVoce ja esta logado.\n");
        }
        else{
            char emailLogin[100] = "davi@email.com";
            char senhaLogin[50] = "davi123";
            printf("--- Login ---\n");
            printf("Email: davi@email.com ");
            printf("Senha: davi123");

            usuarioLogado = loginPorEmailSenha(arq_users, emailLogin, senhaLogin);

            if (usuarioLogado){
                printf("\nLogin bem-sucedido! Bem-vindo(a), %s!\n", usuarioLogado->nome);
            }
            else{
                ERROR("\nEmail ou senha incorretos.\n");
            }
        }

        pausarTela();
    }
    else if(esc02 == 2){
        limpar_tela_ansi(); 
        printf("Informe os dados para o cadastro: \n"); 
        printf("Nome: Davi | Email: davi@email.com | Senha: davi123 | Telefone: (31) 99999-9999 | CPF: 111.222.333-00 | Tipo: Produtor"); 
        cadastrarUsuario(arq_users, "Davi", "davi@email.com", "davi123", "(31) 99999-9999", "111.222.333-00", 0); 
        pausarTela();
    }
    else if (esc02 == 3){
        if (usuarioLogado){
            printf("\n%s deslogado com sucesso.\n", usuarioLogado->nome);
            free(usuarioLogado);
            usuarioLogado = NULL;
        }
        else{
            printf("\nNenhum usuario esta logado.\n");
        }

    pausarTela();
    }

    return usuarioLogado;
}

void menuEventosVisitantes(FILE *arq_eventos) {
    int esc;
    do {
        limpar_tela_ansi();
        printf("--- Menu de Eventos (Visitante) ---\n");
        printf("1 - Listar Eventos\n");
        printf("2 - Voltar ao Menu Principal\n");
        printf("----------------------------------\n");
        printf("Escolha: ");
        scanf("%d", &esc);

        switch (esc) {
        case 1:
            printf("\n--- Lista de Eventos ---\n");
            imprimirBaseEvento(arq_eventos);
            pausarTela();
            break;
        case 2:
            printf("\nVoltando...\n");
            break;
        default:
            ERROR("\nOpção inválida!\n");
            pausarTela();
        }
    } while (esc != 2);
}

void menuEventosProdutor(FILE *arq_eventos) {
    int esc;
    do {
        limpar_tela_ansi();
        printf("--- Menu de Eventos (Produtor) ---\n");
        printf("1 - Cadastrar Novo Evento\n");
        printf("2 - Listar Eventos\n");
        printf("3 - Deletar Evento por ID\n");
        printf("4 - Voltar\n");
        printf("---------------------------------\n");
        printf("Escolha: ");
        scanf("%d", &esc);

        switch (esc) {
        case 1:
            cadastrarEvento(arq_eventos, "Arraial D&A Producoes",
                            "O melhor arraial da regiao!", 100, 20.5);
            pausarTela();
            break;
        case 2:
            imprimirBaseEvento(arq_eventos);
            pausarTela();
            break;
        case 3: {
            int id;
            printf("Digite o ID do evento para excluir: ");
            scanf("%d", &id);
            if (deletarEventoPorId(arq_eventos, id))
                printf("Evento %d excluído com sucesso!\n", id);
            else
                printf("Evento %d não encontrado.\n", id);
            pausarTela();
            break;
        }
        case 4:
            printf("\nVoltando...\n");
            break;
        default:
            ERROR("\nOpção inválida!\n");
            pausarTela();
        }
    } while (esc != 4);
}

void menuEventosCliente(User *usuario, FILE *arq_eventos, FILE *arq_carrinho, FILE *log) {
    int esc;
    do {
        limpar_tela_ansi();
        printf("--- Menu de Eventos (Cliente) ---\n");
        printf("1 - Listar Eventos\n");
        printf("2 - Adicionar Ingresso ao Carrinho\n");
        printf("3 - Voltar\n");
        printf("--------------------------------\n");
        printf("Escolha: ");
        scanf("%d", &esc);

        switch (esc) {
        case 1:
            imprimirBaseEvento(arq_eventos);
            pausarTela();
            break;
        case 2: {
            int idEvento;
            printf("Digite o ID do evento: ");
            scanf("%d", &idEvento);

            Evento *ev = Evento_buscaSequencial_PorId(arq_eventos, idEvento, log);
            if (!ev) {
                printf("Evento não encontrado.\n");
                pausarTela();
                break;
            }

            ItemCarrinho item;
            item.id = 0;
            item.idEvento = ev->id;
            item.quantidade = 1 + rand() % 10;

            adicionarItemAoCarrinho(arq_carrinho, usuario->id, &item);

            printf("Item adicionado ao carrinho!\n");

            free(ev);
            pausarTela();
            break;
        }
        case 3:
            printf("\nVoltando...\n");
            break;
        default:
            ERROR("\nOpção inválida!\n");
            pausarTela();
        }
    } while (esc != 3);
}

void menuEventos(User *usuarioLogado, FILE *arq_eventos, FILE *arq_carrinho, FILE *log) {
    if (usuarioLogado == NULL) {
        menuEventosVisitantes(arq_eventos); 
    } else if (usuarioLogado->tipo == Produtor) {
        menuEventosProdutor(arq_eventos);
    } else if (usuarioLogado->tipo == Cliente) {
        menuEventosCliente(usuarioLogado, arq_eventos, arq_carrinho, log);
    }
}

void menuIngressos(FILE *arq_ingressos, User *usuarioLogado){
    if (usuarioLogado == NULL){
        ERROR("Faça login para visualizar seus ingressos!");
    } 
    else{ 
        rewind(arq_ingressos);
        listarIngressos(arq_ingressos, usuarioLogado->id); 
    } 
    pausarTela();
}

void menuCarrinho(FILE *arq_carrinho, FILE *arq_ingressos, User *usuarioLogado){
    if (usuarioLogado == NULL) {
        ERROR("Faça login para continuar!");
        pausarTela();
        return;
    }

    int esc;
    do {
        limpar_tela_ansi();
        printf("--- Carrinho ---\n");
        printf("1 - Consultar Carrinho\n");
        printf("2 - Limpar Carrinho\n");
        printf("3 - Finalizar Carrinho (comprar)\n");
        printf("4 - Voltar ao menu inicial\n");
        printf("-------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &esc);

        switch (esc) {
            case 1: {
                rewind(arq_carrinho);
                Carrinho *c;
                int encontrou = 0;

                while ((c = lerCarrinho(arq_carrinho)) != NULL) {
                    if (c->idCliente == usuarioLogado->id) {
                        imprimirCarrinho(c);
                        encontrou = 1;
                        free(c);
                        break;
                    }
                    free(c);
                }

                if (!encontrou) {
                    printf("Carrinho vazio ou não encontrado.\n");
                }
                pausarTela();
                break;
            }

            case 2: {
                rewind(arq_carrinho);
                Carrinho *c2;
                int achouRemover = 0;

                while ((c2 = lerCarrinho(arq_carrinho)) != NULL) {
                    if (c2->idCliente == usuarioLogado->id) {
                        achouRemover = 1;
                        if (c2->totalItens > 0) {
                            removerItemDoCarrinho(arq_carrinho, c2->id, c2->itens[0].idEvento);
                            printf("Item removido do carrinho.\n");
                        } else {
                            printf("Carrinho está vazio.\n");
                        }
                        free(c2);
                        break;
                    }
                    free(c2);
                }

                if (!achouRemover) {
                    printf("Carrinho não encontrado.\n");
                }
                pausarTela();
                break;
            }

            case 3: {
                rewind(arq_carrinho);
                Carrinho *c3;
                int finalizado = 0;

                while ((c3 = lerCarrinho(arq_carrinho)) != NULL) {
                    if (c3->idCliente == usuarioLogado->id) {
                        finalizarCarrinho(arq_carrinho, arq_ingressos, c3->id);
                        printf("Carrinho finalizado com sucesso!\n");
                        finalizado = 1;
                        free(c3);
                        break;
                    }
                    free(c3);
                }

                if (!finalizado) {
                    printf("Nenhum carrinho encontrado para finalizar.\n");
                }
                pausarTela();
                break;
            }

            case 4:
                printf("\nVoltando ao menu principal...\n");
                break;

            default:
                ERROR("\nOpção inválida!\n");
                pausarTela();
                break;
        }
    } while (esc != 4);

    pausarTela();
}