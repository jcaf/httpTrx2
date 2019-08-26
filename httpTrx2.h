/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   httpTrx2.h
 * Author: jcaf
 *
 * Created on August 21, 2019, 12:09 PM
 */

#ifndef HTTPTRX2_H
#define HTTPTRX2_H

#include "system.h"
#include "types.h"

#define HTTPTRX_DEBUG

typedef struct _TRXWR
{
    /*Server*/
    char *domain;
    uint8_t *IP;
    uint16_t port;
    /*connection resource*/
    char *URI;
    char *Host;
    char *ApiKey;
    char *HdrLine;
    
//    struct _TRXWRrqst//request message
//    {
//    }rsqtMsg;
    
    struct _TRXWRresp//response message
    {
        int8_t sm0;
        int8_t sm1;
        struct _TRXWRresp_tmr
        {
            unsigned long responseMsg_timeout;
            unsigned long afterServerDisconneted_flushbuffer;
        }timer;
        
        uint32_t idx;
    }respMsg;
    
    #if defined(__AVR__) && defined(__GNUC__)  
        Client *client;
    #elif
    #endif
    
    #ifdef HTTPTRX_DEBUG
    struct _HTTPTRX_Debug
    {
       struct _HTTPTRX_Debug_bf
       {
           unsigned enabled:1;
           unsigned __a:7;
       }bf;
       //PTRFX_retVOID_arg1_PCHAR UART_print;
    }dbg;
    #endif

}TRXWR;

typedef struct _HTTPTRX
{
    TRXWR trxw;
    TRXWR trxr;
    //
    #ifdef HTTPTRX_DEBUG
        #if defined(__AVR__) && defined(__GNUC__)  
            PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print;
        #elif
            PTRFX_retVOID_arg1_PCHAR UART_print;
        #endif
    #endif        
}HTTPTRX;

extern  HTTPTRX httpTrx;

typedef struct _JSON
{
    char *name;
    char *strval;
}JSON;

int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic);
void NIC_getMyIP(char *str, uint16_t sizebuff);

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrx_setClient(TRXWR *trxwr, Client* client);
    
    #define httpTrxWrite_setClient(client) do{httpTrx_setClient(&httpTrx.trxw, client);}while(0)
    #define httpTrxRead_setClient(client) do{httpTrx_setClient(&httpTrx.trxr, client);}while(0)
#endif

#ifdef HTTPTRX_DEBUG
    void httpTrx_UARTdebug_enabled(TRXWR *trxwr, BOOLEAN_T _bool);
    void httpTrx_UARTdebug_print(TRXWR *trxwr, char *str, int8_t mode);
    
    #define httpTrxWrite_UARTdebug_enabled(_bool)   do{httpTrx_UARTdebug_enabled(&httpTrx.trxw, _bool);}while(0)
    //#define httpTrxWrite_UARTdebug_print(str)       do{httpTrx_UARTdebug_print(&httpTrx.trxw, str);}while(0)

    #define httpTrxRead_UARTdebug_enabled(_bool)   do{httpTrx_UARTdebug_enabled(&httpTrx.trxr, _bool);}while(0)
    //#define httpTrxRead_UARTdebug_print(str)       do{httpTrx_UARTdebug_print(&httpTrx.trxr, str);}while(0)

    #if defined(__AVR__) && defined(__GNUC__)
    void httpTrx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print);
    #elif
    void httpTrx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print);
    #endif
#endif

void httpTrx_setHost(TRXWR *trxwr, char *host);
void httpTrx_setURI(TRXWR *trxwr, char *URI);
void httpTrx_setApiKey(TRXWR *trxwr, char *ApiKey);
void httpTrx_setHdrLine(TRXWR *trxwr, char *HdrLine);

#define httpTrxWrite_setHost(host)          do{httpTrx_setHost(&httpTrx.trxw , host);}while(0)
#define httpTrxWrite_setURI(URI)            do{httpTrx_setURI(&httpTrx.trxw , URI);}while(0)
#define httpTrxWrite_setApiKey(ApiKey)      do{httpTrx_setApiKey(&httpTrx.trxw , ApiKey);}while(0)
#define httpTrxWrite_setHdrLine(HdrLine)    do{httpTrx_setHdrLine(&httpTrx.trxw , HdrLine);}while(0)

#define httpTrxRead_setHost(host)          do{httpTrx_setHost(&httpTrx.trxr , host);}while(0)
#define httpTrxRead_setURI(URI)            do{httpTrx_setURI(&httpTrx.trxr , URI);}while(0)
#define httpTrxRead_setApiKey(ApiKey)      do{httpTrx_setApiKey(&httpTrx.trxr , ApiKey);}while(0)
#define httpTrxRead_setHdrLine(HdrLine)    do{httpTrx_setHdrLine(&httpTrx.trxr , HdrLine);}while(0)


void httpTrx_setupServerByDomain(TRXWR *trxwr, char *domain, uint16_t port);
void httpTrx_setupServerByIP(TRXWR *trxwr, uint8_t *IP, uint16_t port);
#define httpTrxWrite_setupServerByDomain(domain,port) do{httpTrx_setupServerByDomain(&httpTrx.trxw, domain, port);}while(0)
#define httpTrxWrite_setupServerByIP(IP,port) do{httpTrx_setupServerByIP(&httpTrx.trxw, IP, port);}while(0)

#define httpTrxRead_setupServerByDomain(domain,port) do{httpTrx_setupServerByDomain(&httpTrx.trxr, domain, port);}while(0)
#define httpTrxRead_setupServerByIP(IP,port) do{httpTrx_setupServerByIP(&httpTrx.trxr, IP, port);}while(0)

/******************************************************************************************/
typedef enum _http_trx_set_exec_mode_e
{
	EM_WAIT_NEW_EXEC_MODE_E = -1,
	EM_STOP_E = 0,
	EM_RUN_ONCE_E,
	EM_RUN_INTERVAL_E,
}
HTTP_TRX_SET_EXEC_MODE_E;

typedef struct HTTP_TRX_SET_EXEC_MODE_T
{
    HTTP_TRX_SET_EXEC_MODE_E k;
}HTTP_TRX_SET_EXEC_MODE;

#define EM_WAIT_NEW_EXEC_MODE   ((HTTP_TRX_SET_EXEC_MODE){EM_WAIT_NEW_EXEC_MODE_E})
#define EM_STOP                 ((HTTP_TRX_SET_EXEC_MODE){EM_STOP_E})
#define EM_RUN_ONCE             ((HTTP_TRX_SET_EXEC_MODE){EM_RUN_ONCE_E})
#define EM_RUN_INTERVAL         ((HTTP_TRX_SET_EXEC_MODE){EM_RUN_INTERVAL_E})

enum _http_trx_set_status
{
	IDLE,
	RUNNING
};

#define HTTP_TRX_RX_BUFFER_MAX_SIZE 400


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* HTTPTRX2_H */


