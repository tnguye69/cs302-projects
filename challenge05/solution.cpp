// Challenge 05: Graph Paths
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <cstdlib>
using namespace std;

// Adjacency list graph implementation
struct Graph{
  map<char, set<char> > adj; 
};

// Read in undirected graph
void ReadIn(Graph &g, int numEdges){
  char source, destination;
  for(int i = 0; i < numEdges; i++){
    cin >> source >> destination;
    g.adj[source].insert(destination);  // insert values into graph
  }
}

void traversal(Graph &g, char &source, char &destination){
	stack<char> q;
	set<char> v;
	char curr;
	set<char>::iterator sit;

	q.push(source);
	while(!q.empty()){
		curr = q.top();
		q.pop();

		//not necessary to print here;
		//this is just to output what's happening
		//printf("%*cPopping: %s\n",2,' ',curr.c_str());

		if(v.insert(curr).second){
			//printf("%*cActually visiting: %s\n",4,' ',curr.c_str());

			for(sit = g.adj[curr].begin(); sit != g.adj[curr].end(); sit++){
				if(v.find(*sit) == v.end()){
					q.push(*sit);
          if(*sit == destination){
            cout << " there is a path from " << source << " to " << destination << endl;
            return;
          }
        }
      }
		}
	}

  cout << " there is no path from " << source << " to " << destination << endl;
}

// Main Execution
int main(int argc, char *argv[]) {
  
  int nEdges, nPaths;
  int graphNum = 1;
  char source, destination;
  map<char, set<char> >::iterator mapIt;
  set<char>::iterator setIt;

  while(cin >> nEdges){
    Graph g;
    ReadIn(g, nEdges); // insert values into graph
    
    cin >> nPaths;
    for(int i = 0; i < nPaths; i++){
      cin >> source >> destination;
      cout << "In Graph " << graphNum;
      traversal(g, source, destination);
    }
    graphNum = graphNum + 1;
    if(graphNum != 6){
      cout << endl;
    }
  }

  return 0;
}