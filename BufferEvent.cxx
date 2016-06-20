#include "BufferEvent.hxx"
#include "EventConnectListener.hxx"
#include "WorkerController.hxx"
#include "BufferEventBaseController.hxx"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <stdexcept>
#include <vector>

#include <event2/buffer.h>

BufferEvent::BufferEvent(int fd, enum bufferevent_options options)
{
	try
	{
		struct event_base* evbase = BufferEventBaseController::get_instance(5)->get_next_event_base();
		bev = bufferevent_socket_new(evbase, fd, options);
		if(bev == NULL)
			throw std::runtime_error(strerror(errno));
	}
	catch(std::exception e)
	{
		printf("bufferevent_socket_new error - %s\n", e.what());
		exit(1);
	}
	bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
	bufferevent_enable(bev, EV_READ|EV_WRITE);

	event_queue = EventQueue::get_instance();
	event_queue->push(bev);
}

void BufferEvent::event_cb(struct bufferevent* bev, short what, void* ctx)
{
	if(what & BEV_EVENT_ERROR)
	{
		printf("bufferevent error - %s\n", strerror(errno));
		event_base_loopexit(bufferevent_get_base(bev), NULL);
	}
	else if(what & BEV_EVENT_EOF)
	{
		printf("client disconnet\n");
		event_base_loopexit(bufferevent_get_base(bev), NULL);
	}
}

void BufferEvent::read_cb(struct bufferevent* bev, void* ctx)
{
	struct evbuffer* input = bufferevent_get_input(bev);
	struct evbuffer* output = bufferevent_get_output(bev);

	evbuffer_add_buffer(output, input);
}
