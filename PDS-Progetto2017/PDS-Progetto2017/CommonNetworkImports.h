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
#define multicastIP "239.255.255.250"
#define multicastPort 55555
#define multicastPortStr "55555"
#define multicastMsgBufSize 256
#define multicastOnlineSendSleepTime 1
#define multicastOfflineSendSleepTime 5