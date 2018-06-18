#ifndef OBSERVER_H
#define OBSERVER_H

struct Message;

class Observer
{
public:
	virtual ~Observer() {}
	virtual void Notify(const Message & message) = 0;
};

#endif