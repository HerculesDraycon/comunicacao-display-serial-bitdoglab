#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "pio_matrix.pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#define UART_ID uart0         // Seleciona a UART0
#define I2C_PORT i2c1         // Porta de inicializacao do display
#define endereco 0x3C         // Endereco de inicializacao do display
#define I2C_SDA 14            // Pino LDA do display
#define I2C_SCL 15            // Pino SCL do display
#define BTN_A 5               // Pino do botao A
#define BTN_B 6               // Pino do botao B
#define OUT_PINO 7            // Pino da matriz de LED's
#define GREEN_PINO 11         // Pino do LED verde do RGB
#define BLUE_PINO 12          // Pino do LED azul do RGB
#define RED_PINO 13           // Pino do LED vermelho do RGB
#define UART_TX_PIN 0         // Pino GPIO usado para TX
#define UART_RX_PIN 1         // Pino GPIO usado para RX
#define BAUD_RATE 115200      // Define a taxa de transmissao serial
#define NUMERO_DE_LEDS 25     // Numero de LED's na matriz
#define DEBOUNCE_LINE 300000  // 300ms em microsegundos

bool cor = true;
ssd1306_t ssd;                      // Inicializa a estrutura do display
volatile int btn_a_acionado = 0;    // Variavel de controle do 'botao a' pressioando
volatile int btn_b_acionado = 0;    // Variavel de controle do 'botao b' pressioando

// Funcao com rotina de interrupcao (IRQ) que detecta e controla os botoes pressionados
void button_callback(uint gpio, uint32_t events){
    // Obtencao do tempo real em microsegundos numa variavel uint
    uint32_t marco = time_us_32();  
    //Utilizacao de debounce controlado por sowtware para fazer com que o botao pressionado execute a instrucao somente uma vez
    if(gpio == BTN_A && (marco -  btn_a_acionado) > DEBOUNCE_LINE){

        btn_a_acionado = marco;

        if(gpio_get(GREEN_PINO)){
            gpio_put(GREEN_PINO, false);
            cor = !cor;
            ssd1306_fill(&ssd, !cor);  // Limpa o display
            ssd1306_draw_string(&ssd, "Verde Desativado", 20, 30);
            ssd1306_send_data(&ssd);  // Atualiza o display
            uart_puts(UART_ID, "\nVerde Desativado!");
        } else {
            gpio_put(GREEN_PINO, true);
            cor = !cor;
            ssd1306_fill(&ssd, !cor);  // Limpa o display
            ssd1306_draw_string(&ssd, "Verde Ativado", 20, 30);
            ssd1306_send_data(&ssd);  // Atualiza o display
            uart_puts(UART_ID, "\nVerde Ativado!");
        }

    } else if(gpio == BTN_B && (marco -  btn_b_acionado) > DEBOUNCE_LINE){

        btn_b_acionado = marco;

        if(gpio_get(BLUE_PINO)){
            gpio_put(BLUE_PINO, false);
            cor = !cor;
            ssd1306_fill(&ssd, !cor);  // Limpa o display
            ssd1306_draw_string(&ssd, "Azul Desativado", 20, 30);
            ssd1306_send_data(&ssd);  // Atualiza o display
            uart_puts(UART_ID, "\nAzul Desativado!");
        } else {
            gpio_put(BLUE_PINO, true);
            cor = !cor;
            ssd1306_fill(&ssd, !cor);  // Limpa o display
            ssd1306_draw_string(&ssd, "Azul Ativado", 20, 30);
            ssd1306_send_data(&ssd);  // Atualiza o display
            uart_puts(UART_ID, "\nAzul Ativado!");
        }

    }
    
}

int main(){

    PIO pio = pio0;                      // Declaracao incial pio
    uint32_t valor_led = 0;              // Declaracao incial valor LED
    double r = 0.0, b = 0.0, g = 0.0;    // Declaração incial valores RGB

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PINO);

    i2c_init(I2C_PORT, 400 * 1000);             // Inicializacao do I2C com 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_pull_up(I2C_SDA);                      // Ativa a transmissao de dados
    gpio_pull_up(I2C_SCL);                      // Ativa o clock
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);  // Inicializa o display
    ssd1306_config(&ssd);                       // Configura o display
    ssd1306_send_data(&ssd);                    // Envia os dados para o display

    // Limpa o display para que inicie com todos os pixels apagados
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    // Inicializa a biblioteca padrao
    stdio_init_all();
    // Inicializa a UART
    uart_init(UART_ID, BAUD_RATE);
    // Iniciacao dos pinos do LED RGB
    gpio_init(RED_PINO);
    gpio_init(GREEN_PINO);
    gpio_init(BLUE_PINO);
    // Configuracao dos pinos do LED RGB
    gpio_set_dir(RED_PINO, GPIO_OUT);
    gpio_set_dir(GREEN_PINO, GPIO_OUT);
    gpio_set_dir(BLUE_PINO, GPIO_OUT);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);  // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);  // Configura o pino 1 para RX

    // Chamada do desenho inicial no comeco da execucao
    //desenho_pio(frame_0, valor_led, pio, sm, r, g, b);
    // Iniciacao dos botoes 'a' e 'b'
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    // Configuracao dos botoes 'a' e 'b'
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    // Definicao de resistores pull-up via software dos botoes 'a' e 'b'
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);
    // Chamada de funcao que executa instrucoes com as rotinas de interrupcao habilitadas para cada botao
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    const char *init_message = "Digite algo e veja o eco:\r\n";
    uart_puts(UART_ID, init_message);

    while (true) {
        
        cor = !cor;
        // Atualiza o conteudo do display
        ssd1306_fill(&ssd, !cor);  // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);  // Desenha um retângulo
        ssd1306_draw_string(&ssd, "Fev de 2025", 8, 10);  // Desenha uma string
        ssd1306_draw_string(&ssd, "TESTANDO", 20, 30);  // Desenha uma string
        ssd1306_draw_string(&ssd, "H S Oliveira", 15, 48);  // Desenha uma string      
        ssd1306_send_data(&ssd);  // Atualiza o display

        if (uart_is_readable(UART_ID)) {
            // Lê um caractere da UART
            char c = uart_getc(UART_ID);
            if (c == 'r'){
                gpio_put(RED_PINO, !gpio_get(RED_PINO));
            }
            if (c == 'g'){
                gpio_put(GREEN_PINO, !gpio_get(GREEN_PINO));
            }
            if (c == 'b'){
                gpio_put(BLUE_PINO, !gpio_get(BLUE_PINO));
            }                        
            // Envia de volta o caractere lido (eco)
            uart_putc(UART_ID, c);

            // Envia uma mensagem adicional para cada caractere recebido
            uart_puts(UART_ID, " <- Eco do RP2\r\n");
        }

        sleep_ms(1000);

    }
}
