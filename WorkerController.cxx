#include "WorkerController.hxx"

#include <cstdio>

WorkerController* WorkerController::get_instance(const int worker_cnt)
{
	static WorkerController worker_controller(worker_cnt);
	return &worker_controller;
}

WorkerController::WorkerController(const int worker_cnt) : worker_cnt_(worker_cnt), next_use_worker(0)
{
	workers.reserve(worker_cnt_);
	for(int i = 0; i < worker_cnt_; i++)
	{
		Worker* worker = new Worker();
		workers.push_back(worker);
	}
	run_all_worker();
}

void WorkerController::run_all_worker()
{
	for(int i = 0; i < worker_cnt_; i++)
		workers[i]->start();
}

Worker* WorkerController::get_next_worker()
{
	Worker* worker = workers[next_use_worker];
	if(next_use_worker > worker_cnt_ - 1)
		next_use_worker = next_use_worker % worker_cnt_;
	return worker;
}

void WorkerController::destroy()
{
	for(int i = 0; i < worker_cnt_; i++)
	{
		delete workers[i];
		workers[i] = NULL;
	}
}
