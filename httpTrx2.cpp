#if defined(__AVR__) && defined(__GNUC__)
    #include <Arduino.h>
    #include <Ethernet.h>

    #define __millis() millis()

    Client *pclient;
    
    void httpTrx_setClient(Client* _pclient)
    {
        pclient = _pclient;
    }
#elif
    
#endif

#include "httpTrx2.h"
#include <string.h>
#include <stdint.h>
/******************************************************************************************/
static HTTPTRX httpTrx;    
/******************************************************************************************/
//struct _NIC
//{
//    uint8_t *MAC;//[6];
//    uint8_t *IP;//[4];
//    struct _NIC_bf
//    {
//       unsigned DHCP:1;
//       unsigned __a:7;
//    }bf;
//};
//struct _NIC NIC;
//
//void NIC_setMAC(uint8_t *MAC)
//{
//    NIC.MAC = MAC;
//}
////void NIC_setIP(uint8_t *IP)
////{
////    NIC.IP = IP;
////}
//void NIC_setDHCP(ENABLED_T DHCPenabled , uint8_t *IP)
//{
//    NIC.bf.DHCP = DHCPenabled;
//    NIC.IP = IP;//Static falling back IP4 if DHCP is disable
//}
/*
 * 1 = configured
 * 0 = not configured
 */
//int8_t NIC_begin(uint8_t *MAC, ENABLED_T DHCPenabled , uint8_t *IPstatic)
int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic)
{
    int8_t cod_ret=0;
    
    //NIC.MAC = MAC;
    //NIC_setDHCP(DHCPenabled, IPstatic);
    
    #if defined(__AVR__) && defined(__GNUC__)
    IPAddress IP(IPstatic);
    if (1)//(DHCPenabled)
    {
        //Only begin(mac) return a value, others overloaded fxs return void
        if (Ethernet.begin(MAC) == 0)
        {
            Ethernet.begin(MAC, IP);//"Static IP was assigned\n"
            //NIC.bf.DHCP = 0;
        }
        else//"IP configured by DHCP\n"
        {}
    }
    else
    {
        Ethernet.begin(MAC, IP);//others overloaded fxs return void
        //NIC.bf.DHCP = 0;
    }
    cod_ret = 1;
    
    #endif // defined
    return cod_ret;
}
void NIC_getMyIP(char *str)
{
}

#ifdef HTTPTRX_DEBUG
void httpTrx_UARTdebug_enabled(BOOLEAN_T enabled, PTRFX_retVOID_arg1_PCHAR UART_print)
{
    httpTrx.dbg.bf.enabled = enabled.k;
    httpTrx.dbg.UART_print = UART_print;
}
void httpTrx_UARTdebug_print(char *str)
{
    if (httpTrx.dbg.bf.enabled)
        httpTrx.dbg.UART_print(str);
}
#endif

/******************************************************************************************/
#if defined(__AVR__) && defined(__GNUC__)
    #define FS_STRING
    #ifdef FS_STRING
      #define FS(s) PSTR(s)
    #else
      #define FS(s) s
    #endif
#else
  #define FS(s) s
  //#define Serial.print(s) do{printf("%s", s);}while(0)
#endif
/******************************************************************************************/

#define nt_debug_print(d) do{Serial.print(d);}while(0)

/******************************************************************************************/
 int8_t tcp_client_available(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
    return pclient->available();
    #elif
    #endif
}
int8_t tcp_client_connected(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
    return pclient->connected();
    #elif
    #endif
}
void tcp_client_stop(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
    pclient->stop();
    #elif
    #endif
}
char http_client_read_char(void) 
{
    #if defined(__AVR__) && defined(__GNUC__)
    return pclient->read();
    #elif
    #endif
}
/******************************************************************************************/
/*Print standard (RAM)*/
void http_print(const char *s)
{
    #if defined(__AVR__) && defined(__GNUC__)
          pclient->print(s);
    #elif
    
    #endif
}
/******************************************************************************************/
void http_printk(const char *s)
{
    #ifdef FS_STRING
        pclient->print(reinterpret_cast <const __FlashStringHelper *> (s) );
    #else
      http_print(s);
    #endif
}
/******************************************************************************************/
uint32_t getContentLength(JSON *json, uint8_t npairs)
{
    uint32_t acc=0;
    int i;
    for (i=0; i<npairs; i++)
    {
        acc += strlen(json[i].name)+strlen(json[i].strval) + 3;//+2"" + 1:
    }
    return acc+2;//+2 curl-braces
}
/******************************************************************************************/
void uint32toa(uint32_t ui32, char *buff)
{
    snprintf(buff, sizeof(buff), "%lu", ui32);
}
/******************************************************************************************/
void http_send_msgbody(JSON *json, uint8_t npairs)
{
    int i;
    for (i=0; i<npairs; i++)
    {
        printf("\"%s\":%s\n", json[i].name, json[i].strval);
        
        http_print(FS("\""));
        http_print(json[i].name);
        http_print(FS("\":"));
        http_print( json[i].strval);
        
        if (i< (npairs-1))
            {http_print(FS(","));}
    }
}
/******************************************************************************************/
void httpTrx_setHost(char *host)
{
    httpTrx.Host = host;
}
void httpTrx_setURI(char *URI)
{
    httpTrx.URI = URI;
    
}
void httpTrx_setHdrLine(char *HdrLine)
{
    httpTrx.HdrLine = HdrLine;
}
void httpTrx_setApiKeyWrite(char *ApiKeyWrite)    
{
    httpTrx.ApiKeyWrite = ApiKeyWrite;
}
void httpTrx_setApiKeyRead(char *ApiKeyRead)    
{
    httpTrx.ApiKeyRead = ApiKeyRead;
}
/******************************************************************************************/
int8_t http_trx_request_msg(JSON *json, uint8_t npairs)//send the request message to HTTP server
{
    int8_t cod_ret = 0;
    char buff[20];
    /*1) Request Line*/
    http_printk(FS("POST "));http_print(httpTrx.URI);http_printk(FS(" HTTP/1.1\n\r"));
    
    /*2) Header lines*/
    http_printk(FS("Host: "));http_print(httpTrx.Host);http_printk(FS("\n\r"));
    http_printk(FS("Connection: close\r\n"));   //http_printk(FS("Connection: keep-alive\r\n"));//HTTP persistent connection
    http_printk(FS("Content-Type: application/json\n\r"));
    
    if (httpTrx.HdrLine != NULL)
        {http_print(httpTrx.HdrLine);}
    
    http_printk(FS("User-Agent: Agent/1.00\n\r"));
    http_printk(FS("Content-Length: "));
    uint32toa(getContentLength(json, npairs), buff);
    http_print(buff);
    
    /*3) Send New Line*/
    http_printk(FS("\n\r"));
    
    /*4) Send New Line*/
    http_send_msgbody(json, npairs);
    
    /*5) Send New Line*/        
    http_printk(FS("\n\r"));
    
    cod_ret = 1;
    return cod_ret;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/******************************************************************************************/
/*******************************************************************************************
HTTP transaction manager:

Exec.mode:

WAIT_NEW_EXEC_MODE = -1,
    STOP = 0,
    RUN_ONCE,
    RUN_INTERVAL,

    Status:

    IDLE,
    RUNNING
********************************************************************************************/
struct _http_trx_t
{
    HTTP_TRX_SET_EXEC_MODE_E exec_mode;
    int8_t status;
    unsigned long exec_interval_ms;//exec with interval
    char *rx_buffer;
};
struct _http_trx_t http_trx_t =
{
    EM_WAIT_NEW_EXEC_MODE_E,//-1
    IDLE,//0
    0,
    NULL
};
void http_trx_set_status(int8_t status)
{
    http_trx_t.status = status;
}
int8_t http_trx_get_status(void)
{
    return http_trx_t.status;
}
void http_trx_set_exec_interval_ms(unsigned long interval_ms)
{
    http_trx_t.exec_interval_ms = interval_ms;
}
unsigned long http_trx_get_exec_interval_ms(void)
{
    return http_trx_t.exec_interval_ms;
}
void http_trx_set_rx_buffer(char *rx_buffer)
{
    http_trx_t.rx_buffer = rx_buffer;
}
char *http_trx_get_rx_buffer(void)
{
    return http_trx_t.rx_buffer;
}
void http_trx_set_exec_mode(HTTP_TRX_SET_EXEC_MODE exec_mode)
{
    http_trx_t.exec_mode = exec_mode.k;
}
HTTP_TRX_SET_EXEC_MODE_E http_trx_get_exec_mode(void)
{
    return http_trx_t.exec_mode;
}

/******************************************************************************************
        HTTP transaction: response message

I’ve had a TCPClient disconnect but still have the connected property == true. So it seams the only way to tell is if a Read from the network stream returns 0 bytes. So right now I’m assuming it is disconnected if it returns 0 bytes. I was wondering if there is a better way?
Here is documented REMARKS section for tcpClient.Connected property:
"The Connected property gets the connection state of the Client socket as of the last I/O
operation. When it returns false, the Client socket was either never connected, or is no
longer connected."
"Because the Connected property only reflects the state of the connection as of the most
recent operation, you should attempt to send or receive a message to determine the current
state. After the message send fails, this property no longer returns true. Note that this
behavior is by design. You cannot reliably test the state of the connection because, in the
time between the test and a send/receive, the connection could have been lost. Your code
should assume the socket is connected, and gracefully handle failed transmissions."
So, I thing yours 'Read' way is the best one and you may forget Connected property
alltogether!



http_trx_readbuffer_ktime: Is the k-time of CPU (and resources) assigned to read data from the buffer if available.

client_stop_in_ktime: If the Server disconnect the client, client has a k-time to read all possible available data from rx buffer. After this time,the connection must be end sending client.stop()

http_trx_response_msg_ktimeout: Is the global k-timeout assigned for all recepcion in an HTTP Transfer response-mode.

    return:
    0: Busy
    1: End response msg
*******************************************************************************************/
const unsigned long http_trx_readbuffer_ktime = 10;//ms
unsigned long client_stop_in_ktime= 700;//ms
unsigned long http_trx_response_msg_ktimeout = 1000;//ms

int8_t http_trx_response_msg(void)
{
    static int8_t sm0;
    static int8_t sm1;
    static unsigned long tmr_response_msg_timeout;
    static unsigned long tmr_client_stop;
    unsigned long tmr_readbuffer;
    int8_t cod_ret = 0;
    char c;
    static uint16_t idx;
    char *prx_buffer;
    if (sm0 == 0)
    {
        tmr_response_msg_timeout = __millis();
        idx = 0;
        sm1 = 0;
        sm0++;
    }
    if (sm0 == 1)
    {
        tmr_readbuffer = __millis();
        prx_buffer = http_trx_get_rx_buffer();
        do
        {
            if (tcp_client_available())
            {
                c = http_client_read_char();//client.read();
                nt_debug_print(c);          //Serial.print(c);

                if (prx_buffer!= NULL)
                {
                    prx_buffer[idx] = c;
                    if (++idx >= HTTP_TRX_RX_BUFFER_MAX_SIZE)
                    {
                        idx = 0;//as circular buffer
                    }
                }

                /*if (c == 'LAST_CHAR_BREAKING')
                	break;				*/
            }
            else
            {
                break;
            }

        }
        while ((__millis() - tmr_readbuffer) <= http_trx_readbuffer_ktime);  //CPU assigned for window

        if (sm1 == 0)
        {
            if (!tcp_client_connected())
            {
                if (client_stop_in_ktime > http_trx_response_msg_ktimeout)
                {
                    client_stop_in_ktime = http_trx_response_msg_ktimeout;
                }
                tmr_client_stop = __millis();
                sm1++;
            }
        }
        else//sm1 == 1
        {
            if ( (__millis() - tmr_client_stop) >= client_stop_in_ktime ) //let a time to read all rx buffer
            {
                tcp_client_stop();
                sm0 = 0;
                cod_ret = 1;
            }
        }

        //connection time-out
        if ( (__millis() - tmr_response_msg_timeout) >= http_trx_response_msg_ktimeout) //abort and stop conection
        {
            tcp_client_stop();
            sm0 = 0;
            cod_ret = 1;
        }
    }
    return cod_ret;
}

/*
                one HTTP transaction

Like most network protocols, HTTP uses the client-server model: An HTTP
client opens a connection and sends a request message to an HTTP server;
the server then returns a response message, usually containing the resource
that was requested. After delivering the response, the server closes the
connection (making HTTP a stateless protocol, i.e. not maintaining any
connection information between transactions).

    return:
    0: Busy in HTTP transaction
    1: End one HTTP transaction, even if client could not connect to the server.
*/

//#define SOCKET_DEBUG
#ifdef SOCKET_DEBUG
#include "utility/w5100.h"
#include "utility/socket.h"
void ShowSocketStatus(void);
#endif

//void httpTrx_setServerDomain(char *domain)
//{
//    httpTrx.Sdomain = domain;
//}
//void httpTrx_setServerIP(uint8_t *IP)
//{
//    httpTrx.Sipaddr = IP;
//}
//void httpTrx_setPort(uint16_t port)
//{
//    httpTrx.Sport = port;
//}
void httpTrx_setupServerByDomain(char *domain, uint16_t port)
{
    httpTrx.Sdomain = domain;
    httpTrx.Sport = port;
}
void httpTrx_setupServerByIP(uint8_t *IP, uint16_t port)
{
    httpTrx.Sipaddr = IP;
    httpTrx.Sport = port;
}

int8_t tcp_client_connection(void)
{
    int8_t cod_ret;
    
    #if defined(__AVR__) && defined(__GNUC__)
    if (httpTrx.Sdomain != NULL)
        cod_ret = pclient->connect(httpTrx.Sdomain, httpTrx.Sport);
    else
        cod_ret = pclient->connect(httpTrx.Sipaddr, httpTrx.Sport);
    #elif
    #endif
    return cod_ret;
}

//int8_t http_trx(void)
int8_t http_trx(JSON *json, uint8_t npairs)
{
    static int8_t sm0;
    int8_t code_ret = 0;

    if (sm0 == 0)// client opens a connection
    {
        if (tcp_client_connection())
        {
            sm0++;
        }
        else
        {
            #ifdef SOCKET_DEBUG
            Serial.println(F("Socket status in error before client.stop():"));
            ShowSocketStatus();
            Serial.println(F("Socket status in error after client.stop()"));
            #endif
            tcp_client_stop();
   
            code_ret = 1;
        }
    }
    if (sm0 == 1)//client->server: send request message
    {
        if (http_trx_request_msg(json, npairs))
        {
            sm0++;
        }
    }
    if (sm0 == 2)//server->client: receive response message
    {
        if (http_trx_response_msg())
        {
            sm0 = 0x00;
            code_ret = 1;
        }
    }
    #ifdef SOCKET_DEBUG
    Serial.println(F("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
    ShowSocketStatus();
    Serial.println(F("----------------------------------------------------------------------------"));
    #endif
    return code_ret;
}

/*
    return:
    0: Busy in HTTP job (synchronize RUN_ONCE, RUN_INTERVAL, STOP)
    1: End one HTTP job (end transaction): Is the time for parsing the http_trx_rx_buffer[]
*/
int8_t http_trx_job(JSON *json, uint8_t npairs)
{
    static HTTP_TRX_SET_EXEC_MODE_E last_exec_mode;
    static int8_t _run_interval=0;
    static unsigned long tmr_run_interval;
    int8_t cod_ret = 0;

    if (http_trx_get_status() == IDLE)
    {
        last_exec_mode = http_trx_get_exec_mode();

        if ( last_exec_mode > EM_WAIT_NEW_EXEC_MODE_E)
        {
            if (last_exec_mode == EM_RUN_INTERVAL_E)
            {
                if (_run_interval == 0)
                {
                    http_trx_set_status(RUNNING);//first time
                    _run_interval++;	//exit and execute
                }
                else if (_run_interval == 1)//for next evaluation
                {
                    tmr_run_interval = __millis();
                    _run_interval++;
                }
                else if (_run_interval == 2)
                {
                    if ( (__millis()-tmr_run_interval) >= http_trx_get_exec_interval_ms())
                    {
                        _run_interval = 0x00;
                    }
                }
            }
            else if (last_exec_mode == EM_RUN_ONCE_E)
            {
                http_trx_set_status(RUNNING);
            }
            else//STOP
            {
                http_trx_set_exec_mode(EM_WAIT_NEW_EXEC_MODE);
                //
                _run_interval = 0x00;	//reset
            }
        }

    }
    else
    {
        if (http_trx(json, npairs) == 1)//end?
        {
            http_trx_set_status(IDLE);

            if (last_exec_mode == EM_RUN_ONCE_E)
            {
                http_trx_set_exec_mode(EM_STOP);
            }
            cod_ret = 1;
        }
    }
    return cod_ret;
}

//////////////////////////////////////////////////////////////////////////////////
#ifdef SOCKET_DEBUG
void ShowSocketStatus(void)
{

    //    ActivityWriteSPICSC("ETHERNET SOCKET LIST");
    //    ActivityWriteSPICSC("#:Status Port Destination DPort");
    //    ActivityWriteSPICSC("0=avail,14=waiting,17=connected,22=UDP");
    //    ActivityWriteSPICSC("1C=close wait");
    String l_line = "";
    l_line.reserve(64);
    char l_buffer[10] = "";

    for (uint8_t i = 0; i < MAX_SOCK_NUM; i++)
    {
        l_line = "#" + String(i);

        uint8_t s = W5100.readSnSR(i); //status
        l_line += ":0x";
        sprintf(l_buffer,"%x",s);
        l_line += l_buffer;
        //
        // if(s == 0x1C)
        //{close(i);}
        //
        l_line += " ";
        l_line += String(W5100.readSnPORT(i)); //port

        l_line += " D:";
        uint8_t dip[4];
        W5100.readSnDIPR(i, dip); //IP Address
        for (int j=0; j<4; j++)
        {
            l_line += int(dip[j]);
            if (j<3) l_line += ".";
        }
        l_line += " (";
        l_line += String(W5100.readSnDPORT(i)); //port on destination
        l_line += ") ";

        //        if (G_SocketConnectionTimes[i] != 0)
        //            l_line += TimeToHHMM(G_SocketConnectionTimes[i]);

        Serial.println(l_line);

        //ActivityWriteSPICSC(l_line);
    }
}
#endif