#pragma once

#include <string>
#include <variant>
#include <vector>
#include <print>
#include <filesystem>

#include "function.hpp"

namespace fs = std::filesystem;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric
{
    struct MetricResult
    {
        using ValueType = std::variant<int, std::string>;
        std::string metric_name; // Название метрики.
        ValueType value; // Значение метрики.
    };

    struct IMetric
    {
        virtual ~IMetric() = default;

        [[nodiscard]] MetricResult Calculate(const function::Function& f) const
        {
            return MetricResult{.metric_name = Name(), .value = CalculateImpl(f)};
        }

    protected:
        [[nodiscard]] virtual MetricResult::ValueType CalculateImpl(const function::Function& f) const = 0;

        [[nodiscard]] virtual std::string Name() const noexcept = 0;
    };

    using MetricResults = std::vector<MetricResult>;

    struct MetricExtractor
    {
        void RegisterMetric(std::unique_ptr<IMetric> metric);

        [[nodiscard]] MetricResults Get(const function::Function& func) const;

    private:
        std::vector<std::unique_ptr<IMetric>> metrics_;
    };

    inline static bool NotBlankOrComment(std::string_view s)
    {
        // Пропускаем пробелы до начала элементов узлов дерева.
        auto dropped_ws = s | rv::drop_while([](auto ch) { return isspace(ch); }) | rs::to<std::string>();
        // Пропускаем пустые строки.
        if (dropped_ws.empty())
        {
            return false;
        }
        // Пропускаем комментарии.
        return !(dropped_ws.starts_with("(string") || dropped_ws.starts_with("(comment"));
    };

    // Делит файл построчно и убирает пробелы в начале каждой строки. Убирает пустые и строки с комментариями.

    inline static auto Filter(const function::Function& f)
    {
        return f.ast | rv::split('\n') | rv::transform([](auto&& r) { return std::string_view(r.begin(), r.end()); }) |
            rv::transform([](auto&& s) -> std::string_view
            {
                const auto pos = s.find_first_not_of(" \t\r\n");
                return (pos == std::string_view::npos) ? std::string_view{} : s.substr(pos);
            }) |
            rv::filter(NotBlankOrComment);
    }

    // -------------------------- pretty-prints --------------------------

    template <typename T>
    inline auto pretty_print(const std::vector<T>& func_to_metrics)
    {
    }

    template <>
    inline auto
    pretty_print(const std::vector<std::pair<function::Function, MetricResults>>& func_to_metrics)
    {
        for (const auto& [func, metrics] : func_to_metrics)
        {
            std::println("---------------------------------------------");
            std::println("Function: {}", func.name);
            for (const auto& m : metrics)
            {
                if (std::holds_alternative<int>(m.value))
                {
                    std::println("{}, {}", m.metric_name, std::get<int>(m.value));
                }
                else if (std::holds_alternative<std::string>(m.value))
                {
                    std::println("{}, {}", m.metric_name, std::get<std::string>(m.value));
                }
            }
        }
    };

    inline auto
    pretty_print_aggregate(const std::vector<std::pair<function::Function, MetricResults>>& func_to_metrics)
    {
        for (const auto& [func, metrics] : func_to_metrics)
        {
            std::println("---------------------------------------------");
            for (const auto& m : metrics)
            {
                if (std::holds_alternative<int>(m.value))
                {
                    std::println("{}, {}", m.metric_name, std::get<int>(m.value));
                }
                else if (std::holds_alternative<std::string>(m.value))
                {
                    std::println("{}, {}", m.metric_name, std::get<std::string>(m.value));
                }
            }
        }
    };

    template <>
    inline auto
    pretty_print(
        const std::vector<std::vector<std::pair<function::Function, MetricResults>>>& files_to_funcs_and_metrics)
    {
        for (const auto& file_to_data : files_to_funcs_and_metrics)
        {
            std::println("---------------------------------------------");
            for (const auto& data : file_to_data)
            {
                for (const auto& [func, metric_results] : file_to_data)
                {
                    std::println("File: {}", func.filename);
                    std::println("Function: {}", func.name);
                    for (const auto& metric : metric_results)
                    {
                        if (std::holds_alternative<int>(metric.value))
                        {
                            std::println("{}, {}", metric.metric_name, std::get<int>(metric.value));
                        }
                        else if (std::holds_alternative<std::string>(metric.value))
                        {
                            std::println("{}, {}", metric.metric_name,
                                         std::get<std::string>(metric.value));
                        }
                    }
                }
            }
        }
    }
} // namespace analyser::metric
