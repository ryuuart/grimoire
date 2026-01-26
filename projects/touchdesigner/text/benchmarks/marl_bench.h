// Copyright 2020 The Marl Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#include "Scene.h"
#include "TextSystem.h"
#include "marl/scheduler.h"
#include "marl/thread.h"
#include "benchmark/benchmark.h"

// Define MARL_FULL_BENCHMARK to 1 if you want to run benchmarks for every
// available logical CPU core.
#ifndef MARL_FULL_BENCHMARK
#define MARL_FULL_BENCHMARK 0
#endif

class Schedule : public benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State&) {
    scene_ = std::make_unique<Scene>();
    textSystem_ = std::make_unique<TextSystem>(*scene_);
  }

  void TearDown(const ::benchmark::State&) {}

  // run() creates a scheduler using the config cfg, sets the number of worker
  // threads from the benchmark arguments, calls f, then unbinds and destructs
  // the scheduler.
  // F must be a function of the signature: void(int numTasks)
  template <typename F>
  void run(const ::benchmark::State& state,
           marl::Scheduler::Config cfg,
           F&& f) {
    cfg.setWorkerThreadCount(numThreads(state));

    marl::Scheduler scheduler(cfg);
    scheduler.bind();
    f(numTasks(state));
    scheduler.unbind();
  }

  // run() creates a scheduler, sets the number of worker threads from the
  // benchmark arguments, calls f, then unbinds and destructs the scheduler.
  // F must be a function of the signature: void(int numTasks)
  template <typename F>
  void run(const ::benchmark::State& state, F&& f) {
    run(state, marl::Scheduler::Config{}, f);
  }

  // args() sets up the benchmark to run a number of tasks over a number of
  // threads.
  // If MARL_FULL_BENCHMARK is enabled, then NumTasks tasks will be run
  // across from 0 to numLogicalCPUs worker threads.
  // If MARL_FULL_BENCHMARK is not enabled, then NumTasks tasks will be run
  // across [0 .. numLogicalCPUs] worker threads in 2^n steps.
  template <int NumTasks = 0x40000>
  static void args(benchmark::internal::Benchmark* b) {
    b->ArgNames({"tasks", "threads"});
    b->Args({NumTasks, 0});
    auto numLogicalCPUs = marl::Thread::numLogicalCPUs();
#if MARL_FULL_BENCHMARK
    for (unsigned int threads = 1U; threads <= numLogicalCPUs; threads++) {
      b->Args({NumTasks, threads});
    }
#else
    for (unsigned int threads = 1U; threads <= numLogicalCPUs; threads *= 2) {
      b->Args({NumTasks, threads});
    }
    if ((numLogicalCPUs & (numLogicalCPUs - 1)) != 0) {
      // numLogicalCPUs is not a power-of-two. Also test with numLogicalCPUs.
      b->Args({NumTasks, numLogicalCPUs});
    }
#endif
  }

  // numThreads() return the number of threads in the benchmark run from the
  // state.
  static int numThreads(const ::benchmark::State& state) {
    return static_cast<int>(state.range(1));
  }

  // numTasks() return the number of tasks in the benchmark run from the state.
  static int numTasks(const ::benchmark::State& state) {
    return static_cast<int>(state.range(0));
  }

protected:
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<TextSystem> textSystem_;
  const std::u8string BENCHMARK_PARAGRAPH_CONTENT = u8R"stress(
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
};