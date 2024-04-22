#include "uart.h"
#include "gpio.h"

#define UART ((NRF_UART_REG *)0x40002000)

typedef struct
{
    volatile uint32_t TASKS_STARTRX;
    volatile uint32_t TASKS_STOPRX;
    volatile uint32_t TASKS_STARTTX;
    volatile uint32_t TASKS_STOPTX;
    volatile uint32_t RESERVED0[3];
    volatile uint32_t TASKS_SUSPEND;
    volatile uint32_t RESERVED1[56];
    volatile uint32_t EVENTS_CTS;
    volatile uint32_t EVENTS_NCTS;
    volatile uint32_t EVENTS_RXDRDY;
    volatile uint32_t RESERVED2[4];
    volatile uint32_t EVENTS_TXDRDY;
    volatile uint32_t RESERVED3;
    volatile uint32_t EVENTS_ERROR;
    volatile uint32_t RESERVED4[7];
    volatile uint32_t EVENTS_RXTO;
    volatile uint32_t RESERVED5[46];
    volatile uint32_t SHORTS;
    volatile uint32_t RESERVED6[64];
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED7[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED8[31];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED9;
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED10;
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED11[17];
    volatile uint32_t CONFIG;
} NRF_UART_REG;

// UART_INT_RX input
// UART_INT_TX output
//Målet med denne funksjonen er å initialisere de nødvendige GPIO-pinnene som input/output.

void uart_init()
{
    GPIO->PIN_CNF[6] = (0 << 0); //configure pin as input pin 
    GPIO->PIN_CNF[8] = (1 << 0); //Configure pin as an output pin
    //UART modulen sin RX port er GPIO modulen sin TX port 
    UART->PSELTXD = 6; //setter pin 6 til output (Pin select for TXD signal)
    UART->PSELRXD = 8; //sette pin 8 til input (Pin select for RXD signal)

    UART->BAUDRATE = 0x00275000; //sette baudraten til 9600 

    //I tillegg er det viktig å faktisk fortelle UART-modulen at vi ikke har CTS- eller RTS-koblinger
    UART->PSELRTS = ~0; //konfigurerer som gpio pinner istedenfor UART. IKKe funksjonalitet 
    //1 er disconnect og dette betyr alt settes til 1 
    UART->PSELCTS = ~0;

  
    // ID OG VERDI (sende inn til første bit)
    UART->ENABLE =4;   //(4<<0) == =4 dette betyr det samme. leftshifte 
    UART->TASKS_STARTRX = 1; //start UART receiver


}

void uart_send(char letter)  {
    //hvilke signaler som kommer inn. 1 = one letter 
    UART->TASKS_STARTTX =1; //UART transmission sequence started by triggering 
    UART->TXD = letter; //bytes transmitted, skrive til TXD registeret  
    while (!(UART->EVENTS_TXDRDY)); //checking if successfully transmitted 
    UART->EVENTS_TXDRDY = 0; //cleare interruptet som genereres når dere er ferdige å sende
    UART->TASKS_STOPTX = 1; //Stop transmission sequence 

}

char uart_read() {

    char letter; 
    UART->EVENTS_RXDRDY = 0; 
    UART->TASKS_STARTTX =1;
    if(UART->EVENTS_RXDRDY)
    {
        letter = UART->RXD; //hvis UART har en bokstav, sett letter til denne 
    }    
    else
    {
        letter = '\0'; //hvis ikke, send ut dette 
    }
    return letter; 
}



