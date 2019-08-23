#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "httpTrx2.h"

//NIC setup
uint8_t IP[4]= {192,168,1,47};
uint8_t MAC[6]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Connect to server - setting
char server[] = "api.quanticoservices.net";
uint8_t IPaddr_server[]={192, 168, 1, 52};

//HTTP transaction: receive response message from Server (if parsing)
//#define HTTP_TRX_RX_BUFFER
#ifdef HTTP_TRX_RX_BUFFER
char http_trx_rx_buffer[HTTP_TRX_RX_BUFFER_MAX_SIZE];//as circular buffer
#endif

void spi_deselect_devices(void)//only for QUANTICO BOARD
{
    #ifdef Arduino_h
    #define WIZNET_CS 10
    #define SDCARD_CS 4
    #define LTC6804_CS 5
    pinMode(WIZNET_CS, OUTPUT);
    pinMode(SDCARD_CS, OUTPUT);
    pinMode(LTC6804_CS, OUTPUT);
    digitalWrite(WIZNET_CS, HIGH);
    digitalWrite(SDCARD_CS, HIGH);
    digitalWrite(LTC6804_CS, HIGH);
    #endif
}
void UART_setup(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
    Serial.begin(9600);
    #elif
    #endif
}
void UART_print(char *str)
{
    #if defined(__AVR__) && defined(__GNUC__)
    #elif
    #endif
}

EthernetClient client;

void setup(void)
{
    spi_deselect_devices();
    
    UART_setup();
    httpTrx_UARTdebug_enabled(TRUE, UART_print);//library point to this funcion()
    
    //1) local network setting
    NIC_begin(MAC, IP);
    delay(1000);
    Serial.print(Ethernet.localIP());
    
    httpTrx_setClient( (Client *) &client);//Only for Arduino
    
    //2) Client config connection to server
    httpTrx_setupServerByIP(IPaddr_server, 80);
    //client.setTimeout(1000);
    
    //3) Set HTTP transaction
    httpTrx_setURI("/v0/devices/data/add");
    httpTrx_setHost("api.quanticoservices.net");
    httpTrx_setHdrLine("Api_Key_Write: 2a6559e6b2750d60\
                            \r\nDevice_id: 1539814055_5c50372e3f5582ed8646\
                            \r\n");//must be end with \r\n

    
    //4) Http transaction setting
    //http_trx_set_exec_interval_ms(0);//ms
    //http_trx_set_exec_mode(EM_RUN_INTERVAL);//RUN_ONCE
        //http_trx_set_rx_buffer(http_trx_rx_buffer);//NULL pointer by defect
}

void loop(void)
{
/*
    if (http_trx_job())//complete a client->server transaction, then...
    {
      //its safe to read http_trx_rx_buffer[]
        #ifdef HTTP_TRX_RX_BUFFER
          nt_debug_print("Printing http_trx_rx_buffer[]:\n\n");
          if (http_trx_get_rx_buffer()!= NULL)
          {
            for (int i=0; i<HTTP_TRX_RX_BUFFER_MAX_SIZE; i++)
              {nt_debug_print(http_trx_rx_buffer[i]);}  
          }
        #endif
    }
    */
}

