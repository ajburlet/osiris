#include <stdio.h>
#include <OsirisSDK/OException.h>

#include "DemoSimulation.h"

int main(int argc, char** argv)
{
	try {
		DemoSimulation app(argc, argv);
		app.start();
	}
	catch (OException &e) {
		fprintf(stderr, "[Exception caught] %s\n", e.what());
		return 1;
	}

	return 0;
}
