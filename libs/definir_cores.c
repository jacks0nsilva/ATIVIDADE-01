#include "definicoes.h"
#include <string.h>
#include <math.h>

void determinar_cores(float resistencia, char cor1[10], char cor2[10], char cor3[10], faixa_cor_t cores[10]) {
    // Função para determinar as cores das três primeiras faixas de um resistor de 4 faixas
    // com base no valor da resistência fornecido. Os nomes das cores são armazenados nas strings cor1, cor2 e cor3.


    if (resistencia < 1) {
        // Se a resistência for menor que 1 ohm, não é possível determinar as cores
        // seguindo o padrão de 4 faixas. Retornamos "N/A" para indicar isso.
        strcpy(cor1, "N/A");
        strcpy(cor2, "N/A");
        strcpy(cor3, "N/A");
        return;
    }

    int valor = (int)round(resistencia); // Arredonda o valor da resistência para o inteiro mais próximo.
                                         
    int digito1 = 0;     // Variável para armazenar o valor do primeiro dígito significativo.
    int digito2 = 0;     // Variável para armazenar o valor do segundo dígito significativo.
    int multiplicador = 0; // Variável para armazenar o valor do multiplicador (potência de 10).
    int temp_valor = valor; // Uma cópia temporária da variável 'valor' para contar o número de dígitos
                            // sem modificar o valor original que será usado para extrair os dígitos.
    int num_digitos = 0;   // Variável para armazenar o número total de dígitos do valor da resistência.

    // Loop para contar o número de dígitos do valor da resistência.
    while (temp_valor > 0) {
        temp_valor /= 10; 
        num_digitos++;    
    }

    // A lógica a seguir determina os dois primeiros dígitos significativos e o multiplicador
    // com base no número de dígitos do valor da resistência.

    if (num_digitos >= 3) {
        // Se o número de dígitos for 3 ou mais, os dois primeiros dígitos são os significativos,
        // e o multiplicador é determinado pela ordem de grandeza (número de dígitos - 2).
        multiplicador = num_digitos - 2;
        digito1 = valor / (int)pow(10, multiplicador + 1); // Extrai o primeiro dígito significativo.
        digito2 = (valor % (int)pow(10, multiplicador + 1)) / (int)pow(10, multiplicador); // Extrai o segundo dígito significativo.
    } else if (num_digitos == 2) {
        // Se o número de dígitos for 2, o multiplicador é 0 (10^0 = 1).
        digito1 = valor / 10; // Extrai o primeiro dígito.
        digito2 = valor % 10;  // Extrai o segundo dígito.
        multiplicador = 0;
    } else if (num_digitos == 1) {
        // Se o número de dígitos for 1, o multiplicador também é 0. Consideramos o segundo dígito como 0.
        digito1 = valor;
        digito2 = 0; // Consideramos o segundo dígito como 0 para resistores de um único dígito.
        multiplicador = 0;
    }

    // Encontra os nomes das cores correspondentes aos valores dos dígitos e do multiplicador
    strcpy(cor1, cores[digito1].nome);
    strcpy(cor2, cores[digito2].nome);

    if (multiplicador < 10) {
        strcpy(cor3, cores[multiplicador].nome); // Obtém o nome da cor correspondente ao multiplicador.
    } else {
        strcpy(cor3, "NA"); // Se o multiplicador for maior ou igual a 10, indica que não há
                             // uma cor padrão definida para esse multiplicador em nosso array.
    }
}