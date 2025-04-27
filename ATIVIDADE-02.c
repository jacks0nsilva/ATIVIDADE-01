#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "libs/ssd1306.h"
#include "libs/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADRESS 0x3c
#define ADC_PIN 28

int R_conhecido = 10000; // Resistor de 10k ohms
float R_x = 0.0; // Resistor desconhecido
float ADC_VREF = 3.31; // Tensão de referência do ADC
int ADC_MAX = 4095; // Valor máximo do ADC (12 bits)
ssd1306_t ssd; // Estrutura do display OLED

// Definição das cores e seus valores
typedef struct {
    char *nome;
    int valor;
} faixa_cor_t;

faixa_cor_t cores[] = {
    {"PRETO", 0},
    {"MARROM", 1},
    {"VERMELHO", 2},
    {"LARANJA", 3},
    {"AMARELO", 4},
    {"VERDE", 5},
    {"AZUL", 6},
    {"VIOLETA", 7},
    {"CINZA", 8},
    {"BRANCO", 9}
};

void determinar_cores(float resistencia, char cor1[10], char cor2[10], char cor3[10]);

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define BUTTON_B 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}



int main()
{
    stdio_init_all();

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa o I2C com 400kHz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(ADC_PIN); // GPIO28 como entrada do ADC

    char str_media[6]; // String para armazenar a média
    char str_resitor[15]; // String para armazenar o valor do resistor desconhecido
    char cor1[10], cor2[10], cor3[10]; // Strings para as cores

    while (true) {
        adc_select_input(2); // Seleciona o canal 2 do ADC (GPIO28)

        float soma = 0.0f;
        for (int i = 0; i < 500; i++) {
            soma += adc_read();
            sleep_ms(1); // Aguarda 1ms entre as leituras
        }
        float media = soma / 500.0f; // Calcula a média das leituras

        // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_MAX - adc_encontrado)
        R_x = R_conhecido * media / (ADC_MAX - media);

        determinar_cores(R_x, cor1, cor2, cor3); // Determina as cores

        sprintf(str_media, "%1.0f", media); // Formata a média como string
        sprintf(str_resitor, "%1.0f %s", R_x, "$"); // Formata o valor do resistor como string

        ssd1306_rect(&ssd, 0,0, 128, 64,true, false);
        ssd1306_draw_string(&ssd, "COR 1:", 7, 2);
        ssd1306_draw_string(&ssd, cor1, 7 + 56, 2);
        ssd1306_draw_string(&ssd, "COR 2:", 7, 11);
        ssd1306_draw_string(&ssd, cor2, 7 + 56, 11);
        ssd1306_draw_string(&ssd, "COR 3:", 7, 20);
        ssd1306_draw_string(&ssd, cor3, 7 + 56, 20);
        ssd1306_hline(&ssd,1, 127, 29, true);

        ssd1306_draw_string(&ssd, "ADC", 7, 35);
        ssd1306_draw_string(&ssd, str_media, 4, 53);
        ssd1306_vline(&ssd, 45, 29, 63, true);

        ssd1306_draw_string(&ssd, "RESISTEN", 54, 35);
        ssd1306_draw_string(&ssd, str_resitor, 62, 53);

        printf("Media: %s\n", str_media);
        printf("R_x: %1.0f (%s %s %s)\n", R_x, cor1, cor2, cor3);
        ssd1306_send_data(&ssd);
        sleep_ms(1000);
    }
}

// Função para determinar as cores
void determinar_cores(float resistencia, char cor1[10], char cor2[10], char cor3[10]) {
    if (resistencia < 1) {
        strcpy(cor1, "N/A");
        strcpy(cor2, "N/A");
        strcpy(cor3, "N/A");
        return;
    }

    int valor = (int)round(resistencia); // Arredonda para o inteiro mais próximo
    int digito1 = 0;
    int digito2 = 0;
    int multiplicador = 0;
    int temp_valor = valor;
    int num_digitos = 0;

    while (temp_valor > 0) {
        temp_valor /= 10;
        num_digitos++;
    }

    if (num_digitos >= 3) {
        multiplicador = num_digitos - 2;
        digito1 = valor / (int)pow(10, multiplicador + 1);
        digito2 = (valor % (int)pow(10, multiplicador + 1)) / (int)pow(10, multiplicador);
    } else if (num_digitos == 2) {
        multiplicador = 0;
        digito1 = valor / 10;
        digito2 = valor % 10;
    } else if (num_digitos == 1) {
        multiplicador = 0;
        digito1 = valor;
        digito2 = 0; // Consideramos o segundo dígito como 0 para resistores de 1 dígito
    }

    // Encontra os nomes das cores correspondentes
    strcpy(cor1, cores[digito1 % 10].nome);
    strcpy(cor2, cores[digito2 % 10].nome);

    if (multiplicador < 10) {
        strcpy(cor3, cores[multiplicador].nome);
    } else {
        strcpy(cor3, "N/A"); // Multiplicadores maiores que 10 não estão definidos
    }
}