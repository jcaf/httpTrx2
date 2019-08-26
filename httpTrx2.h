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

#ifndef HTTPTRSX2_H
#define HTTPTRSX2_H

#include "system.h"
#include "types.h"

#define HTTPTRSX_DEBUG

typedef struct _TRSXWR
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
    
//    struct _TRSXWRrqst//request message
//    {
//    }rsqtMsg;
    
    struct _TRSXWRresp//response message
    {
        int8_t sm0;
        int8_t sm1;
        struct _TRSXWRresp_tmr
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
    
    #ifdef HTTPTRSX_DEBUG
    struct _HTTPTRSX_Debug
    {
       struct _HTTPTRSX_Debug_bf
       {
           unsigned enabled:1;
           unsigned __a:7;
       }bf;
       //PTRFX_retVOID_arg1_PCHAR UART_print;
    }dbg;
    #endif

}TRSXWR;

typedef struct _HTTPTRSX
{
    TRSXWR trxw;
    TRSXWR trxr;
    //
    #ifdef HTTPTRSX_DEBUG
        #if defined(__AVR__) && defined(__GNUC__)  
            PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print;
        #elif
            PTRFX_retVOID_arg1_PCHAR UART_print;
        #endif
    #endif        
}HTTPTRSX;

extern  HTTPTRSX httpTrx;

typedef struct _JSON
{
    char *name;
    char *strval;
}JSON;

int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic);
void NIC_getMyIP(char *str, uint16_t sizebuff);

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrx_setClient(TRSXWR *trxwr, Client* client);
    
    #define httpTrxWrite_setClient(client) do{httpTrx_setClient(&httpTrx.trxw, client);}while(0)
    #define httpTrxRead_setClient(client) do{httpTrx_setClient(&httpTrx.trxr, client);}while(0)
#endif

#ifdef HTTPTRSX_DEBUG
    void httpTrx_UARTdebug_enabled(TRSXWR *trxwr, BOOLEAN_T _bool);
    void httpTrx_UARTdebug_print(TRSXWR *trxwr, char *str, int8_t mode);
    
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

void httpTrx_setHost(TRSXWR *trxwr, char *host);
void httpTrx_setURI(TRSXWR *trxwr, char *URI);
void httpTrx_setApiKey(TRSXWR *trxwr, char *ApiKey);
void httpTrx_setHdrLine(TRSXWR *trxwr, char *HdrLine);

#define httpTrxWrite_setHost(host)          do{httpTrx_setHost(&httpTrx.trxw , host);}while(0)
#define httpTrxWrite_setURI(URI)            do{httpTrx_setURI(&httpTrx.trxw , URI);}while(0)
#define httpTrxWrite_setApiKey(ApiKey)      do{httpTrx_setApiKey(&httpTrx.trxw , ApiKey);}while(0)
#define httpTrxWrite_setHdrLine(HdrLine)    do{httpTrx_setHdrLine(&httpTrx.trxw , HdrLine);}while(0)

#define httpTrxRead_setHost(host)          do{httpTrx_setHost(&httpTrx.trxr , host);}while(0)
#define httpTrxRead_setURI(URI)            do{httpTrx_setURI(&httpTrx.trxr , URI);}while(0)
#define httpTrxRead_setApiKey(ApiKey)      do{httpTrx_setApiKey(&httpTrx.trxr , ApiKey);}while(0)
#define httpTrxRead_setHdrLine(HdrLine)    do{httpTrx_setHdrLine(&httpTrx.trxr , HdrLine);}while(0)


void httpTrx_setupServerByDomain(TRSXWR *trxwr, char *domain, uint16_t port);
void httpTrx_setupServerByIP(TRSXWR *trxwr, uint8_t *IP, uint16_t port);
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
HTTP_TRSX_SET_EXEC_MODE_E;

typedef struct HTTP_TRSX_SET_EXEC_MODE_T
{
    HTTP_TRSX_SET_EXEC_MODE_E k;
}HTTP_TRSX_SET_EXEC_MODE;

#define EM_WAIT_NEW_EXEC_MODE   ((HTTP_TRSX_SET_EXEC_MODE){EM_WAIT_NEW_EXEC_MODE_E})
#define EM_STOP                 ((HTTP_TRSX_SET_EXEC_MODE){EM_STOP_E})
#define EM_RUN_ONCE             ((HTTP_TRSX_SET_EXEC_MODE){EM_RUN_ONCE_E})
#define EM_RUN_INTERVAL         ((HTTP_TRSX_SET_EXEC_MODE){EM_RUN_INTERVAL_E})

enum _http_trx_set_status
{
	IDLE,
	RUNNING
};

#define HTTP_TRSX_RX_BUFFER_MAX_SIZE 400


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* HTTPTRSX2_H */


