#include <ranges>
#include <variant>
#include <vector>
#include <memory>

#include "metric.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
    metrics_.push_back(std::move(metric));
}

MetricResults MetricExtractor::Get(const function::Function& func) const {
    return metrics_ | rv::all | rv::transform([&](auto&& up) { return up->Calculate(func); }) | rs::to<std::vector>();
}

}  // namespace analyser::metric
