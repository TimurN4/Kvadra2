#pragma once
#include <userver/server/websocket/websocket_handler.hpp>


namespace web {

class WebsocketsHandler final : public userver::server::websocket::WebsocketHandlerBase {
public:
    // `kName` is used as the component name in static config
    static constexpr std::string_view kName = "websocket-handler";
 
    WebsocketsHandler(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& component_context);
 
    void Handle(userver::server::websocket::WebSocketConnection& chat, userver::server::request::RequestContext&) const override;

private:
    userver::engine::TaskProcessor& fs_tp;
};

} // end namespace web