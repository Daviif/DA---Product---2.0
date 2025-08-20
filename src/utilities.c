#include "../include/utilities.h"
#include "../include/users.h"
#include "../include/HeapSort.h"

#include <time.h>
#include <stdio.h>


void limpar_tela_ansi()
{
    printf("\033[2J\033[H");
}

char *NomeSeq(char *primeiraLetra)
{
    int tamanho = strlen(primeiraLetra);
    int i = tamanho - 1;

    while (i >= 0)
    {
        primeiraLetra[i]++;

        if (primeiraLetra[i] <= 'Z')
            return primeiraLetra;

        primeiraLetra[i] = 'A';
        i--;
    }

    int novoTam = tamanho + 1;
    char *novaLetra = (char *)malloc(novoTam + 1);
    if (novaLetra == NULL)
        return 0;

    novaLetra[0] = 'A';
    strcpy(novaLetra + 1, primeiraLetra);

    free(primeiraLetra);

    return novaLetra;
}

void embaralhar(int *array, int n)
{
    if (n > 1)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

void pausarTela()
{
    printf("\n\nPressione ENTER para continuar...");
    getchar();
    getchar();
}

int tamanho_registroEv() {
    return sizeof(int) +
           sizeof(char) * 100 +
           sizeof(char) * 150 +
           sizeof(int) +
           sizeof(double);
}

int tamanho_registroUs() {
    return sizeof(User);
}

size_t tamanhoRegistro(int tipoRegistro){
    if (tipoRegistro == TIPO_Evento) {
        return tamanho_registroEv();
    } else {
        return tamanho_registroUs();
    }
}
int tamanho_arquivoEv(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroEv());
    return tam;
}

int tamanho_arquivoUs(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroUs());
    return tam;
}

void gerarDataAtual(char *dest)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // dd/mm/aaaa
    sprintf(dest, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

int gerarIdUnico(FILE *in, size_t tamanhoRegistro)
{
    rewind(in);

    void *registro = malloc(tamanhoRegistro);
    if (!registro)
        return 1;

    int maiorId = 0;
    
    while (fread(registro, tamanhoRegistro, 1, in) == 1)
    {
        int id = *(int *)registro; 
        if (id > maiorId) 
            maiorId = id;
    }

    free(registro);
    return maiorId + 1;

}