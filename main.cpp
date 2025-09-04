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

// Copilot define estas funciones porque al parecer no están definidas en la librería estándar ???
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

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
    Chat nos ayudó con la lectura del arhivo y partición del string de entrada
*/
              
enum class FunctionType { ES, INSTANCE, ATRIBUTTE };


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

        std::vector<std::string> funcion;
        funcion.push_back(nombreFuncion);
        for (const auto& arg : argumentos) funcion.push_back(arg);

        funciones.push_back(funcion);
    }

    archivo.close();

    std::vector<std::vector<std::string>>::iterator it;

    for (it = funciones.begin(); it != funciones.end(); ++it) {

        if((*it)[0] == "es_un"){
            red.insertVertex((*it)[1]);
            red.insertVertex((*it)[2]);
            red.insertEdge((*it)[1], (*it)[0], (*it)[2]);
        } else if ((*it)[0] == "atributo"){
            red.insertVertex((*it)[1]);
            red.insertVertex((*it)[3]);
            red.insertEdge((*it)[1], (*it)[2], (*it)[3]);
        } else if ((*it)[0] == "instancia"){
            red.insertVertex((*it)[1]);
            red.insertVertex((*it)[2]);
            red.insertEdge((*it)[1], (*it)[0], (*it)[2]);
        } else {
            std::cerr << "Función desconocida: " << (*it)[0] << std::endl;
        }
    }


  int opcion=0;
    std::cout<<"elije una de las opcions";
    std::cout<<"1.Preguntar por el valor de un atributo de una clase o instancia particular: atributo X de Y? \n";
    std::cout<<"2.Preguntar por todas las clases o instancias que tienen un atributo con un valor particular: clases o instancias con atributo X y valor Z? \n";
    std::cin>>opcion;
    if (opcion == 1)
        preguntaUno()
    else if(opcion == 2)
        preguntaDos()
    else
        std::cout<<"pon un numero que sea el uno o el dos\n";
    
    


   return 0;
}


void preguntaUno(){

    std::String atributo = "X"
    std::String instanciaClase = "Y"
    std::cout<<"INGRESA EL ATRIBUTO (X) \n";
    std::cin>>atributo;
    std::cout<<"INGRESA LA CLASE O INSTANCIA (Y) \n";
    std::cin>>instanciaClase;


} 

void preguntaDos(){

    std::String atributo = "X"
    std::String instanciaClase = "Y"
    std::cout<<"INGRESA EL ATRIBUTO (X) \n";
    std::cin>>atributo;
    std::cout<<"INGRESA VALOR DE LA VARIABLE (Z) \n";
    std::cin>>instanciaClase;

}

bool atributoEnClaseOInstancia(String x, String y){
    find


}
