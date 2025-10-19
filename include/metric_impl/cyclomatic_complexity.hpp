#pragma once

#include <string>
#include <variant>
#include <vector>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

static constexpr int basic_complexity = 1;

struct CyclomaticComplexityMetric final: IMetric {
    protected:
    [[nodiscard]] MetricResult::ValueType CalculateImpl(const function::Function& f) const override;

    [[nodiscard]] std::string Name() const noexcept override;
};

}  // namespace analyser::metric::metric_impl
