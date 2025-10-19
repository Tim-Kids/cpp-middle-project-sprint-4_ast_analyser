#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulator, CalculatesSumAndAverage) {
    SumAverageAccumulator acc;

    acc.Accumulate(metric::MetricResult{"CyclomaticComplexity", 2});
    acc.Accumulate(metric::MetricResult{"CyclomaticComplexity", 4});
    acc.Accumulate(metric::MetricResult{"CyclomaticComplexity", 6});
    acc.Finalize();

    auto result = acc.Get();
    EXPECT_EQ(result.sum, 12);
    EXPECT_DOUBLE_EQ(result.average, 4.0);
}

TEST(SumAverageAccumulator, HandlesEmptyInput) {
    SumAverageAccumulator acc;
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 0);
    EXPECT_DOUBLE_EQ(result.average, 0.0);
}

TEST(SumAverageAccumulator, ResetsProperly) {
    SumAverageAccumulator acc;

    acc.Accumulate(metric::MetricResult{"CodeLinesCount", 5});
    acc.Accumulate(metric::MetricResult{"CodeLinesCount", 5});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 10);
    EXPECT_DOUBLE_EQ(result.average, 5.0);

    acc.Reset();
    acc.Accumulate(metric::MetricResult{"CodeLinesCount", 3});
    acc.Accumulate(metric::MetricResult{"CodeLinesCount", 9});
    acc.Finalize();
    result = acc.Get();
    EXPECT_EQ(result.sum, 12);
    EXPECT_DOUBLE_EQ(result.average, 6.0);
}

TEST(SumAverageAccumulator, ThrowsOnWrongType) {
    SumAverageAccumulator acc;
    metric::MetricResult wrong{"CodeLinesCount", std::string("invalid")};
    EXPECT_THROW(acc.Accumulate(wrong), std::runtime_error);
}
} // namespace analyser::metric_accumulator::metric_accumulator_impl::test
