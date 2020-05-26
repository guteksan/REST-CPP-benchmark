#include <pistache/endpoint.h>
#include <pistache/http_headers.h>
#include <pistache/router.h>
#include <pistache/mime.h>
#include <pistache/http.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
//#include <future>

using namespace Pistache;

void pretendThatThinkingHard(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

class BasicService 
{
    int time;
public:
    explicit BasicService(Address addr) : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { 
        logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    }

    void init(size_t thr, int _time) 
    {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        time = _time;
        httpEndpoint->init(opts);
        setupRoutes();
        httpEndpoint->setHandler(router.handler());
	LOG4CPLUS_INFO(logger, "Basic pistache service running on " << thr << " threads");
    }

    void start() 
    {
        httpEndpoint->serve();
    }

private:
    void setupRoutes() 
    {
        using namespace Rest;

        Routes::Post(router, "/test", Routes::bind(&BasicService::handlePost, this));
        Routes::Get(router, "/test/status", Routes::bind(&BasicService::status, this));
        Routes::Get(router, "/test/labels/:country", Routes::bind(&BasicService::handleGet, this));
    }

    void handlePost(const Rest::Request& request, Http::ResponseWriter response) {

	using namespace Http::Mime;
	using namespace Http;

        LOG4CPLUS_INFO(logger, "Received request");
	std::string response_body = "Sending response: " + request.body();
	pretendThatThinkingHard(time);
	response.headers().add<Header::ContentType>(MIME(Application, OctetStream));
        response.send(Http::Code::Ok, response_body);
        LOG4CPLUS_INFO(logger, "Processing finished");

    }

    void handleGet(const Rest::Request& request, Http::ResponseWriter response) {
        auto country = request.param(":country").as<std::string>();

        if(country.size()==3) {
            std::string responseText = "GET request for country: " + country;
            response.send(Http::Code::Ok, responseText);
        }
        else {
            response.send(Http::Code::Not_Found, "Country not handled");
        }
    }

    void status(const Rest::Request& request, Http::ResponseWriter response) {
         response.send(Http::Code::Ok, "OK!");
    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
    log4cplus::Logger logger;
};


int main(int argc, char **argv) 
{
    log4cplus::PropertyConfigurator logConfig("logger-config.ini");
    logConfig.configure();

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
    Port port(2001);
    Address addr(Ipv4::any(), port);
    BasicService service(addr);

    service.init(th, time);
    service.start();
}
