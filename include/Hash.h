#ifndef _HASH_H
#define _HASH_H

#include "users.h"
#include <stdio.h>

#define HASH_FILE "users.hash"
#define OVERFLOW_FILE "users.overflow"
#define TAMANHO_TABELA 101

typedef struct No {
    User* us;
    struct No* proximo;
} No;

typedef struct {
    No* tabela[TAMANHO_TABELA];
    int total_usuarios;
    int colisoes;
} HashTable;

HashTable *criarTabelaHash(char* hashFileName);
int funcaoHash(int id);
User *buscarUsuario_Hash(FILE *hashFile, FILE *overflowFile, int id);
void inserirUsuario_Hash(FILE *hashFile, FILE *overflowFile, User *us);
void removerUsuario_Hash(FILE *hashFile, FILE *overflowFile, int id);
void liberarTabelaHash_Hash(HashTable* tabela);

#endif