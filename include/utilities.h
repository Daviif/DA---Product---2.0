#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void limpar_tela_ansi();

typedef void (*FuncaoSalvar)(void *dados, FILE *out);

char *NomeSeq(char *primeiraLetra);

void embaralhar(int *array, int n);

void pausarTela();

int tamanho_arquivo(FILE *arq);

int tamanho_registroUs();

int tamanho_registroEv();

int tamanho_arquivoEv(FILE *arq);

int tamanho_arquivoUs(FILE *arq);

size_t tamanhoRegistro(int tipoRegistro);

void gerarDataAtual(char *dest);

int gerarIdUnico(FILE *in, size_t tamanhoRegistro);

#endif