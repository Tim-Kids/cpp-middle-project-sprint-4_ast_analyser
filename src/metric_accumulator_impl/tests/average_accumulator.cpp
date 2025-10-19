#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

using namespace analyser::metric;
using namespace analyser::metric_accumulator::metric_accumulator_impl;

TEST(AverageAccumulator, AccumulatesCorrectly) {
    AverageAccumulator acc;

    acc.Accumulate(MetricResult{"ParametersCount", 1});
    acc.Accumulate(MetricResult{"ParametersCount", 3});
    acc.Accumulate(MetricResult{"ParametersCount", 2});
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 2.0);
}

TEST(AverageAccumulator, HandlesEmptyInput) {
    AverageAccumulator acc;
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 0.0);
}

TEST(AverageAccumulator, ResetsProperly) {
    AverageAccumulator acc;
    acc.Accumulate(MetricResult{"ParametersCount", 10});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 10.0);

    acc.Reset();
    acc.Accumulate(MetricResult{"ParametersCount", 4});
    acc.Accumulate(MetricResult{"ParametersCount", 6});
    acc.Finalize();

    EXPECT_DOUBLE_EQ(acc.Get(), 5.0);
}

TEST(AverageAccumulator, ThrowsOnWrongType) {
    AverageAccumulator acc;
    MetricResult wrong{"ParametersCount", std::string("oops")};
    EXPECT_THROW(acc.Accumulate(wrong), std::runtime_error);
}

} // namespace analyser::metric_accumulator::metric_accumulator_impl::test
