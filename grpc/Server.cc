

#include <grpcpp/grpcpp.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include "proto/rpc.grpc.pb.h"

/**
 * 编译：
     protoc --cpp_out=. proto/rpc.proto
     protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/rpc.proto
 *
 * api: https://grpc.github.io/grpc/cpp/index.html
 *
 *
 * 元数据集
 *
 **/
using namespace grpctest;

class TestImpl final : public grpctest::SendServer::Service {
    virtual ::grpc::Status SayTest(::grpc::ServerContext* context,
            const ::grpctest::TestRequest* request,
            ::grpctest::TestReply* response) override
    {
        std::cout << "get name: " << request->name() << std::endl;
        response->set_code(100);
        return ::grpc::Status::OK;
    }
};

class RequestVoke {
public:
    RequestVoke(grpctest::SendServer::AsyncService* service, grpc::ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_) { Work(); }

    // 相当于 event loop
    void Work() {
        switch(state) {
            case 0:
                // 在asyc server上进行注册，相当于监听？
                service_->RequestSayTest(&ctx_, &request_, &responder_, cq_, cq_, this);
                break;

            case 1:
                // 生成另一个实例进行等待，当前实例只执行当前命令了
                new RequestVoke(service_, cq_);

                std::cout << "get name: " << request_.name() << std::endl;
                reply_.set_code(100);

                // 将response返回出去
                responder_.Finish(reply_, grpc::Status::OK, this);
                break;

            case 2:
                // 需要访问内部成员（reply等），必须回应完成之后再进行 delete
                std::cout << "delete" << std::endl;
                delete this;
                break;

            default:
                assert(0);
                break;
        }
        state++;
    }

private:
    int state = 0;
    grpctest::SendServer::AsyncService* service_;
    grpc::ServerCompletionQueue* cq_;

    grpc::ServerContext ctx_;
    grpc::ServerAsyncResponseWriter<TestReply> responder_;

    grpctest::TestRequest request_;
    grpctest::TestReply  reply_;

};

int
main(int argc, char* argv[]) {
    std::string server_address("0.0.0.0:50051");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    bool async = true;
    if (async) {
        auto cq = builder.AddCompletionQueue();
        grpctest::SendServer::AsyncService service;
        builder.RegisterService(&service);

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        new RequestVoke(&service, cq.get());
        void* tag;
        bool ok;

        while (true) {
            cq->Next(&tag, &ok);
            static_cast<RequestVoke*>(tag)->Work();
        }

    } else {
        TestImpl impl;
        builder.RegisterService(&impl);

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        server->Wait();
    }

    return 0;
}