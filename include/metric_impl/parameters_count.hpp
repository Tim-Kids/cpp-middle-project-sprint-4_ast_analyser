#include "metric.hpp"

#include <string>

namespace analyser::metric::metric_impl {

struct CountParametersMetric final: IMetric {
    protected:
    [[nodiscard]] MetricResult::ValueType CalculateImpl(const function::Function& f) const override;

    [[nodiscard]] std::string Name() const noexcept override;
};

}  // namespace analyser::metric::metric_impl
