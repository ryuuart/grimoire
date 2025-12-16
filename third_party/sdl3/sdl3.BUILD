load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "sdl3",
    generate_args = [
        "-G Ninja",
    ],
    lib_source = ":all_srcs",
    includes = ["include"],
    out_shared_libs = ["libSDL3.0.dylib"],
    visibility = ["//visibility:public"],
)
