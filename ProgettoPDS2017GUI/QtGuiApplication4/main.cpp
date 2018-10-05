#include "MainWin.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWin window;
    
	window.show();

	return app.exec();
}