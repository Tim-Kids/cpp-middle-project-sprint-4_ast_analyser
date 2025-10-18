#pragma once

#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <variant>
#include <type_traits>
#include <algorithm>
#include <ranges>

namespace helper {

inline std::string MakePyFileName(const std::string& path) {
    auto name = path;
    constexpr std::string_view suffix = "_AST";
    std::string name_py{};
    if(name.ends_with(suffix)) {
        name_py = name.erase(path.size() - suffix.size()) + ".py";
    }
    else {
        throw std::runtime_error("No file name with \"AST\" given");
    }
    return name_py;
}

inline std::filesystem::path WriteTempPy(std::string_view code, std::string_view name_hint = "snippet") {
    auto tmp = std::filesystem::temp_directory_path() /
               std::filesystem::path(std::string("analyser_") + std::string(name_hint) + ".py");
    std::ofstream out(tmp);
    out << code;
    out.close();
    return tmp;
}

template<class T> int GetInt(const T& metricResultValue) {
    if constexpr(std::is_same_v<T, int>) {
        return metricResultValue;
    }
    else {
        return std::get<int>(metricResultValue);
    }
}

template<class T> std::string GetStr(const T& metricResultValue) {
    if constexpr(std::is_same_v<T, std::string>) {
        return metricResultValue;
    }
    else {
        return std::get<std::string>(metricResultValue);
    }
}

inline const std::pair<std::string, std::string> many_parameters {
    "def __test_multiparameters__(a, b, c=5, *args, **kwargs):\n"
    "    assert a + b == c",

    "(module [0, 0] - [1, 21]\n"
    "  (function_definition [0, 0] - [1, 21]\n"
    "    name: (identifier [0, 4] - [0, 28])\n"
    "    parameters: (parameters [0, 28] - [0, 56]\n"
    "      (identifier [0, 29] - [0, 30])\n"
    "      (identifier [0, 32] - [0, 33])\n"
    "      (default_parameter [0, 35] - [0, 38]\n"
    "        name: (identifier [0, 35] - [0, 36])\n"
    "        value: (integer [0, 37] - [0, 38]))\n"
    "      (list_splat_pattern [0, 40] - [0, 45]\n"
    "        (identifier [0, 41] - [0, 45]))\n"
    "      (dictionary_splat_pattern [0, 47] - [0, 55]\n"
    "        (identifier [0, 49] - [0, 55])))\n"
    "    body: (block [1, 4] - [1, 21]\n"
    "      (assert_statement [1, 4] - [1, 21]\n"
    "        (comparison_operator [1, 11] - [1, 21]\n"
    "          (binary_operator [1, 11] - [1, 16]\n"
    "            left: (identifier [1, 11] - [1, 12])\n"
    "            right: (identifier [1, 15] - [1, 16]))\n"
    "          (identifier [1, 20] - [1, 21]))))))"};

inline const std::pair<std::string, std::string> simple {"def test_simple():\n"
                                                         "    x = 1\n"
                                                         "    y = 2\n"
                                                         "    z = x + y\n"
                                                         "    print(z)\n"
                                                         "\n"
                                                         "    assert z == 3",

                                                         "(module [0, 0] - [6, 17]\n"
                                                         "  (function_definition [0, 0] - [6, 17]\n"
                                                         "    name: (identifier [0, 4] - [0, 15])\n"
                                                         "    parameters: (parameters [0, 15] - [0, 17])\n"
                                                         "    body: (block [1, 4] - [6, 17]\n"
                                                         "      (assignment [1, 4] - [1, 9]\n"
                                                         "        left: (identifier [1, 4] - [1, 5])\n"
                                                         "        right: (integer [1, 8] - [1, 9]))\n"
                                                         "      (assignment [2, 4] - [2, 9]\n"
                                                         "        left: (identifier [2, 4] - [2, 5])\n"
                                                         "        right: (integer [2, 8] - [2, 9]))\n"
                                                         "      (assignment [3, 4] - [3, 13]\n"
                                                         "        left: (identifier [3, 4] - [3, 5])\n"
                                                         "        right: (binary_operator [3, 8] - [3, 13]\n"
                                                         "          left: (identifier [3, 8] - [3, 9])\n"
                                                         "          right: (identifier [3, 12] - [3, 13])))\n"
                                                         "      (call [4, 4] - [4, 12]\n"
                                                         "        function: (identifier [4, 4] - [4, 9])\n"
                                                         "        arguments: (argument_list [4, 9] - [4, 12]\n"
                                                         "          (identifier [4, 10] - [4, 11])))\n"
                                                         "      (assert_statement [6, 4] - [6, 17]\n"
                                                         "        (comparison_operator [6, 11] - [6, 17]\n"
                                                         "          (identifier [6, 11] - [6, 12])\n"
                                                         "          (integer [6, 16] - [6, 17]))))))"};

inline const std::pair<std::string, std::string> many_lines {"def testmultiline():\n"
                                                             "    data = [\n"
                                                             "        1, 2, 3,\n"
                                                             "        4, 5, 6\n"
                                                             "    ]\n"
                                                             "    total = sum(data)\n"
                                                             "    \n"
                                                             "    print(\"Больше 10\")\n"
                                                             "    print(\"Меньше или равно 10\")\n"
                                                             "        \n"
                                                             "    call_unexisting_function1(total)\n"
                                                             "    call_unexisting_function2(total)\n"
                                                             "    call_unexisting_function3(total)\n"
                                                             "\n"
                                                             "    assert total == 6",

                                                             "(module [0, 0] - [14, 21]\n"
                                                             "  (function_definition [0, 0] - [14, 21]\n"
                                                             "    name: (identifier [0, 4] - [0, 17])\n"
                                                             "    parameters: (parameters [0, 17] - [0, 19])\n"
                                                             "    body: (block [1, 4] - [14, 21]\n"
                                                             "      (assignment [1, 4] - [4, 5]\n"
                                                             "        left: (identifier [1, 4] - [1, 8])\n"
                                                             "        right: (list [1, 11] - [4, 5]\n"
                                                             "          (integer [2, 8] - [2, 9])\n"
                                                             "          (integer [2, 11] - [2, 12])\n"
                                                             "          (integer [2, 14] - [2, 15])\n"
                                                             "          (integer [3, 8] - [3, 9])\n"
                                                             "          (integer [3, 11] - [3, 12])\n"
                                                             "          (integer [3, 14] - [3, 15])))\n"
                                                             "      (assignment [5, 4] - [5, 21]\n"
                                                             "        left: (identifier [5, 4] - [5, 9])\n"
                                                             "        right: (call [5, 12] - [5, 21]\n"
                                                             "          function: (identifier [5, 12] - [5, 15])\n"
                                                             "          arguments: (argument_list [5, 15] - [5, 21]\n"
                                                             "            (identifier [5, 16] - [5, 20]))))\n"
                                                             "      (call [7, 4] - [7, 28]\n"
                                                             "        function: (identifier [7, 4] - [7, 9])\n"
                                                             "        arguments: (argument_list [7, 9] - [7, 28]\n"
                                                             "          (string [7, 10] - [7, 27]\n"
                                                             "            (string_start [7, 10] - [7, 11])\n"
                                                             "            (string_content [7, 11] - [7, 26])\n"
                                                             "            (string_end [7, 26] - [7, 27]))))\n"
                                                             "      (call [8, 4] - [8, 46]\n"
                                                             "        function: (identifier [8, 4] - [8, 9])\n"
                                                             "        arguments: (argument_list [8, 9] - [8, 46]\n"
                                                             "          (string [8, 10] - [8, 45]\n"
                                                             "            (string_start [8, 10] - [8, 11])\n"
                                                             "            (string_content [8, 11] - [8, 44])\n"
                                                             "            (string_end [8, 44] - [8, 45]))))\n"
                                                             "      (call [10, 4] - [10, 36]\n"
                                                             "        function: (identifier [10, 4] - [10, 29])\n"
                                                             "        arguments: (argument_list [10, 29] - [10, 36]\n"
                                                             "          (identifier [10, 30] - [10, 35])))\n"
                                                             "      (call [11, 4] - [11, 36]\n"
                                                             "        function: (identifier [11, 4] - [11, 29])\n"
                                                             "        arguments: (argument_list [11, 29] - [11, 36]\n"
                                                             "          (identifier [11, 30] - [11, 35])))\n"
                                                             "      (call [12, 4] - [12, 36]\n"
                                                             "        function: (identifier [12, 4] - [12, 29])\n"
                                                             "        arguments: (argument_list [12, 29] - [12, 36]\n"
                                                             "          (identifier [12, 30] - [12, 35])))\n"
                                                             "      (assert_statement [14, 4] - [14, 21]\n"
                                                             "        (comparison_operator [14, 11] - [14, 21]\n"
                                                             "          (identifier [14, 11] - [14, 16])\n"
                                                             "          (integer [14, 20] - [14, 21]))))))"};

inline const std::pair<std::string, std::string> comments {"def Func_comments(result, a, b):\n"
                                                           "    # Это комментарий\n"
                                                           "    a = 10\n"
                                                           "    # Это ещё комментарий\n"
                                                           "    # Это ещё комментарий\n"
                                                           "    b = 20\n"
                                                           "    result = a + b",

                                                           "(module [0, 0] - [6, 18]\n"
                                                           "  (function_definition [0, 0] - [6, 18]\n"
                                                           "    name: (identifier [0, 4] - [0, 17])\n"
                                                           "    parameters: (parameters [0, 17] - [0, 31]\n"
                                                           "      (identifier [0, 18] - [0, 24])\n"
                                                           "      (identifier [0, 26] - [0, 27])\n"
                                                           "      (identifier [0, 29] - [0, 30]))\n"
                                                           "    (comment [1, 4] - [1, 35])\n"
                                                           "    body: (block [2, 4] - [6, 18]\n"
                                                           "      (assignment [2, 4] - [2, 10]\n"
                                                           "        left: (identifier [2, 4] - [2, 5])\n"
                                                           "        right: (integer [2, 8] - [2, 10]))\n"
                                                           "      (comment [3, 4] - [3, 42])\n"
                                                           "      (comment [4, 4] - [4, 42])\n"
                                                           "      (assignment [5, 4] - [5, 10]\n"
                                                           "        left: (identifier [5, 4] - [5, 5])\n"
                                                           "        right: (integer [5, 8] - [5, 10]))\n"
                                                           "      (assignment [6, 4] - [6, 18]\n"
                                                           "        left: (identifier [6, 4] - [6, 10])\n"
                                                           "        right: (binary_operator [6, 13] - [6, 18]\n"
                                                           "          left: (identifier [6, 13] - [6, 14])\n"
                                                           "          right: (identifier [6, 17] - [6, 18]))))))"};

inline const std::pair<std::string, std::string> exceptions {
    "def Try_Exceptions():\n"
    "    try:\n"
    "        x = 1 / 0\n"
    "        assert x == NaN\n"
    "    except ZeroDivisionError:\n"
    "        print(\"Ошибка\")\n"
    "    finally:\n"
    "        print(\"Завершено\")",

    "(module [0, 0] - [8, 0]\n"
    "  (function_definition [0, 0] - [7, 35]\n"
    "    name: (identifier [0, 4] - [0, 18])\n"
    "    parameters: (parameters [0, 18] - [0, 20])\n"
    "    body: (block [1, 4] - [7, 35]\n"
    "      (try_statement [1, 4] - [7, 35]\n"
    "        body: (block [2, 8] - [3, 23]\n"
    "          (assignment [2, 8] - [2, 17]\n"
    "            left: (identifier [2, 8] - [2, 9])\n"
    "            right: (binary_operator [2, 12] - [2, 17]\n"
    "              left: (integer [2, 12] - [2, 13])\n"
    "              right: (integer [2, 16] - [2, 17])))\n"
    "          (assert_statement [3, 8] - [3, 23]\n"
    "            (comparison_operator [3, 15] - [3, 23]\n"
    "              (identifier [3, 15] - [3, 16])\n"
    "              (identifier [3, 20] - [3, 23]))))\n"
    "        (except_clause [4, 4] - [5, 29]\n"
    "          value: (identifier [4, 11] - [4, 28])\n"
    "          (block [5, 8] - [5, 29]\n"
    "            (call [5, 8] - [5, 29]\n"
    "              function: (identifier [5, 8] - [5, 13])\n"
    "              arguments: (argument_list [5, 13] - [5, 29]\n"
    "                (string [5, 14] - [5, 28]\n"
    "                  (string_start [5, 14] - [5, 15])\n"
    "                  (string_content [5, 15] - [5, 27])\n"
    "                  (string_end [5, 27] - [5, 28]))))))\n"
    "        (finally_clause [6, 4] - [7, 35]\n"
    "          (block [7, 8] - [7, 35]\n"
    "            (call [7, 8] - [7, 35]\n"
    "              function: (identifier [7, 8] - [7, 13])\n"
    "              arguments: (argument_list [7, 13] - [7, 35]\n"
    "                (string [7, 14] - [7, 34]\n"
    "                  (string_start [7, 14] - [7, 15])\n"
    "                  (string_content [7, 15] - [7, 33])\n"
    "                  (string_end [7, 33] - [7, 34]))))))))))"};

inline const std::pair<std::string, std::string> iff {"def testIf(x):\n"
                                                      "    if x > 0:\n"
                                                      "        return True\n"
                                                      "    return False",

                                                      "(module [0, 0] - [3, 16]\n"
                                                      "  (function_definition [0, 0] - [3, 16]\n"
                                                      "    name: (identifier [0, 4] - [0, 10])\n"
                                                      "    parameters: (parameters [0, 10] - [0, 13]\n"
                                                      "      (identifier [0, 11] - [0, 12]))\n"
                                                      "    body: (block [1, 4] - [3, 16]\n"
                                                      "      (if_statement [1, 4] - [2, 19]\n"
                                                      "        condition: (comparison_operator [1, 7] - [1, 12]\n"
                                                      "          (identifier [1, 7] - [1, 8])\n"
                                                      "          (integer [1, 11] - [1, 12]))\n"
                                                      "        consequence: (block [2, 8] - [2, 19]\n"
                                                      "          (return_statement [2, 8] - [2, 19]\n"
                                                      "            (true [2, 15] - [2, 19]))))\n"
                                                      "      (return_statement [3, 4] - [3, 16]\n"
                                                      "        (false [3, 11] - [3, 16])))))"};

inline const std::pair<std::string, std::string> loops {
    "def TestLoops(n):\n"
    "    for i in range(n):\n"
    "        while (i < n):\n"
    "            if i % 2 == 0:\n"
    "                print(i)\n"
    "            i += 1\n"
    "    return True",

    "(module [0, 0] - [6, 15]\n"
    "  (function_definition [0, 0] - [6, 15]\n"
    "    name: (identifier [0, 4] - [0, 13])\n"
    "    parameters: (parameters [0, 13] - [0, 16]\n"
    "      (identifier [0, 14] - [0, 15]))\n"
    "    body: (block [1, 4] - [6, 15]\n"
    "      (for_statement [1, 4] - [5, 18]\n"
    "        left: (identifier [1, 8] - [1, 9])\n"
    "        right: (call [1, 13] - [1, 21]\n"
    "          function: (identifier [1, 13] - [1, 18])\n"
    "          arguments: (argument_list [1, 18] - [1, 21]\n"
    "            (identifier [1, 19] - [1, 20])))\n"
    "        body: (block [2, 8] - [5, 18]\n"
    "          (while_statement [2, 8] - [5, 18]\n"
    "            condition: (parenthesized_expression [2, 14] - [2, 21]\n"
    "              (comparison_operator [2, 15] - [2, 20]\n"
    "                (identifier [2, 15] - [2, 16])\n"
    "                (identifier [2, 19] - [2, 20])))\n"
    "            body: (block [3, 12] - [5, 18]\n"
    "              (if_statement [3, 12] - [4, 24]\n"
    "                condition: (comparison_operator [3, 15] - [3, 25]\n"
    "                  (binary_operator [3, 15] - [3, 20]\n"
    "                    left: (identifier [3, 15] - [3, 16])\n"
    "                    right: (integer [3, 19] - [3, 20]))\n"
    "                  (integer [3, 24] - [3, 25]))\n"
    "                consequence: (block [4, 16] - [4, 24]\n"
    "                  (call [4, 16] - [4, 24]\n"
    "                    function: (identifier [4, 16] - [4, 21])\n"
    "                    arguments: (argument_list [4, 21] - [4, 24]\n"
    "                      (identifier [4, 22] - [4, 23])))))\n"
    "              (augmented_assignment [5, 12] - [5, 18]\n"
    "                left: (identifier [5, 12] - [5, 13])\n"
    "                right: (integer [5, 17] - [5, 18]))))))\n"
    "      (return_statement [6, 4] - [6, 15]\n"
    "        (true [6, 11] - [6, 15])))))"};

inline const std::pair<std::string, std::string> match_case {
    "def test_Match_case(x):\n"
    "    match x:\n"
    "        case 1:\n"
    "            return \"one\"\n"
    "        case 2:\n"
    "            return \"two\"\n"
    "        case _:\n"
    "            return \"many\"",

    "(module [0, 0] - [7, 25]\n"
    "  (function_definition [0, 0] - [7, 25]\n"
    "    name: (identifier [0, 4] - [0, 19])\n"
    "    parameters: (parameters [0, 19] - [0, 22]\n"
    "      (identifier [0, 20] - [0, 21]))\n"
    "    body: (block [1, 4] - [7, 25]\n"
    "      (match_statement [1, 4] - [7, 25]\n"
    "        subject: (identifier [1, 10] - [1, 11])\n"
    "        body: (block [1, 12] - [7, 25]\n"
    "          alternative: (case_clause [2, 8] - [3, 24]\n"
    "            (case_pattern [2, 13] - [2, 14]\n"
    "              (integer [2, 13] - [2, 14]))\n"
    "            consequence: (block [3, 12] - [3, 24]\n"
    "              (return_statement [3, 12] - [3, 24]\n"
    "                (string [3, 19] - [3, 24]\n"
    "                  (string_start [3, 19] - [3, 20])\n"
    "                  (string_content [3, 20] - [3, 23])\n"
    "                  (string_end [3, 23] - [3, 24])))))\n"
    "          alternative: (case_clause [4, 8] - [5, 24]\n"
    "            (case_pattern [4, 13] - [4, 14]\n"
    "              (integer [4, 13] - [4, 14]))\n"
    "            consequence: (block [5, 12] - [5, 24]\n"
    "              (return_statement [5, 12] - [5, 24]\n"
    "                (string [5, 19] - [5, 24]\n"
    "                  (string_start [5, 19] - [5, 20])\n"
    "                  (string_content [5, 20] - [5, 23])\n"
    "                  (string_end [5, 23] - [5, 24])))))\n"
    "          alternative: (case_clause [6, 8] - [7, 25]\n"
    "            (case_pattern [6, 13] - [6, 14])\n"
    "            consequence: (block [7, 12] - [7, 25]\n"
    "              (return_statement [7, 12] - [7, 25]\n"
    "                (string [7, 19] - [7, 25]\n"
    "                  (string_start [7, 19] - [7, 20])\n"
    "                  (string_content [7, 20] - [7, 24])\n"
    "                  (string_end [7, 24] - [7, 25]))))))))))"};

inline const std::pair<std::string, std::string> nested_if {
    "def Testnestedif(x, y):\n"
    "    if x > 0:\n"
    "        if y > 0:\n"
    "            assert x == y\n"
    "        elif x < 0:\n"
    "            return 0\n"
    "        else:\n"
    "            return 1\n"
    "    return -1",

    "(module [0, 0] - [9, 0]\n"
    "  (function_definition [0, 0] - [8, 13]\n"
    "    name: (identifier [0, 4] - [0, 16])\n"
    "    parameters: (parameters [0, 16] - [0, 22]\n"
    "      (identifier [0, 17] - [0, 18])\n"
    "      (identifier [0, 20] - [0, 21]))\n"
    "    body: (block [1, 4] - [8, 13]\n"
    "      (if_statement [1, 4] - [7, 20]\n"
    "        condition: (comparison_operator [1, 7] - [1, 12]\n"
    "          (identifier [1, 7] - [1, 8])\n"
    "          (integer [1, 11] - [1, 12]))\n"
    "        consequence: (block [2, 8] - [7, 20]\n"
    "          (if_statement [2, 8] - [7, 20]\n"
    "            condition: (comparison_operator [2, 11] - [2, 16]\n"
    "              (identifier [2, 11] - [2, 12])\n"
    "              (integer [2, 15] - [2, 16]))\n"
    "            consequence: (block [3, 12] - [3, 25]\n"
    "              (assert_statement [3, 12] - [3, 25]\n"
    "                (comparison_operator [3, 19] - [3, 25]\n"
    "                  (identifier [3, 19] - [3, 20])\n"
    "                  (identifier [3, 24] - [3, 25]))))\n"
    "            alternative: (elif_clause [4, 8] - [5, 20]\n"
    "              condition: (comparison_operator [4, 13] - [4, 18]\n"
    "                (identifier [4, 13] - [4, 14])\n"
    "                (integer [4, 17] - [4, 18]))\n"
    "              consequence: (block [5, 12] - [5, 20]\n"
    "                (return_statement [5, 12] - [5, 20]\n"
    "                  (integer [5, 19] - [5, 20]))))\n"
    "            alternative: (else_clause [6, 8] - [7, 20]\n"
    "              body: (block [7, 12] - [7, 20]\n"
    "                (return_statement [7, 12] - [7, 20]\n"
    "                  (integer [7, 19] - [7, 20])))))))\n"
    "      (return_statement [8, 4] - [8, 13]\n"
    "        (unary_operator [8, 11] - [8, 13]\n"
    "          argument: (integer [8, 12] - [8, 13]))))))"};

inline const std::pair<std::string, std::string> ternary {
    "def teSt_ternary(x):\n"
    "    return \"positive\" if (42 if x > 0 else -3) > 0 else \"non-positive\"",

    "(module [0, 0] - [1, 70]\n"
    "  (function_definition [0, 0] - [1, 70]\n"
    "    name: (identifier [0, 4] - [0, 16])\n"
    "    parameters: (parameters [0, 16] - [0, 19]\n"
    "      (identifier [0, 17] - [0, 18]))\n"
    "    body: (block [1, 4] - [1, 70]\n"
    "      (return_statement [1, 4] - [1, 70]\n"
    "        (conditional_expression [1, 11] - [1, 70]\n"
    "          (string [1, 11] - [1, 21]\n"
    "            (string_start [1, 11] - [1, 12])\n"
    "            (string_content [1, 12] - [1, 20])\n"
    "            (string_end [1, 20] - [1, 21]))\n"
    "          (comparison_operator [1, 25] - [1, 50]\n"
    "            (parenthesized_expression [1, 25] - [1, 46]\n"
    "              (conditional_expression [1, 26] - [1, 45]\n"
    "                (integer [1, 26] - [1, 28])\n"
    "                (comparison_operator [1, 32] - [1, 37]\n"
    "                  (identifier [1, 32] - [1, 33])\n"
    "                  (integer [1, 36] - [1, 37]))\n"
    "                (unary_operator [1, 43] - [1, 45]\n"
    "                  argument: (integer [1, 44] - [1, 45]))))\n"
    "            (integer [1, 49] - [1, 50]))\n"
    "          (string [1, 56] - [1, 70]\n"
    "            (string_start [1, 56] - [1, 57])\n"
    "            (string_content [1, 57] - [1, 69])\n"
    "            (string_end [1, 69] - [1, 70])))))))"};

}  // namespace helper
