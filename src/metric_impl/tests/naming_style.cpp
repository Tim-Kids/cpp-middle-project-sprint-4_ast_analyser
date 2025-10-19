#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

static function::Function MakeFunctionWithName(const std::string& name) {
    function::Function f;
    f.name     = name;
    f.filename = "dummy.py";
    return f;
}

TEST(NamingStyleMetric, DetectsSnakeCase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("process_data");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "snake_case");
}

TEST(NamingStyleMetric, DetectsCamelCase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("processData");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "camelCase");
}

TEST(NamingStyleMetric, DetectsPascalCase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("ProcessData");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "PascalCase");
}

TEST(NamingStyleMetric, DetectsLowercase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("process");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "lowercase");
}

TEST(NamingStyleMetric, DetectsUppercase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("PROCESS");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "UPPERCASE");
}

TEST(NamingStyleMetric, DetectsMagicSnakeCase) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("__init__");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "snake_case"); // underscores + lowercase
}

TEST(NamingStyleMetric, DetectsUnknownMixedUnderscoreAndCaps) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("Test_function");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "unknown"); // both underscore and caps
}

TEST(NamingStyleMetric, DetectsUnknownMixedRandom) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("teSt_FuNc");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "unknown");
}

TEST(NamingStyleMetric, HandlesEmptyName) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "unknown");
}

TEST(NamingStyleMetric, HandlesSingleLetter) {
    NamingStyleMetric metric;
    auto func         = MakeFunctionWithName("x");
    std::string style = std::get<std::string>(metric.Calculate(func).value);
    EXPECT_EQ(style, "lowercase");
}

} // namespace analyser::metric::metric_impl
