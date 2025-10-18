#include "metric_accumulator_impl/categorical_accumulator.hpp"


namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult& metric_results) {
    if (!std::holds_alternative<std::string>(metric_results.value))
        throw std::runtime_error("CategoricalAccumulator: metric value must be string");

    const auto& category = std::get<std::string>(metric_results.value);
    ++categories_freq_[category];
    is_finalized = false;
}

void CategoricalAccumulator::Finalize() noexcept {
    is_finalized = true;
}

void CategoricalAccumulator::Reset() noexcept {
    categories_freq_.clear();
    is_finalized = false;
}

const std::unordered_map<std::string, int>& CategoricalAccumulator::Get() const noexcept {
    return categories_freq_;
}

[[nodiscard]] metric::MetricResult::ValueType CategoricalAccumulator::GetResult() const noexcept {
    std::ostringstream oss;
    for (auto it = categories_freq_.begin(); it != categories_freq_.end(); ++it) {
        if (it != categories_freq_.begin()) oss << ", ";
        oss << it->first << " = " << it->second;
    }
    return oss.str();
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
