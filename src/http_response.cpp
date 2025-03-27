#include "http_request.h"
#include "http_response.h"
#include <sstream>

HttpResponse::HttpResponse() {
    statusCode = 200;
    statusMessage = "OK";
    headers["Content-Type"] = "text/html";
}

// 设置状态码和状态消息
void HttpResponse::setStatus(int code, const std::string& message) {
    statusCode = code;
    statusMessage = message;
}

// 设置 HTTP 头
void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

// 设置响应体
void HttpResponse::setBody(const std::string& bodyContent) {
    body = bodyContent;
    headers["Content-Length"] = std::to_string(body.size());
}

// 生成 HTTP 响应字符串
std::string HttpResponse::toString() const {
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    for (const auto& header : headers) {
        response << header.first << ": " << header.second << "\r\n";
    }
    response << "\r\n" << body;
    return response.str();
}
