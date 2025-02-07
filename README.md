# Tarefa - Comunicação UART e SSD1306 - Raspberry Pi Pico W

**LINK DO VÍDEO ENSAIO:** 

# Documentação do Código

## **Descrição Geral**

O código controla o LED RGB, a matriz de LEDs 5x5, o monitor SSD1306, a comunicação UART e os botões A e B. Sendo capaz de:
- Captar o caracter informado pela comunicação UART e exibi-lo no display SSD1306.
- Captar o digito informado pela comunicação UART e exibi-lo na matriz de LEDs 5x5.
- Captar o acionamento dos botões através das interrupções no sistema e tratar o seu bouncing via software.
- Alternar entre os LEDs acesos, conforme o comando acionado nos botões.
- Comunicar-se com o usuário mutuamente antravés do Monitor Serial e do Display SSD1306.

## **Bibliotecas Utilizadas**

### **Padrão do C**
- `<stdio.h>`: Manipulação de entrada/saída.

### **Pico SDK**
- `pico/stdlib.h`: Configuração padrão para Raspberry Pi Pico. Pôde apresentar toda a estrutura necessária para que esse código funcionasse corretamente, sem a necessidade de mais bibliotecas de *hardware*.
- `hardware/clocks.h`: Biblioteca que realiza controle de clock do sistema.
- `hardware/pio.h`: Manipulação de entradas e saídas programáveis.
- `hardware/i2c.h`: Configuração e comunicação do dispositivo I2C com o sistema.
- `hardware/uart.h`: Biblioteca para comunicação UART entre dispositivos.

### **Customizadas**
- `"pio_matrix.pio.h"`: Configuração do PIO para controle da matriz de LEDs.
- `lib/frames.h`: Frames que representam os números exibidos na matriz de LEDs.

## **Constantes**
- `UART_ID`: Seleciona a UART0.
- `I2C_PORT`: Porta de inicialização do display.
- `ENDERECO`: Enderoço de inicialização do display.
- `I2C_SDA`: Pino SDA do display.
- `I2C_SCL`: Pino SCL do display.
- `OUT_PINO`: Pino da matriz de LEDs.
- `UART_TX_PINO`: Pino GPIO usado para TX.
- `UART_RX_PINO`: Pino GPIO usado para RX.
- `BAUD_RATE`: Define a taxa de transmissão serial.
- `NUMERO_DE_LEDS`: Define o numero de LEDs na matriz.
- `GREEN_PINO`: Define o pino de saída utilizado pelo LED verde do RGB.
- `BLUE_PINO`: Define o pino de saída utilizado pelo LED azul do RGB.
- `RED_PINO`: Define o pino de saída utilizado pelo LED vermelho do RGB.
- `BTN_A`: Define o pino de entrada utilizado pelo Botão A.
- `BTN_B`: Define o pino de entrada utilizado pelo Botão B.
- `DEBOUNCE_LINE`: Define em microsegundos o tempo a ser utilizado no debounce do botão.

## **Funções**