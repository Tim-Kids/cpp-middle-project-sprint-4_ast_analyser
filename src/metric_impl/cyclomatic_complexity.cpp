#include <algorithm>

#include "metric_impl/cyclomatic_complexity.hpp"

namespace analyser::metric::metric_impl {

std::string CyclomaticComplexityMetric::Name() const noexcept {
    return "CyclomaticComplexity";
}

static constexpr std::array<std::string_view, 13> cyclomatic_complexity_metric_names = {
    "(try_statement",
    "(except_clause",
    "(finally_clause",
    "(if_statement",
    "(elif_clause",
    "alternative: (elif_clause",
    "(for_statement",
    "(while_statement",
    "(match_statement",
    "(case_clause",
    "alternative: (case_clause",
    "(conditional_expression",
    "(lambda"
};

static bool IsCyclomaticValue(std::string_view value) {
    return rs::any_of(cyclomatic_complexity_metric_names, [&](std::string_view name) {
        return value.starts_with(name);
    });
}

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function& f) const {
    auto filtered = Filter(f);
    auto c        = std::ranges::count_if(filtered, IsCyclomaticValue);
    return static_cast<int>(basic_complexity + c);
}
} // namespace analyser::metric::metric_impl
