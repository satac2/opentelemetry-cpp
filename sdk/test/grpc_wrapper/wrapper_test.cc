#include "opentelemetry/sdk/grpc_wrapper/wrapper.h"
#include "opentelemetry/trace/span_context.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sdk/test/grpc_wrapper/helloworld.grpc.pb.h"
#include "sdk/test/grpc_wrapper/helloworld_mock.grpc.pb.h"

#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

TEST(GrpcWrapperTest, creation){
    opentelemetry::trace::SpanContext testSpan = opentelemetry::trace::SpanContext(true,true);
    
    std::string header = opentelemetry::sdk::grpc::ToGrpcHeader(testSpan);
   
    ClientContext context;
    context.AddMetadata("header", header);
   
    HelloRequest request;
    HelloReply reply;
    /*
    std::shared_ptr<Channel> channel = grpc::CreateChannel(
         "127.0.0.1", grpc::InsecureChannelCredentials());
    std::unique_ptr<Greeter::Stub> stub = Greeter::NewStub(channel);
    */
    helloworld::MockGreeterStub stub;
    //MockGreeterStub stub;
    
    //EXPECT_CALL(stub, SayHello(_, _)).Times(AtLeast(1)).WillOnce(DoAll(SetArgPointee<2>(resp), Return(Status::OK)));
     
    opentelemetry::trace::SpanContext backSpan =  opentelemetry::sdk::grpc::FromGrpcHeader(header);
    
    EXPECT_TRUE(backSpan.HasRemoteParent());
    EXPECT_TRUE(backSpan.IsSampled());
}

TEST(GrpcWrapperTest, creationfalse){
    opentelemetry::trace::SpanContext testSpan = opentelemetry::trace::SpanContext(false,true);
    
    std::string header = opentelemetry::sdk::grpc::ToGrpcHeader(testSpan);
    
    opentelemetry::trace::SpanContext backSpan =  opentelemetry::sdk::grpc::FromGrpcHeader(header);
    
    EXPECT_TRUE(backSpan.HasRemoteParent());
    EXPECT_FALSE(backSpan.IsSampled());
}
