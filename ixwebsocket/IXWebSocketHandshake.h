/*
 *  IXWebSocketHandshake.h
 *  Author: Benjamin Sergeant
 *  Copyright (c) 2019 Machine Zone, Inc. All rights reserved.
 */

#pragma once

#include "IXCancellationRequest.h"
#include "IXWebSocketHttpHeaders.h"
#include "IXWebSocketPerMessageDeflate.h"
#include "IXWebSocketPerMessageDeflateOptions.h"
#include "IXSocket.h"

#include <string>
#include <atomic>
#include <chrono>
#include <memory>
#include <tuple>

namespace ix 
{
    struct WebSocketInitResult
    {
        bool success;
        int http_status;
        std::string errorStr;
        WebSocketHttpHeaders headers;

        WebSocketInitResult(bool s = false,
                            int status = 0,
                            const std::string& e = std::string(),
                            WebSocketHttpHeaders h = WebSocketHttpHeaders())
        {
            success = s;
            http_status = status;
            errorStr = e;
            headers = h;
        }
    };

    class WebSocketHandshake {
    public:
        WebSocketHandshake(std::atomic<bool>& requestInitCancellation,
                           std::shared_ptr<Socket> _socket,
                           WebSocketPerMessageDeflate& perMessageDeflate,
                           WebSocketPerMessageDeflateOptions& perMessageDeflateOptions,
                           std::atomic<bool>& enablePerMessageDeflate);

        WebSocketInitResult clientHandshake(const std::string& url,
                                            const std::string& host,
                                            const std::string& path,
                                            int port);
        WebSocketInitResult serverHandshake(int fd);

        static bool parseUrl(const std::string& url,
                             std::string& protocol,
                             std::string& host,
                             std::string& path,
                             std::string& query,
                             int& port);

    private:
        static void printUrl(const std::string& url);
        std::string genRandomString(const int len);

        // Parse HTTP headers
        std::pair<bool, WebSocketHttpHeaders> parseHttpHeaders(const CancellationRequest& isCancellationRequested);
        WebSocketInitResult sendErrorResponse(int code, const std::string& reason);

        std::tuple<std::string, std::string, std::string> parseRequestLine(const std::string& line);

        std::atomic<bool>& _requestInitCancellation;
        std::shared_ptr<Socket> _socket;
        WebSocketPerMessageDeflate& _perMessageDeflate;
        WebSocketPerMessageDeflateOptions& _perMessageDeflateOptions;
        std::atomic<bool>& _enablePerMessageDeflate;
    };
}