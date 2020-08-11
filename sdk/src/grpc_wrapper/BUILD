load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

package(default_visibility = ["//visibility:public"])
cc_library(
    name = "grpc_wrapper",
    srcs = glob(["**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    include_prefix = "src/grpc_wrapper",
    deps = [
    "//api",
    "//sdk:headers",
    "@com_github_grpc_grpc//:grpc++",
    ],
    )
