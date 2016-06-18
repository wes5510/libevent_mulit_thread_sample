#include "Worker.hxx"

#include <event2/bufferevent.h>
#include <event2/event.h>

Worker::Worker()
{
	event_queue = EventQueue::get_instance();
}

void* Worker::run()
{
	struct bufferevent* bev = NULL;
	while(1)
	{
		bev = event_queue->pop();
		event_base_dispatch(bufferevent_get_base(bev));
	}

	return NULL;
}
