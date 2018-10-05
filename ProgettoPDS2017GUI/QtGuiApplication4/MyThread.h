#pragma once
#include <QThread>


/*Classe che eredita la classe QThread.
 Quando si crea un oggetto di tipo QThread, è possibile avviare un nuovo
 thread chimando il metodo start(). Start farà partire il metodo run() in nuovo thread().
 Per questo motivo andiamo a riscrivere il comportamento del metodo run().*/
class MyThread : public QThread
{
	Q_OBJECT

	public:
		MyThread(QObject *parent = 0);
		~MyThread();

	//run() emetterà il segnale startJob().
	signals:
		void startJob(int);

	//Riscrivo il comportamento del metodo run();
	protected:
		void run() override;
};
