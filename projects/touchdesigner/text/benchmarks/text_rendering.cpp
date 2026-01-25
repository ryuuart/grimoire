//
// Created by Long Nguyen on 1/18/26.
//

#include "benchmark/benchmark.h"
#include "marl_bench.h"
#include "Scene.h"
#include "Renderer.h"
#include "marl/waitgroup.h"

BENCHMARK_DEFINE_F(Schedule, TextRender)(benchmark::State &state) {
    std::u8string benchmark_paragraph_content = u8R"stress(
The fundamental challenge of rendering begins when the Unicode Bidirectional Algorithm (UBA)
encounters forced overrides like ‪ (LRE) and ‫ (RLE) interspersed with script-heavy clusters
such as क + ् + ष = क्ष or the intricate naskh of Arabic words like ٱلْجُمْهُورِيَّةُ.
When we introduce CJK typography, we trigger **Kinsoku Shori** logic: 「這是一個測試。」
This sentence uses "hanging punctuation" which forces the engine to calculate if the closing
bracket should overflow the margin or pull the entire word to the next line. If we then mix
in **Kanji** variants (like 刃 vs 刄) and force the engine to handle **Han Unification** without a defined locale, it may default to a "fallback" font that breaks the baseline.
We then introduce heavy diacritics—lͩiͩkͩeͩ tͩhͩiͩsͩ—forcing vertical overflows, followed
by a massive sequence of Zero-Width Joiners (ZWJ) between emojis like 👩‍👩‍👧‍👦. Imagine a
line that starts in English, shifts to Hebrew (שָׁלוֹם), then forces a right-to-left
override mid-sentence: ‪This text is technically RTL but forced LTR.‬ We then saturate
the buffer with Zalgo clusters like H̵͌͝e̵͋̚l̷͒̍l̸̰̎o, forcing the renderer to stack marks,
potentially causing a "stack overflow" in the layout engine's bounds-checking logic. By
inserting **Ideographic Spaces** (　) and mixing them with thin-space characters ( ),
we prevent easy line-break optimizations. By the time the engine reaches the end of
this monstrosity—filled with ﷽ and the Cuneiform 𒀱—it must solve a multi-variable
geometric puzzle, balancing the $x$-advance of a Devanagari cluster against the
fixed-width $1em$ grid of a Chinese glyph, all while handling the contextual
substitution required by modern variable fonts.
)stress";
    scene_->text.content.content = std::string(benchmark_paragraph_content.begin(), benchmark_paragraph_content.end()) + std::string(benchmark_paragraph_content.begin(), benchmark_paragraph_content.end()) + std::string(benchmark_paragraph_content.begin(), benchmark_paragraph_content.end());
    scene_->text.style.fontSize = 1024;

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

BENCHMARK_REGISTER_F(Schedule, TextRender)->Apply(Schedule::args<1>)->Unit(benchmark::kMillisecond);
