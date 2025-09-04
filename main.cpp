#include <iostream>
#include <iterator>
#include <chrono>
#include <climits>
#include <stack>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "graph.h"
#include "aux.h"

/*
    Chat recomienda la creación y uso de estructuras 
    auxiliares para la implementación del algoritmo Best-First
    Y le pedí que las creara xd
*/

struct Alt {
    Jarra state;               
    int g;                     
    std::size_t visited_size;  
};

struct JarraHash {
    std::size_t operator()(const Jarra& s) const noexcept {
        std::size_t h1 = std::hash<int>{}(std::get<0>(s));
        std::size_t h2 = std::hash<int>{}(std::get<1>(s));
        return h1 ^ (h2 + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
    }
};

struct Node {
    Jarra state;
    int g; 
    int f; 
};

struct NodeCmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.f > b.f; 
    }
};

int main () {


    // Versión 1: Heurística + backtracking 
    // La propuesta heurística se realiza con la implementación de las funciones
    // creadas en aux.h
    // Chat ayuda con cómo plantear el backtracking del algoritmo 
    // que nos permite recorrer rutas alternativas cuando el problema se estanca
    // Chat también ayuda con el debugging y unas cosas de sintaxis redundantes
    {
        graph<Jarra> problem_graph;
        Jarra current = std::make_tuple(LOWERLIMIT, LOWERLIMIT);
        Jarra heuristic_solution = current;

        int current_cost = 0;   
        bool heuristic = true;

        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::microseconds duration;

        t1 = std::chrono::high_resolution_clock::now();

        std::stack<Alt> alternatives;   
        std::list<Jarra> visited;       

        while (heuristic) {

            if (aux::isGoal(current)) {
                heuristic_solution = current;
                break;
            }

            visited.push_back(current);
            current_cost++;

            problem_graph.insertVertex(current);

            std::list<Jarra> current_succesors = aux::succesors(current);

            // Filtrar válidos y no visitados
            std::list<Jarra> valid_succesors;
            std::list<Jarra>::iterator it;
            for (it = current_succesors.begin(); it != current_succesors.end(); ++it) {
                if (!aux::isValid(*it)) continue;

                bool seen = false;
                std::list<Jarra>::const_iterator vit;
                for (vit = visited.begin(); vit != visited.end(); ++vit) {
                    if (*vit == *it) { 
                        seen = true; 
                        break; }
                }
                if (!seen) valid_succesors.push_back(*it);
            }
            current_succesors.swap(valid_succesors);

            if (current_succesors.empty()) {
                if (!alternatives.empty()) {
                    Alt alt = alternatives.top();
                    alternatives.pop();
                    while (visited.size() > alt.visited_size) {
                        visited.pop_back();
                    }
                    current_cost = alt.g;   
                    current = alt.state;   
                    continue;
                } else {
                    std::cerr << "No hay sucesores ni alternativas; fin (heurística).\n";
                    break;
                }
            }

            // Hallar la función para cada sucesor en una nueva lista
            int best_cost = INT_MAX;
            std::list<Jarra>::iterator best_it = current_succesors.end();

            std::vector< std::pair<int, Jarra> > all;
            for (it = current_succesors.begin(); it != current_succesors.end(); ++it) {
                problem_graph.insertEdge(current, *it);

                int f_val = current_cost + aux::heuristic(*it);
                all.push_back(std::make_pair(f_val, *it));

                if (f_val < best_cost) {
                    best_cost = f_val;
                    best_it   = it;
                }
            }

            if (best_it == current_succesors.end()) {
                if (!alternatives.empty()) {
                    Alt alt = alternatives.top();
                    alternatives.pop();

                    while (visited.size() > alt.visited_size) {
                        visited.pop_back();
                    }
                    current_cost = alt.g;
                    current = alt.state;
                } else {
                    std::cerr << "No se halló mejor sucesor; fin (heurística).\n";
                }
                continue;
            }

            std::size_t ctx_size = visited.size();     
            int next_g = current_cost + 1;             
            std::size_t i;
            for (i = 0; i < all.size(); ++i) {
                if (all[i].second == *best_it) 
                    continue;  
                Alt alt;
                alt.state = all[i].second;
                alt.g = next_g;
                alt.visited_size = ctx_size;
                alternatives.push(alt);
            }

            current = *best_it;
        }

        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

        std::cout << "[Heurística + Backtracking]\n";
        std::cout << "Tupla de solución: "   << aux::to_string(heuristic_solution) << "\n";
        std::cout << "Cantidad de pasos (g): "   << current_cost << "\n";
        std::cout << "Tiempo de ejecución: " << duration.count() << " us\n\n";
    }

    // Versión 2: Best-First (frontera/priority_queue) con gscore (estilo A*)
    // Como se menciona en la declaración de las estructuras, gepeto ayuda con los nodos y la definición 
    // de la frontera (NodeCmp), que nos permite usar el best-first con una cola de prioridad
    // Como se menciona en el Aux.cpp, es posible usar el unordered_set con las jarras
    // por el JarraHash escrito por gepeto en ese archivo.
    // Ayudó a corregir la lógica del algoritmo y debugging. (Porque apuntadores y referencias
    // aún dan duro a veces jajaja)
    {
        graph<Jarra> problem_graph2;
        Jarra start = std::make_tuple(LOWERLIMIT, LOWERLIMIT);

        std::chrono::high_resolution_clock::time_point t1b;
        std::chrono::high_resolution_clock::time_point t2b;
        std::chrono::microseconds duration_bf;

        t1b = std::chrono::high_resolution_clock::now();

        std::priority_queue<Node, std::vector<Node>, NodeCmp> open;
        
        std::unordered_set<Jarra, JarraHash> closed;
        std::unordered_map<Jarra, int, JarraHash> gscore;          
        std::unordered_map<Jarra, Jarra, JarraHash> parent;        

        int h0 = aux::heuristic(start);
        open.push(Node{ start, 0, h0 });
        gscore[start] = 0;

        bool found = false;
        Jarra goal = start;
        int goal_g = 0;

        while (!open.empty()) {
            Node cur = open.top(); 
            open.pop();

            Jarra u = cur.state;

            std::unordered_map<Jarra, int, JarraHash>::iterator git_u = gscore.find(u);
            if (git_u != gscore.end() && cur.g > git_u -> second) {
                continue;
            }

            if (aux::isGoal(u)) {
                found  = true;
                goal   = u;
                goal_g = cur.g;
                break;
            }

            closed.insert(u);

            problem_graph2.insertVertex(u);
            std::list<Jarra> succ = aux::succesors(u);

            std::list<Jarra>::iterator it;
            for (it = succ.begin(); it != succ.end(); ++it) {
                const Jarra& v = *it;

                if (!aux::isValid(v)) continue;

                int g2 = cur.g + 1;

                if (closed.find(v) != closed.end()) {
                    std::unordered_map<Jarra, int, JarraHash>::iterator git_vc = gscore.find(v);
                    if (git_vc != gscore.end() && g2 >= git_vc->second) continue;
                }

                std::unordered_map<Jarra, int, JarraHash>::iterator git_v = gscore.find(v);
                if (git_v == gscore.end() || g2 < git_v->second) {
                    gscore[v] = g2;
                    
                    int f2 = g2 + aux::heuristic(v);
                    open.push(Node{ v, g2, f2 });

                    problem_graph2.insertVertex(v);
                    problem_graph2.insertEdge(u, v);

                    parent[v] = u;
                }
            }
        }

        t2b = std::chrono::high_resolution_clock::now();
        duration_bf = std::chrono::duration_cast<std::chrono::microseconds>(t2b - t1b);

        std::cout << "[Best-First]\n";
        if (found) {
            std::cout << "Tupla de solución: " << aux::to_string(goal) << "\n";
            std::cout << "Cantidad de pasos (g): " << goal_g << "\n";

            std::list<Jarra> path;
            Jarra cur = goal;
            path.push_front(cur);
            while (!(cur == start)) {
                std::unordered_map<Jarra, Jarra, JarraHash>::iterator pit = parent.find(cur);
                if (pit == parent.end()) break;
                cur = pit->second;
                path.push_front(cur);
            }
            std::cout << "Camino:\n";
            std::list<Jarra>::iterator itp;
            for (itp = path.begin(); itp != path.end(); ++itp) {
                std::cout << "  " << aux::to_string(*itp) << "\n";
            }
        } else {
            std::cout << "No se encontró solución.\n";
            std::cout << "Cantidad de pasos (g): 0\n";
        }
        std::cout << "Tiempo de ejecución: " << duration_bf.count() << " us\n";
    }

    return 0;
}