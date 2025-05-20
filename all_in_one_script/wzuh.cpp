#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <sstream>

// settings
bool codeoncode = true; // true = run code in script, false = enter in console

// Write the code here if you chose of course "bool codeoncode = true;"
const std::string script = R"(
§ Variable setup
set x = 10
set y = 2

§ Arithmetic
add x + y
sub x - y
mul x * y
div x / y
cmp x == y

§ User input
input name = What's your name?
pik<Hello, $name>
)";

// code
std::unordered_map<std::string, std::string> vars;

// Trim whitespace
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Check if string is number
bool is_number(const std::string& s) {
    return std::regex_match(s, std::regex("^-?\\d+$"));
}

// Resolve variable or direct number
int resolve_value(const std::string& token) {
    std::string clean = trim(token);
    if (vars.count(clean)) {
        std::string val = vars[clean];
        if (is_number(val)) return std::stoi(val);
        std::cout << "\033[31m[err] Variable '" << clean << "' is not a valid number.\033[0m\n";
        return 0;
    }
    if (is_number(clean)) return std::stoi(clean);
    std::cout << "\033[31m[err] Token '" << clean << "' is not a number or known variable.\033[0m\n";
    return 0;
}

// like $name
std::string replace_vars(const std::string& text) {
    std::string result = text;
    std::regex var_pattern(R"(\$(\w+))");
    std::smatch match;
    while (std::regex_search(result, match, var_pattern)) {
        std::string var_name = match[1];
        std::string value = vars.count(var_name) ? vars[var_name] : "[undefined]";
        result.replace(match.position(0), match.length(0), value);
    }
    return result;
}

// single line
void run_wzuh_line(const std::string& line_raw) {
    std::string line = trim(line_raw);
    if (line.empty() || line.rfind("§", 0) == 0) return;

    if (line.rfind("pik<", 0) == 0) {
        std::string content = line.substr(4, line.length() - 5);
        std::cout << replace_vars(content) << std::endl;

    } else if (line.rfind("set ", 0) == 0) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string var = trim(line.substr(4, eq - 4));
            std::string val = trim(line.substr(eq + 1));
            vars[var] = val;
        } else {
            std::cout << "\033[31m[err] Invalid set syntax. Try: set name = value\033[0m\n";
        }

    } else if (line.rfind("add ", 0) == 0) {
        std::regex expr(R"(add\s+(\w+)\s+\+\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a + b) << std::endl;
        } else {
            std::cout << "\033[31m[err] Invalid add syntax. Use: add x + y\033[0m\n";
        }

    } else if (line.rfind("sub ", 0) == 0) {
        std::regex expr(R"(sub\s+(\w+)\s+-\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a - b) << std::endl;
        } else {
            std::cout << "\033[31m[err] Invalid sub syntax. Use: sub x - y\033[0m\n";
        }

    } else if (line.rfind("mul ", 0) == 0) {
        std::regex expr(R"(mul\s+(\w+)\s+\*\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a * b) << std::endl;
        } else {
            std::cout << "\033[31m[err] Invalid mul syntax. Use: mul x * y\033[0m\n";
        }

    } else if (line.rfind("div ", 0) == 0) {
        std::regex expr(R"(div\s+(\w+)\s+\/\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            if (b == 0) {
                std::cout << "\033[33m[warn] Division by zero\033[0m\n";
            } else {
                std::cout << (a / b) << std::endl;
            }
        } else {
            std::cout << "\033[31m[err] Invalid div syntax. Use: div x / y\033[0m\n";
        }

    } else if (line.rfind("cmp ", 0) == 0) {
        std::regex expr(R"(cmp\s+(\w+)\s+==\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a == b ? "true" : "false") << std::endl;
        } else {
            std::cout << "\033[31m[err] Invalid cmp syntax. Use: cmp x == y\033[0m\n";
        }

    } else if (line.rfind("input ", 0) == 0) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string var = trim(line.substr(6, eq - 6));
            std::string prompt = trim(line.substr(eq + 1));
            std::cout << prompt << " ";
            std::string value;
            std::getline(std::cin, value);
            vars[var] = value;
        } else {
            std::cout << "\033[31m[err] Invalid input syntax. Try: input name = prompt\033[0m\n";
        }

    } else {
        std::cout << "\033[31m[err] Unknown command: " << line << "\033[0m\n";
    }
}

// main loop
int main() {
    std::cout << "wzuuuuuuh\n";

    if (codeoncode) {
        std::stringstream ss(script);
        std::string line;
        while (std::getline(ss, line)) {
            run_wzuh_line(line);
        }
    } else {
        std::string line;
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, line);
            if (line == "exit") break;
            run_wzuh_line(line);
        }
    }

    return 0;
}
