#include "ReceiverThread.h"
#include "SenderThread.h"

//Import per lettura/scrittura file.
#include <fstream>

using namespace std;

//Definizione classi main (forward declaration).
void getUserData(string &name);
string insertData();

int main()
{
	string name;

	//Variabile di mutua esclusione.
	shared_ptr<mutex> sharedMainRecvMutex = make_shared<mutex>();
	//Queue condivisa, contenente la lista degli utenti presenti nella rete locale.
	shared_ptr<set<UserData, UserDataComparator>> sharedUserSet = make_shared<set<UserData, UserDataComparator>>();
	//Variabile di condizionamento per il colloquio tra receiver e main.
	shared_ptr<condition_variable> sharedMainRecvCondVar = make_shared<condition_variable>();
	//Variabile che consente di evitare notifiche spurie nel main durante il colloquio con il receiver.
	shared_ptr<bool> sharedMainRecvFlag = make_shared<bool>();
	//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
	shared_ptr<bool> sharedOnlineFlag = make_shared<bool>();
	
	//Setto a true il flag in modo da essere online di default.
	*sharedOnlineFlag = true;

	//Apro il file dei dati e leggo il nome dell'utente. 
	//Se non è presente alcun file lo creo e chiedo all'utente i suoi dati.
	//TODO: leggere anche posizione foto.
	getUserData(name);

	//Creo un thread di ricezione dei messaggi multicast.
	ReceiverThread rt(sharedMainRecvMutex, sharedUserSet, sharedMainRecvCondVar, sharedMainRecvFlag);
	//Avvio il thread per la ricezione dei messaggi multicast.
	rt.start();
	//Creo un thread di l'invio di messaggi multicast.
	SenderThread st(name, sharedOnlineFlag);
	//Avvio il thread per l'invio di messaggi multicast.
	st.start();

	//Setto a false il flag per le notifiche spurie.
	*sharedMainRecvFlag = false;

	//TODO: ciclo infinito: vedere se tenerlo o meno.
	while (1)
	{
		//Creo un lock per accedere ai dati della queue.
		unique_lock<mutex> uniqueLock(*sharedMainRecvMutex);
		//Protezione contro notifiche spurie.
		while (*sharedMainRecvFlag == false)
		{  		
			//Attendo lo sblocco del lock.
			sharedMainRecvCondVar->wait(uniqueLock);
		}

		int i = 0;
		for (set<UserData, UserDataComparator>::iterator it= sharedUserSet->begin(); it != sharedUserSet->end(); it++)
		{
			cout << "Thread principale: Elemento " << i << ": " << it->getIP() << endl;
		}
		//Prendo il dato presente nella queue e lo salvo in una stringa.
		//Riabilito il flag per evitare notifiche spurie.
		*sharedMainRecvFlag = false;
		//Sblocco il mutex.
		uniqueLock.unlock();
	}

	//Termino il programma.
	cout << "Thread principale: Uscita wait e terminazione dell'esecuzione del programma." << endl;

	//Pulitura degli shared pointers.
	sharedMainRecvMutex = nullptr;
	sharedUserSet = nullptr;
	sharedMainRecvCondVar = nullptr;
	sharedMainRecvFlag = nullptr;
	sharedOnlineFlag = nullptr;

	return 0;
}

/*Funzione di lettura dei dati dell'utente da file.*/
void getUserData(string &name)
{
	//Creo un nuovo oggetto di tipo ifstream che punta al file dei dati dell'utente
	ifstream userData("data");
	//Se il file esiste entro qui.
	if (userData.good())
	{
		userData >> name;
	}
	//Se invece il file non esiste entro qui.
	else
	{
		//Richiedo all'utente il suo nome.
		name = insertData();
		//Creo un nuovo file.
		fstream newUserData;
		newUserData.open("data", ostream::out);
		//Scrivo il nome dell'utente.
		newUserData << name;
		newUserData.close();
	}
	userData.close();
}

/*Funzione di richiesta inserimento nome (in seguito aggiungere anche foto!)*/
string insertData()
{
	string name;
	cout << "Nuovo utente." << endl;
	cout << "Digita il tuo nome: ";
	cin >> name;
	cout << endl;
	return name;
}