#ifndef _EVENT_CONNECT_LISTENER_H_
#define _EVENT_CONNECT_LISTENER_H_

#include "Worker.hxx"

#include <event2/listener.h>

class EventConnectListener
{
	private:
		static struct event_base* evbase;
		struct evconnlistener* listener;
		struct sockaddr_in sin;
		static void accept_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void error_cb(struct evconnlistener* listener, void* ctx);
	public:
		EventConnectListener(){}
		void set_addr(const char* ip_addr, const char* port);
		void create_listener();
		void dispatch();
		void shutdown();
};

#endif
