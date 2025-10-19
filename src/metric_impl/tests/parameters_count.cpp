#include "metric_impl/parameters_count.hpp"
#include "helpers.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

static function::Function MakeFunction(const std::string& ast) {
    function::Function func;
    func.ast = ast;
    return func;
}

TEST(ParametersCountMetric, HandlesCommentsFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::comments.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 3);
}

TEST(ParametersCountMetric, HandlesSimpleFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::simple.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 0);
}

TEST(ParametersCountMetric, HandlesIfFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::iff.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(ParametersCountMetric, HandlesLoopsFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::loops.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(ParametersCountMetric, HandlesNestedIfFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::nested_if.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 2);
}

TEST(ParametersCountMetric, HandlesManyParametersFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::many_parameters.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 2);
}

TEST(ParametersCountMetric, HandlesMatchCaseFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::match_case.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(ParametersCountMetric, HandlesExceptionsFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::exceptions.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 0);
}

TEST(ParametersCountMetric, HandlesManyLinesFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::many_lines.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 0);
}

TEST(ParametersCountMetric, HandlesTernaryFile) {
    CountParametersMetric metric;
    auto func = MakeFunction(helper::ternary.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

} // namespace analyser::metric::metric_impl
