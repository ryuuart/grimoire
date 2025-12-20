load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "srcs",
    srcs = glob(["src/choreograph/**/*.cpp"])
)

filegroup(
    name = "hdrs",
    srcs = glob(["src/choreograph/**/*.h"]) + glob(["src/choreograph/**/*.hpp"])
)

cc_library(
    name = "choreograph",
    srcs = [":srcs"],
    hdrs = [":hdrs"],
    includes = ["src"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"]
)
