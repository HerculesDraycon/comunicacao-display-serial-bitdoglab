#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "pio_matrix.pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

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
#define NUMERO_DE_LEDS 25     // Numero de LED's na matriz
#define DEBOUNCE_LINE 300000  // 300ms em microsegundos

volatile int btn_a_acionado = 0;    // Variavel de controle do 'botao a' pressioando
volatile int btn_b_acionado = 0;    // Variavel de controle do 'botao b' pressioando

int main(){

    PIO pio = pio0;                      // Declaracao incial pio
    uint32_t valor_led = 0;              // Declaracao incial valor LED
    double r = 0.0, b = 0.0, g = 0.0;    // Declaração incial valores RGB

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PINO);

    bool cor = true;

    i2c_init(I2C_PORT, 400 * 1000);             // Inicializacao do I2C com 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Seta a funcao do GPIO para I2C
    gpio_pull_up(I2C_SDA);                      // Ativa a transmissao de dados
    gpio_pull_up(I2C_SCL);                      // Ativa o clock
    ssd1306_t ssd;                              // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);  // Inicializa o display
    ssd1306_config(&ssd);                       // Configura o display
    ssd1306_send_data(&ssd);                    // Envia os dados para o display

    // Limpa o display para que inicie com todos os pixels apagados
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    stdio_init_all();

    // Iniciacao dos pinos do LED RGB
    gpio_init(RED_PINO);
    gpio_init(GREEN_PINO);
    gpio_init(BLUE_PINO);
    // Configuracao dos pinos do LED RGB
    gpio_set_dir(RED_PINO, GPIO_OUT);
    gpio_set_dir(GREEN_PINO, GPIO_OUT);
    gpio_set_dir(BLUE_PINO, GPIO_OUT);
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
    //gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    //gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    while (true) {
        
        cor = !cor;
        // Atualiza o conteudo do display
        ssd1306_fill(&ssd, !cor);  // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, "Fev de 2025", 8, 10); // Desenha uma string
        ssd1306_draw_string(&ssd, "TESTANDO", 20, 30); // Desenha uma string
        ssd1306_draw_string(&ssd, "H S Oliveira", 15, 48); // Desenha uma string      
        ssd1306_send_data(&ssd); // Atualiza o display

        sleep_ms(1000);

    }
}
