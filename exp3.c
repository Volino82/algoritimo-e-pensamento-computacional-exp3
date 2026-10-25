
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif


// Definição das constantes do cenário de Maricá - RJ 
#define DIAS 7 // Período de monitoramento (7 dias) 
#define BAIRROS 4 // Cordeirinho, Ponta Negra, Guaratiba e Barra de Maricá 
#define TAM_NOME 30 // Tamanho máximo do nome do bairro 

// Função para classificar o nível de intensidade pluviométrica (0 a 3) 
int calcular_nivel_intensidade(float mm) 
{ 
    if (mm == 0.0f) return 0; // Sem chuva 
    if (mm < 10.0f) return 1; // Chuva Fraca 
    if (mm < 25.0f) return 2; // Chuva Moderada 
    return 3; // Chuva Forte / Alerta Crítico 
} 
    
// Retorna o símbolo visual para o Heatmap de acordo com o nível 
char obter_simbolo_heatmap(int nivel) 
{ 
    switch (nivel) 
    { 
        case 0: return '0'; // Sem chuva 
        case 1: return '🌥️ '; // Chuva fraca 
        case 2: return '🌦️ '; // Chuva moderada 
        case 3: return '🌧️ '; // Chuva forte 
        default: return '?'; 
    } 
} 

int main() 
{ 
    #ifdef _WIN32
        // Configura o console do Windows para aceitar a codificação UTF-8 (Code Page 65001)
        SetConsoleOutputCP(CP_UTF8);
    #endif
    // 1. Mapeamento dos Bairros de Maricá (Matriz de Char) 
    char nomes_bairros[BAIRROS][TAM_NOME] = { "Cordeirinho", "Ponta Negra", "Guaratiba", "Barra de Marica" }; 
    // 2. Matriz Principal: volume de chuva acumulado (em mm) [DIAS][BAIRROS] 
    // Dados simulados para uma semana típica de monitoramento 
    float matriz_chuvas[DIAS][BAIRROS] = 
    { 
        { 0.0f, 0.0f, 2.5f, 0.0f}, // Dia 1 (Garoa isolada) 
        {12.5f, 8.0f, 15.0f, 9.5f}, // Dia 2 (Chuva moderada) 
        {35.0f, 42.0f, 28.5f, 31.0f}, // Dia 3 (Tempestade/Frente fria) 
        { 5.0f, 3.0f, 4.0f, 6.0f}, // Dia 4 (Chuva fraca) 
        { 0.0f, 0.0f, 0.0f, 0.0f}, // Dia 5 (Tempo seco) 
        {18.0f, 22.5f, 14.0f, 19.0f}, // Dia 6 (Pancadas à tarde) 
        { 2.0f, 1.5f, 0.0f, 3.0f} // Dia 7 (Residual) 
    }; 
        
    // 3\. Vetores e Matrizes Auxiliares 
    float total_por_dia[DIAS] = {0}; 
    float media_por_bairro[BAIRROS] = {0}; 
    int mapa_intensidade[DIAS][BAIRROS];
    
    // --- PROCESSAMENTO DOS DADOS --- 
    for (int d = 0; d < DIAS; d++) 
    { 
        for (int b = 0; b < BAIRROS; b++) 
        { 
            float mm = matriz_chuvas[d][b]; // Acumula o total pluviométrico do dia na cidade 
            total_por_dia[d] += mm; // Acumula o total do bairro (para calcular a média posteriormente) 
            media_por_bairro[b] += mm; // Mapeia a intensidade para a matriz do Heatmap 
            mapa_intensidade[d][b] = calcular_nivel_intensidade(mm); 
        } 
    } 
    // Calcula a média diária de chuva para cada bairro 
    for (int b = 0; b < BAIRROS; b++) 
    { 
        media_por_bairro[b] /= DIAS; 
    } 
    
    // --- IMPRESSÃO DOS RELATÓRIOS --- 
    printf("========================================================================\n"); 
    printf(" MONITORAMENTO PLUVIOMETRICO - MUNICIPIO DE MARICA / RJ (7 DIAS) \n"); 
    printf("========================================================================\n\n"); 
    
    // Tabela 1: Dados Quantitativos (mm) 
    printf("1. MATRIZ DE REGISTRO PLUVIOMETRICO (mm):\n"); 
    printf("%-8s", "Dia"); 
    
    for (int b = 0; b < BAIRROS; b++) 
    { 
        printf("%-18s", nomes_bairros[b]); 
    } 
    
    printf("%-12s\n", "Total Dia"); 
    printf("------------------------------------------------------------------------\n"); 
    
    for (int d = 0; d < DIAS; d++) 
    { 
        printf("Dia %d ", d + 1); 
        for (int b = 0; b < BAIRROS; b++) 
        { 
            printf("%-18.1f", matriz_chuvas[d][b]); 
        } 
        printf("%-12.1f\n", total_por_dia[d]); 
    } 
    
    printf("------------------------------------------------------------------------\n");
    
    // Exibição das Médias 
    printf("%-8s", "Media"); 
    for (int b = 0; b < BAIRROS; b++) 
    { 
        printf("%-18.1f", media_por_bairro[b]); 
    } 
    printf("\n\n"); 
    
    // Tabela 2: Heatmap Visual de Intensidade 
    printf("2. HEATMAP DE INTENSIDADE PLUVIOMETRICA:\n"); 
    printf("Legenda: [ 🌤️ ] Sem Chuva [ 🌥️ ] Fraca (<10mm) [ 🌦️ ] Moderada (<25mm) [ 🌧️ ] Forte (>=25mm)\n\n"); 
    printf("%-8s", "Dia"); 
    
    for (int b = 0; b < BAIRROS; b++) 
    { 
        printf("%-18s", nomes_bairros[b]); 
    } 
    printf("\n------------------------------------------------------------------------\n"); 
    
    for (int d = 0; d < DIAS; d++) 
    { 
        printf("Dia %d ", d + 1); 
        for (int b = 0; b < BAIRROS; b++) 
        { 
            char simbolo = obter_simbolo_heatmap(mapa_intensidade[d][b]); 
            printf(" [ %c ] ", simbolo); 
        } 
        printf("\n"); 
    } 
    printf("------------------------------------------------------------------------\n"); 
    return 0; 
}