#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccumulator, CountsCategories) {
    CategoricalAccumulator acc;

    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("snake_case")});
    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("PascalCase")});
    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("snake_case")});
    acc.Finalize();

    const auto& freq = acc.Get();
    EXPECT_EQ(freq.at("snake_case"), 2);
    EXPECT_EQ(freq.at("PascalCase"), 1);
}

TEST(CategoricalAccumulator, HandlesEmptyInput) {
    CategoricalAccumulator acc;
    acc.Finalize();
    EXPECT_TRUE(acc.Get().empty());
}

TEST(CategoricalAccumulator, ResetsProperly) {
    CategoricalAccumulator acc;

    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("camelCase")});
    acc.Finalize();
    EXPECT_EQ(acc.Get().at("camelCase"), 1);

    acc.Reset();
    EXPECT_TRUE(acc.Get().empty());

    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("lowercase")});
    acc.Accumulate(metric::MetricResult{"NamingStyle", std::string("lowercase")});
    acc.Finalize();
    EXPECT_EQ(acc.Get().at("lowercase"), 2);
}

TEST(CategoricalAccumulator, ThrowsOnWrongType) {
    CategoricalAccumulator acc;
    metric::MetricResult wrong{"NamingStyle", 123};
    EXPECT_THROW(acc.Accumulate(wrong), std::runtime_error);
}
}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
