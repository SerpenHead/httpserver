# pragma once
#include <string>
#include <unordered_map>
#include <sstream>


class HttpResponse {
public:
    HttpResponse();
    void setStatus(int code, const std::string& message);  // 设置状态码和消息
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);

    std::string toString() const;  // 生成 HTTP 响应字符串

private:
    int statusCode;
    std::string statusMessage;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};





