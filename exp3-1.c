#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIAS 7
#define BAIRROS 5
#define TAM 30

void entrada_dados(char bairros[BAIRROS][TAM], float intensidade_chuva[DIAS][BAIRROS])
{
    for (int b = 0; b < BAIRROS; b++)
    {
        printf("Dados pluviometricos - %s \n", bairros[b]);
       
        for (int d = 0; d < DIAS; d++)
        {
            float valor_lido;
            int retorno_scanf;

            do
            {
                printf("Dia %d: ", d + 1);
                retorno_scanf = scanf("%f", &valor_lido);

                if (retorno_scanf != 1 || valor_lido < 0.0f)
                {
                    printf("[ERROR] Valor Inválido!!");
                    while (getchar() != '\n');
                }
                else
                {
                    intensidade_chuva[d][b] = valor_lido;
                    break;
                }
            }
            while (1);
        }
    }
}

void exibir_dados(char bairros[BAIRROS][TAM], float intensidade_chuva[DIAS][BAIRROS])
{
    printf("========================\n");
    printf("DADOS PLUVIOMÉTRICOS\n");
    printf("========================\n");

    printf("%-18s", "DIA");
    for (int b = 0; b < BAIRROS; b++)
    {
        printf("%-18s", bairros[b]);
    }

    for (int d = 0; d < DIAS; d++)
    {
        printf("DIA %d    ", d + 1);
        for (int b = 0; b < BAIRROS; b++)
        {
            printf("%-18.1f", intensidade_chuva[d][b]);
        }
        printf("\n");
    }
    printf("-------------------------------\n");
}

int main (void)
{
    float intensidade_chuva[DIAS][BAIRROS];
    char bairros[BAIRROS][TAM] = {"Barra de Marica", "Guaratiba", "Cordeirinho", "Ponta Negra", "Jacone"};

    entrada_dados(bairros, intensidade_chuva);
    exibir_dados(bairros, intensidade_chuva);

    return 0;
}