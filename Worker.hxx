#ifndef _WORKER_H_
#define _WORKER_H_

#include "Thread.hxx"
#include "EventQueue.hxx"

#include <event2/bufferevent.h>

class Worker : public Thread
{
	private:
		EventQueue* event_queue;
	public:
		Worker();
		~Worker(){}
		void* run();
};

#endif
