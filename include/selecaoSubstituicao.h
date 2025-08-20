#ifndef _SELECAO_SUBSTITUICAO_
#define _SELECAO_SUBSTITUICAO

#include <stdio.h>
#include <limits.h>

#include "eventos.h"
#include "utilities.h"


int obterIdEvento(void *reservatorio, int indice);

void lerEventoParaReservatorio(void *reservatorio, int indice, FILE *arqEventos);

void escreverEventoDoReservatorio(void *reservatorio, int indice, FILE *arqSaida);

int selecaoSubstituicao(FILE *arqEventos, int memDisponivel, int totalRegistros, FILE *log);

#endif