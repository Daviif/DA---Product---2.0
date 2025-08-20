#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef struct
{
    int id;
    char nome[100];
    char descricao[150];
    int qtdIngresso;
    double valorIngresso;
} Evento;

Evento *criarEvento(int id, char *nome, char *descricao, int qtdIngresso, double valorIngresso);

Evento *lerEventos(FILE *in);

void salvarEvento(Evento *ev, FILE *out);

void criarBaseEventos(FILE *out, int qtdEventos);

void imprimirEvento(Evento *ev);

void imprimirBaseEvento(FILE *out);


void *cadastrarEvento(FILE *out, char *nome, char *descricao, int qtdIngresso, double valorIngresso);

int deletarEventoPorId(FILE *out, int idParaDeletar);

#endif