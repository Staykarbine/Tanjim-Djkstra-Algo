#include<fstream>
#include<iostream>
using namespace std;
struct edge {
	struct vertex * Vertex;
	int weight;
	edge * nextedge;
	edge(edge * e = 0, struct vertex * v = 0, int w = 0)
	{
		Vertex = v;
		weight = w;
		nextedge = e;
	}
};
struct vertex {
	char name;
	vertex * nextvertex;
	edge * edgelist;
	int index;
	bool final;
	vertex * pre;
	vertex(char n = '\0', vertex * v = 0)
	{
		name = n;
		nextvertex = v;
		edgelist = 0;
		index = -1;
		final = false;
		pre = 0;
	}
};
int main() {
	char input_file[128];
	cout << "In what file is the data for the graph contained?\n> ";
	cin.getline(input_file, 128);
	ifstream infile(input_file);
	vertex * graph = 0;
	vertex * startptr = 0, *finishptr = 0;
	vertex * vertexsearch = 0, *vptr = 0;
	vertex * last;
	vertex * w;
	edge * edgeptr = 0;
	int weight;
	char start, finish, comma;
	bool startnotfound = true, finishnotfound = true;
	infile >> start >> comma >> finish >> comma >> weight;
	while (!infile.eof()) {
		/* build the edge list */
		if (graph == 0) {
			graph = new vertex(start);
			startptr = graph;
		}
		else {
			vertexsearch = graph;
			while (vertexsearch) {
				if (vertexsearch->name == start) break;
				vertexsearch = vertexsearch->nextvertex;
			}
			if (vertexsearch)
				startptr = vertexsearch;
			else {
				last = graph;
				while (last->nextvertex)
					last = last->nextvertex;
				last->nextvertex = new vertex(start);
				startptr = last->nextvertex;
			}
		}
		if (graph == 0) {
			graph = new vertex(finish);
			finishptr = graph;
		}
		else {
			vertexsearch = graph;
			while (vertexsearch) {
				if (vertexsearch->name == finish) break;
				vertexsearch = vertexsearch->nextvertex;
			}
			if (vertexsearch)
				finishptr = vertexsearch;
			else {
				last = graph;
				while (last->nextvertex)
					last = last->nextvertex;
				last->nextvertex = new vertex(finish);
				finishptr = last->nextvertex;
			}
		}
		if (startptr->edgelist == 0)
			startptr->edgelist = new edge(nullptr, finishptr, weight);
		else {
			edgeptr = startptr->edgelist;
			while (edgeptr->nextedge)
				edgeptr = edgeptr->nextedge;
			edgeptr->nextedge = new edge(nullptr, finishptr, weight);
		}
		infile >> start >> comma >> finish >> comma >> weight;
	}
	// Output the graph
	vptr = graph;
	while (vptr) {
		cout << vptr->name << '\n';
		edgeptr = vptr->edgelist;
		while (edgeptr) {
			cout << " Edge to " << edgeptr->Vertex->name
				<< " with weight " << edgeptr->weight << '\n';
			edgeptr = edgeptr->nextedge;
		}
		vptr = vptr->nextvertex;
	}
	// From where to where
	cout << "From where: ";
	cin >> start;
	cout << "to where: ";
	cin >> finish;
	vertex * s = graph;
	startptr = finishptr = 0;
	while (s) {
		if (s->name == start) {
			startptr = s;
		}
		if (s->name == finish) {
			finishptr = s;
		}
		s = s->nextvertex;
	}
	if (!startptr) {
		cout << "Start point given is not a valid vertex.\n";
		return 1;
	}
	if (!finishptr) {
		cout << "Finish point given is not a valid vertex.\n";
		return 1;
	}
	last = startptr;
	last->index = 0;
	last->final = true;
	while (!(finishptr->final)) {
		/* Search for shortest path */
		vptr = graph;
		while (vptr != NULL)
		{
			vptr->index = INT16_MAX;
			vptr->final = false;
			vptr = vptr->nextvertex;
		}
		startptr->index = 0;
		startptr->final = true;
		last = startptr;
		while (!finishptr->final)
		{
			edgeptr = last->edgelist;
			while (edgeptr != NULL)
			{
				vptr = edgeptr->Vertex;
				if (!vptr->final && vptr->index > last->index + edgeptr->weight)
				{
					vptr->index = last->index + edgeptr->weight;
					vptr->pre = last;
				}
				edgeptr = edgeptr->nextedge;
			}
			w = 0;
			vertexsearch = graph;
			while (vertexsearch) {
				if (vertexsearch->final == false) {
					w = vertexsearch;
					break;
				}
				vertexsearch = vertexsearch->nextvertex;
			}
			if (w != 0) {
				vertexsearch = graph;
				while (vertexsearch) {
					if (w->index > vertexsearch->index && vertexsearch->final == false)
						w = vertexsearch;
					vertexsearch = vertexsearch->nextvertex;
				}
				w->final = true;
				last = w;
			}
		}
	}
	vptr = finishptr;
	if (vptr->pre)
		while (vptr) {
			cout << vptr->name << '\n';
			vptr = vptr->pre;
		}
	else
		cout << "No such path.\n";
	return 0;
}