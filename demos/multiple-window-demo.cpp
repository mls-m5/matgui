

#include "application.h"
#include "window.h"
#include "button.h"

using namespace MatGui;

int main(int argc, char **argv) {
	Application app(argc, argv);

	Window window1("hej");
	Window window2("hej2");

	window1.addChild(new Button("hej 1"));
	window2.addChild(new Button("hej 2"));

	app.mainLoop();
}
