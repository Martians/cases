
#include <cstdlib>
#include <iostream>
#include <grpcpp/security/credentials.h>
#include <grpcpp/create_channel.h>
#include "proto/rpc.grpc.pb.h"


void
SyncRequest(std::unique_ptr<grpctest::SendServer::Stub>& stub)
{
    grpc::ClientContext context;

    grpctest::TestRequest request;
    request.set_name("client_abc");

    grpctest::TestReply reply;
    grpc::Status status = stub->SayTest(&context, request, &reply);

    if (status.ok()) {
        std::cout << "rpc success: " << reply.code() << std::endl;
    } else {
        std::cout << "rpc failed: " << status.error_message() << std::endl;
    }
}

// https://grpc.io/docs/tutorials/async/helloasync-cpp/
void
AsyncRequest(std::unique_ptr<grpctest::SendServer::Stub>& stub)
{
    grpc::ClientContext context;

    grpctest::TestRequest request;
    request.set_name("client_abc");

    grpc::CompletionQueue cq;

    // 生成请求，绑定到队列cq中
    std::unique_ptr<grpc::ClientAsyncResponseReader<grpctest::TestReply>> rpc(
            stub->AsyncSayTest(&context, request, &cq));

    grpctest::TestReply reply;
    grpc::Status status;

    //rpc->StartCall();

    // 返回请求，对应于一个指针
    rpc->Finish(&reply, &status, (void*)1);

    bool ok = false;
    void* tag;

    // 同步等待
    cq.Next(&tag, &ok);

    if (ok && tag == (void*)1) {
        std::cout << "async rpc success: " << reply.code() << std::endl;
    } else {
        std::cout << "async rpc failed: " << status.error_message() << std::endl;
    }
}

int
main(int argc, char* argv[]) {
    std::shared_ptr<::grpc::Channel> channel =
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());

    std::unique_ptr<grpctest::SendServer::Stub> stub =
            grpctest::SendServer::NewStub(channel);

    SyncRequest(stub);
    AsyncRequest(stub);
    return 0;
}