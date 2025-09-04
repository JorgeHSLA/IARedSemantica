#include <iostream>
#include <iterator>
#include <chrono>
#include <climits>
#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <cctype>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "graph.h"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

/*
    Chat recomienda la creación y uso de estructuras 
    auxiliares para la implementación del algoritmo Best-First
    Y le pedí que las creara xd
*/
              
enum class FunctionType { ES, INSTANCE, ATRIBUTTE };

static inline std::string trim(std::string s) {
    size_t i = 0, j = s.size();
    while (i < j && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return s.substr(i, j - i);
}

std::vector<std::string> splitArgsRespectingNesting(const std::string& args) {
    std::vector<std::string> out;
    std::string cur;
    int paren = 0; // ()
    int angle = 0; // <>
    bool inStr = false;
    char q = 0;

    for (size_t i = 0; i < args.size(); ++i) {
        char c = args[i];

        if (inStr) {
            cur.push_back(c);
            if (c == q && (i == 0 || args[i-1] != '\\')) inStr = false;
            continue;
        } else if (c == '"' || c == '\'') {
            inStr = true; q = c; cur.push_back(c);
            continue;
        }

        if (c == '(') ++paren;
        else if (c == ')' && paren > 0) --paren;
        else if (c == '<') ++angle;
        else if (c == '>' && angle > 0) --angle;

        if (c == ',' && paren == 0 && angle == 0) {
            out.push_back(trim(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!trim(cur).empty()) out.push_back(trim(cur));
    if (out.size() == 1 && out[0].empty()) out.clear();
    return out;
}

std::vector<std::string> parseSignature(const std::string& s) {
    static const std::regex re(R"(^\s*([A-Za-z_]\w*)\s*\(\s*(.*?)\s*\)\s*$)");
    std::smatch m;
    if (!std::regex_match(s, m, re)) return {}; 

    std::string name = m[1].str();
    std::string args = m[2].str();

    std::vector<std::string> result;
    result.push_back(name);

    auto parts = splitArgsRespectingNesting(args);
    result.insert(result.end(), parts.begin(), parts.end());
    return result;
}

int main () {

    std::cout << "Escriba el nombre del archivo (con .txt): ";
    std::string filename;
    std::getline(std::cin, filename);
    std::string linea;
    std::vector<std::string> entrada;
    std::vector<std::vector<std::string>> funciones;

    graph<std::string> red;
    std::ifstream archivo(filename);

    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return 1;
    }
    
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t pos = linea.find("(");
        if (pos == std::string::npos) {
            std::cerr << "Línea inválida (no contiene '('): " << linea << std::endl;
            continue;
        }
        std::string nombreFuncion = linea.substr(0, pos);
        std::string argumento = linea.substr(pos + 1, linea.size() - pos - 2); 
        std::vector<std::string>  argumentos = split(argumento, ',');

    }

    //1. Leer er archivo de txt por línea y guardarlo en un arreglo dinámico (lista)
    //2. Dividir cada valor de la lista en una sublista, donde se guarda a través de un expresión regular
    // el tipo de función y sus atributos

    


   return 0;
}