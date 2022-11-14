#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <map>

namespace HttpRequest 
{

    enum class RequestType
    {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
        HEAD,
        CONNECT,
        OPTIONS,
        TRACE,
    };

    

    class Request{
        private:
            RequestType request;
            std::string route;

        public:
            Request(RequestType request, std::string route)
            {
                this->request = request;
                this->route = route; 
            }

            void setRequest(RequestType request){this->request = request;}
            void setRoute(std::string route){this->route = route;}
            RequestType getRequest(){return this->request;}
            std::string getRoute(){return this->route;}

            static RequestType convertToRequest(std::string requestString)
            {
                std::map<std::string, RequestType> requestToString = 
                {
                    {"GET", RequestType::GET},
                    {"POST", RequestType::POST},
                    {"PUT", RequestType::PUT},
                    {"PATCH", RequestType::PATCH},
                    {"DELETE", RequestType::DELETE},
                    {"HEAD", RequestType::HEAD},
                    {"CONNECT", RequestType::CONNECT},
                    {"OPTIONS", RequestType::OPTIONS},
                    {"TRACE", RequestType::TRACE}
                };
                return requestToString.at(requestString);
            }

            static std::string requestToString(RequestType request)
            {
                std::map<RequestType, std::string> requestToString = 
                {
                    {RequestType::GET, "GET"},
                    {RequestType::POST, "POST"},
                    {RequestType::PUT, "PUT"},
                    {RequestType::PATCH, "PATCH"},
                    {RequestType::DELETE, "DELETE"},
                    {RequestType::HEAD, "HEAD"},
                    {RequestType::CONNECT, "CONNECT"},
                    {RequestType::OPTIONS, "OPTIONS"},
                    {RequestType::TRACE, "TRACE"}
                };
                return requestToString.at(request);
            }
    };
};
#endif