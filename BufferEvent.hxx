#ifndef _BUFFER_EVENT_H_
#define _BUFFER_EVENT_H_

#include "EventQueue.hxx"
#include <event2/bufferevent.h>

class BufferEvent
{
	private:
		EventQueue* event_queue;
		struct bufferevent *bev;
		static void event_cb(struct bufferevent* bev, short what, void* ctx);
		static void read_cb(struct bufferevent* bev, void* ctx);
	public:
		BufferEvent(int fd, enum bufferevent_options options);
		~BufferEvent();
};

#endif
