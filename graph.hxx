#include "graph.h"
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <vector>
#include <climits>
#include <stdexcept>   // std::logic_error
#include <string>

template <class T>
graph<T>::graph() {
  numVertices = 0;
  edges = nullptr; // std::string** (ver getEdges)
}

template <class T>
graph<T>::~graph() {
  this->vertices.clear();
  for (int i = 0; i < this->numVertices; ++i) {
    delete[] this->edges[i];
  }
  delete[] this->edges;
}

template <class T>
void graph<T>::printGraph() {
  std::cout << "Adjacency Matrix:" << std::endl;
  std::cout << "  ";
  for (T vertex : this->vertices) {
    std::cout << vertex << " ";
  }
  std::cout << std::endl;
  typename std::list<T>::iterator it = this->vertices.begin();

  for (int i = 0; i < this->numVertices; ++i) {
    std::cout << *it << " ";
    it++;
    for (int j = 0; j < this->numVertices; ++j) {
      // Imprime "-" si no hay arista (cadena vacía)
      std::cout << (this->edges[i][j].empty() ? std::string("-") : this->edges[i][j]) << " ";
    }
    std::cout << std::endl;
  }

  
}

template <class T>
void graph<T>::printElements() {
  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
    std::cout << *it << " " << std::endl;
  }
}

template <class T>
std::list<T> graph<T>::getVertices() {
  return this->vertices;
}

template <class T>
std::string** graph<T>::getEdges() {   // <- cambiado a std::string**
  return this->edges;
}

template <class T>
int graph<T>::vertexCount() {
  return this->vertices.size();
}

template <class T>
int graph<T>::edgeCount() {
  int totalCount = 0;
  for (int i = 0; i < this->numVertices; ++i) {
    for (int j = 0; j < this->numVertices; ++j) {
      if (!this->edges[i][j].empty()) totalCount++;
    }
  }
  return totalCount;
}

template <class T>
bool graph<T>::insertVertex(T vertex) {
  if (findVertex(vertex)) return false;
  this->vertices.push_back(vertex);

  // Nueva matriz dinámica (numVertices + 1)
  std::string** newEdges = new std::string*[numVertices + 1];
  for (int i = 0; i < numVertices + 1; ++i) {
      newEdges[i] = new std::string[numVertices + 1];
  }

  // Copia de datos previos
  for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
          newEdges[i][j] = edges ? edges[i][j] : std::string();
      }
  }

  // Inicializa nueva fila/columna con "" (sin arista)
  for (int i = 0; i < numVertices + 1; ++i) {
      newEdges[numVertices][i] = "";
      newEdges[i][numVertices] = "";
  }

  // Libera matriz anterior
  if (edges != nullptr) {
      for (int i = 0; i < numVertices; ++i) {
          delete[] edges[i];
      }
      delete[] edges;
  }

  // Asigna nueva matriz
  this->edges = newEdges;
  this->numVertices++;

  return true;
}

template <class T>
bool graph<T>::insertEdge(T origin, T destination) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  if (!this->edges[indexOrigin][indexDestination].empty()) return false;
  this->edges[indexOrigin][indexDestination] = "1"; // costo por defecto como "1"
  return true;
}

template <class T>
bool graph<T>::insertEdge(T origin, std::string cost, T destination) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  if (!this->edges[indexOrigin][indexDestination].empty()) return false;
  this->edges[indexOrigin][indexDestination] = cost; // ahora string
  return true;
}

template <class T>
int graph<T>::findIndex(T vertex) {
  int index = 0;

  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
    if (*it == vertex) return index;
    index++;
  }

  return -1;
}

template <class T>
bool graph<T>::findVertex(T vertex) {
  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); it++) {
    if (*it == vertex) return true;  
  }
  return false;
}

template <class T>
T graph<T>::findVertex(int index) {
  int tempIndex;
  T temp{};

  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); it++) {
    tempIndex = findIndex(*it);
    if (tempIndex == index) temp = *it;
  }
  return temp;
}

template <class T>
bool graph<T>::findEdge(T origin, T destination) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  return !this->edges[indexOrigin][indexDestination].empty();
}

template <class T>
bool graph<T>::removeVertex(T vertex) {
  int index = findIndex(vertex);
  if (index == -1) return false;

  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); it++) {
    if (*it == vertex) {
      this->vertices.erase(it);
      break;
    }
  }

  // Desplazar columnas a la izquierda desde 'index'
  for (int i = 0; i < this->numVertices; ++i) {
    for (int j = index; j < this->numVertices - 1; ++j) {
      this->edges[i][j] = this->edges[i][j+1];
    }
    edges[i][this->numVertices - 1] = "";
  }

  // Desplazar filas hacia arriba desde 'index'
  for (int i = index; i < this->numVertices - 1; ++i) {
    for (int j = 0; j < this->numVertices; ++j) {
      this->edges[i][j] = this->edges[i+1][j];
    }
  }

  for (int i = 0; i < this->numVertices; ++i) {
    edges[this->numVertices - 1][i] = "";
  }

  std::string** newEdges = new std::string*[this->numVertices - 1];
  for (int i = 0; i < this->numVertices - 1; ++i) {
    newEdges[i] = new std::string[this->numVertices - 1];
  }

  for (int i = 0; i < this->numVertices - 1; ++i) {
    for (int j = 0; j < this->numVertices - 1; ++j) {
      newEdges[i][j] = this->edges[i][j];
    }
  }

  for (int i = 0; i < this->numVertices - 1; ++i) {
    delete[] this->edges[i];
  }
  delete [] this->edges;

  this->edges = newEdges;
  this->numVertices--;

  return true;
}

template <class T>
bool graph<T>::removeEdge(T origin, T destination) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  if (this->edges[indexOrigin][indexDestination].empty()) return false;

  this->edges[indexOrigin][indexDestination] = "";
  return true;
}

template <class T>
void graph<T>::flatTraversal() {
  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

template <class T>
std::list<T> graph<T>::depthTraversal(T vertex) {
  std::map<T, bool> visited;
  typename std::list<T>::iterator it;

  for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
    visited.insert(std::pair<T, bool>(*it, false));
  }

  depthTraversal(vertex, visited);

  typename std::map<T, bool>::iterator itm;
  std::list<T> list;
  for (itm = visited.begin(); itm != visited.end(); ++itm) {
    if (itm->second) list.push_back(itm->first);
  }
  return list;
}

template <class T>
void graph<T>::depthTraversal(T vertex, std::map<T, bool> &visited) {
    visited[vertex] = true;

    typename std::list<T>::iterator it;
    for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
      if (!this->edges[findIndex(vertex)][findIndex(*it)].empty() && !visited[*it]) {
        depthTraversal(*it, visited);
      }
    }
}

template <class T>
void graph<T>::breadthTraversal(T vertex) {
  std::map<T, bool> visited;
  typename std::list<T>::iterator it;
  for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
    visited.insert(std::pair<T, bool>(*it, false));
  }

  std::queue<T> queue;

  visited[vertex] = true;
  queue.push(vertex);
  std::cout << "Breadth-first traversal from " << vertex << ": ";
  while (!queue.empty()) {
    T currentVertex = queue.front();
    queue.pop();
    std::cout << currentVertex << " ";

    for (it = this->vertices.begin(); it != this->vertices.end(); ++it) {
      if (!this->edges[findIndex(currentVertex)][findIndex(*it)].empty() && !visited[*it]) {
        visited[*it] = true;
        queue.push(*it);
      }
    }    
  }

  std::cout << std::endl;
}

template <class T>
void graph<T>::Euler() {
    bool hasPath = false;

    // Grados por vértice (cuenta aristas existentes, independientemente del "costo" string)
    std::map<T, int> degrees;
    for (T vertex : this->vertices) {
        degrees[vertex] = 0;
    }

    typename std::list<T>::iterator itI;

    itI = this->vertices.begin();
    for (int i = 0; i < this->numVertices; ++i) {
      T vi = *itI;
      typename std::list<T>::iterator itJ = this->vertices.begin();
      for (int j = 0; j < this->numVertices; ++j, ++itJ) {
          if (!this->edges[i][j].empty()) {
              degrees[vi]++; // cuenta salida
          }
      }
      ++itI;
    }

    int odd = 0;
    typename std::map<T, int>::iterator it;
    for (it = degrees.begin(); it != degrees.end(); ++it) {
      if ((it->second) % 2 != 0) odd++;
    }

    if (odd == 0 || odd == 2)
      hasPath = true;

    if (!hasPath) {
        std::cout << "The graph does not have an Eulerian path." << std::endl;
        return;
    }

    std::cout << "The graph has an Eulerian path." << std::endl;

    // Si necesitas continuar con una construcción del camino Euleriano, aquí
    // deberías implementar el algoritmo de Hierholzer adaptado a matriz con strings.
    // (Omitido por brevedad)
}

template <class T>
std::vector<long> graph<T>::Dijkstra(T /*startVertex*/, T /*endVertex*/) {
  // Dijkstra requiere pesos numéricos (sumables y comparables).
  // Con costos tipo std::string no es aplicable directamente.
  throw std::logic_error("Dijkstra is not supported with string edge costs. Map your string costs to numeric weights first.");
}