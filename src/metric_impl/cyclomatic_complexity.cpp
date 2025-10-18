#include <algorithm>

#include "metric_impl/cyclomatic_complexity.hpp"

namespace analyser::metric::metric_impl {

std::string CyclomaticComplexityMetric::Name() const noexcept {
    return "CyclomaticComplexity";
}

static bool IsCyclomaticValue(std::string_view s) {
        return
        (s.starts_with("(try_statement")
        || s.starts_with("(except_clause")
        || s.starts_with("(finally_clause")
        || s.starts_with("(if_statement")
        || s.starts_with("(elif_clause")
        || s.starts_with("alternative: (elif_clause")
        || s.starts_with("(for_statement")
        || s.starts_with("(while_statement")
        || s.starts_with("(match_statement")
        || s.starts_with("(case_clause")
        || s.starts_with("alternative: (case_clause")
        || s.starts_with("(conditional_expression")
        || s.starts_with("(lambda"));
}

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function& f) const {
    auto filtered = Filter(f);
    auto c = std::ranges::count_if(filtered, IsCyclomaticValue);
    return static_cast<int>(basic_complexity + c);
}
}  // namespace analyser::metric::metric_impl
