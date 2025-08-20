#include "../include/selecaoSubstituicao.h"

int obterIdEvento(void *reservatorio, int indice) {
    Evento *eventos = (Evento*)reservatorio;
    return eventos[indice].id;
}

void lerEventoParaReservatorio(void *reservatorio, int indice, FILE *arqEventos) {
    Evento *eventos = (Evento*)reservatorio;
    Evento *ev = lerEventos(arqEventos);
    if (ev) {
        eventos[indice] = *ev;
        free(ev);
    }
}

void escreverEventoDoReservatorio(void *reservatorio, int indice, FILE *arqSaida) {
    Evento *eventos = (Evento*)reservatorio;
    salvarEvento(&eventos[indice], arqSaida);
}

int selecaoSubstituicao(FILE *arqEventos, int memDisponivel, int totalRegistros, FILE *log) {
    clock_t inicioT = clock();
    long comparacoes = 0;
    
    int tamReg = tamanho_registroEv();
    
    // Aloca o reservatório (heap) com a memória disponível
    Evento *reservatorio = malloc(memDisponivel * sizeof(Evento));
    int *congelado = calloc(memDisponivel, sizeof(int));
    
    if (!reservatorio || !congelado) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return -1;
    }
    
    rewind(arqEventos);
    
    int numRuns = 0;
    int registrosProcessados = 0;
    
    // Carrega inicialmente o reservatório
    int registrosLidos = 0;
    for (int i = 0; i < memDisponivel && registrosLidos < totalRegistros; i++) {
        Evento *ev = lerEventos(arqEventos);
        if (ev) {
            reservatorio[i] = *ev;
            free(ev);
            registrosLidos++;
        } else {
            congelado[i] = 1; // Marca como congelado se não conseguiu ler
        }
    }
    
    while (registrosProcessados < totalRegistros) {
        numRuns++;
        char nomeParticao[100];
        sprintf(nomeParticao, "run_eventos_%d.dat", numRuns - 1);
        
        FILE *arqSaida = fopen(nomeParticao, "wb");
        if (!arqSaida) {
            fprintf(stderr, "Erro ao criar partição %s.\n", nomeParticao);
            free(reservatorio);
            free(congelado);
            return -1;
        }
        
        int ultimoId = -1;
        int registrosAtivos = 0;
        
        // Conta registros ativos (não congelados)
        for (int i = 0; i < memDisponivel; i++) {
            if (!congelado[i]) {
                registrosAtivos++;
            }
        }
        
        // Processa a corrida atual
        while (registrosAtivos > 0) {
            int minIndice = -1;
            int minId = INT_MAX;
            
            // Encontra o menor ID entre os registros ativos
            for (int i = 0; i < memDisponivel; i++) {
                if (!congelado[i]) {
                    comparacoes++;
                    if (reservatorio[i].id < minId) {
                        minId = reservatorio[i].id;
                        minIndice = i;
                    }
                }
            }
            
            if (minIndice == -1) {
                break; // Não há mais registros ativos
            }
            
            // Escreve o registro com menor ID
            salvarEvento(&reservatorio[minIndice], arqSaida);
            ultimoId = reservatorio[minIndice].id;
            registrosProcessados++;
            
            // Tenta ler um novo registro
            if (registrosLidos < totalRegistros) {
                Evento *novoEv = lerEventos(arqEventos);
                if (novoEv) {
                    reservatorio[minIndice] = *novoEv;
                    free(novoEv);
                    registrosLidos++;
                    
                    // Verifica se o novo registro deve ser congelado
                    if (reservatorio[minIndice].id < ultimoId) {
                        congelado[minIndice] = 1;
                        registrosAtivos--;
                    }
                } else {
                    // Não há mais registros para ler
                    congelado[minIndice] = 1;
                    registrosAtivos--;
                }
            } else {
                // Não há mais registros para ler
                congelado[minIndice] = 1;
                registrosAtivos--;
            }
        }
        
        fclose(arqSaida);
        
        // Reseta os congelados para a próxima corrida
        // Reseta congelado e limpa posições do reservatório
        for (int i = 0; i < memDisponivel; i++) {
            congelado[i] = 0;
        }

    }
    
    free(reservatorio);
    free(congelado);
    
    clock_t fimT = clock();
    double tempo_execucao = ((double)(fimT - inicioT)) / CLOCKS_PER_SEC;
    
    fprintf(log, "\n--------------%d Execução----------------\n", numRuns);
    fprintf(log, "Algoritmo: Seleção com Substituição (Corridas de Eventos)\n");
    fprintf(log, "Tempo de Execução: %.6f segundos\n", tempo_execucao);
    fprintf(log, "Comparações: %ld\n", comparacoes);
    fprintf(log, "Número de corridas geradas: %d\n", numRuns);
    fprintf(log, "Arquivos gerados: run_eventos_0.dat, ...\n");
    fprintf(log, "------------------------------\n");
    
    printf("\nCriação de corridas finalizada. %d corridas foram geradas.\n", numRuns);
    
    return numRuns;
}