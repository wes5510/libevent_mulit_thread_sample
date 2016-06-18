#ifndef _BUFFER_EVENT_BASE_CONTROLLER_H_
#define _BUFFER_EVENT_BASE_CONTROLLER_H_

#include <vector>

#include <event2/event.h>

class BufferEventBaseController
{
	private:
		std::vector<struct event_base*> event_bases;
		const int event_base_cnt_;
		int next_use_event_base;
		BufferEventBaseController(const int event_base_cnt);
	public:
		static BufferEventBaseController* get_instance(const int event_base_cnt);
		struct event_base* get_next_event_base();
		void destroy();
};

#endif
