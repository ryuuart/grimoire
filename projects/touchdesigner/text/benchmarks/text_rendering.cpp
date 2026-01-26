//
// Created by Long Nguyen on 1/18/26.
//

#include "benchmark/benchmark.h"
#include "marl_bench.h"
#include "Scene.h"
#include "Renderer.h"
#include "marl/waitgroup.h"

BENCHMARK_DEFINE_F(Schedule, TextRender)(benchmark::State &state) {
    scene_->text.content.content = std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end()) +
                                   std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end()) +
                                   std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.style.fontSize = 512;

    run(state, [&](int numTasks) {
        for (auto _: state) {
            Renderer renderer{Renderer::RenderConfig{.width = 5000, .height = 5000, .parallel = true}};
            textSystem_->update(renderer.width());
            for (auto i = 0; i < numTasks; i++) {
                renderer.render(*scene_);
            }
        }
    });
}
// BENCHMARK_REGISTER_F(Schedule, TextRender)->Apply(Schedule::args<3>)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(Schedule, TextRenderGpu)(benchmark::State &state) {
    scene_->text.content.content = std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end()) +
                                   std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end()) +
                                   std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.style.fontSize = 512;

    run(state, [&](int numTasks) {
        for (auto _: state) {
            Renderer renderer{Renderer::RenderConfig{.width = 5000, .height = 5000, .parallel = false, .useGpu = true}};
            textSystem_->update(renderer.width());
            for (auto i = 0; i < numTasks; i++) {
                renderer.render(*scene_);
            }
        }
    });
}
BENCHMARK_REGISTER_F(Schedule, TextRenderGpu)->Apply(Schedule::args<3>)->Unit(benchmark::kMillisecond);
