#include <userver/utils/trivial_map.hpp>

#include <mutex>
#include <optional>
#include <unordered_map>

#include <benchmark/benchmark.h>

USERVER_NAMESPACE_BEGIN

// Compiler is too aggressive with optimizations and without this laundering
// all the utils::TrivialBiMap give an answer in less than a nanosecond.
static std::string_view MyLaunder(std::string_view value) {
  static std::mutex static_mutex{};
  static std::optional<std::string_view> static_value{};
  {
    std::lock_guard lock(static_mutex);
    static_value.emplace(std::move(value));
  }
  benchmark::ClobberMemory();
  {
    std::lock_guard lock(static_mutex);
    auto result = std::move(static_value.value());
    static_value.reset();
    return result;
  }
}

constexpr utils::TrivialBiMap kSmallTrivialBiMap = [](auto selector) {
  return selector().Case("hello", 1).Case("world", 2).Case("z", 42);
};

const auto kSmallUnorderedMapping = std::unordered_map<std::string_view, int>{
    {"hello", 1},
    {"world", 2},
    {"z", 42},
};

constexpr utils::TrivialBiMap kMediumTrivialBiMap = [](auto selector) {
  return selector()
      .Case("hello", 1)
      .Case("world", 2)
      .Case("a", 3)
      .Case("b", 4)
      .Case("c", 5)
      .Case("d", 6)
      .Case("e", 7)
      .Case("f", 8)
      .Case("z", 42);
};

const auto kMediumUnorderedMapping = std::unordered_map<std::string_view, int>{
    {"hello", 1}, {"world", 2}, {"a", 3}, {"b", 4},  {"c", 5},
    {"d", 6},     {"e", 7},     {"f", 8}, {"z", 42},
};

constexpr utils::TrivialBiMap kHugeTrivialBiMap = [](auto selector) {
  return selector()
      .Case("aaaaaaaaaaaaaaaa_hello", 1)
      .Case("aaaaaaaaaaaaaaaa_world", 2)
      .Case("aaaaaaaaaaaaaaaa_a", 3)
      .Case("aaaaaaaaaaaaaaaa_b", 4)
      .Case("aaaaaaaaaaaaaaaa_c", 5)
      .Case("aaaaaaaaaaaaaaaa_d", 6)
      .Case("aaaaaaaaaaaaaaaa_e", 7)
      .Case("aaaaaaaaaaaaaaaa_f", 8)
      .Case("aaaaaaaaaaaaaaaa_f1", 8)
      .Case("aaaaaaaaaaaaaaaa_f2", 8)
      .Case("aaaaaaaaaaaaaaaa_f3", 8)
      .Case("aaaaaaaaaaaaaaaa_f4", 8)
      .Case("aaaaaaaaaaaaaaaa_f5", 8)
      .Case("aaaaaaaaaaaaaaaa_f6", 8)
      .Case("aaaaaaaaaaaaaaaa_f7", 8)
      .Case("aaaaaaaaaaaaaaaa_f8", 8)
      .Case("aaaaaaaaaaaaaaaa_f9", 8)
      .Case("aaaaaaaaaaaaaaaa_z", 42)
      .Case("aaaaaaaaaaaaaaaa_z1", 42)
      .Case("aaaaaaaaaaaaaaaa_z2", 42)
      .Case("aaaaaaaaaaaaaaaa_z3", 42)
      .Case("aaaaaaaaaaaaaaaa_z4", 42)
      .Case("aaaaaaaaaaaaaaaa_z5", 42)
      .Case("aaaaaaaaaaaaaaaa_z6", 42)
      .Case("aaaaaaaaaaaaaaaa_z7", 42)
      .Case("aaaaaaaaaaaaaaaa_z8", 42)
      .Case("aaaaaaaaaaaaaaaa_z9", 42)
      .Case("aaaaaaaaaaaaaaaa_x", 42)
      .Case("aaaaaaaaaaaaaaaa_x1", 42)
      .Case("aaaaaaaaaaaaaaaa_x2", 42)
      .Case("aaaaaaaaaaaaaaaa_x3", 42)
      .Case("aaaaaaaaaaaaaaaa_x4", 42)
      .Case("aaaaaaaaaaaaaaaa_x5", 42)
      .Case("aaaaaaaaaaaaaaaa_x6", 42)
      .Case("aaaaaaaaaaaaaaaa_x7", 42)
      .Case("aaaaaaaaaaaaaaaa_x8", 42)
      .Case("aaaaaaaaaaaaaaaa_x9", 42);
};

const auto kHugeUnorderedMapping = std::unordered_map<std::string_view, int>{
    {"aaaaaaaaaaaaaaaa_hello", 1}, {"aaaaaaaaaaaaaaaa_world", 2},
    {"aaaaaaaaaaaaaaaa_a", 3},     {"aaaaaaaaaaaaaaaa_b", 4},
    {"aaaaaaaaaaaaaaaa_c", 5},     {"aaaaaaaaaaaaaaaa_d", 6},
    {"aaaaaaaaaaaaaaaa_e", 7},     {"aaaaaaaaaaaaaaaa_f", 8},
    {"aaaaaaaaaaaaaaaa_f1", 8},    {"aaaaaaaaaaaaaaaa_f2", 8},
    {"aaaaaaaaaaaaaaaa_f3", 8},    {"aaaaaaaaaaaaaaaa_f4", 8},
    {"aaaaaaaaaaaaaaaa_f5", 8},    {"aaaaaaaaaaaaaaaa_f6", 8},
    {"aaaaaaaaaaaaaaaa_f7", 8},    {"aaaaaaaaaaaaaaaa_f8", 8},
    {"aaaaaaaaaaaaaaaa_f9", 8},    {"aaaaaaaaaaaaaaaa_z", 42},
    {"aaaaaaaaaaaaaaaa_z1", 42},   {"aaaaaaaaaaaaaaaa_z2", 42},
    {"aaaaaaaaaaaaaaaa_z3", 42},   {"aaaaaaaaaaaaaaaa_z4", 42},
    {"aaaaaaaaaaaaaaaa_z5", 42},   {"aaaaaaaaaaaaaaaa_z6", 42},
    {"aaaaaaaaaaaaaaaa_z7", 42},   {"aaaaaaaaaaaaaaaa_z8", 42},
    {"aaaaaaaaaaaaaaaa_z9", 42},   {"aaaaaaaaaaaaaaaa_x", 42},
    {"aaaaaaaaaaaaaaaa_x1", 42},   {"aaaaaaaaaaaaaaaa_x2", 42},
    {"aaaaaaaaaaaaaaaa_x3", 42},   {"aaaaaaaaaaaaaaaa_x4", 42},
    {"aaaaaaaaaaaaaaaa_x5", 42},   {"aaaaaaaaaaaaaaaa_x6", 42},
    {"aaaaaaaaaaaaaaaa_x7", 42},   {"aaaaaaaaaaaaaaaa_x8", 42},
    {"aaaaaaaaaaaaaaaa_x9", 42},
};

void MappingSmallTrivialBiMap(benchmark::State& state) {
  auto hello = MyLaunder("hello");
  auto world = MyLaunder("world");
  auto a = MyLaunder("a");
  auto b = MyLaunder("b");
  auto c = MyLaunder("c");

  auto d = MyLaunder("d");
  auto e = MyLaunder("e");
  auto f = MyLaunder("f");
  auto z = MyLaunder("z");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(hello));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(world));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(a));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(b));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(c));

    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(d));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(e));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(f));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(z));
    benchmark::DoNotOptimize(kSmallTrivialBiMap.TryFind(z));
  }
}
BENCHMARK(MappingSmallTrivialBiMap);

void MappingSmallUnordered(benchmark::State& state) {
  auto hello = MyLaunder("hello");
  auto world = MyLaunder("world");
  auto a = MyLaunder("a");
  auto b = MyLaunder("b");
  auto c = MyLaunder("c");

  auto d = MyLaunder("d");
  auto e = MyLaunder("e");
  auto f = MyLaunder("f");
  auto z = MyLaunder("z");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(hello));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(world));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(a));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(b));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(c));

    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(d));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(e));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(f));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(z));
    benchmark::DoNotOptimize(kSmallUnorderedMapping.find(z));
  }
}
BENCHMARK(MappingSmallUnordered);

void MappingMediumTrivialBiMap(benchmark::State& state) {
  auto hello = MyLaunder("hello");
  auto world = MyLaunder("world");
  auto a = MyLaunder("a");
  auto b = MyLaunder("b");
  auto c = MyLaunder("c");

  auto d = MyLaunder("d");
  auto e = MyLaunder("e");
  auto f = MyLaunder("f");
  auto z = MyLaunder("z");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(hello));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(world));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(a));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(b));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(c));

    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(d));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(e));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(f));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(z));
    benchmark::DoNotOptimize(kMediumTrivialBiMap.TryFind(z));
  }
}
BENCHMARK(MappingMediumTrivialBiMap);

void MappingMediumUnordered(benchmark::State& state) {
  auto hello = MyLaunder("hello");
  auto world = MyLaunder("world");
  auto a = MyLaunder("a");
  auto b = MyLaunder("b");
  auto c = MyLaunder("c");

  auto d = MyLaunder("d");
  auto e = MyLaunder("e");
  auto f = MyLaunder("f");
  auto z = MyLaunder("z");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(hello));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(world));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(a));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(b));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(c));

    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(d));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(e));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(f));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(z));
    benchmark::DoNotOptimize(kMediumUnorderedMapping.find(z));
  }
}
BENCHMARK(MappingMediumUnordered);

void MappingHugeTrivialBiMap(benchmark::State& state) {
  auto hello = MyLaunder("aaaaaaaaaaaaaaaa_hello");
  auto world = MyLaunder("aaaaaaaaaaaaaaaa_world");
  auto a = MyLaunder("aaaaaaaaaaaaaaaa_a");
  auto b = MyLaunder("aaaaaaaaaaaaaaaa_b");
  auto c = MyLaunder("aaaaaaaaaaaaaaaa_c");

  auto d = MyLaunder("aaaaaaaaaaaaaaaa_d");
  auto e = MyLaunder("aaaaaaaaaaaaaaaa_e");
  auto f9 = MyLaunder("aaaaaaaaaaaaaaaa_f9");
  auto z = MyLaunder("aaaaaaaaaaaaaaaa_z");
  auto z9 = MyLaunder("aaaaaaaaaaaaaaaa_z9");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(hello));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(world));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(a));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(b));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(c));

    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(d));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(e));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(f9));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(z));
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(z9));
  }
}
BENCHMARK(MappingHugeTrivialBiMap);

void MappingHugeUnordered(benchmark::State& state) {
  auto hello = MyLaunder("aaaaaaaaaaaaaaaa_hello");
  auto world = MyLaunder("aaaaaaaaaaaaaaaa_world");
  auto a = MyLaunder("aaaaaaaaaaaaaaaa_a");
  auto b = MyLaunder("aaaaaaaaaaaaaaaa_b");
  auto c = MyLaunder("aaaaaaaaaaaaaaaa_c");

  auto d = MyLaunder("aaaaaaaaaaaaaaaa_d");
  auto e = MyLaunder("aaaaaaaaaaaaaaaa_e");
  auto f9 = MyLaunder("aaaaaaaaaaaaaaaa_f9");
  auto z = MyLaunder("aaaaaaaaaaaaaaaa_z");
  auto z9 = MyLaunder("aaaaaaaaaaaaaaaa_z9");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(hello));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(world));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(a));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(b));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(c));

    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(d));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(e));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(f9));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(z));
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(z9));
  }
}
BENCHMARK(MappingHugeUnordered);

void MappingHugeTrivialBiMapLast(benchmark::State& state) {
  auto z9 = MyLaunder("aaaaaaaaaaaaaaaa_z9");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kHugeTrivialBiMap.TryFind(z9));
  }
}
BENCHMARK(MappingHugeTrivialBiMapLast);

void MappingHugeUnorderedLast(benchmark::State& state) {
  auto z9 = MyLaunder("aaaaaaaaaaaaaaaa_z9");

  for (auto _ : state) {
    benchmark::DoNotOptimize(kHugeUnorderedMapping.find(z9));
  }
}
BENCHMARK(MappingHugeUnorderedLast);

USERVER_NAMESPACE_END
