#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "crow/crow_all.h"

void pretendThatThinkingHard(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

class ExampleLogHandler : public crow::ILogHandler {
	
        log4cplus::Logger logger;
    public:
        ExampleLogHandler()
        {
 	    logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("crow"));
        }

        void log(std::string message, crow::LogLevel level) override {
	switch(level)
        {
	case crow::LogLevel::Debug:
		LOG4CPLUS_DEBUG(logger, message); break;
	case crow::LogLevel::Info:
		LOG4CPLUS_INFO(logger, message); break;
	case crow::LogLevel::Warning:
		LOG4CPLUS_WARN(logger, message); break;
	case crow::LogLevel::Error:
		LOG4CPLUS_ERROR(logger, message); break;
	case crow::LogLevel::Critical:
		LOG4CPLUS_FATAL(logger, message); break;
        }
	}
};


int main(int argc, char **argv)
{

  log4cplus::PropertyConfigurator logConfig("logger-config.ini");
  logConfig.configure();
  log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

  crow::SimpleApp app;
  app.loglevel(crow::LogLevel::INFO);

  ExampleLogHandler handler;
  crow::logger::setHandler(&handler);

  int time = 0;
  if(argc > 1)
  {
     time = std::stoi(argv[1]);
  }

  CROW_ROUTE(app, "/test/status")
  ([](){
  	return "OK";
  });

  CROW_ROUTE(app, "/test/labels/<string>")
  ([](std::string country){
        std::string responseText = "GET request for country: " + country;
  	return crow::response{responseText};
  });

  CROW_ROUTE(app, "/test").methods("POST"_method)
  ([&](const crow::request& req) {
    //LOG4CPLUS_INFO(logger, "Received request");
    if(time > 0)
    	pretendThatThinkingHard(time);
    auto responseText = "Sending response: " + req.body;
    //LOG4CPLUS_INFO(logger, "Processing finished");
    return crow::response{responseText};
  });

  if(argc > 2)
  {
      app.port(2001).concurrency(std::stoi(argv[2])).run();
  }
  else
  {
      app.port(2001).multithreaded().run();
  }
}
