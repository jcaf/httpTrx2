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



typedef struct _HTTPTRX
{
    /*Server*/
    char *Sdomain;
    uint8_t *Sipaddr;
    uint16_t Sport;
    //
    char *URI;
    char *Host;
    char *HdrLine;
    //
    char *ApiKeyWrite;
    char *ApiKeyRead;
    
#ifdef HTTPTRX_DEBUG
    struct _HTTPTRX_Debug
    {
       struct _HTTPTRX_Debug_bf
       {
           unsigned enabled:1;
           unsigned __a:7;
       }bf;
       PTRFX_retVOID_arg1_PCHAR UART_print;
    }dbg;
#endif    
}HTTPTRX;

typedef struct _JSON
{
    char *name;
    char *strval;
}JSON;


#if defined(__AVR__) && defined(__GNUC__)
    void httpTrx_setClient(Client* _pclient);
#endif

int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic);

void httpTrx_setupServerByDomain(char *domain, uint16_t port);
void httpTrx_setupServerByIP(uint8_t *IP, uint16_t port);


#ifdef HTTPTRX_DEBUG
void httpTrx_UARTdebug_enabled(BOOLEAN_T enabled, PTRFX_retVOID_arg1_PCHAR UART_print);
void httpTrx_UARTdebug_print(char *str);
#endif
    
void httpTrx_setHost(char *host);
void httpTrx_setURI(char *URI);
void httpTrx_setHdrLine(char *HdrLine);
void httpTrx_setApiKeyWrite(char *ApiKeyWrite);

void httpTrx_setApiKeyRead(char *ApiKeyRead);       
    
    
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


