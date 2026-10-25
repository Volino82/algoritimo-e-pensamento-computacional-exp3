#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================================================================
//  VARIÁVEIS GLOBAIS DO SISTEMA
//=============================================================================================================================
#define DIAS 7          // Dias analisados
#define BAIRROS 5       // Total de bairros analisados
#define TAM 30          // Tamanho máximo do nome do Bairro
#define TABELA 130      // Largura padrão da tabela de visualização

//=============================================================================================================================
//  FUNÇÕES AUXILIARES DE VISUALIZAÇÃO
//=============================================================================================================================

//Centraliza uma string com uma largura especificada
void centraliza (const char *txt, int largura)
{
    int tamanho = strlen(txt);
    // Caso o texto seja maior ou igual a largura, imprime diretamente
    if (tamanho >= largura)
    {
        printf("%s", txt);
    }
    // Calcula os espaços em branco para a centralização dos texto
    else
    {
        int espaco_esq = (largura - tamanho) / 2;                   // Calcula os espaços em branco à esquerda
        int espaco_dir = largura - tamanho - espaco_esq;            // Calcula os espaços em branco à direita
        printf("%*s%s%*s", espaco_esq, "", txt, espaco_dir, "");
    }
}

// Cria uma linha a partir divisória a repetindo a string "c" "n" vezes
void linha (const char *c,int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s",c);
    }
    printf("\n");
}

// Cria um banner de Título para representação das tabelas
void titulo (char *txt, int n)
{
    linha ("=-", n/2);
    centraliza(txt, n);
    printf("\n");
    linha("=-", n/2);
}

//=============================================================================================================================
//  LEITURA E PROCESSAMENTO DE DADOS
//=============================================================================================================================

// Faz a leitura dos dados presentos no arquivo contendo as informações dos vetores bairros e datas e da matriz intensidade de chuvas.
int leitura_dados_csv(char bairros[BAIRROS][TAM], float intensidade_chuva[DIAS][BAIRROS], char datas[DIAS][10])
{
    FILE *fp = fopen("dados.csv", "r"); //arquivo localizado na pasta /output

    // Validação de existência do arquivo
    if (fp == NULL)
    {
        printf("[ERRO] - Não foi possível abrir o arquivo!\n");
        printf("Verifique a exitência do arquivo no local correto.\n");
        return 0; // Retorna 0 indicando falha
    }
    
    char linha_csv[256]; // Buffer temporário para leitura de  cada linha no arquivo dados.csv

    // 1 - Leitura da primeira linha do arquivo contendo o cabeçalho (Bairros)
    if (fgets(linha_csv, sizeof(linha_csv), fp) != NULL)
    {
        char *token = strtok(linha_csv, ",\n\r");  // Ignora a primeira coluna do cabeçalho ("Dia")

        int b = 0;
        // Fatia a string nos pontos contendo "," ou "enter"(/n)
        while ((token = strtok(NULL, ",\n\r")) != NULL && b < BAIRROS) //  Uso do "/r" para evitar erros de leitura 
        {
            strncpy(bairros[b], token, TAM - 1);
            bairros[b][TAM - 1] = '\0';     // Garante o término da string com caracter nulo
            b++;
        }
    }

    // 2 - Leitura das linhas seguintes 
    int d = 0;
    while (fgets(linha_csv, sizeof(linha_csv), fp) != NULL && d < DIAS)
    {
        // Token[0] - Armazena a data no formato string (ex.: "18/10")
        char *token = strtok(linha_csv, ",\n\r");
        if (token != NULL)
        {
            strncpy(datas[d], token, 9);
            datas[d][9] = '\0'; // Término seguro da string da data
        }

        // Tokens seguintes - Converte e salva os valores das chuvas em milimetros
        int b = 0;
        while ((token = strtok(NULL, ",\n\r")) != NULL && b < BAIRROS)
        {
            intensidade_chuva[d][b] = (float)atof(token); // Converte string para float
            b++;
        }
        d++;
    }

    fclose(fp);
    printf("Dados lidos com Sucesso do arquivo 'dados.csv'!\n");
    return 1;
}

// Calcula os totais acumulados por dia e as médias semanais por bairro
void calculo_total_media (float intensidade_chuva[DIAS][BAIRROS], float media[BAIRROS],float total[DIAS])
{
    // Inicialização dos vetores zerados
    for (int d = 0; d < DIAS; d++) total[d] = 0.0f;
    for (int b = 0; b < BAIRROS; b++) media[b] = 0.0f;

    // Laço aninhado da matriz [d][b] para somatório
    for (int d = 0; d < DIAS; d++)
    {
        for (int b = 0; b < BAIRROS; b++)
        {
            total[d] += intensidade_chuva[d][b];    // Saldo Acumulado das chuvas por dia
            media[b] +=  intensidade_chuva[d][b];   // Saldo Acumulado das chuvas por bairro
        }
    }
    // Cálculo da média semanal de chuvas por bairro
    for (int b = 0; b < BAIRROS; b++)
    {
        media[b] = media[b]/DIAS;
    }
    
}

// Classifica o volume de chuva (em milimetros) em níveis discretos de intensidade (0 - 3)
int intensidade (float mm)
{
    if (mm == 0) return 0;
    if(mm <= 10) return 1;
    if(mm <= 40) return 2;
    else return 3;

}

// Faz a leitura dos dados pluviométricos da matriz e correlaciona ao nível de intensidade correspondente
void converter_intensidade (float mm[DIAS][BAIRROS], int mapa[DIAS][BAIRROS])
{
    for (int d = 0; d < DIAS; d++)
    {
        for (int b = 0; b < BAIRROS; b++)
        {
            mapa[d][b] = intensidade(mm[d][b]);
        }
    }
}

//=============================================================================================================================
//    EXIBIÇÃO DOS RELATÓRIOS NO CONSOLE
//=============================================================================================================================

// Exibe a tabela quantitativa de medições em milimetros, totais diários e média semanal por bairro
void exibir_dados(char bairros[BAIRROS][TAM], float intensidade_chuva[DIAS][BAIRROS], float total[DIAS], float media[BAIRROS],char data[DIAS][10])
{
    titulo("DADOS PLUVIOMETRICOS - MARICA", TABELA); // Banner de identificação da tabela Dados Pluviométricos

    // Imprime a linha de cabeçalho da tabela
    printf("%-10s", " DIAS");
    for (int b = 0; b < BAIRROS; b++)
    {
        centraliza(bairros[b], 20);
        if (b == BAIRROS -1)
        {
            centraliza("Total Diario",12);
            printf("\n");
        }
    }
    linha("-", TABELA);

    // Imprime no terminal a estrutura de tabela com as informações qualitativas de intensidade de chuva em milimetros
    for (int d = 0; d < DIAS; d++)
    {
        printf("%-10s", data[d]); // Exibe a data lida no arquivo csv

        // Centraliza o valor dos dados para a coluna com valor definido
        for (int b = 0; b < BAIRROS; b++)
        {
            char buffer[TAM];  
            snprintf(buffer, sizeof(buffer),"%.1f mm", intensidade_chuva[d][b]);
            centraliza(buffer, 20);
        }
        printf("%.1f mm", total[d]); // Acrescenta a coluna de totais por dia
        printf("\n");
    }
    linha("-", TABELA);
    printf("Media%-5s","");

    // Linha de mostragem da média semanal de chuvas por bairro no Rodapé da tabela
    for (int b = 0; b < BAIRROS; b++)
    {
        char buffer [TAM]; // Variável 
        snprintf(buffer, sizeof(buffer), "%.1f mm", media[b]);
        centraliza(buffer, 20);
    }
    printf("\n");
    linha("=", TABELA);
}

// Exibe o HEATMAP textual mapeando os rótulos de intensidade para rótulos qualitativos
void exibir_mapa (char bairros[BAIRROS][TAM],int mapa[DIAS][BAIRROS],char data[DIAS][10])
{
    
    titulo("HEATMAP - Intensidade de Chuvas", TABELA); // Banner de identificação da tabela HeatMap
    
    // Imprime a linha de cabeçalho da tabela
    printf("%-10s", "DIA");
    for (int b = 0; b < BAIRROS; b++)
    {
        centraliza(bairros[b], 20);
        if (b == BAIRROS -1)
        {
            printf("\n");
        }
    }
    linha("-", TABELA);
    
    // Imprime no terminal a estrutura de tabela com as informações qualitativas de intensidade de chuva
    for (int d = 0; d < DIAS; d++)
    {
        printf("%-10s", data[d]);
        for (int b = 0; b < BAIRROS; b++)
        {
            const char *clima;
             if (mapa[d][b] == 0)
            {
                clima = "Sem Chuva";   
            }
            else if (mapa[d][b] == 1)
            {
                clima = "Fraca";
            }
            else if (mapa[d][b] == 2)
            {
                clima = "Moderada";
            }else if (mapa[d][b] == 3)
            {
                clima = "Intensa";
            }
            
            centraliza(clima, 20); // Centraliza o dado qualitativo na coluna com largura definida
           
            if (b == BAIRROS - 1)
            {
                printf("\n");
            }
        }
    }

}

//=============================================================================================================================
//  FUNÇÃO PRINCIPAL DO CÓDIGO FONTE
//=============================================================================================================================
int main (void)
{
    // Declaraçaõ das matrizes e vetores princpais
    float intensidade_chuva[DIAS][BAIRROS]; 
    char bairros[BAIRROS][TAM] = 
    {
        "Barra de Marica", 
        "Guaratiba", 
        "Cordeirinho", 
        "Ponta Negra", 
        "Jacone"
    };
    char datas[DIAS][10];

    float media_chuva_bairro[BAIRROS];
    float total_chuva_dias[DIAS];
    int mapa_intensidade[DIAS][BAIRROS];

    // 1- Leitura do arquivo base - Encerra com código de erro 1 caso a leitura do arquivo falhe
    if (!leitura_dados_csv(bairros, intensidade_chuva, datas))
    {
        return 1;
    }
    // 2 - Realiza os Cálculos dos totais diários e médias por bairro dos dados lidos
    calculo_total_media(intensidade_chuva, media_chuva_bairro, total_chuva_dias);
    // 3 - Exibe no terminal a tabela de Dados Pluviométricos
    exibir_dados(bairros, intensidade_chuva, total_chuva_dias, media_chuva_bairro, datas);
    // 4 - Convertea intensidade de chuva em string qualitativas
    converter_intensidade(intensidade_chuva, mapa_intensidade);
    // 5 - Exibe o HEATMAP das informações qualitativas das chuvas
    exibir_mapa(bairros, mapa_intensidade, datas);
    // 6 - Encerra a apresentação no terminal 
    titulo("VOLTE SEMPRE!", TABELA);

    return 0; // Execução finalizada corretamente! 
}