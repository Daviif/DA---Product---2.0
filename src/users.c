#include "../include/users.h"
#include "../include/utilities.h"

User *CriarUsuario(int id, char *nome, char *email, char *senha, char *telefone, char *cpf, Tipo usuario)
{
    User *us = (User *)malloc(sizeof(User));

    if (us)
        memset(us, 0, sizeof(User));

    us->id = id;
    strncpy(us->nome, nome, sizeof(us->nome) - 1);
    us->nome[sizeof(us->nome) - 1] = '\0';
    strncpy(us->email, email, sizeof(us->email) - 1);
    us->email[sizeof(us->email) - 1] = '\0';
    strncpy(us->senha, senha, sizeof(us->senha) - 1);
    us->senha[sizeof(us->senha) - 1] = '\0';
    strncpy(us->telefone, telefone, sizeof(us->telefone) - 1);
    us->telefone[sizeof(us->telefone) - 1] = '\0';
    strncpy(us->cpf, cpf, sizeof(us->cpf) - 1);
    us->cpf[sizeof(us->cpf) - 1] = '\0';
    us->tipo = usuario;

    return us;
}

void salvarUsuario(User *us, FILE *out)
{
    fwrite(&us->id, sizeof(int), 1, out);
    fwrite(us->nome, sizeof(char), sizeof(us->nome), out);
    fwrite(us->email, sizeof(char), sizeof(us->email), out);
    fwrite(us->senha, sizeof(char), sizeof(us->senha), out);
    fwrite(us->telefone, sizeof(char), sizeof(us->telefone), out);
    fwrite(us->cpf, sizeof(char), sizeof(us->cpf), out);
    fwrite(&us->tipo, sizeof(Tipo), 1, out);
}

User *lerUsuario(FILE *in)
{
    User *us = (User *)malloc(sizeof(User));

    if (!us)
        return NULL;
    if (fread(&us->id, sizeof(int), 1, in) < 1)
    {
        free(us);
        return NULL;
    }
    fread(us->nome, sizeof(char), sizeof(us->nome), in);
    fread(us->email, sizeof(char), sizeof(us->email), in);
    fread(us->senha, sizeof(char), sizeof(us->senha), in);
    fread(us->telefone, sizeof(char), sizeof(us->telefone), in);
    fread(us->cpf, sizeof(char), sizeof(us->cpf), in);
    fread(&us->tipo, sizeof(Tipo), 1, in);
    return us;
}

void criarBaseUsuarios(FILE *out, int qtdUser)
{
    User *us;

    char nomeUs[100];
    char emailFmt[100];
    char senhaFmt[50];
    char telFmt[50];
    char cpfFmt[20];

    char *letra = (char *)malloc(2);
    if (letra == NULL)
    {
        perror("Falha na alocação inicial");
        return;
    }
    strcpy(letra, "A");

    int *piscina_ids = (int *)malloc(qtdUser * sizeof(int));
    if (!piscina_ids)
    {
        free(letra);
        return;
    }

    for (int i = 0; i < qtdUser; i++)
    {
        piscina_ids[i] = 1 + i;
    }
    embaralhar(piscina_ids, qtdUser);
    printf("Gerando a base de Usuarios...\n");

    rewind(out);

    for (int i = 0; i < qtdUser; i++)
    {
        int idUnico = piscina_ids[i];

        sprintf(nomeUs, "Usuario %s", letra);
        sprintf(emailFmt, "email%d@email.com", i);
        sprintf(senhaFmt, "user%d", i);
        sprintf(telFmt, "(00) 99988-766%d%d", idUnico % 10, (idUnico + 1) % 10);
        sprintf(cpfFmt, "000.000.000-%02d", idUnico % 100);

        us = CriarUsuario(idUnico, nomeUs, emailFmt, senhaFmt, telFmt, cpfFmt, idUnico % 5 == 0 ? Produtor : Cliente);

        if (us)
        {
            salvarUsuario(us, out);
            free(us); //
        }

        letra = NomeSeq(letra);
        if (letra == NULL)
        {
            perror("Falha ao gerar próxima sequência de nome");
            break;
        }
    }

    free(letra);
    free(piscina_ids);
    printf("Base de Usuários gerada com sucesso!\n");
}

void imprimirUser(User *us)
{
    if (!us)
        return;
    printf("**********************************************");
    printf("\nID do Usuario: ");
    printf("%d", us->id);
    printf("\nNome: ");
    printf("%s", us->nome);
    printf("\nEmail: ");
    printf("%s", us->email);
    printf("\nTelefone: ");
    printf("%s", us->telefone);
    printf("\nCPF: ");
    printf("%s", us->cpf);
    printf("\nTipo do Usuario: ");
    printf("%d - %s", us->tipo, us->tipo == 0 ? "Produtor" : "Cliente");
    printf("\n**********************************************\n");
}

void imprimirBaseUser(FILE *out)
{
    rewind(out);
    User *us;

    while ((us = lerUsuario(out)) != NULL)
    {
        imprimirUser(us);
        free(us);
    }
}

void *cadastrarUsuario(FILE *out, char *nome, char *email, char *senha, char *telefone, char *cpf, Tipo Usuario)
{
    int novoId = gerarIdUnico(out, tamanho_registroUs());
    User *us = CriarUsuario(novoId, nome, email, senha, telefone, cpf, Usuario);

    fseek(out, 0, SEEK_END);

    if (us)
    {
        salvarUsuario(us, out);
        printf("\nUsuario '%s' cadastrado com sucesso com o ID: %d\n", us->nome, us->id);
        free(us);
    }
    return NULL;
}

User *loginPorEmailSenha(FILE *in, const char *email, const char *senha)
{
    rewind(in);
    User *us;

    while ((us = lerUsuario(in)) != NULL)
    {
        if (strcmp(us->email, email) == 0 && strcmp(us->senha, senha) == 0)
        {
            return us;
        }
        free(us);
    }

    return NULL;
}