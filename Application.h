#ifndef A_RETRO_UI_IAPPLICATION_H
#define A_RETRO_UI_IAPPLICATION_H

class IApplication
{
public:
	virtual ~IApplication() {}
	virtual void loop() = 0;
	virtual void tick() = 0;
	virtual bool is_alive() = 0;
};

#endif //A_RETRO_UI_IAPPLICATION_H
