#ifndef DEFINICOES_H
#define DEFINICOES_H

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADRESS 0x3c
#define ADC_PIN 28
#define MATRIZ_LEDS 7

typedef struct {
    char *nome;
    int valor;
} faixa_cor_t;

#endif