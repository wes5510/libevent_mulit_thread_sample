#include "Worker.hxx"

#include <event2/bufferevent.h>
#include <event2/event.h>

Worker::Worker(): terminate(false)
{
	event_queue = EventQueue::get_instance();
}

void Worker::set_terminate(const bool flag)
{
	terminate = flag;
}

void* Worker::run()
{
	struct bufferevent* bev = NULL;
	struct event_base* evbase = NULL;
	while(1)
	{
		bev = event_queue->pop(terminate);

		if(terminate)
			break;
		evbase = bufferevent_get_base(bev);

		event_base_dispatch(evbase);

		if(bev != NULL){
			bufferevent_free(bev);
			bev = NULL;
		}
	}

	pthread_exit(NULL);
	return NULL;
}
