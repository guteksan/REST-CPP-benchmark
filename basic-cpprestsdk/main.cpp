#include <thread>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "BasicService.hpp"
#include "InterruptHandler.hpp"
#include <pplx/threadpool.h>

int main(int argc, const char * argv[]) 
{
    log4cplus::PropertyConfigurator logConfig("logger-config.ini");
    logConfig.configure();

    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

    InterruptHandler::hookSIGINT();

    int time = 0;
    if(argc > 1)
    {
       time = std::stoi(argv[1]);
    }

    int th = std::thread::hardware_concurrency();
    if(argc > 2)
    {
       th = std::stoi(argv[2]);
       crossplat::threadpool::initialize_with_threads(th);
    }
    BasicService server(time);
    server.setEndpoint("http://0.0.0.0:2001/test");    
    try {
        server.accept().wait();
        LOG4CPLUS_INFO(logger, "Basic CPP Rest SDK service running on " << th << " threads");
        
        InterruptHandler::waitForUserInterrupt();
        server.shutdown().wait();
    }
    catch(std::exception & e) {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(e.what()));
    }
    return 0;
}
