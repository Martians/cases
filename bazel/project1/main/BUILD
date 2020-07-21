load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],

    # 增加 include 路径，这样 hello-greet.cc 中就不用写完整路径了
    #   https://docs.bazel.build/versions/1.2.0/cpp-use-cases.html#adding-include-paths
    copts = ["-Ilib"],
)

# 设置更多选项
#   copts、linkopts
#cc_library(
#    name = "main",
#    srcs = glob(
#        ["src/*.cc"],
#        exclude = ["src/gtest-all.cc"]
#    ),
#    hdrs = glob([
#        "include/**/*.h",
#        "src/*.h"
#    ]),
#    copts = ["-Iexternal/gtest/include"],
#    linkopts = ["-pthread"],
#    visibility = ["//visibility:public"],
#)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [

        # root 目录下的目标： //:target-name
        # 其他目录下的目标：   //lib:target-name
        # 当前目录下的目标：   :hello-greet
        ":hello-greet",
        "//lib:hello-time",

        # 引入工程之后，相当于也将对应工程的 root 引入到了 include path 中，代码中可以直接使用此路径
        "@from_project2//main:print",
    ],
)

cc_test(
    name = "hello-world_test",
    srcs = ["hello-world_test.cc"],
    deps = [
        ":hello-greet",
        ":hello-world",
        "@com_google_googletest//:gtest_main",
    ],
)
