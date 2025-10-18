#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <ranges>
#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult& metric_result) {
    if (!std::holds_alternative<int>(metric_result.value))
        throw std::runtime_error("SumAverageAccumulator: metric value must be integer");

    sum_ += std::get<int>(metric_result.value);
    ++count_;
    is_finalized = false;
}

void SumAverageAccumulator::Finalize() noexcept {
    average_ = count_ == 0 ? 0.0 : static_cast<double>(sum_) / static_cast<double>(count_);
    is_finalized = true;
}

void SumAverageAccumulator::Reset() noexcept {
    sum_ = 0;
    count_ = 0;
    average_ = 0.0;
    is_finalized = false;
}

[[nodiscard]] SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const noexcept {
    return SumAverage{ sum_, average_ };
}

[[nodiscard]] metric::MetricResult::ValueType SumAverageAccumulator::GetResult() const noexcept {
    auto result = Get();
    return "sum = " + std::to_string(result.sum) + ", avg = " + std::to_string(result.average);
}
}  // namespace analyser::metric_accumulator::metric_accumulator_impl
