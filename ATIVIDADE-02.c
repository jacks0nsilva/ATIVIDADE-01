#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "libs/ssd1306.h"
#include "libs/font.h"
#include "libs/pio_config.h"
#include "libs/definicoes.h"
#include "libs/definir_cores.h"


int R_conhecido = 10000; // Resistor de 10k ohms
float R_x = 0.0; // Resistor desconhecido
float ADC_VREF = 3.31; // Tensão de referência do ADC
int ADC_MAX = 4095; // Valor máximo do ADC (12 bits)
ssd1306_t ssd; // Estrutura do display OLED
char cor1[10], cor2[10], cor3[10]; // Strings para as cores
bool matriz[25] = {0};

faixa_cor_t cores[] = {
    {"PRETO", 0, 0, 0, 0},       
    {"MARROM", 1, 150, 75, 25},    
    {"VERMELHO", 2, 255, 10, 0},    
    {"LARANJA", 3, 255, 150, 20},   
    {"AMARELO", 4, 255, 240, 10},   
    {"VERDE", 5, 0, 150, 20},      
    {"AZUL", 6, 10, 0, 255},      
    {"VIOLETA", 7, 140, 0, 160},   
    {"CINZA", 8, 100, 100, 100},   
    {"BRANCO", 9, 255, 250, 240}    
};

void cores_matriz(); // Declaração da função cores_matriz

int main()
{
    stdio_init_all();

    // Inicializa o I2C com 400kHz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    adc_init();
    adc_gpio_init(ADC_PIN); // GPIO28 como entrada do ADC

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);

    npInit(MATRIZ_LEDS); // Inicializa a matriz de LEDs
    npClear(); // Limpa a matriz de LEDs
    npWrite(); // Envia os dados para a matriz de LEDs
 
    char str_media[6]; // String para armazenar a média
    char str_resitor[15]; // String para armazenar o valor do resistor desconhecido
    

    while (true) {
        adc_select_input(2); // Seleciona o canal 2 do ADC (GPIO28)
        ssd1306_fill(&ssd, false);
        float soma = 0.0f;
        for (int i = 0; i < 500; i++) {
            soma += adc_read();
            sleep_ms(1); // Aguarda 1ms entre as leituras
        }
        float media = soma / 500.0f; // Calcula a média das leituras

        // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_MAX - adc_encontrado)
        R_x = R_conhecido * media / (ADC_MAX - media);

        determinar_cores(R_x, cor1, cor2, cor3, cores); // Determina as cores
        
        sprintf(str_media, "%1.0f", media); // Formata a média como string
        sprintf(str_resitor, "%1.0f %s", R_x, "$"); // Formata o valor do resistor como string


        ssd1306_rect(&ssd, 0,0, 128, 64,true, false);
        ssd1306_draw_string(&ssd, "COR 1:", 7, 2);
        ssd1306_draw_string(&ssd, cor1, 63, 2);
        ssd1306_draw_string(&ssd, "COR 2:", 7, 11);
        ssd1306_draw_string(&ssd, cor2, 63, 11);
        ssd1306_draw_string(&ssd, "COR 3:", 7, 20);
        ssd1306_draw_string(&ssd, cor3, 63, 20);
        ssd1306_hline(&ssd,1, 127, 29, true);

        ssd1306_draw_string(&ssd, "ADC", 7, 35);
        ssd1306_draw_string(&ssd, str_media, 4, 53);
        ssd1306_vline(&ssd, 45, 29, 63, true);
        printf("MEDIA: %s\n", str_media);


        ssd1306_draw_string(&ssd, "RESISTEN", 54, 35);
        ssd1306_draw_string(&ssd, str_resitor, 62, 53);
        printf("RESISTOR: %s\n", str_resitor);
        cores_matriz(); // Atualiza a matriz de LEDs com as cores determinadas
        ssd1306_send_data(&ssd);
        sleep_ms(1000);
    }
}


// Função para determinar as cores com base no valor da resistência
void cores_matriz() {
    npClear(); // Limpa tudo antes

    // Define LEDs e cores
    for (int j = 0; j < 10; j++) {
        if (strcmp(cores[j].nome, cor1) == 0) {
            npSetLED(13, cores[j].red, cores[j].green, cores[j].blue);
            break;
        }
    }

    for (int j = 0; j < 10; j++) {
        if (strcmp(cores[j].nome, cor2) == 0) {
            npSetLED(12, cores[j].red, cores[j].green, cores[j].blue);
            break;
        }
    }

    for (int j = 0; j < 10; j++) {
        if (strcmp(cores[j].nome, cor3) == 0) {
            npSetLED(11, cores[j].red, cores[j].green, cores[j].blue);
            break;
        }
    }

    npWrite(); // Atualiza a matriz de LEDs
}