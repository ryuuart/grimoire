//
// Created by Long Nguyen on 1/20/26.
//
#include "marl_bench.h"
#include "marl/sanitizers.h"

int main(int argc, char** argv) {
#if MARL_ADDRESS_SANITIZER_ENABLED
    printf(
        "***WARNING*** Marl built with address sanitizer enabled. "
        "Timings will be affected\n");
#endif
#if MARL_MEMORY_SANITIZER_ENABLED
    printf(
        "***WARNING*** Marl built with memory sanitizer enabled. "
        "Timings will be affected\n");
#endif
#if MARL_THREAD_SANITIZER_ENABLED
    printf(
        "***WARNING*** Marl built with thread sanitizer enabled. "
        "Timings will be affected\n");
#endif
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}

