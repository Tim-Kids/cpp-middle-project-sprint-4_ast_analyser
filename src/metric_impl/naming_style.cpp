#include <algorithm>
#include <ranges>
#include <string>
#include <variant>

#include "metric_impl/naming_style.hpp"

namespace analyser::metric::metric_impl {

static std::string DetermineNamingStyle(std::string_view name) {
    const bool underscore = name.contains('_');
    const bool upper      = rs::any_of(name, [](unsigned char c) {
        return std::isupper(c);
    });
    const bool lower = rs::any_of(name, [](unsigned char c) {
        return std::islower(c);
    });

    using namespace std::string_literals;

    if(underscore)
        return (lower && !upper) ? "snake_case"s : "unknown"s;

    if(upper && lower)
        return std::isupper(static_cast<unsigned char>(name.front())) ? "PascalCase"s : "camelCase"s;

    if(lower && !upper)
        return "lowercase"s;

    if(upper && !lower)
        return "UPPERCASE"s;

    return "unknown"s;
}

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function& f) const {
    return DetermineNamingStyle(f.name);
}

std::string NamingStyleMetric::Name() const noexcept {
    return "NamingStyle";
}
} // namespace analyser::metric::metric_impl
