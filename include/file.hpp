#pragma once

#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <print>

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::file {

struct File {
    inline static const std::string command_prefix = "cat ";
//        "tree-sitter parse --config-path /root/.config/tree-sitter/config.json ";

    File() = default;
    explicit File(const std::string& filename);

    std::string name;
    std::string ast;
    std::vector<std::string> source_lines;

    private:
    static std::vector<std::string> ReadSourceFile(std::ifstream& file);
    std::string GetAst(const std::string& filename);
    std::string GetAstFromFile(const std::string& filename);
};

template<typename T> inline auto pretty_print(std::vector<T>& file_funcs) {
    for(auto& func: file_funcs) {
        std::println("{}", func.filename);
        if(func.class_name) {
            std::println("Class name: {}", *func.class_name);
        }
        std::println("Function name: {}", func.name);
        //        std::println("{}", func.ast);
    }
};

template<> inline auto pretty_print(std::vector<File>& files) {
    for(auto& file: files) {
        std::println("File's AST: {}", file.ast);
    }
};

template<typename T> inline auto pretty_print(std::vector<std::vector<T>>& files) {
    for(auto& file: files) {
        std::println("---------------------------------------------");
        for(auto& func: file) {
            std::println("***");
            std::println("Function from file: {}", func.filename);
            if(func.class_name) {
                std::println("Function from class name: {}", *func.class_name);
            }
            else {
                std::println("Function from class name: Unknown");
            }
            std::println("Function name: {}", func.name);
            //        std::println("{}", func.ast);
        }
    }
};

}  // namespace analyser::file
