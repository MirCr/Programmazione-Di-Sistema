
/* oltre al nome utente potremmo iserire nel messaggio inviato anche un "flag" per identificare la modalità on line o of line. 
oppure semplicemente non invia il messaggio. In questo caso bisogna gestire la possibilità 
di attivare o disattivare l'invio del messaggio. Vedere le specifiche per questo.  */

#include<thread> 
#include<winsock2.h>
#include<iostream>



#pragma warning(disable:4996) // magari da modificare.

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "192.168.1.255"  /*ip address of udp server. funziona ma da rivedere. 
Se cambio sottorete, potrebbe cambiare. dovrei quindi ogni volta recuperare 
il mio IP e settare 255 come ultima cifra? O c'è un altro modo?*/
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data


void Sender(); /*invia il suo User Name ogni secondo in broadcast per sapere chi è online.
			   se necessario potrebbe inviare una stringa con le informazioni necessarie 
			   come un flag per dire se on line o no ecc....*/
void Receiver(); /* resta in ascolto delle informazioni, riceve ip e username.*/



int main()
{
	


	std::thread t1{ Sender };
	std::thread t2{ Receiver };

	t1.join();
	t2.join();

	return 0;
}



void Sender() {

	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN] = "Mirko"; /* messaggio da inviare. qui dobbiamo impostare le informazioni 
									che vogliamo inserire nel payload: username, on-line o no, foto(?).*/
	WSADATA wsa;					 

	//Initialise winsock
	std::cout << "\nInitialising Winsock..." << std::endl;

	//printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout<<"Failed. Error Code :"<< WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout<<"Initialised."<<std::endl;

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		std::cout << "socket() failed with error code : "<< WSAGetLastError();
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);


	//start communication
	while (1)
	{
		//send the message
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			std::cout<<"sendto() failed with error code: "<< WSAGetLastError();
			exit(EXIT_FAILURE);
		}
		Sleep(1000);
	}

	closesocket(s);
	WSACleanup();







}


void Receiver()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	


	slen = sizeof(si_other);



	//Initialise winsock
	std::cout<< std::endl <<"Initialising Winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "Failed. Error Code : "<< WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout << "Initialised." << std::endl;

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Could not create socket : "<< WSAGetLastError();
	}
	std::cout << "Socket created.\n";

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error code : "<< WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout << "Bind done" << std::endl;

	//keep listening for data 
	while (1)
	{
		std::cout << "Waiting for data...";
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			std::cout << "recvfrom() failed with error code : " << WSAGetLastError();
			exit(EXIT_FAILURE);
		}

		//print details of the client/peer and the data received
		std::cout << "Received packet from "<< inet_ntoa(si_other.sin_addr)<<" : "<< ntohs(si_other.sin_port)<< std::endl;
		std::cout << "Data: " << buf << std::endl;
		//onLine.insert(inet_ntoa(si_other.sin_addr));
		

	}

	closesocket(s);
	WSACleanup();
}