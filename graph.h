#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <list>
#include <map>
#include <vector>

template <class T> class graph {
protected:
  std::list<T> vertices;
  // ADJACENCY MATRIX
  float **edges;
  int numVertices;

public:
  graph();
  ~graph();
  std::list<T> getVertices();
  float **getEdges();
  int vertexCount();
  int edgeCount();
  int findIndex(T vertex);
  bool insertVertex(T vertex);
  bool insertEdge(T origin, T destination);
  bool insertEdge(T origin, T destination, float cost);
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
  std::vector<long> Dijkstra(T startVertex, T endVertex);
};

#include "graph.hxx"

#endif