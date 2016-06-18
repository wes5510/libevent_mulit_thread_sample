#include "EventConnectListener.hxx"
#include "BufferEvent.hxx"

#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <stdexcept>

#include <arpa/inet.h>

#include <event2/bufferevent.h>

struct event_base* EventConnectListener::evbase = event_base_new();

void EventConnectListener::set_addr(const char* ip_addr, const char* port)
{
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_addr);
	sin.sin_port = htons(atoi(port));
}

void EventConnectListener::create_listener()
{
	try
	{
		listener = evconnlistener_new_bind(evbase, accept_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*) &sin, sizeof(sin));

		if(!listener)
			throw std::runtime_error(strerror(errno));
	}
	catch(std::exception e)
	{
		printf("evconnlistener_new_bind error - %s\n", e.what());
		shutdown();
	}

	evconnlistener_set_error_cb(listener, error_cb);
}

void EventConnectListener::accept_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	printf("accepted connection from %s\n", inet_ntoa(((struct sockaddr_in*)address)->sin_addr));
	BufferEvent be(fd, BEV_OPT_CLOSE_ON_FREE);
}

void EventConnectListener::error_cb(struct evconnlistener* listener, void* ctx)
{
	int err = EVUTIL_SOCKET_ERROR();
	printf("got an error %d (%s) on the listener.shutting down\n", err, evutil_socket_error_to_string(err));

	event_base_loopexit(evbase, NULL);
}

void EventConnectListener::dispatch()
{
	event_base_dispatch(evbase);
	shutdown();
}

void EventConnectListener::shutdown()
{
	if(evbase != NULL){
		event_base_loopexit(evbase, NULL);
		evbase = NULL;
	}
	if(!listener) evconnlistener_free(listener);
}
