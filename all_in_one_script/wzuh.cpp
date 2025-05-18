#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <algorithm>

// settings
bool codeoncode = true; // true = run code in script, false = enter in console
// Write the code here if you chose of course "bool codeoncode = true;"
const std::string script = R"(
pik<Abobus>
set x = 10
set y = 5
add x + y
sub x - y
input name = What's your name?
pik<Hello, $name>
)";

// === INTERPRETER CORE ===
std::unordered_map<std::string, std::string> vars;

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool is_number(const std::string& s) {
    return std::regex_match(s, std::regex("^-?\\d+$"));
}

int resolve_value(const std::string& token) {
    std::string clean = trim(token);
    if (vars.count(clean)) {
        std::string val = vars[clean];
        if (is_number(val)) return std::stoi(val);
        std::cerr << "[error] Variable '" << clean << "' is not a valid number.\n";
        return 0;
    }
    if (is_number(clean)) return std::stoi(clean);
    std::cerr << "[error] Token '" << clean << "' is not a number or known variable.\n";
    return 0;
}

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

void run_wzuh_line(const std::string& line) {
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
            std::cerr << "[error] Invalid set syntax. Go pls this: set name = value\n";
        }
    } else if (line.rfind("add ", 0) == 0) {
        std::regex expr(R"(add\s+(\w+)\s+\+\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a + b) << std::endl;
        } else {
            std::cerr << "[error] Invalid add syntax. Use: add x + y\n";
        }
    } else if (line.rfind("sub ", 0) == 0) {
        std::regex expr(R"(sub\s+(\w+)\s+-\s+(\w+))");
        std::smatch match;
        if (std::regex_match(line, match, expr)) {
            int a = resolve_value(match[1]);
            int b = resolve_value(match[2]);
            std::cout << (a - b) << std::endl;
        } else {
            std::cerr << "[error] Invalid sub syntax. Maybe: sub x - y\n";
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
            std::cerr << "[error] Invalid input syntax. maybe you wanted: input name = prompt\n";
        }
    } else {
        std::cerr << "[error] Unknown command: " << line << std::endl;
    }
}

int main() {
    std::cout << "wzuuuuuuh\n";

    if (codeoncode) {
        std::stringstream ss(script);
        std::string line;
        while (std::getline(ss, line)) {
            if (!line.empty()) run_wzuh_line(line);
        }
    } else {
        std::string line;
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, line);
            if (line == "exit") break;
            if (!line.empty()) run_wzuh_line(line);
        }
    }

    return 0;
}
