#include "../include/carrinho.h"
#include "../include/utilities.h"

Carrinho *criarCarrinho(int id, int idCliente)
{
    Carrinho *car = (Carrinho *)malloc(sizeof(Carrinho));
    if (car)
        memset(car, 0, sizeof(Carrinho));

    car->id = id;
    car->idCliente = idCliente;
    car->totalItens = 0;

    return car;
}

void salvarCarrinho(Carrinho *car, FILE *out)
{
    fwrite(&car->id, sizeof(int), 1, out);
    fwrite(&car->idCliente, sizeof(int), 1, out);
    fwrite(&car->totalItens, sizeof(int), 1, out);
    fwrite(car->itens, sizeof(ItemCarrinho), car->totalItens, out);
}

Carrinho *lerCarrinho(FILE *in)
{
    Carrinho *car = (Carrinho *)malloc(sizeof(Carrinho));
    if (!car)
        return NULL;

    if (fread(&car->id, sizeof(int), 1, in) < 1)
    {
        free(car);
        return NULL;
    }

    fread(&car->idCliente, sizeof(int), 1, in);
    fread(&car->totalItens, sizeof(int), 1, in);
    fread(car->itens, sizeof(ItemCarrinho), car->totalItens, in);

    return car;
}

void imprimirCarrinho(Carrinho *car)
{
    if (!car)
        return;

    printf("\n*************** CARRINHO ***************\n");
    printf("ID: %d\n", car->id);
    printf("Cliente: %d\n", car->idCliente);
    printf("Total de Itens: %d\n", car->totalItens);

    for (int i = 0; i < car->totalItens; i++)
    {
        printf("\nItem %d:\n", i + 1);
        printf("  Evento ID: %d\n", car->itens[i].idEvento);
        printf("  Quantidade: %d\n", car->itens[i].quantidade);
    }
    printf("***************************************\n");
}

void adicionarItemAoCarrinho(FILE *out, int idCarrinho, ItemCarrinho *item)
{
    rewind(out);
    Carrinho *car;
    FILE *temp = tmpfile();

    while ((car = lerCarrinho(out)) != NULL)
    {
        if (car->id == idCarrinho)
        {
            if (car->totalItens < 50)
            {
                car->itens[car->totalItens++] = *item;
            }
        }
        salvarCarrinho(car, temp);
        free(car);
    }

    rewind(out);
    freopen(NULL, "wb+", out);
    rewind(temp);

    while ((car = lerCarrinho(temp)) != NULL)
    {
        salvarCarrinho(car, out);
        free(car);
    }

    fclose(temp);
}

void removerItemDoCarrinho(FILE *out, int idCarrinho, int idEvento)
{
    rewind(out);
    Carrinho *car;
    FILE *temp = tmpfile();

    while ((car = lerCarrinho(out)) != NULL)
    {
        if (car->id == idCarrinho)
        {
            int novaQtd = 0;
            for (int i = 0; i < car->totalItens; i++)
            {
                if (car->itens[i].idEvento != idEvento)
                {
                    car->itens[novaQtd++] = car->itens[i];
                }
            }
            car->totalItens = novaQtd;
        }
        salvarCarrinho(car, temp);
        free(car);
    }

    rewind(out);
    freopen(NULL, "wb+", out);
    rewind(temp);

    while ((car = lerCarrinho(temp)) != NULL)
    {
        salvarCarrinho(car, out);
        free(car);
    }

    fclose(temp);
}

int limparCarrinho(FILE *out, int idCarrinho)
{
    rewind(out);
    Carrinho *car;
    FILE *temp = tmpfile();
    int achou = 0;

    while ((car = lerCarrinho(out)) != NULL)
    {
        if (car->id == idCarrinho)
        {
            car->totalItens = 0;
            memset(car->itens, 0, sizeof(car->itens));

            achou = 1;
        }
        salvarCarrinho(car, temp);
        free(car);
    }

    rewind(out);
    freopen(NULL, "wb+", out);
    rewind(temp);

    while ((car = lerCarrinho(temp)) != NULL)
    {
        salvarCarrinho(car, out);
        free(car);
    }

    fclose(temp);
    return achou;
}

void finalizarCarrinho(FILE *car_out, FILE *ingressos_out, int idCarrinho)
{
    rewind(car_out);
    Carrinho *car;
    FILE *temp = tmpfile();

    while ((car = lerCarrinho(car_out)) != NULL)
    {
        if (car->id == idCarrinho)
        {
            for (int i = 0; i < car->totalItens; i++)
            {
                Ingresso ing;
                ing.id = gerarIdUnico(ingressos_out, sizeof(Ingresso));
                ing.idEvento = car->itens[i].idEvento;
                ing.idCliente = car->idCliente;
                gerarDataAtual(ing.dtCompra);

                fwrite(&ing, sizeof(Ingresso), 1, ingressos_out);
            }
            car->totalItens = 0;
        }
        salvarCarrinho(car, temp);
        free(car);
    }

    rewind(car_out);
    freopen(NULL, "wb+", car_out);
    rewind(temp);

    while ((car = lerCarrinho(temp)) != NULL)
    {
        salvarCarrinho(car, car_out);
        free(car);
    }

    fclose(temp);
}

void listarIngressos(FILE *out, int idCliente)
{
    rewind(out);
    Ingresso ing;

    printf("\n*********** Ingressos do Cliente %d ***********\n", idCliente);

    while (fread(&ing, sizeof(Ingresso), 1, out) == 1)
    {
        if (ing.idCliente == idCliente)
        {
            printf("\nID Ingresso: %d\n", ing.id);
            printf("Evento: %d\n", ing.idEvento);
            printf("Data da Compra: %s\n", ing.dtCompra);
        }
    }

    printf("*********************************************\n");
}
