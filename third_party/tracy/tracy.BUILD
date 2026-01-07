load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "tracy",
    generate_args = [
        "-G Ninja",
    ],
    lib_source = ":all_srcs",
    includes = ["include"],
    out_shared_libs = ["libTracyClient.0.so"],
    visibility = ["//visibility:public"],
)
