#ifndef _INTERCALACAO_OTIMA_
#define _INTERCALACAO_OTIMA_

#include <stdio.h>
#include <limits.h>

#include "eventos.h"
#include "utilities.h"
#include "selecaoSubstituicao.h"

// Funções para Intercalação Ótima
typedef struct {
    FILE *arquivo;
    Evento registroAtual;
    int ativo;
    char nomeArquivo[100];
} ArquivoEntrada;

int encontrarMenorId(ArquivoEntrada *arquivos, int numArquivos);
int lerProximoRegistro(ArquivoEntrada *arquivo);
int intercalarArquivos(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log);
int intercalacaoOtima(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log);
int ordenarPorIntercalacaoOtima(FILE *arqEventos, int memDisponivel, int totalRegistros, FILE *log);


#endif