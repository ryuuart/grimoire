//
// Created by Long Nguyen on 1/18/26.
//

#include "BitmapBuffer.h"
#include "benchmark/benchmark.h"
#include "marl_bench.h"
#include "Scene.h"
#include "Renderer.h"
#include "marl/waitgroup.h"

BENCHMARK_DEFINE_F(Schedule, TextRender)(benchmark::State &state) {
    scene_->text.content.content = std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.content.content += std::string(BENCHMARK_PARAGRAPH_CONTENT.begin(), BENCHMARK_PARAGRAPH_CONTENT.end());
    scene_->text.style.fontSize = 128;
    Renderer renderer{Renderer::RenderConfig{.initialWidth = 4096, .initialHeight = 4096}};
    BitmapBuffer buffer {renderer.imageInfo()};

    run(state, [&](int numTasks) {
        for (auto _: state) {
            textSystem_->update(renderer.imageInfo().width());

            for (auto i = 0; i < numTasks; i++) {
                if (renderer.getStatus() == Renderer::Status::RENDER_READY) {
                    renderer.startRender(buffer, *scene_);
                }
            }
        }
    });
}
BENCHMARK_REGISTER_F(Schedule, TextRender)->Apply(Schedule::args<1>)->Unit(benchmark::kMillisecond);
