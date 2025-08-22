#include "../include/Hash.h"

#include <stdlib.h>
#include <string.h>

void criarTabelaHash(char* hashFileName){
    FILE *arq_Hash = fopen(hashFileName, "wb");
    if (!arq_Hash) {
        perror("Nao foi possivel criar o arquivo de hash");
        exit(1);
    }

    User adressVoid;
    adressVoid.id = -1;
    adressVoid.proximo = -1;

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        fwrite(&adressVoid, sizeof(User), 1, arq_Hash);
    }

    fclose(arq_Hash);
}

int funcaoHash(int id) {
    return id % TAMANHO_TABELA;
}

User *buscarUsuario_Hash(FILE *hashFile, FILE *overflowFile, int id){
    int indice = funcaoHash(id);
    long posicao = (long)indice * sizeof(User);

    User *address = (User*) malloc(sizeof(User));
    if(!address) return NULL;

    fseek(hashFile, posicao, SEEK_SET);
    if(fread(address, sizeof(User), 1, hashFile) != 1){
        free(address);
        return NULL;
    }

    if(address->id == -1) {
        free(address);
        return NULL;
    }
    User atual = *address;

    // Percorre a lista encadeada (address principal + overflow)
    while (1) {
        if (atual.id == -1) {
            break; // Fim da busca, não encontrado
        }
        if (atual.id == id) {
            *address = atual;
            return address;
        }
        if (atual.proximo == -1) {
            break;
        }
        fseek(overflowFile, atual.proximo, SEEK_SET);
        if (fread(&atual, sizeof(User), 1, overflowFile) != 1){
            break;
        }
    }

    free(address);
    return NULL; // Não encontrado
}


void inserirUsuario_Hash(FILE *hashFile, FILE *overflowFile, User *us){
  
    int indice = funcaoHash(us -> id);
    long posicaoAddress = (long)indice * sizeof(User);
    long novaPosicaoOverflow;

    fseek(hashFile, posicaoAddress, SEEK_SET);
    User address;
    fread(&address, sizeof(User), 1, hashFile);

    us -> proximo = -1;

    if (address.id == -1){
        fseek(hashFile, posicaoAddress, SEEK_SET);
        fwrite(us, sizeof(User), 1, hashFile);
        fflush(hashFile);

        printf("[DEBUG] Usuario %d inserido direto no bucket %d (pos=%ld)\n",
               us ->id, indice, posicaoAddress);
        return;
    }
    else{
        fseek(overflowFile, 0, SEEK_END);
        novaPosicaoOverflow = ftell(overflowFile);
        fwrite(us, sizeof(User), 1, overflowFile);
        fflush(overflowFile);

        printf("[DEBUG] Colisão para usuario %d no bucket %d\n", us->id, address);
        printf("        Usuario foi gravado no overflow (pos=%ld)\n", novaPosicaoOverflow);
    }

    User atual = address;
    long posicaoAtual = posicaoAddress;
    FILE * arqAtual = hashFile;

    while(atual.proximo != -1){
        posicaoAtual = atual.proximo;
        arqAtual = overflowFile;
        fseek(overflowFile, posicaoAtual, SEEK_SET);
        fread(&atual, sizeof(User), 1, overflowFile);
    }

    

    atual.proximo = novaPosicaoOverflow;
    fseek(arqAtual, posicaoAtual, SEEK_SET);
    fwrite(&atual, sizeof(User), 1, arqAtual);
    fflush(arqAtual);
}

void removerUsuario_Hash(FILE *hashFile, FILE *overflowFile, int id){
    int indice = funcaoHash(id);
    long posicao = (long)indice * sizeof(User);

    User atual, anterior;
    long posicaoAnterior = -1;
    FILE *arqAnterior = NULL;

    fseek(hashFile, posicao, SEEK_SET);
    fread(&atual, sizeof(User), 1, hashFile);
    
    while (atual.id != -1){
        if(atual.id == id){
            if (posicaoAnterior == -1){
                if(atual.proximo != -1){
                    fseek(overflowFile, atual.proximo, SEEK_SET);
                    User proximoUser;
                    fread(&proximoUser, sizeof(User), 1, overflowFile);
                    fseek(hashFile, posicao, SEEK_SET);
                    fwrite(&proximoUser, sizeof(User), 1, hashFile);
                }
                else{
                    atual.id = -1;
                    atual.proximo = -1;
                    fseek(hashFile, posicao, SEEK_SET);
                    fwrite(&atual, sizeof(User), 1, hashFile);
                }
            }
            else { 
                anterior.proximo = atual.proximo;
                fseek(arqAnterior, posicaoAnterior, SEEK_SET);
                fwrite(&anterior, sizeof(User), 1, arqAnterior);
            }
            return;
        }

        posicaoAnterior = (arqAnterior == hashFile) ? posicao : ftell(overflowFile) - sizeof(User);
        arqAnterior = (posicaoAnterior == posicao) ? hashFile : overflowFile;
        anterior = atual;

        if (atual.proximo == -1) break;

        fseek(overflowFile, atual.proximo, SEEK_SET);
        fread(&atual, sizeof(User), 1, overflowFile);
    }
}