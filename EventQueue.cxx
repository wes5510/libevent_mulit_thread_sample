#include "EventQueue.hxx"

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include <stdexcept>

EventQueue* EventQueue::get_instance()
{
	static EventQueue event_queue;
	return &event_queue;
}

EventQueue::EventQueue()
{
	if(pthread_cond_and_mutex_init() == -1)
		printf("pthread cond and mutex init error\n");
	queue = new std::deque<struct bufferevent*>();
}

int EventQueue::pthread_cond_and_mutex_init()
{
	if(pthread_cond_init(&cond, NULL) != 0)
		return -1;
	if(pthread_mutex_init(&mutex, NULL) != 0)
		return -1;

	return 0;
}

EventQueue::~EventQueue()
{
	delete queue;
	queue = NULL;

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

bufferevent* EventQueue::pop()
{
	struct bufferevent* bev = NULL;
	try
	{
		if(pthread_mutex_lock(&mutex) != 0)
			throw std::runtime_error(strerror(errno));

		while(queue->empty())
		{
			if(pthread_cond_wait(&cond, &mutex) != 0)
				throw std::runtime_error(strerror(errno));
		}
		bev = queue->front();
		queue->pop_front();

		if(pthread_mutex_unlock(&mutex) != 0)
			throw std::runtime_error(strerror(errno));
	}
	catch(std::exception e)
	{
		printf("%s\n", e.what());
		exit(1);
	}

	return bev;
}

void EventQueue::push(struct bufferevent* bev)
{
	pthread_mutex_lock(&mutex);
	queue->push_back(bev);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}
