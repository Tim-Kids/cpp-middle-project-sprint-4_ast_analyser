#include <gtest/gtest.h>
#include <cassert>

#include "file.hpp"
#include "metric.hpp"
#include "helpers.hpp"
#include "metric_impl/code_lines_count.hpp"

namespace analyser::metric::metric_impl {

static int RunExractor(const std::string& ast, std::string_view filename) {
    auto p = helper::WriteTempPy(ast, "loc");
    analyser::file::File f {p};
    f.name     = filename;
    auto funcs = analyser::function::FunctionExtractor::Get(f);
    assert(!funcs.empty());
    CodeLinesCountMetric cl_metric {};
    int count = 0;
    for(const auto& func: funcs) {
        auto m_res = cl_metric.Calculate(func);
        count += helper::GetInt(m_res.value);
    }
    return count;
}

TEST(CodeLinesCount, simple_ast) {
    EXPECT_EQ(RunExractor(helper::simple.second, "simple.py"), 6);
}

TEST(CodeLinesCount, many_lines_ast) {
    EXPECT_EQ(RunExractor(helper::many_lines.second, "many_lines.py"), 12);
}

TEST(CodeLinesCount, comments_ast) {
    EXPECT_EQ(RunExractor(helper::comments.second, "comments.py"), 4);
}

TEST(CodeLinesCount, exceptions_ast) {
    EXPECT_EQ(RunExractor(helper::exceptions.second, "exceptions.py"), 8);
}

TEST(CodeLinesCount, if_ast) {
    EXPECT_EQ(RunExractor(helper::iff.second, "if.py"), 4);
}

TEST(CodeLinesCount, loops_ast) {
    EXPECT_EQ(RunExractor(helper::loops.second, "loops.py"), 7);
}

TEST(CodeLinesCount, match_case_ast) {
    EXPECT_EQ(RunExractor(helper::match_case.second, "match_case.py"), 8);
}

TEST(CodeLinesCount, nested_if_ast) {
    EXPECT_EQ(RunExractor(helper::nested_if.second, "nested_if.py"), 9);
}

TEST(CodeLinesCount, ternary_ast) {
    EXPECT_EQ(RunExractor(helper::ternary.second, "ternary.py"), 2);
}

}  // namespace analyser::metric::metric_impl
