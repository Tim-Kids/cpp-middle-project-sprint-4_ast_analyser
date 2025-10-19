#pragma once

#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "metric.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric_accumulator
{
    struct IAccumulator
    {
        virtual void Accumulate(const metric::MetricResult& metric_result) = 0;
        virtual void Finalize() noexcept = 0;
        virtual void Reset() noexcept = 0;
        [[nodiscard]] virtual metric::MetricResult::ValueType GetResult() const = 0;
        virtual ~IAccumulator() = default;

        //    protected:
        bool is_finalized = false;
    };

    struct MetricsAccumulator
    {
        // Регистрируем аггрегатор по имени метрики.
        template <typename Accumulator>
        void RegisterAccumulator(const std::string& metric_name, std::unique_ptr<Accumulator> acc)
        {
            if (!acc)
            {
                throw std::runtime_error("MetricsAccumulator: null accumulator for metric '" + metric_name + "'");
            }
            accumulators_.try_emplace(metric_name, std::move(acc));
        }

        // Вернем ссылку на завершенный аггрегатор.
        const std::unique_ptr<IAccumulator>& GetFinalizedAccumulator(const std::string& metric_name) const noexcept;

        // Аггрегируем метрики каждого Function-объекта.
        void AccumulateNextFunctionResults(const std::vector<metric::MetricResult>& metric_results) const noexcept;

        // Финализируем работу всех агрегаторов - после того, как отработаются все Function-объекты.
        void FinalizeAll();

        // Сбрасываем все агрегаторы в исходное положение.
        void ResetAccumulators();

        size_t Size() const noexcept
        {
            return accumulators_.size();
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<IAccumulator>> accumulators_;
    };
} // namespace analyser::metric_accumulator
