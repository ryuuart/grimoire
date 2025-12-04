load("@bazel_skylib//rules:common_settings.bzl", "string_list_flag")
load("@rules_cc//cc:defs.bzl", "cc_library")

string_list_flag(
    name = "imgui-backend",
    build_setting_default = ["sdlgpu3"],
)

config_setting(
    name = "metal",
    flag_values = {
        ":imgui-backend": "metal",
    },
)

config_setting(
    name = "sdlgpu3",
    flag_values = {
        ":imgui-backend": "sdlgpu3",
    },
)

filegroup(
    name = "base_srcs",
    srcs = glob([
        "*.cpp",
    ]) + ["misc/cpp/imgui_stdlib.*"],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "base_hdrs",
    srcs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "metal_srcs",
    srcs = [
        "backends/imgui_impl_metal.mm",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "metal_hdrs",
    srcs = [
        "backends/imgui_impl_metal.h",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "sdl3_srcs",
    srcs = [
        "backends/imgui_impl_sdl3.cpp",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "sdl3_hdrs",
    srcs = [
        "backends/imgui_impl_sdl3.h",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "sdlgpu3_srcs",
    srcs = [
        "backends/imgui_impl_sdlgpu3.cpp",
        ":sdl3_srcs",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "sdlgpu3_hdrs",
    srcs = [
        "backends/imgui_impl_sdlgpu3.h",
        "backends/imgui_impl_sdlgpu3_shaders.h",
        ":sdl3_hdrs",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "imgui",
    srcs = ["@imgui//:base_srcs"] +
           select({
               ":sdlgpu3": ["@imgui//:sdlgpu3_srcs"],
               "//conditions:default": [],
           }) +
           select({
               ":metal": ["@imgui//:metal_srcs"],
               "//conditions:default": [],
           }),
    hdrs = ["@imgui//:base_hdrs"] +
           select({
               ":sdlgpu3": ["@imgui//:sdlgpu3_hdrs"],
               "//conditions:default": [],
           }) +
           select({
               ":metal": ["@imgui//:metal_hdrs"],
               "//conditions:default": [],
           }),
    includes = [
        ".",
        "backends",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@sdl3",
    ],
)
