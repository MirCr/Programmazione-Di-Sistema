#pragma once

#include "ReceiverThread.h"
#include "SenderThread.h"

class Main
{
public:
	Main();
	~Main();
private:
	//Thread della classe.
	std::thread thread;
};