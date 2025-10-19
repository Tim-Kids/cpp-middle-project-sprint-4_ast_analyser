#pragma once

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <metric_accumulator.hpp>
#include <metric_accumulator_impl/accumulators.hpp>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser {

// Группирует функции и соотвествующие им метрики.
inline std::vector<std::pair<function::Function, metric::MetricResults>> AnalyseFunctions(const std::vector<std::string>& paths,
                      const analyser::metric::MetricExtractor& metric_extractor) {
    auto files = paths | rv::transform([](auto&& p) { return file::File {p}; }) | rs::to<std::vector>();

    auto all_functions = files | rv::transform([](auto&& f) { return function::FunctionExtractor::Get(f); }) |
                         rv::join | rs::to<std::vector>();

    auto analysed = all_functions | rv::transform([&](auto const& fn) {
                        return std::pair<function::Function, metric::MetricResults> {fn, metric_extractor.Get(fn)};
                    }) |
                    rs::to<std::vector>();

    return analysed;
}

// Отфильтровывает результат вычисления AnalyseFunctions так, чтобы в нём остались только методы классов. Разбивает
// отфильтрованную последовательность на части, в которых содержатся функции и результаты вычисления метрик,
// принадлежащие одному классу.
inline auto SplitByClasses(const auto& analysis) {
    auto result = analysis |
                  rv::filter([](auto&& func_to_metrics) { return func_to_metrics.first.class_name.has_value(); }) |
                  rv::chunk_by([](auto&& a, auto&& b) { return a.first.class_name == b.first.class_name; }) |
                  rv::transform([](auto&& group) { return group | rs::to<std::vector>(); }) | rs::to<std::vector>();
    return result;
    // returns std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>
}

// Разбивает результат вычисления AnalyseFunctions на части так, чтобы в каждой части содержались функции и результаты
// вычисления метрик, принадлежащие одному файлу.
inline auto SplitByFiles(const auto& analysis) {
    auto result = analysis | rv::chunk_by([](auto&& a, auto&& b) { return a.first.filename == b.first.filename; }) |
                  rv::transform([](auto group) { return group | rs::to<std::vector>(); }) | rs::to<std::vector>();
    return result;
    // returns std::vector<std::vector<std::pair<function::Function, metric::MetricResults>>>
}

// Для каждого элемента результата вычисления AnalyseFunctions с помощью объекта MetricsAccumulator агрегирует
// результаты вычисления метрик для каждой функции.
inline auto
AccumulateFunctionAnalysis(
    const std::vector<std::pair<function::Function, metric::MetricResults>>& funcs_to_metrics,
    analyser::metric_accumulator::MetricsAccumulator& accumulator)
    -> std::vector<std::pair<function::Function, metric::MetricResults>> {

    accumulator.ResetAccumulators();

    for (const auto& [func, metrics] : funcs_to_metrics) {
        accumulator.AccumulateNextFunctionResults(metrics);
    }

    accumulator.FinalizeAll();

    analyser::metric::MetricResults aggregated_metrics;
    aggregated_metrics.reserve(accumulator.Size());

    if (!funcs_to_metrics.empty()) {
        for (const auto& metric : funcs_to_metrics.front().second) {
            const auto& acc_ptr = accumulator.GetFinalizedAccumulator(metric.metric_name);
            aggregated_metrics.emplace_back(metric.metric_name, acc_ptr->GetResult());
        }
    }

    std::vector<std::pair<function::Function, analyser::metric::MetricResults>> aggregated{};

    if (!funcs_to_metrics.empty()) {
        aggregated.emplace_back(funcs_to_metrics.front().first, std::move(aggregated_metrics));
    }

    return aggregated;
}


}  // namespace analyser
