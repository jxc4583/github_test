/* Author: Akshitha Sriraman
   Ph.D. Candidate at the University of Michigan - Ann Arbor*/

#include <iostream>
#include <memory>
#include <random>
#include <stdlib.h> 
#include <string>
#include <sys/time.h>

#include <grpc++/grpc++.h>
#include <thread>
#include <unistd.h>

#include "load_generator/helper_files/loadgen_router_client_helper.h"
#include "mid_tier_service/service/helper_files/timing.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using router::RouterRequest;
using router::LookupResponse;
using router::RouterService;

std::string ip = "localhost";
bool kill_ack = false;
std::mutex kill_ack_mutex;

class RouterServiceClient {
    public:
        explicit RouterServiceClient(std::shared_ptr<Channel> channel)
            : stub_(RouterService::NewStub(channel)) {}

        // Assembles the client's payload and sends it to the server.
        void Router(const bool kill) {
            RouterRequest router_request;
            router_request.set_kill(kill);
            // Call object to store rpc data
            AsyncClientCall* call = new AsyncClientCall;

            // stub_->AsyncSayHello() performs the RPC call, returning an instance to
            // store in "call". Because we are using the asynchronous API, we need to
            // hold on to the "call" instance in order to get updates on the ongoing RPC.
            call->response_reader = stub_->AsyncRouter(&call->context, router_request, &cq_);

            // Request that, upon completion of the RPC, "reply" be updated with the
            // server's response; "status" with the indication of whether the operation
            // was successful. Tag the request with the memory address of the call object.
            call->response_reader->Finish(&call->router_reply, &call->status, (void*)call);
        }

        // Loop while listening for completed responses.
        // Prints out the response from the server.
        void AsyncCompleteRpc() {
            void* got_tag;
            bool ok = false;
            while (cq_.Next(&got_tag, &ok)) {

                // The tag in this example is the memory location of the call object
                AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
                if (call->router_reply.kill_ack()) {
                    kill_ack = true;
                    std::cout << "got kill ack\n";
                }


                // Once we're complete, deallocate the call object.
                delete call;
            }

        }

    private:

        // struct for keeping state and data information
        struct AsyncClientCall {
            // Container for the data we expect from the server.
            LookupResponse router_reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;

            // Storage for the status of the RPC upon completion.
            Status status;


            std::unique_ptr<ClientAsyncResponseReader<LookupResponse>> response_reader;
        };

        // Out of the passed in Channel comes the stub, stored here, our view of the
        // server's exposed services.
        std::unique_ptr<RouterService::Stub> stub_;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq_;
};

void FinalKill()
{
    long int sleep_time = 50 * 1000 * 1000;
    usleep(sleep_time);
    CHECK(false, "couldn't die, so timer killed it\n");
}

int main(int argc, char** argv) {
    if (argc != 2) {
        CHECK(false, "Format: <./kill pgm> <router server IP>\n");
    }
    ip = argv[1];
    std::string ip_port = ip;
    std::cout << ip_port << std::endl;
    RouterServiceClient router(grpc::CreateChannel(
                ip_port, grpc::InsecureChannelCredentials()));
    std::thread thread_ = std::thread(&RouterServiceClient::AsyncCompleteRpc, &router);
    std::thread final_kill = std::thread(FinalKill);

    while (true) {
        std::cout << "trying to send kill\n";
        std::cout << std::flush;
        if (kill_ack) {
            std::cout << "got kill ack dying\n";
            std::cout << std::flush;
            CHECK(false, "");
        }
        std::cout << "sent kill\n";
        std::cout << std::flush;
        sleep(2);
        router.Router(true);
    }
    thread_.join();
    final_kill.join();
    return 0;
}
