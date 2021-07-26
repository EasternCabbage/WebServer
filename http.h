#include <sys/socket.h>

enum HttpPraseState{
    NO_REQUEST = 0,
    GET_REQUEST,
    BAD_REQUEST,
    NO_RESOURCE,
    FORBIDDEN_REQUEST,
    FILE_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION

};
enum HttpRequestMethod{
    GET = 0,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATH
};



class Http{
    public:
        void HttpInit();
        std::string HttpFirstLine();
        HttpPraseState HttpPraseStatu();
        HttpRequestMethod HttpPraseMethod();
        std::string HttpPraseUrl();
        void do_request();

        void HttpLoop();
        void HttpResponse();
 /*   public:
        Http();
        ~Http(); */
    private:
        int sfd,cfd;
        std::string recv_content;
        std::string send_content;
        std::string http_method;
        std::string http_url;
};