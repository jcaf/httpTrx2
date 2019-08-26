#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "httpTrx2.h"

//NIC setup
uint8_t IP[4]= {192,168,1,60};//fallback
uint8_t MAC[6]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Connect to server - setting
//char server[] = "api.quanticoservices.net";
uint8_t IPaddr_server[]={192, 168, 1, 54};

//HTTP transaction: receive response message from Server (if parsing)
//#define HTTP_TRX_RX_BUFFER
#ifdef HTTP_TRX_RX_BUFFER
char http_trx_rx_buffer[HTTP_TRX_RX_BUFFER_MAX_SIZE];//as circular buffer
#endif

//void json_cIntegerArr
void json_cFloatArr(float *v, int size, char *outbuff)
{
    char buff[10];
    
    int i=0;
    strcpy(outbuff,"[");
    
    for (i=0; i< size; i++)
    {
        //sprintf(buff, "%.2f", v[i]);
        snprintf(buff, sizeof(buff), "%.2f", v[i]);
        
        strcat(outbuff, buff);//alternatevly use strlcat
        
        if (i != size-1)
            strcat(outbuff, ",");
    }
    strcat(outbuff,"]");
}

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
    while (!Serial) {;}// wait for serial port to connect. Needed for native USB port only
    #elif
    #endif
}

#if defined(__AVR__) && defined(__GNUC__)
    
    void UART_print(char *str, int8_t mode)
    {
        if (mode == 0)
            Serial.print(str);
        else
            Serial.print( reinterpret_cast <const __FlashStringHelper *> (str));
    }
#elif
    void UART_print(char *str)
    {

    }
#endif

EthernetClient client;

#define JSON_SIZEMAX 1//4
JSON json[JSON_SIZEMAX];
void setup(void)
{
  char buff[30];
    spi_deselect_devices();
    
    UART_setup();
    
    //1) local network setting
    NIC_begin(MAC, IP);//by default DHCP
    NIC_getMyIP(buff, sizeof(buff));
    UART_print(buff,0);
    delay(1000);
    
    //2) Client config connection to server
    
    httpTrxWrite_setClient((Client*)&client);//Only for Arduino
    httpTrx_UARTdebug_setPrintFx(UART_print);//library point to this funcion()
    httpTrxWrite_UARTdebug_enabled(TRUE);
    
    httpTrxWrite_setupServerByIP(IPaddr_server, 80);
    //client.setTimeout(1000);
    
    //3) Set HTTP transaction
    httpTrxWrite_setURI("/jsondecode.php");
    httpTrxWrite_setHost("192.168.1.54");
    httpTrxWrite_setApiKey("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");

    
    //4) Http transaction setting
    //http_trx_set_exec_interval_ms(0);//ms
    //http_trx_set_exec_mode(EM_RUN_INTERVAL);//RUN_ONCE
        //http_trx_set_rx_buffer(http_trx_rx_buffer);//NULL pointer by defect
    //
    json[0].name = "m1";
    
    // float value[4] = {1,2,3,4};
    // char strval[30];
    // json_cFloatArr(value, sizeof(strval)/sizeof(strval[0]), strval);
    //json[0].strval = strval;
    json[0].strval = "[7,865,4,4]";
}

char outmsg[HTTP_TRX_RX_BUFFER_MAX_SIZE];

int8_t http_trx(TRXWR *trxwr, JSON *json, uint8_t npairs, char *outmsg);

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

    if (http_trx(&httpTrx.trxw, json, 1, outmsg) )
    {
        while (1);
    }
}
