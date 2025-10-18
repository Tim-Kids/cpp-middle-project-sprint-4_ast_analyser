#include <ranges>
#include <algorithm>
#include <string_view>

#include "metric_impl/parameters_count.hpp"

namespace analyser::metric::metric_impl {

std::string CountParametersMetric::Name() const noexcept {
    return "CountParameters";
}

static auto CountParams(const std::vector<std::string_view>& lines) {
    auto it    = rs::adjacent_find(lines, [](auto a, auto b) {
        return a.starts_with("name: (identifier") && b.starts_with("parameters: (parameters");
    });
    it         = std::next(std::next(it));
    int params = 0;
    while(it->starts_with("(identifier")) {
        ++params;
        it = std::next(it);
    }
    while(it->starts_with("(default_parameter")) {
       ++params;
        it = std::next(std::next(std::next(it)));   // Пропускаем литералы 'name' и 'value'.
    }
    while(it->starts_with("(list_splat_pattern") || it->starts_with("(dictionary_splat_pattern")) {
        ++params;
        it = std::next(std::next(it));   // Пропускаем литерал 'identifier'.
    }
    return params;
}

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function& f) const {
    auto filtered = Filter(f) | rs::to<std::vector>();
    return CountParams(filtered);
}
}  // namespace analyser::metric::metric_impl
