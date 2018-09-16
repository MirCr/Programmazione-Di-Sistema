#pragma once
#pragma warning(disable:4996)
// prima di Windows.h, altrimenti Winsock 1 va in conflitto.
#include <Winsock2.h> 
//Necessaria per la definizione di ip_mreq per il multicast.
#include <Ws2tcpip.h> 
#include <Windows.h>

//Libreria Winsock.
#pragma comment(lib,"ws2_32.lib") 

//Dati ricorrenti, definiti in maniera univoca qui.
#define multicastIP "224.0.0.1"
#define multicastPort 55555
#define multicastMsgBufSize 256
#define multicastSendSleepTime 1
