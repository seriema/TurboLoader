#ifndef A_RETRO_UI_APPLICATION_H
#define A_RETRO_UI_APPLICATION_H

class IApplication
{
public:
	virtual ~IApplication() {}
	virtual void loop() = 0;
	virtual void tick() = 0;
	virtual bool is_alive() = 0;
};

#endif //A_RETRO_UI_APPLICATION_H
