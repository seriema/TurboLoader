#ifndef INPUT_H
#define INPUT_H


class Input
{

public:
	Input();
	~Input();

	void poll_events();
	// Returns true if the application should quit.
	bool quit_requested();

private:
	bool _quit;
	int _num_controllers;
	SDL_Joystick* _controller;

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;

	void init_controller();
	void handle_keys( SDL_KeyboardEvent event );
	void handle_controller( SDL_JoyAxisEvent axisEvent, SDL_JoyButtonEvent buttonEvent );
};


#endif // INPUT_H
