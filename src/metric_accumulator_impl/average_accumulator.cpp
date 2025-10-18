#include "metric_accumulator_impl/average_accumulator.hpp"

#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult& metric_result) {
    if(!std::holds_alternative<int>(metric_result.value)) {
        throw std::runtime_error("AverageAccumulator: metric value must be integer");
    }

    sum_ += std::get<int>(metric_result.value);
    ++count_;
    is_finalized = false;
}

void AverageAccumulator::Finalize() noexcept {
    average_ = count_ == 0 ? 0.0 : static_cast<double>(sum_) / static_cast<double>(count_);
    is_finalized = true;
}

void AverageAccumulator::Reset() noexcept {
    sum_         = 0;
    count_       = 0;
    average_     = 0.0;
    is_finalized = false;
}

double AverageAccumulator::Get() const noexcept {
    return average_;
}

[[nodiscard]] metric::MetricResult::ValueType AverageAccumulator::GetResult() const noexcept {
    return "avg = " + std::to_string(Get());
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
