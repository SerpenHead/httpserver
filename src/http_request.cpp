#include "http_request.h"
#include <sstream>
#include <algorithm>

// 构造函数：初始化状态
HttpRequest::HttpRequest() : state(HttpRequestState::REQUEST_LINE) {}

// 解析 HTTP 请求（分步处理）
bool HttpRequest::parseRequest(const std::string& requestData) {
    std::istringstream stream(requestData);
    std::string line;

    // 按行读取数据
    while (std::getline(stream, line, '\r')) {
        stream.ignore(1);  // 忽略 '\n'

        if (state == HttpRequestState::REQUEST_LINE) {
            if (!parseRequestLine(line)) {
                return false;  // 请求行解析失败
            }
            state = HttpRequestState::HEADERS;  // 进入解析头部状态
        }
        else if (state == HttpRequestState::HEADERS) {
            if (line.empty()) {
                // 空行 -> 头部解析完成，切换到 BODY 或 COMPLETE
                state = (headers.find("Content-Length") != headers.end()) ? HttpRequestState::BODY : HttpRequestState::COMPLETE;
            } else {
                parseHeaders(line);
            }
        }
        else if (state == HttpRequestState::BODY) {
            parseBody(line);
            state = HttpRequestState::COMPLETE;
        }

        if (state == HttpRequestState::COMPLETE) {
            break;  // 请求解析完成，退出循环
        }
    }

    return state == HttpRequestState::COMPLETE;
}

// 解析请求行
bool HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream stream(line);
    if (!(stream >> method >> url >> version)) {
        return false;
    }
    return true;
}

// 解析请求头部
void HttpRequest::parseHeaders(const std::string& line) {
    size_t pos = line.find(": ");
    if (pos != std::string::npos) {
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        headers[key] = value;
    }
}

// 解析请求体
void HttpRequest::parseBody(const std::string& bodyData) {
    body += bodyData;  // 追加请求体数据
}

// 获取 Header 值
std::string HttpRequest::getHeader(const std::string& key) const {
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}
