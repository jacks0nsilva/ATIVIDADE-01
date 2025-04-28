# 📟 Ohmímetro inteligente

Um projeto para o Raspberry Pi Pico que mede resistências desconhecidas usando um divisor de tensão e exibe o valor no display OLED, juntamente com as cores correspondentes ao código de resistores.

## 🛠️ Tecnologias Utilizadas

- Microcontrolador: Raspberry Pi Pico (RP2040)
- Linguagem: C (com SDK do Pico)
- IDE: Visual Studio Code (com extensão Pico-SDK)

## ⚡ Funcionalidades

- Medição de resistência via divisor de tensão (ADC)
- Exibição das 3 faixas de cores no OLED
- Visualização em matriz de LEDs RGB

## 🖥️ Hardware Necessário

- Raspberry Pi Pico
- Display OLED 128x64 (I2C)
- Resistor conhecido (10kΩ)
- Resistor a ser medido
- Matriz de LEDs RGB
- Fios de conexão
- Protoboard

## Como executar o projeto 🛠️

1.  **Configuração do Ambiente:**

    - Certifique-se de ter o SDK do Raspberry Pi Pico instalado e configurado corretamente.
    - Configure as ferramentas de compilação (CMake, etc.).

2.  **Clone o repositório**
    ```
    git clone https://github.com/jacks0nsilva/ATIVIDADE-02.git
    ```
3.  **Instale a extensão do Raspberry Pi Pico no seu VsCode**
4.  **Usando a extensão do Raspberry Pi Pico, siga os passos:**
5.  **Clean CMake: Para garantir que o projeto será compilado do zero**

6.  **Compile Project: Compilação dos binários**

7.  **Run Project [USB]: Compila e copia o firmware para a placa automaticamente**
