#include <cstdlib>
#include <filesystem>

#include "helpers.hpp"
#include "utils.hpp"
#include "file.hpp"
#include "metric.hpp"
#include "analyse.hpp"
#include "function.hpp"
#include "cmd_options.hpp"
#include "metric_accumulator.hpp"
#include "metric_impl/metrics.hpp"
#include "metric_accumulator_impl/accumulators.hpp"

// -f comments_AST exceptions_AST if_AST loops_AST many_lines_AST many_parameters_AST match_case_AST nested_if_AST simple_AST math_utils_AST string_utils_AST

int main(int argc, char* argv[]) {
    try {
        // 1. Парсинг входных параметров.
        analyser::cmd::ProgramOptions options;

        if(!options.Parse(argc, argv)) {
            return EXIT_FAILURE;
        }

        // 2. Регистрация метрик.
        analyser::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::CountParametersMetric>());
        metric_extractor.RegisterMetric(std::make_unique<analyser::metric::metric_impl::NamingStyleMetric>());

        // 3. Синтаксический анализ функций и вывод на экран.
        auto analysis = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);
        std::println("\n================================== Per-function metrics ==================================\n");
        analyser::metric::pretty_print(analysis);


        // 4. Регистрация агрегаторов.
        analyser::metric_accumulator::MetricsAccumulator accumulator;
        accumulator.RegisterAccumulator(
            "CodeLinesCount",
            std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>());
        accumulator.RegisterAccumulator(
            "CyclomaticComplexity",
            std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>());
        accumulator.RegisterAccumulator(
            "CountParameters",
            std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::AverageAccumulator>());
        accumulator.RegisterAccumulator(
            "NamingStyle",
            std::make_unique<analyser::metric_accumulator::metric_accumulator_impl::CategoricalAccumulator>());

        // 5. Разделение по файлам и вывод на экран.
        std::println("\n=================================== Splitted by files ====================================\n");
        auto splitted_by_files = analyser::SplitByFiles(analysis);
        for (auto const& file_group : splitted_by_files) {
            const auto& filename = file_group.at(0).first.filename;
            std::println("File: {}", filename);

            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(file_group, accumulator);
            accumulator.FinalizeAll();
            analyser::metric::pretty_print(file_group);
            std::println();
        }

        // 6. Разделение по классам и вывод на экран.
        auto splitted_by_classes = analyser::SplitByClasses(analysis);
        std::println("\n============================== Global aggregated metrics by classes ======================\n");
        for (auto const& class_group : splitted_by_classes) {
            std::string_view class_name = class_group.at(0).first.class_name ? class_name = class_group.at(0).first.class_name.value() : "unknown";
            std::println("Class: {}", class_name);

            accumulator.ResetAccumulators();
            analyser::AccumulateFunctionAnalysis(class_group, accumulator);
            accumulator.FinalizeAll();
            analyser::metric::pretty_print(class_group);

            const auto& filename = class_group.at(0).first.filename;
            std::println("File: {}", filename);
            std::println();
        }

        // 6. Агрегация результатов для всех метрик.
        std::println("\n=============================== Global aggregated metrics by files =======================\n");
        for(auto const& funcs_in_file : splitted_by_files) {
            const auto& filename = funcs_in_file.at(0).first.filename;
            std::println("File: {}", filename);

            auto aggregated_by_files = analyser::AccumulateFunctionAnalysis(funcs_in_file, accumulator);

            analyser::metric::pretty_print_aggregate(aggregated_by_files);
            std::println();
        }

        std::println("\n============================= Global aggregated metrics by classes =======================\n");
        for(auto const& funcs_in_file : splitted_by_classes) {
            const auto& filename = funcs_in_file.at(0).first.filename;
            std::println("File: {}", filename);

            auto aggregated_by_files = analyser::AccumulateFunctionAnalysis(funcs_in_file, accumulator);

            analyser::metric::pretty_print_aggregate(aggregated_by_files);
            std::println();
        }

        return EXIT_SUCCESS;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
