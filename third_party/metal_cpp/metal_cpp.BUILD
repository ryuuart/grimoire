load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "hdrs",
    srcs = glob(["**/*.hpp"])
)

cc_library(
    name = "metal_cpp",
    hdrs = [":hdrs"],
    linkopts = [
        "-framework Foundation",
        "-framework QuartzCore",
        "-framework Metal",
    ],
    visibility = ["//visibility:public"]
)
