#include "EventConnectListener.hxx"
#include "WorkerController.hxx"
#include "BufferEventBaseController.hxx"

#include <cstring>

#include <signal.h>

EventConnectListener ecl;

void signal_handler(int signo)
{
	printf("Received signal %d: %s.  Shutting down.\n", signo, strsignal(signo));
	BufferEventBaseController::get_instance(5)->destroy();
	WorkerController::get_instance(5)->destroy();
	ecl.shutdown();
}

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Usage : ./echosvr server_ip_addr port\n");
		return -1;
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	BufferEventBaseController::get_instance(5);
	WorkerController::get_instance(5);

	ecl.set_addr(argv[1], argv[2]);
	ecl.create_listener();
	ecl.dispatch();
	return 0;
}
