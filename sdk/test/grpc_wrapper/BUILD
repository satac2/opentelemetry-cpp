load("//bazel:otel_cc_benchmark.bzl", "otel_cc_benchmark")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("@com_github_grpc_grpc//bazel:grpc_build_system.bzl", "grpc_proto_library")

grpc_proto_library(
    name = "hello_proto",
    srcs = ["helloworld.proto"],
    generate_mocks = True,
    )

#cc_proto_library(
#    name = "hello_cc_proto",
#    deps = [":hello_proto"],
#    )

#cc_grpc_library(
#    name = "hello_cc_grpc",
#    srcs = [":hello_proto"],
#    grpc_only = True,
#    deps = [":hello_cc_proto"],
#    )

cc_test(
    name = "wrapper_test",
    srcs = [
    "wrapper_test.cc",
    ],
    deps = [
    "//sdk/src/grpc_wrapper",
    "@com_google_googletest//:gtest_main",
    #":hello_cc_grpc",
    #":hello_cc_proto",
     ":hello_proto",
    ],
    )

