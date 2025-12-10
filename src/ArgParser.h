//
// Created by gnilk on 28.11.25.
//

#ifndef GNILK_ARGPARSER_H
#define GNILK_ARGPARSER_H


#include <span>
#include <string>
#include <string_view>
#include <ranges>
#include <charconv>
#include <vector>

//
// simple decent modern argument parser
//
// See tests/test_argparser.cpp for examples or the example/ex1_app.cpp
//
// TODO:
//  - Count occurrences (like increase verbose on multiple '-v' or '-vvv')
//  - Collect all at end, like:  ./app -out output.bin input_a.bin input_b.bin input_c.bin ...
//
// Use from main like:
//      argParser = ArgParse(argc, argv);
//      auto value = TryParse("-i", "--input_file", {});        // if no default
//
class ArgParser {
public:
    ArgParser() = delete;
    ArgParser(size_t argc, const char **argv) : args{argv, argc} {
    }
    virtual ~ArgParser() = default;

    template<typename TValue>
    [[nodiscard]]
    std::optional<TValue> TryParse(const std::string &shortParamName, const std::string &longParamName = {}) const {
        return TryParse<TValue>(shortParamName, longParamName, {});
    }


    template<typename TValue>
    [[nodiscard]]
    std::optional<TValue> TryParse(const std::string &shortParamName, const std::string &longParamName, const TValue &defaultValue) const {
        for(size_t i=0;i<args.size();++i) {
            std::string_view arg = args[i];
            if (arg.empty() || arg[0] != '-') {
                continue;
            }

            if ((arg == shortParamName) || (arg == longParamName)) {
                if ((i + 1) >= args.size()) {
                    fprintf(stderr, "Argument missing for '%s' (check argc in CTOR)\n", shortParamName.c_str());
                    return {};
                }
                const std::string_view argValue = args[++i];
                return convert_to<TValue>(argValue);
            }
        }
        return defaultValue;
    }

    template<typename TValue>
    [[nodiscard]]
    std::optional<TValue> TryParse(const std::string &shortParamName, const std::string &longParamName, const TValue &&defaultValue) const {
        return TryParse(shortParamName, longParamName, defaultValue);
    }

    [[nodiscard]]
    bool TryParse(const std::string &shortParamName, const std::string &longParamName = {}) const {
        for(size_t i=0;i<args.size();++i) {
            std::string_view arg = args[i];
            if (arg.empty() || arg[0] != '-') {
                continue;
            }
            if (!longParamName.empty() && (arg == longParamName)) {
                return true;
            }
            // check every char in this combined short-option (no arg) string
            // -Ivd --next_arg
            if (shortParamName.empty()) {
                continue;
            }
            for (auto c : arg) {
                if (c == shortParamName[1]) {
                    return true;
                }
            }
        }
        // not present
        return false;
    }

    [[nodiscard]]
    bool TryParseAny(const std::vector<std::string> &shortParamName, const std::vector<std::string> &longParamName = {}) const {
        for(auto &s : shortParamName) {
            if (TryParse(s)) {
                return true;
            }
        }
        for(auto &s : longParamName) {
            if (TryParse({}, s)) {
                return true;
            }
        }
        return false;
    }



protected:
    template<typename T>
    [[nodiscard]]
    std::optional<T> convert_to(std::string_view sv) const {
        //T value;

        if constexpr (std::is_same_v<T, std::string>) {
            return std::string{sv};
        }
        else if constexpr (std::is_same_v<T, std::string_view>) {
            return sv;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            if (sv == "1" || sv == "true"  || sv == "TRUE")  return true;
            if (sv == "0" || sv == "false" || sv == "FALSE") return false;
            return std::nullopt;
        }
        else if constexpr (std::is_arithmetic_v<T>) {
            T out{};
            auto first = sv.data();
            auto last  = sv.data() + sv.size();

            // std::from_chars handles all integers and floating point (C++17+)
            auto [ptr, ec] = std::from_chars(first, last, out);
            if (ec == std::errc{} && ptr == last)
                return out;

            return std::nullopt;
        }
        else {
            // Unsupported type; static assert provides helpful compile-time error
            static_assert(!sizeof(T*), "convert_to<T>: Unsupported type");
        }
    }
private:
    std::span<const char *> args;
};

#endif