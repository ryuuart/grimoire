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
    includes = ["include"],
    lib_source = ":all_srcs",
    out_lib_dir = select({
        "@platforms//os:linux": "lib64",
        "//conditions:default": "",
    }),
    out_shared_libs = select({
        "@platforms//os:linux": ["libSDL3.so"],
        "@platforms//os:macos": ["libSDL3.0.dylib"],
    }),
    visibility = ["//visibility:public"],
)
