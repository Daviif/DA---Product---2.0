#include "../include/IntercalacaoOtima.h"


int encontrarMenorId(ArquivoEntrada *arquivos, int numArquivos) {
    int menorIndice = -1;
    int menorId = INT_MAX;
    
    for (int i = 0; i < numArquivos; i++) {
        if (arquivos[i].ativo && arquivos[i].registroAtual.id < menorId) {
            menorId = arquivos[i].registroAtual.id;
            menorIndice = i;
        }
    }
    
    return menorIndice;
}

// Função para ler próximo registro de um arquivo
int lerProximoRegistro(ArquivoEntrada *arquivo) {
    Evento *ev = lerEventos(arquivo->arquivo);
    if (ev) {
        arquivo->registroAtual = *ev;
        free(ev);
        return 1;
    } else {
        arquivo->ativo = 0;
        return 0;
    }
}

// Função para intercalar múltiplos arquivos ordenados
int intercalarArquivos(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log) {
    clock_t inicioT = clock();
    long comparacoes = 0;
    
    ArquivoEntrada *arquivos = malloc(numArquivos * sizeof(ArquivoEntrada));
    if (!arquivos) {
        fprintf(stderr, "Erro ao alocar memória para arquivos.\n");
        return -1;
    }
    
    // Abre todos os arquivos de entrada
    for (int i = 0; i < numArquivos; i++) {
        char caminhoCompleto[200];
        sprintf(caminhoCompleto, "%s", nomesArquivos[i]);
        arquivos[i].arquivo = fopen(caminhoCompleto, "rb");
        if (!arquivos[i].arquivo) {
            fprintf(stderr, "Erro ao abrir arquivo %s.\n", nomesArquivos[i]);
            for (int j = 0; j < i; j++) {
                fclose(arquivos[j].arquivo);
            }
            free(arquivos);
            return -1;
        }
        strcpy(arquivos[i].nomeArquivo, nomesArquivos[i]);
        arquivos[i].ativo = 1;
        
        // Lê o primeiro registro de cada arquivo
        if (!lerProximoRegistro(&arquivos[i])) {
            arquivos[i].ativo = 0;
        }
    }
    
    // Abre arquivo de saída
    FILE *arquivoSaida = fopen(nomeSaida, "wb");
    if (!arquivoSaida) {
        fprintf(stderr, "Erro ao criar arquivo de saída %s.\n", nomeSaida);
        for (int i = 0; i < numArquivos; i++) {
            fclose(arquivos[i].arquivo);
        }
        free(arquivos);
        return -1;
    }
    
    int registrosEscritos = 0;
    
    // Intercala os registros
    while (1) {
        int menorIndice = encontrarMenorId(arquivos, numArquivos);
        
        if (menorIndice == -1) {
            break; // Todos os arquivos foram processados
        }
        
        // Escreve o registro com menor ID
        salvarEvento(&arquivos[menorIndice].registroAtual, arquivoSaida);
        registrosEscritos++;
        comparacoes++;
        
        // Lê próximo registro do arquivo que foi usado
        lerProximoRegistro(&arquivos[menorIndice]);
    }
    
    // Fecha todos os arquivos
    for (int i = 0; i < numArquivos; i++) {
        fclose(arquivos[i].arquivo);
    }
    fclose(arquivoSaida);
    free(arquivos);
    
    clock_t fimT = clock();
    double tempo_execucao = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    fprintf(log, "\n------------------------------\n");
    fprintf(log, "Algoritmo: Intercalação Ótima (Eventos)\n");
    fprintf(log, "Tempo de Execução: %.6f segundos\n", tempo_execucao);
    fprintf(log, "Comparações: %ld\n", comparacoes);
    fprintf(log, "Registros escritos: %d\n", registrosEscritos);
    fprintf(log, "Arquivo de saída: %s\n", nomeSaida);
    fprintf(log, "------------------------------\n");
    
    return registrosEscritos;
}

// Função principal de Intercalação Ótima
int intercalacaoOtima(char **nomesArquivos, int numArquivos, char *nomeSaida, FILE *log) {
    if (numArquivos <= 0) {
        fprintf(stderr, "Número inválido de arquivos para intercalar.\n");
        return -1;
    }
    
    if (numArquivos == 1) {
        // Se há apenas um arquivo, apenas copia
        char caminhoCompleto[200];
        char caminhoCompleto2[200];
        sprintf(caminhoCompleto, "%s", nomesArquivos[0]);
        sprintf(caminhoCompleto2, "%s", nomeSaida);
        FILE *origem = fopen(caminhoCompleto, "rb");
        FILE *destino = fopen(caminhoCompleto2, "wb");
        
        if (!origem || !destino) {
            fprintf(stderr, "Erro ao copiar arquivo único.\n");
            if (origem) fclose(origem);
            if (destino) fclose(destino);
            return -1;
        }
        
        Evento *ev;
        int registrosCopiados = 0;
        while ((ev = lerEventos(origem)) != NULL) {
            salvarEvento(ev, destino);
            free(ev);
            registrosCopiados++;
        }
        
        fclose(origem);
        fclose(destino);
        
        fprintf(log, "\n------------------------------\n");
        fprintf(log, "Algoritmo: Intercalação Ótima (Cópia Simples)\n");
        fprintf(log, "Registros copiados: %d\n", registrosCopiados);
        fprintf(log, "Arquivo de saída: %s\n", nomeSaida);
        fprintf(log, "------------------------------\n");
        
        return registrosCopiados;
    }
    
    return intercalarArquivos(nomesArquivos, numArquivos, nomeSaida, log);
}

// Função para ordenar arquivo usando intercalação ótima
int ordenarPorIntercalacaoOtima(FILE *arqEventos, int memDisponivel, int totalRegistros, FILE *log) {
    clock_t inicioT = clock();
    
    // Primeiro, cria as corridas usando seleção por substituição
    int numRuns = selecaoSubstituicao(arqEventos, memDisponivel, totalRegistros, log);
    
    if (numRuns <= 0) {
        fprintf(stderr, "Erro na criacao das corridas.\n");
        return -1;
    }
    
    // Aloca array com nomes dos arquivos de corrida
    char **nomesArquivos = malloc(numRuns * sizeof(char*));
    if (!nomesArquivos) {
        fprintf(stderr, "Erro ao alocar memoria para nomes de arquivos.\n");
        return -1;
    }
    
    // Preenche nomes dos arquivos de corrida
    for (int i = 0; i < numRuns; i++) {
        nomesArquivos[i] = malloc(100 * sizeof(char));
        sprintf(nomesArquivos[i], "run_eventos_%d.dat", i);
    }
    
    // Intercala as corridas para criar arquivo final ordenado
    int resultado = intercalacaoOtima(nomesArquivos, numRuns, "eventos_ordenados.dat", log);
    
    // Libera memória
    for (int i = 0; i < numRuns; i++) {
        free(nomesArquivos[i]);
    }
    free(nomesArquivos);
    
    clock_t fimT = clock();
    double tempo_total = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    fprintf(log, "\nTempo total de ordenacao: %.6f segundos\n", tempo_total);
    printf("\nOrdenacao por Intercalacao Otima concluida!\n");
    printf("Arquivo ordenado: eventos_ordenados.dat\n");
    
    return resultado;
}