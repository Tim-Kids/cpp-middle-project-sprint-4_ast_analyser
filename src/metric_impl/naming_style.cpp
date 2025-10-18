#include <algorithm>
#include <ranges>
#include <string>
#include <variant>

#include "metric_impl/naming_style.hpp"

namespace analyser::metric::metric_impl {

static std::string DetermineNamingStyle(std::string_view name) {
    bool has_underscore = name.contains('_');
    bool has_uppercase  = rs::any_of(name, [](char c) { return std::isupper(static_cast<unsigned char>(c)); });
    bool has_lowercase  = rs::any_of(name, [](char c) { return std::islower(static_cast<unsigned char>(c)); });

    if(has_underscore && has_lowercase && !has_uppercase) {
        return "snake_case";
    }
    if(has_uppercase && has_lowercase && !has_underscore && std::isupper(name.front())) {
        return "PascalCase";
    }
    if(has_uppercase && has_lowercase && !has_underscore && std::islower(name.front())) {
        return "camelCase";
    }
    if(has_lowercase && !has_uppercase && !has_underscore) {
        return "lowercase";
    }
    if(!has_lowercase && has_uppercase && !has_underscore) {
        return "UPPERCASE";
    }
    return "unknown";
}

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function& f) const {
    return DetermineNamingStyle(f.name);
}

std::string NamingStyleMetric::Name() const noexcept {
    return "NamingStyle";
}
}  // namespace analyser::metric::metric_impl
