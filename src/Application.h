#ifndef A_RETRO_UI_APPLICATION_H
#define A_RETRO_UI_APPLICATION_H

#include "platform.h"

class IApplication
{
public:
	virtual ~IApplication() {}
	virtual void loop() = 0;
	virtual void tick( u32 dt ) = 0;
	virtual bool is_alive() = 0;
};

#endif //A_RETRO_UI_APPLICATION_H
