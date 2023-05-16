#include "App.h"

#include "Http3Response.h"
#include "Http3Request.h"
#include "Http3Context.h"

namespace uWS {

    struct H3App {
        Http3Context *httpContext;

        H3App(SocketContextOptions options = {}) {
            /* This conversion should not be needed */
            us_quic_socket_context_options_t h3options = {};

            h3options.key_file_name = strdup(options.key_file_name);
            h3options.cert_file_name = strdup(options.cert_file_name);
            h3options.passphrase = strdup(options.passphrase);

            /* Create the http3 context */
            httpContext = Http3Context::create((us_loop_t *)Loop::get(), h3options);

            httpContext->init();
        }

        /* Disallow copying, only move */
        H3App(const H3App &other) = delete;

        H3App(H3App &&other) {
            /* Move HttpContext */
            httpContext = other.httpContext;
            other.httpContext = nullptr;
        }

        /* Host, port, callback */
        H3App &&listen(std::string host, int port, MoveOnlyFunction<void(us_listen_socket_t *)> &&handler) {
            if (!host.length()) {
                return listen(port, std::move(handler));
            }
            handler(httpContext ? (us_listen_socket_t *) httpContext->listen(host.c_str(), port) : nullptr);
            return std::move(*this);
        }

        /* Host, port, options, callback */
        H3App &&listen(std::string host, int port, int options, MoveOnlyFunction<void(us_listen_socket_t *)> &&handler) {
            if (!host.length()) {
                return listen(port, options, std::move(handler));
            }
            handler(httpContext ? (us_listen_socket_t *) httpContext->listen(host.c_str(), port) : nullptr);
            return std::move(*this);
        }

        /* Port, callback */
        H3App &&listen(int port, MoveOnlyFunction<void(us_listen_socket_t *)> &&handler) {
            handler(httpContext ? (us_listen_socket_t *) httpContext->listen(nullptr, port) : nullptr);
            return std::move(*this);
        }

        /* Port, options, callback */
        H3App &&listen(int port, int options, MoveOnlyFunction<void(us_listen_socket_t *)> &&handler) {
            handler(httpContext ? (us_listen_socket_t *) httpContext->listen(nullptr, port) : nullptr);
            return std::move(*this);
        }

        H3App &&get(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("GET", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&post(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("POST", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&options(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("OPTIONS", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&del(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("DELETE", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&patch(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("PATCH", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&put(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("PUT", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&head(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("HEAD", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&connect(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("CONNECT", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        H3App &&trace(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("TRACE", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        /* This one catches any method */
        H3App &&any(std::string pattern, MoveOnlyFunction<void(Http3Response *, Http3Request *)> &&handler) {
            if (httpContext) {
                httpContext->onHttp("*", pattern, std::move(handler));
            }
            return std::move(*this);
        }

        void run() {
            uWS::Loop::get()->run();
        }
    };
}