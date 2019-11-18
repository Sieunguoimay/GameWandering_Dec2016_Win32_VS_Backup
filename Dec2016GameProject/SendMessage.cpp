#include "SendMessage.h"



SendMessage::SendMessage()
{
	this->shouldBeDestroyed = false;
}


SendMessage::~SendMessage()
{
	this->shouldBeDestroyed = false;
}

bool SendMessage::isItTimeToDestroy()const
{
	return this->shouldBeDestroyed;
}

void SendMessage::destroyNow()
{
	this->shouldBeDestroyed = true;
}
