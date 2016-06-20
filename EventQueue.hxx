#ifndef _EVENT_QUEUE_H
#define _EVENT_QUEUE_H

#include <deque>

#include <pthread.h>

#include <event2/bufferevent.h>

class EventQueue
{
	private:
		std::deque<struct bufferevent*> queue;
		pthread_mutex_t mutex;
		pthread_cond_t cond;
		EventQueue();
		int pthread_cond_and_mutex_init();
	public:
		static EventQueue* get_instance();
		struct bufferevent* pop(bool stop);
		void push(struct bufferevent* bev);
		void cond_broadcast();
};

#endif
