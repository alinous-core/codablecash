
#include "osenv/sockets.h"
#include "test_utils/TestExecutor.h"
using namespace alinous;

const char* prog = nullptr;

int main(int ac, char** av) {
	prog = av[0];

	SocketPipeSigHandler::setSigPipeHander(SocketPipeSigHandler::defaultPipeHandler);

	TestExecutor* exec = TestExecutor::get();
	return exec->execute(ac, av);
}

