#include "graph.h"
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <vector>
#include <climits>

template <class T>
graph<T>::graph() {
  numVertices = 0;
  edges = nullptr;
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
      std::cout << this->edges[i][j] << " ";
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
float** graph<T>::getEdges() {
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
      if (this->edges[i][j] != 0) totalCount++;
    }
  }
  return totalCount;
}

template <class T>
bool graph<T>::insertVertex(T vertex) {
  if (findVertex(vertex)) return false;
  this->vertices.push_back(vertex);

  // Dynamic memory for the new matrix with size = vertices + 1
  float** newEdges = new float*[numVertices + 1];
  for (int i = 0; i < numVertices + 1; ++i) {
      newEdges[i] = new float[numVertices + 1];
  }

  // Copy the existing data from the previous matrix into the new matrix
  for (int i = 0; i < numVertices; ++i) {
      for (int j = 0; j < numVertices; ++j) {
          newEdges[i][j] = edges[i][j];
      }
  }

  // Initialize the new row and column with zeros 
  for (int i = 0; i < numVertices + 1; ++i) {
      newEdges[numVertices][i] = 0;
      newEdges[i][numVertices] = 0;
  }

  // Delete the old matrix
  if (edges != nullptr) {
      for (int i = 0; i < numVertices; ++i) {
          delete[] edges[i];
      }
      delete[] edges;
  }

  // Copy new matrix into the original one
  this->edges = newEdges;
  this->numVertices++;

  return true;
}

template <class T>
bool graph<T>::insertEdge(T origin, T destination) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  if (this->edges[indexOrigin][indexDestination] != 0) return false;
  this->edges[indexOrigin][indexDestination] = 1;
  return true;
}

template <class T>
bool graph<T>::insertEdge(T origin, T destination, float cost) {
  int indexOrigin = findIndex(origin);
  int indexDestination = findIndex(destination);
  if (indexOrigin == -1 || indexDestination == -1) return false;
  if (this->edges[indexOrigin][indexDestination] != 0) return false;
  this->edges[indexOrigin][indexDestination] = cost;
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
  T temp;

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
  if (this->edges[indexOrigin][indexDestination] != 0) return true;
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

  for (int i = 0; i < this->numVertices; ++i) {
    for (int j = index; j < this->numVertices - 1; ++j) {
      this->edges[i][j] = this->edges[i][j+1];
    }
    edges[i][this->numVertices - 1] = 0;
  }
  for (int i = index; i < this->numVertices - 1; ++i) {
    for (int j = 0; j < this->numVertices; ++j) {
      this->edges[i][j] = this->edges[i+1][j];
    }
  }

  for (int i = 0; i < this->numVertices; ++i) {
    edges[this->numVertices - 1][i] = 0;
  }

  float** newEdges = new float*[this->numVertices - 1];

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
  if (this->edges[indexOrigin][indexDestination] == 0) return false;

  this->edges[indexOrigin][indexDestination] = 0;
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
      if (this->edges[findIndex(vertex)][findIndex(*it)] != 0 && !visited[*it]) {
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
      if (this->edges[findIndex(currentVertex)][findIndex(*it)] != 0 && !visited[*it]) {
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

    // Count degrees of each vertex
    std::map<T, int> degrees;
    for (T vertex : this->vertices) {
        degrees[vertex] = 0;
    }

    typename std::list<T>::iterator itI;

    for (int i = 0; i < this->numVertices; ++i) {
      itI = this->vertices.begin();
      for (int j = 0; j < this->numVertices; ++j) {
          if (this->edges[i][j] == 1) {
              T tempVer = *itI;
              degrees[tempVer]++;
          }
      }
      itI++;
    }

    int odd = 0;
    typename std::map<T, int>::iterator it;
    for (it = degrees.begin(); it != degrees.end(); ++it) {
      if ((*it).second % 2 != 0) odd++;
      T tempOdd = (*it).first;
    }

    if (odd == 0 || odd == 2)
      hasPath = true;

    if (!hasPath) {
        std::cout << "The graph does not have an Eulerian path." << std::endl;
        return;
    }

    std::cout << "The graph has an Eulerian path." << std::endl;

    float **tempEdges = this->edges;
    int numVerticesTemp = this->numVertices;

    // CONTINUE IMPLEMENTATION
}

template <class T>
std::vector<long> graph<T>::Dijkstra(T startVertex, T endVertex) {
  std::vector<long> distance(this->numVertices, LONG_MAX);
  std::vector<long> path(this->numVertices, -1);
  std::set<std::pair<long, T>> queue;

  int start = findIndex(startVertex);
  int end = findIndex(endVertex);

  if (start == -1 || end == -1) return path;

  distance[start] = 0;
  queue.insert(std::make_pair(0, startVertex));

  while (!queue.empty()) {
      T current = queue.begin()->second;
      queue.erase(queue.begin());

      int currentIndex = findIndex(current);

      for (T neighbor : this->vertices) {
          int neighborIndex = findIndex(neighbor);
          if (this->edges[currentIndex][neighborIndex] != 0) {
              float weight = this->edges[currentIndex][neighborIndex];
              if (distance[neighborIndex] > distance[currentIndex] + weight) {
                  queue.erase(std::make_pair(distance[neighborIndex], neighbor));
                  distance[neighborIndex] = distance[currentIndex] + weight;
                  path[neighborIndex] = currentIndex;
                  queue.insert(std::make_pair(distance[neighborIndex], neighbor));
              }
          }
      }
  }

  std::vector<long> route;
  long i = end;
  while (i != -1) {
      route.push_back(i);
      i = path[i];
  }  
  return route;
}