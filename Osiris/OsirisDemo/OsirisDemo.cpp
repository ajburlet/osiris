#include <stdio.h>
#include <OsirisSDK/OException.h>

#include "DemoApplication.h"

int main(int argc, char** argv)
{
	try {
		DemoApplication app(argc, argv);
		app.start();
	}
	catch (OException &e) {
		fprintf(stderr, "[Exception caught] %s\n", e.what());
		return 1;
	}

	return 0;
}
