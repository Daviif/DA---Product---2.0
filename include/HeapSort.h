#ifndef _HEAP_SORT_
#define _HEAP_SORT_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventos.h"
#include "users.h"

typedef enum{
    TIPO_Evento,
    TIPO_User
} TipoR;

typedef struct {
    TipoR tipo;
    union {
        Evento evento;
        User user;
    } dado;
} RegistroHeap;

void lerRegistro(FILE *arq, void *registro, int pos, int tipoRegistro);

void escreverRegistro(FILE *arq, void *registro, int pos, int tipoRegistro);

int obterId(void *registro, int tipoRegistro);

void trocarRegistros(FILE *arq, int i, int j, int tipoRegistro);

void heapify(FILE *arq, int n, int i, int tipoRegistro, int *c, int *t);

void heapSort(FILE *arq, int n, int tipoRegistro, int *c, int *t,FILE *log);
#endif