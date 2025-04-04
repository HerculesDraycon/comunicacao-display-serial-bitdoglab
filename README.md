# Tarefa - Comunicação UART e SSD1306 - Raspberry Pi Pico W

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
- `NUMERO_DE_LEDS`: Define o numero de LEDs na matriz.
- `GREEN_PINO`: Define o pino de saída utilizado pelo LED verde do RGB.
- `BLUE_PINO`: Define o pino de saída utilizado pelo LED azul do RGB.
- `RED_PINO`: Define o pino de saída utilizado pelo LED vermelho do RGB.
- `BTN_A`: Define o pino de entrada utilizado pelo Botão A.
- `BTN_B`: Define o pino de entrada utilizado pelo Botão B.
- `DEBOUNCE_LINE`: Define em microsegundos o tempo a ser utilizado no debounce do botão.

## **Funções**

### **1. Manipulação de LEDs**
#### `uint32_t matrix_rgb(double r, double g, double b)`
- **Descrição**: Converte os valores de vermelho (r), verde (g), e azul (b) para um formato hexadecimal de 32 bits.
- **Parâmetros**:
  - `r`, `g`, `b`: Intensidades de vermelho, verde e azul (0.0 a 1.0).
- **Retorno**: Valor RGB no formato hexadecimal.
- **Uso**: Criação de cores para o LED.

---

#### `void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)`
- **Descrição**: Atualiza os LEDs com base em um padrão (`desenho`), aplicando uma cor uniforme.
- **Parâmetros**:
  - `desenho`: Array com o padrão dos LEDs.
  - `valor_led`: Valor RGB.
  - `pio`: Instância do PIO.
  - `sm`: State Machine do PIO.
  - `r`, `g`, `b`: Intensidades de vermelho, verde e azul.

### **2. Controle dos Botões**

#### `button_callback(uint gpio, uint32_t events)`
- **Descrição**: Realiza as rotinas de interrupção (IRQ) e executa a sequência de instruções ao respectivo botão pressionado.
- **Parâmetros**:
  - `gpio`: uint que corresponde ao pino GPIO em que o botão está conectado e gerou a interrupção.
  - `events`: Passa as informações sobre que tipo evento na GPIO ocorreu.

### **3. Função Principal (`main`)**
- **Descrição**:
  - Configura e inicializa periféricos.
  - Inicializa a matriz de LEDs desligados.
  - Entra em um loop infinito que:
    1. Formata a mensagem padrão exibida no SSD1306.
    2. Mantém a verificação de algo enviado pela comunicação serial.
    3. Exibe no display o caracter escolhido e se for um número, também exibe na matriz de LEDs.
- **Destaques**:
  - Inicializa o PIO com a configuração `pio_matrix`.

## **Fluxo do programa**

- O Display SSD1306 inicialmente apresenta uma mensagem padrão e a matriz de LEDs 5x5 está apagada.

- Ao acionar um dos botões pode-se notar que o estado de LED correspondente é alterado e uma mensagem se apresenta tanto no display quanto na comunicação serial.

- Ao digitar um caracter qualquer e envia-lo pela comunicação serial, ele é exibido no Display SSD1306, caso seja um dígito, ele será representado na matriz de LEDs 5x5.
