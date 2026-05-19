#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/congestion_control/component.hpp>

#include <WebSocket.hpp>


int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::components::HttpClientCore>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::clients::dns::Component>();
            
            // .AppendComponentList(userver::clients::http::ComponentList())
            // .Append<userver::congestion_control::Component>()

    component_list.Append<web::WebsocketsHandler>();

    

    return userver::utils::DaemonMain(argc, argv, component_list);
}