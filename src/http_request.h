# pragma once

#include <string>
#include <unordered_map>

enum class HttpMethod { GET, POST, UNKNOWN };
enum class HttpParseState { REQUEST_LINE, HEADERS, BODY, FINISHED };

class HttpRequest {
public:
    HttpRequest();
    void reset();                      // 重置请求
    bool parseRequest(const std::string& data);  // 解析 HTTP 请求

    HttpMethod getMethod() const;
    std::string getPath() const;
    std::string getHeader(const std::string& key) const;
    std::string getBody() const;

private:
    bool parseRequestLine(const std::string& line);
    bool parseHeaders(const std::string& line);
    bool parseBody(const std::string& data);

    HttpMethod method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    HttpParseState state;

    size_t contentLength;  // 记录 Content-Length
    size_t bodyRead;       // 已读取的请求体长度
};
