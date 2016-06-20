#include "BufferEventBaseController.hxx"

BufferEventBaseController* BufferEventBaseController::get_instance(const int event_base_cnt)
{
	static BufferEventBaseController bebc(event_base_cnt);
	return &bebc;
}

BufferEventBaseController::BufferEventBaseController(const int event_base_cnt) : event_base_cnt_(event_base_cnt), next_use_event_base(0)
{
	event_bases.reserve(event_base_cnt_);
	for(int i = 0; i < event_base_cnt_; i++)
		event_bases.push_back(event_base_new());
}

struct event_base* BufferEventBaseController::get_next_event_base()
{
	int tmp = next_use_event_base;

	if(++next_use_event_base >= event_base_cnt_)
		next_use_event_base = next_use_event_base % event_base_cnt_;

	return event_bases[tmp];
}

void BufferEventBaseController::destroy()
{
	for(int i = 0; i < event_base_cnt_; i++){
		event_base_free(event_bases[i]);
		event_bases[i] = NULL;
	}
}
