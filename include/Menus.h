#ifndef _MENUS_H
#define _MENUS_H

#include "eventos.h"
#include "users.h"
#include "utilities.h"
#include "HeapSort.h"
#include "buscas.h"
#include "carrinho.h"
#include "IntercalacaoOtima.h"
#include "selecaoSubstituicao.h"
#include "Hash.h"

void menuOrdenacao(FILE *arq_eventos, FILE *arq_users, FILE *log, int memDisponivel, int comp, int tro);
void menuBusca(FILE *arq_eventos, FILE *arq_users, FILE *arq_hash, FILE *arq_overflow, FILE *log, int comp, int tro); 
User *login_logout(FILE *arq_users, User *usuarioLogado);
void menuEventosVisitantes(FILE *arq_eventos); 
void menuEventosProdutor(FILE *arq_eventos);
void menuEventosCliente(User *usuario, FILE *arq_eventos, FILE *arq_carrinho, FILE *log);
void menuEventos(User *usuarioLogado, FILE *arq_eventos, FILE *arq_carrinho, FILE *log);
void menuIngressos(FILE *arq_ingressos, User *usuarioLogado);
void menuCarrinho(FILE *arq_carrinho, FILE *arq_ingressos, User *usuarioLogado);

#endif