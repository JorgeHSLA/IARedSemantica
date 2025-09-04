#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <list>
#include <map>
#include <vector>
#include <string>   // <-- necesario para std::string

template <class T>
class graph {
protected:
  std::list<T> vertices;
  // ADJACENCY MATRIX con costos string: "" => no hay arista
  std::string **edges;
  int numVertices;

public:
  graph();
  ~graph();

  std::list<T> getVertices();
  std::string **getEdges();                 // <- cambiado

  int vertexCount();
  int edgeCount();

  int findIndex(T vertex);

  bool insertVertex(T vertex);

  // Inserta arista con costo por defecto "1"
  bool insertEdge(T origin, T destination);

  // Inserta arista con costo string
  bool insertEdge(T origin, std::string cost, T destination);

  bool findVertex(T vertex);
  T findVertex(int index);

  bool findEdge(T origin, T destination);

  bool removeVertex(T vertex);
  bool removeEdge(T origin, T destination);

  void flatTraversal();
  std::list<T> depthTraversal(T vertex);
  void depthTraversal(T vertex, std::map<T, bool> &visited);
  void breadthTraversal(T vertex);

  void printGraph();
  void printElements();

  void Euler();

  // Nota: Con costos string, Dijkstra no aplica directamente (ver .hxx)
  std::vector<long> Dijkstra(T startVertex, T endVertex);
};

#include "graph.hxx"

#endif // __GRAPH_H__