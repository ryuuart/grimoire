load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**/*"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "TracyClient",
    generate_args = [
        "-G Ninja",
    ],
    cache_entries = {
        "TRACY_ENABLE": "ON",
        "TRACY_ON_DEMAND": "ON"
    },
    lib_source = ":all_srcs",
    includes = ["public/client", "public/common", "public/tracy"],
    out_static_libs = ["libTracyClient.a"],
    out_include_dir = "include/tracy",
    visibility = ["//visibility:public"],
)

genrule(
    name = "run_tracy_profiler",
    # srcs = [":all_srcs"],
    srcs = glob(["**/*"]),
    outs = ["tracy-profiler"],
    cmd = """
        ROOT_DIR=$$PWD
        export PATH="$$PATH:/usr/local/bin:/opt/homebrew/bin"
        ORIGINAL_SOURCE_DIR=$$(dirname $(execpath @tracy//:CMakeLists.txt))

        if [ ! -d "tracy_source" ]; then
            mkdir -p tracy_source
            cp -R "$$(pwd)/$$ORIGINAL_SOURCE_DIR"/* tracy_source/
            cp -R tracy_source/dot_git tracy_source/.git
            SOURCE_DIR="$$PWD/tracy_source"
        fi

        cd $$SOURCE_DIR/profiler
        mkdir -p build
        cmake -S . -B build -G "Ninja Multi-Config" \
            -DTRACY_ENABLE=ON \
            -DCMAKE_BUILD_TYPE=Release \
            -DTRACY_NO_BROADCAST=OFF
        cmake --build build --parallel $$(nproc) --config Release

        cp build/Release/tracy-profiler $$ROOT_DIR/$(location tracy-profiler)
    """,
    executable = True,
    tags = ["no-sandbox"]
)
