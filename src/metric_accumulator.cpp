#include <vector>
#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator {

// Проходим по метрикам конкретного Function-объекта и передаем их соответствующему аггрегатору.
void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult>& metric_results) const noexcept {
    // Пропускаем метрики, к которым нет зарегистрированных аггрегаторов.
    auto filtered = metric_results | std::views::filter([&](auto&& m) { return accumulators_.contains(m.metric_name);});
    rs::for_each(filtered, [&](auto&& metric) {
        accumulators_.at(metric.metric_name)->Accumulate(metric);
    });
}

// Сбрасываем все агрегаторы в исходное положение.
void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators_, [](auto&& name_to_acc) {
        auto& [_, acc] = name_to_acc;
        acc->Reset();
        acc->is_finalized = false;
    });
}

// Вернем ссылку на завершенный аггрегатор.
const std::unique_ptr<IAccumulator>& MetricsAccumulator::GetFinalizedAccumulator(const std::string& metric_name) const noexcept {
    auto it = accumulators_.find(metric_name);
    if(it == accumulators_.end()) {
        throw std::runtime_error("MetricsAccumulator: accumulator not found for metric '" + metric_name + "'");
    }

    const auto& acc = it->second;
    if(!acc->is_finalized) {
        throw std::runtime_error("MetricsAccumulator: accumulator for '" + metric_name + "' not finalized yet");
    }

    return acc;
}

// Финализируем работу всех агрегаторов - после того, как отработаются все Function-объекты.
void MetricsAccumulator::FinalizeAll() {
    rs::for_each(accumulators_, [](auto&& name_to_acc) {
        auto& [name, acc] = name_to_acc;
        if(!acc->is_finalized) {
            acc->Finalize();
            acc->is_finalized = true;
        }
    });
}

}  // namespace analyser::metric_accumulator