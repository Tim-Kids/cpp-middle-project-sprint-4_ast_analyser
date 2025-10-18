
#add_custom_target(unit_tests)

# Cmd_options lib building.
add_library(cmd_options cmd_options.cpp)
target_include_directories(cmd_options PUBLIC ../include)
target_link_libraries(cmd_options PUBLIC ${Boost_LIBRARIES})

# Function lib building.
add_library(function function.cpp)
target_include_directories(function PUBLIC ../include)

# File lib building.
add_library(file file.cpp)
target_include_directories(file PUBLIC ../include)

# Metric lib building.
add_library(metric
    metric.cpp
    metric_impl/code_lines_count.cpp
    metric_impl/cyclomatic_complexity.cpp
    metric_impl/parameters_count.cpp
    metric_impl/naming_style.cpp
)
target_include_directories(metric PUBLIC ../include)
target_link_libraries(metric PUBLIC function)
add_subdirectory(metric_impl)

# Metric_acumulator lib building.
add_library(metric_accumulator
    metric_accumulator.cpp
    metric_accumulator_impl/average_accumulator.cpp
    metric_accumulator_impl/sum_average_accumulator.cpp
    metric_accumulator_impl/categorical_accumulator.cpp
)
target_link_libraries(metric_accumulator PUBLIC function file)
add_subdirectory(metric_accumulator_impl)

