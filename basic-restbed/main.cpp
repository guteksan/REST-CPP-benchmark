#include <memory>
#include <restbed>
#include <iostream>
#include <future>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace restbed;

void pretendThatThinkingHard(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

class TestHandler
{
        int time; 
    	log4cplus::Logger logger;

public:
	TestHandler(int _time) : time(_time)
 	{
		logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
	}

	void operator()(const shared_ptr<Session> session, const Bytes& body)
	{
	    LOG4CPLUS_INFO(logger, "Received request");
	    std::string resp = "Sending response: ";
    	    //std::vector<char> body_buffer(body.begin(), body.end());

	    pretendThatThinkingHard(time);
	    std::vector<char> response;

	    std::copy(resp.begin(), resp.end(), std::back_inserter(response));
	    std::copy(body.begin(), body.end(), std::back_inserter(response));

	    restbed::Bytes outputBody(response.begin(), response.end());
	    session->yield(
		restbed::OK,
		outputBody,
		{
		    { "Content-Type", "application/octet-stream" },
		    { "Content-Length", std::to_string(outputBody.size()) },
		    { "Connection", "keep-alive" }
		}
	    );
	    LOG4CPLUS_INFO(logger, "Processing finished");
	}
};

int main( const int argc, const char** argv)
{
    log4cplus::PropertyConfigurator logConfig("logger-config.ini");
    logConfig.configure();
    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(2001);


    int time = 0;
    if(argc > 1)
    {
       time = std::stoi(argv[1]);
    }
    int th = std::thread::hardware_concurrency();
    if(argc > 2)
    {
       th = std::stoi(argv[2]);
    }

    settings->set_worker_limit(th);
    TestHandler handler(time);

    restbed::Service service;
    auto resourceTest = std::make_shared<restbed::Resource>();
    auto resourceStatus = std::make_shared<restbed::Resource>();
    auto resourceLabels = std::make_shared<restbed::Resource>();

    resourceTest->set_path("/test");
    resourceTest->set_method_handler(
        "POST",
        {
//            { "Accept", "application/octet-stream" },
//            { "Content-Type", "application/octet-stream" },
        },
        [&](const shared_ptr<Session> session) {
            const auto request = session->get_request();
            size_t content_length = request->get_header("Content-Length", 0);
            session->fetch(content_length, std::ref(handler));
    });

    resourceStatus->set_path("/test/status");
    resourceStatus->set_method_handler(
        "GET",
        [](const shared_ptr<Session> session) {
		session->yield(
			restbed::OK,
			"OK!");
	});

    resourceLabels->set_path("/test/labels/{country: .*}");
    resourceLabels->set_method_handler(
        "GET",
        [](const shared_ptr<Session> session) {

    		const auto& request = session->get_request();
    		const std::string country = request->get_path_parameter("country");
                std::string responseText = "GET request for country: " + country;
		session->yield(
			restbed::OK,
			responseText);
	});

    service.publish(resourceTest);
    service.publish(resourceStatus);
    service.publish(resourceLabels);

    LOG4CPLUS_INFO(logger, "Basic restbed service running on " << th << " threads");
    service.start(settings);
    return EXIT_SUCCESS;
}
