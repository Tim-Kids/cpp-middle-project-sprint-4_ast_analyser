#pragma once
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl
{
    struct AverageAccumulator final : IAccumulator
    {
        void Accumulate(const metric::MetricResult& metric_result) override;

        void Finalize() noexcept override;

        void Reset() noexcept override;

        [[nodiscard]] metric::MetricResult::ValueType GetResult() const noexcept override;

        [[nodiscard]] double Get() const noexcept;

    private:
        int sum_ = 0;
        int count_ = 0;
        double average_ = 0;
    };
} // namespace analyser::metric_accumulator::metric_accumulator_impl
