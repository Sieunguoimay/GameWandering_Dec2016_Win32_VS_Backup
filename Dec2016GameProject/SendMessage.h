#pragma once
//This class is used to send a message of dying from brick to all the items hung on it
//All of the items must derive from this base class => nice story
#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

class SendMessage
{
public:
	SendMessage();
	~SendMessage();
	bool isItTimeToDestroy()const;
	void destroyNow();
	bool shouldBeDestroyed;
};

#endif // !SENDMESSAGE_H
