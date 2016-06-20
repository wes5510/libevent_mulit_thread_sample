#ifndef _WORKER_H_
#define _WORKER_H_

#include "Thread.hxx"
#include "EventQueue.hxx"

#include <event2/bufferevent.h>

class Worker : public Thread
{
	private:
		EventQueue* event_queue;
		bool terminate;
	public:
		Worker();
		~Worker(){}
		void set_terminate(bool flag);
		void* run();
};

#endif
