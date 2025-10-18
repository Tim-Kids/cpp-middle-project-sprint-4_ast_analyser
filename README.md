# 🧩 Analyser

*A modular C++20/23 static code analyzer for Python functions — built with ranges, Tree-Sitter, and GoogleTest.*

---

## 🚀 Overview

**Analyser** is a modern C++ application that performs static analysis of Python source code.  
It extracts syntax trees with Tree-Sitter and computes several function-level metrics such as  
code lines count, cyclomatic complexity, parameter count, and naming style.  

Results can be aggregated by file or class, producing average, total, or categorical summaries.

---

## 🧠 Key Features

- **Metrics**
  - Code line count (excluding comments and blanks)
  - Cyclomatic complexity
  - Function parameter count
  - Naming style (snake_case, camelCase, PascalCase, lowercase)

- **Aggregation**
  - Average and total metrics per file or class
  - Frequency analysis for categorical metrics

- **Architecture**
  - Modular metrics and accumulators
  - Modern C++ Ranges-based data flow
  - GoogleTest unit testing support

---

## ⚙️ Build Instructions

### Requirements
- CMake ≥ 3.22  
- GCC ≥ 12 / Clang ≥ 15  
- Boost.Program_options  
- Tree-Sitter CLI  
- GoogleTest  

### Build

```bash
git clone https://github.com/<your-username>/Analyser.git
cd Analyser
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

---

## 🧮 Usage

Run Analyser with a list of Python files to analyze:

```bash
./analyser file1.py file2.py ...
```

**Example output:**

```
Accumulated Analysis for file files/sample.py:
CodeLinesCount: sum=23, avg=7.6
CyclomaticComplexity: sum=8, avg=2.6
ParametersCount: avg=1.8
NamingStyle: snake_case=5, PascalCase=1
```

---

## 🧰 Extending Analyser

To add a new metric:
1. Create a header/source in `include/metric_impl/` and `src/metric_impl/`.
2. Derive from `IMetric` and override `Name()` and `CalculateImpl()`.
3. Register your metric in `main.cpp` using `MetricExtractor::RegisterMetric()`.

To add a new accumulator:
1. Create a class derived from `IAccumulator`.
2. Implement `Accumulate()`, `Finalize()`, and `GetResult()`.
3. Register it in `main.cpp` using `MetricsAccumulator::RegisterAccumulator()`.

---

## 🧪 Testing

Build and run unit tests:

```bash
cmake -S . -B build -DANALYSER_BUILD_TESTS=ON
cmake --build build -j
cd build && ctest
```

---

## 🧾 License

Released under the **MIT License**.  
See [LICENSE](LICENSE) for details.

---

## 📘 Notes

- Uses Tree-Sitter for Python AST extraction.  
- Core logic implemented using `std::ranges` and composable views.  
- A UML diagram (`UML.png`) is included in the repository root.

---

**Developed with ❤️ in Modern C++**
