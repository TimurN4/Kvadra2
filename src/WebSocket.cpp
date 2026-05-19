#include <WebSocket.hpp>
#include <chrono>
#include <parser.hpp>
#include <userver/formats/json.hpp>
#include <userver/engine/async.hpp>
#include <userver/engine/deadline.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/components/component.hpp>
#include <userver/engine/sleep.hpp>




// userver::engine::AsyncNoSpan

namespace web {

WebsocketsHandler::WebsocketsHandler(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& component_context) 
    : WebsocketHandlerBase(config, component_context), fs_tp(component_context.GetTaskProcessor("fs-task-processor")) {}

void WebsocketsHandler::Handle(userver::server::websocket::WebSocketConnection& chat, userver::server::request::RequestContext&) const {
    // userver::server::websocket::Message message;
    while (!userver::engine::current_task::ShouldCancel()) {
        // chat.Recv(message);  // throws on closed/dropped connection
        // if (message.close_status) {
        //     break;  // explicit close if any
        // }
        
        userver::formats::json::ValueBuilder json;
        auto task = userver::engine::AsyncNoSpan(fs_tp, parser::parse, std::ref(json));
        task.Get();

        chat.Send({userver::formats::json::ToString(json.ExtractValue()), std::nullopt, true});  // throws on closed/dropped connection

        userver::engine::InterruptibleSleepFor(std::chrono::seconds(1));
    }
    // if (message.close_status) {
    //     chat.Close(*message.close_status);
    // }
}

} // end namespace web