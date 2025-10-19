#pragma once

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl
{
    struct CategoricalAccumulator final : IAccumulator
    {
        void Accumulate(const metric::MetricResult& metric_result) override;

        void Finalize() noexcept override;

        void Reset() noexcept override;

        const std::unordered_map<std::string, int>& Get() const noexcept;

        [[nodiscard]] metric::MetricResult::ValueType GetResult() const noexcept override;

    private:
        std::unordered_map<std::string, int> categories_freq_;
    };
} // namespace analyser::metric_accumulator::metric_accumulator_impl
