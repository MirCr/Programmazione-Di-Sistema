#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <string>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <memory>
#include <chrono>

class NetworkThread
{
	public:
		NetworkThread () {};		
		~NetworkThread ();
		void init (const std::shared_ptr<std::mutex>& mutexPointer, const  std::shared_ptr<std::queue<std::string>>& queuePointer, const  std::shared_ptr<std::condition_variable>& notEmptyCondVar, const  std::shared_ptr<std::condition_variable>& emptyCondVar);
		void start ();
		void waitForData ();
	private:
		//Thread della classe.
		std::thread thread;
		//Queue condivisa.
		std::shared_ptr<std::queue<std::string>> queuePointer;
		//Mutua esclusione: protegge la coda.
		std::shared_ptr<std::mutex> mutexPointer;
		//Variabile di condizionamento: indica che la coda non è vuota.
		std::shared_ptr<std::condition_variable> notEmptyCondVar;
		//Variabile di condizionamento: indica che la coda è vuota.
		std::shared_ptr<std::condition_variable> emptyCondVar;
};
