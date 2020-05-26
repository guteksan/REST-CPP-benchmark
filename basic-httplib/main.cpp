//#define CPPHTTPLIB_THREAD_POOL_COUNT 4

#include <httplib.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

using namespace httplib;

void pretendThatThinkingHard(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}


int main(int argc, char **argv)
{
  log4cplus::PropertyConfigurator logConfig("logger-config.ini");
  logConfig.configure();
  log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

  int time = 0;
  if(argc > 1)
  {
     time = std::stoi(argv[1]);
  }

  Server svr;

  svr.Get("/test/status", [](const Request& req, Response& res) {
    res.set_content("OK!", "text/plain");
  });

  svr.Get(R"(/test/labels/([A-Z]{3}))", [&](const Request& req, Response& res) {
    std::string country = req.matches[1];
    std::string responseText = "GET request for country: " + country;
    res.set_content(responseText, "text/plain");
  });

  svr.Post("/test", [&](const Request& req, Response& res) {
	
	if(req.has_header("Content-type") && req.get_header_value("Content-type").compare("application/json") == 0)
	{
	    LOG4CPLUS_INFO(logger, "Received request");
	    pretendThatThinkingHard(time);
	    auto responseText = "Sending response: " + req.body;
	    res.set_content(responseText, "text/plain");
	    LOG4CPLUS_INFO(logger, "Processing finished");
	}
	else
	{
	    res.status = 400;
	} 
  });

#ifdef CPPHTTPLIB_THREAD_POOL_COUNT
  LOG4CPLUS_INFO(logger, "Basic httplib service running on " << CPPHTTPLIB_THREAD_POOL_COUNT << " threads");
#else
  LOG4CPLUS_INFO(logger, "Basic httplib running on " << std::thread::hardware_concurrency() << " threads");
#endif
  //int port = svr.bind_to_any_port("0.0.0.0");
  //svr.listen_after_bind();
  svr.listen("0.0.0.0", 2001);
}
