#include <ranges>
#include <string>
#include <fstream>
#include <string_view>

#include "metric_impl/code_lines_count.hpp"

#include "helpers.hpp"

namespace analyser::metric::metric_impl {

static bool IsCodeLine(std::string_view raw_line, bool& in_docstring_block) noexcept {
    auto ltrim = [](std::string_view s) -> std::string_view {
        const auto pos = s.find_first_not_of(" \t\r\n");
        return (pos == std::string_view::npos) ? std::string_view {} : s.substr(pos);
    };

    auto s = ltrim(raw_line);
    if(s.empty()) {
        return false;
    }

    const bool starts_doc = s.starts_with("\"\"\"") || s.starts_with("'''");
    const bool ends_doc   = s.ends_with("\"\"\"") || s.ends_with("'''");

    // Обработка """...""".
    if(starts_doc && ends_doc && s.size() > 6) {
        return false;
    }

    if(starts_doc && !ends_doc) {
        in_docstring_block = !in_docstring_block;
        return false;
    }

    if(in_docstring_block) {
        if(ends_doc) {
            in_docstring_block = false;
        }
        return false;
    }

    if(s.front() == '#') {
        return false;
    }

    return true;
}

static std::vector<std::string> ReadSourceLines(const std::string& filename) {
    std::ifstream in{filename};
    if (!in.is_open()) {
        throw std::runtime_error("CodeLinesCountMetric: cannot open file " + filename);
    }

    std::vector<std::string> lines;
    for (std::string line; std::getline(in, line);)
        lines.push_back(std::move(line));

    return lines;
}

static std::pair<int, int> FindFunctionRange(const std::vector<std::string>& lines,
                                             const std::string& func_name) {
    const std::string def_prefix = "def " + func_name;
    int start_line = -1, end_line = -1;

    // Find function definition
    for (int i = 0; i < static_cast<int>(lines.size()); ++i) {
        if (lines[i].find(def_prefix) != std::string::npos) {
            start_line = i;
            break;
        }
    }

    if (start_line == -1)
        return {-1, -1};

    // Определяем отступы.
    auto indent_level = lines[start_line].find_first_not_of(' ');
    if (indent_level == std::string::npos)
        indent_level = 0;

    // Находим конец функции.
    for (int i = start_line + 1; i < static_cast<int>(lines.size()); ++i) {
        const auto& line = lines[i];
        if (line.empty()) continue;

        auto cur_indent = line.find_first_not_of(' ');
        if (cur_indent != std::string::npos && cur_indent <= indent_level &&
            !line.starts_with('#')) {
            end_line = i - 1;
            break;
            }
    }

    if (end_line == -1)
        end_line = static_cast<int>(lines.size()) - 1;

    return {start_line + 1, end_line};
}

static std::vector<std::string_view>
ExtractFunctionLines(const std::vector<std::string>& lines,
                     int start, int end) {
    std::vector<std::string_view> func_lines;
    func_lines.reserve(end - start + 1);

    for (int i = start; i <= end && i < static_cast<int>(lines.size()); ++i)
        func_lines.emplace_back(lines[i]);

    return func_lines;
}

static int CountCodeLines(const std::vector<std::string_view>& func_lines) {
    bool in_docstring_block = false;

    return static_cast<int>(
        std::ranges::count_if(func_lines, [&](std::string_view line) mutable {
            return IsCodeLine(line, in_docstring_block);
        }));
}


MetricResult::ValueType
CodeLinesCountMetric::CalculateImpl(const function::Function& f) const {
    auto lines = ReadSourceLines(f.filename);

    const auto [start, end] = FindFunctionRange(lines, f.name);
    if (start == -1 || end == -1)
        throw std::runtime_error("CodeLinesCountMetric: cannot locate function '" +
                                 f.name + "' in file " + f.filename);

    auto func_lines = ExtractFunctionLines(lines, start, end);

    int count = CountCodeLines(func_lines);

    return count;
}


std::string CodeLinesCountMetric::Name() const noexcept {
    return "CodeLinesCount";
}
}  // namespace analyser::metric::metric_impl
