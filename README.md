#matgui

Matgui is a simple gui that aims to
* Be platform independent (linux, windows, mac, android, iphone/ipad)
* Have a minimal set of dependencies
* Easy to import in project
* Implement signals and slots to do callback
* Thread safe
* Super easy to build and used (compared to gtk, qt, ect)



example usage:


```
class Program {
	
	LinearLayout layout;
	Button button;
	
	Program() {
		layout.add(&button);
		button.connect(this, Program::click);
	}
	
	void click(View::pointerArgument arg){
		cout << "click at " << arg.x << ", " << arg.y << endl;
	}
	
	
	
	//render function
	draw() {
		layout.draw(); //draws all children
	}
	
	//handle events
	void backendEventCallbackExampleFromSDL() {
		//...
		button.onMouseDown(0, x, y);
		//...
	}
	
}
```



It is currently in early development phase, please contact me if you want more information if how to use it
