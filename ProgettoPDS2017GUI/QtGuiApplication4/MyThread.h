#pragma once
#include <QThread>


/*Classe che eredita la classe QThread.
 Quando si crea un oggetto di tipo QThread, � possibile avviare un nuovo
 thread chimando il metodo start(). Start far� partire il metodo run() in nuovo thread().
 Per questo motivo andiamo a riscrivere il comportamento del metodo run().*/
class MyThread : public QThread
{
	Q_OBJECT

	public:
		MyThread(QObject *parent = 0);
		~MyThread();

	//run() emetter� il segnale startJob().
	signals:
		void startJob(int);

	//Riscrivo il comportamento del metodo run();
	protected:
		void run() override;
};
