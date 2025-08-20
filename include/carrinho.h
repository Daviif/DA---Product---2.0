#ifndef _CARRINHO_H
#define _CARRINHO_H

#include "eventos.h"
#include "users.h"

typedef struct
{
    int id;
    int idEvento;
    int quantidade;
} ItemCarrinho;

typedef struct
{
    int id;
    int idCliente;
    ItemCarrinho itens[50];
    int totalItens;
} Carrinho;

typedef struct
{
    int id;
    int idEvento;
    int idCliente;
    char dtCompra[11]; // dd/mm/aaaa
} Ingresso;

Carrinho *criarCarrinho(int id, int idCliente);

Carrinho *lerCarrinho(FILE *in);

void salvarCarrinho(Carrinho *car, FILE *out);

void imprimirCarrinho(Carrinho *car);

void adicionarItemAoCarrinho(FILE *out, int idCarrinho, ItemCarrinho *item);

void removerItemDoCarrinho(FILE *out, int idCarrinho, int idItem);

int limparCarrinho(FILE *out, int id);

void finalizarCarrinho(FILE *car_out, FILE *ingressos_out, int idCarrinho);

void listarIngressos(FILE *out, int idCliente);

#endif
