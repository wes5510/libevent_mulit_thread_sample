#ifndef _WORKER_CONTROLLER_H_
#define _WORKER_CONTROLLER_H_

#include "Worker.hxx"

#include <vector>

class WorkerController
{
	private:
		std::vector<Worker*> workers;
		const int worker_cnt_;
		int next_use_worker;
	public:
		static WorkerController* get_instance(const int worker_cnt);
		WorkerController(const int worker_cnt);
		void run_all_worker();
		Worker* get_next_worker();
		void destroy();
};

#endif
