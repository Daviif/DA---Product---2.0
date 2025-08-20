#ifndef USERS_H
#define USERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROD 100
#define MAX_CLI 500000

typedef enum{
    Produtor,
    Cliente
} Tipo;

typedef struct{
    int id;
    char nome[100];
    char email[100];
    char senha[50];
    char telefone[11];
    char cpf[11];
    Tipo tipo;
    long proximo;
} User;

User *CriarUsuario(int id, char *nome, char *email, char *senha, char *telefone, char *cpf, Tipo usuario);

User *lerUsuario(FILE *in);

User *loginPorEmailSenha(FILE *in, const char *email, const char *senha);

void salvarUsuario(User *us, FILE *out);

void criarBaseUsuarios(FILE *out, int qtdUser);

void imprimirUser(User *us);

void imprimirBaseUser(FILE *out);

void *cadastrarUsuario(FILE *out, char *nome, char *email, char *senha, char *telefone, char *cpf, Tipo Usuario);

#endif