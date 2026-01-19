//
// Created by Long Nguyen on 1/18/26.
//

#include "benchmark/benchmark.h"

static void BM_TextRender(benchmark::State& state) {
    for (auto _ : state) {

    }
}

BENCHMARK(BM_TextRender);

BENCHMARK_MAIN();
