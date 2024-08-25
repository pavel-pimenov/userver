#pragma once

#include <userver/server/http/http_response.hpp>

USERVER_NAMESPACE_BEGIN

namespace server::http {

class Http2RequestParser;

void WriteHttp2ResponseToSocket(engine::io::RwBase& socket,
                                HttpResponse& response,
                                Http2RequestParser& parser);

}  // namespace server::http

USERVER_NAMESPACE_END
