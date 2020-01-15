// Challenge 7: Minimum Spanning Tree

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
using namespace std;

// Main Execution
int main(int argc, char *argv[]) {
  int numVertices, value;
  int totalWeight = 0;

  while(cin >> numVertices){   // read in number of vertices
    
    vector<vector<int> > adjMatrix(numVertices);  // create adjacency matrix
    for(int i = 0; i < adjMatrix.size(); i++){
      adjMatrix[i].resize(numVertices);
    }
    for(int r = 0; r < numVertices; r++){  // read values into adj matrix
      for(int c = 0; c < numVertices; c++){
        cin >> value;
        adjMatrix[r][c] = value;
      }
    }

    /*PRIM'S ALGORITHM*/
    set<string> edges;   // initialize empty set for edges and vertices
    set<int> vertices;
    vertices.insert(0);  // select an arbritrary vertex to begin with (0)
    string edge1, edge2, edge;

    while(vertices.size() != numVertices){
      multimap<int, pair<int,int> > frontier; // frontier will hold weight values
      set<int>::iterator setIter, vertexCheck;
      map<int, pair<int,int> >::iterator mapIter;

      for(setIter = vertices.begin(); setIter != vertices.end(); setIter++){
        for(int row = 0; row < numVertices; row++){
          vertexCheck = vertices.find(row);  // check if second edge is already in vertices set
          if(adjMatrix[*setIter][row]!=-1 && vertexCheck==vertices.end()){
            frontier.insert( make_pair(adjMatrix[*setIter][row], make_pair(*setIter,row)) );
          }
        }
      }

      mapIter = frontier.begin();  // find the edge with smallest weight
      totalWeight += mapIter->first; // compute total edge

      vertices.insert(mapIter->second.first);  // add vertices to set
      vertices.insert(mapIter->second.second); 
      edge1 = mapIter->second.first + 65;  // convert edges to letters
      edge2 = mapIter->second.second + 65;
      if(edge1 < edge2){
        edge = edge1 + edge2;
        edges.insert(edge);
      }
      else{
        edge = edge2 + edge1;
        edges.insert(edge);
      }

      frontier.clear();
    }

  cout << totalWeight << endl;
  set<string>::iterator it;
  for(it = edges.begin(); it != edges.end(); it++){
    cout << *it << endl;
  }
  cout << endl;
  totalWeight = 0;
  
  }
  return 0;
}

