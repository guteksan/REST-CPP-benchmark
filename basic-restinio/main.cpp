#include <restinio/all.hpp>
#include <fmt/format.h>
//#include <future>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

using router_t = restinio::router::express_router_t<>;

void pretendThatThinkingHard(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

class RequestHandler
{
    log4cplus::Logger logger;
    int time;
public:
    RequestHandler(int _time) : time(_time)
    {
	logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    }

    restinio::request_handling_status_t operator()(restinio::request_handle_t req, restinio::router::route_params_t params)
    {
        using output_type_t = restinio::user_controlled_output_t;

        LOG4CPLUS_INFO(logger, "Received request");
	pretendThatThinkingHard(time);
	std::string response_body = "Sending response: " + req->body();
        auto resp = req->create_response<output_type_t>(restinio::status_ok());
     	resp.append_header( restinio::http_field::content_type, "application/octet-stream" );
        resp.set_content_length(response_body.size());
        resp.set_body(response_body);
        LOG4CPLUS_INFO(logger, "Processing finished");
        return resp.done();
    }
};

auto service_handler(RequestHandler& handler, int th)
{
    auto router = std::make_unique<router_t>();

    router->http_post("/test", handler); 

    router->http_get("/test/status", 
        [](auto req, auto param) {
            return req->create_response()
                //.append_header_date_field()
                .append_header( restinio::http_field::content_type, "text/plain; charset=utf-8" )
                .set_body( "OK!")
                .done();
        });

    router->http_get(R"(/test/labels/:country([A-Z]{3}))",
        [](auto req, auto params) {
            return req->create_response()
                //.append_header_date_field()
           	.append_header( restinio::http_field::content_type, "text/plain; charset=utf-8" )
                .set_body(fmt::format(
                    "GET request for country: '{}'", params["country"] ))
                .done();   
        });              


    router->non_matched_request_handler(
		[]( auto req ){
			return req->create_response(restinio::status_not_found())
			    	.append_header_date_field()
				.connection_close()
				.done();
		}); 

    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    LOG4CPLUS_INFO(logger, "Basic-restinio service running on " << th << " threads");
    return router;
}

int main(int argc, char **argv)
{
    log4cplus::PropertyConfigurator logConfig("logger-config.ini");
    logConfig.configure();

    using traits_t = 
    restinio::traits_t<
        restinio::asio_timer_manager_t,
        restinio::null_logger_t,
        router_t >;

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

    RequestHandler handler(time);

    restinio::run(
        restinio::on_thread_pool<traits_t>(th)
        .port(2001)
        .address("0.0.0.0")
        .request_handler(service_handler(handler,th))
    );
    return 0;
}
