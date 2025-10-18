#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

#include "helpers.hpp"

namespace analyser::metric::metric_impl {

function::Function MakeFunction(const std::string& ast) {
    function::Function func;
    func.ast = ast;
    return func;
}

TEST(CyclomaticComplexityMetric, HandlesSimpleFunction) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::simple.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(CyclomaticComplexityMetric, HandlesIfStatement) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::iff.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 2);
}

TEST(CyclomaticComplexityMetric, HandlesLoops) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::loops.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 4);
}

TEST(CyclomaticComplexityMetric, HandlesNestedIf) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::nested_if.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 4);
}

TEST(CyclomaticComplexityMetric, HandlesMatchCase) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::match_case.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 5);
}

TEST(CyclomaticComplexityMetric, HandlesExceptions) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::exceptions.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 4);
}

TEST(CyclomaticComplexityMetric, HandlesManyParameters) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::many_parameters.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(CyclomaticComplexityMetric, HandlesTernaryOperator) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::ternary.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 3);
}

TEST(CyclomaticComplexityMetric, HandlesCommentsOnly) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::comments.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

TEST(CyclomaticComplexityMetric, HandlesManyLines) {
    CyclomaticComplexityMetric metric;
    auto func = MakeFunction(helper::many_lines.second);
    int value = std::get<int>(metric.Calculate(func).value);
    EXPECT_EQ(value, 1);
}

}  // namespace analyser::metric::metric_impl
