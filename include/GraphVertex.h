#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H

#include <iostream>
#include <vector>
#include <GraphEdge.h>
#include <Node.h>

using namespace std;

class GraphEdge;

class GraphVertex
{
public:

	enum Direcction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
		NONE
	};

	GraphVertex(Node& data);
	~GraphVertex();

	Node getData() { return _data; }
	void addEdge(GraphEdge* pEdge) { _edges.push_back(pEdge); }

	std::vector<GraphEdge*> getEdges() const { return _edges; }
	std::vector<GraphVertex*> adjacents() const;

	static GraphVertex* nextVertx(Direcction direction, GraphVertex* actualVertx);
	static GraphVertex* GraphVertex::closerNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx);
	static bool checkVertex(GraphVertex* actualVertx, std::string type);


private:
	Node _data;
	std::vector<GraphEdge*> _edges;
};

#endif
