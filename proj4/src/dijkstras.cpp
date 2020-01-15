// dijsktras.cpp
#include <iostream>
#include <vector>
#include <map>
using namespace std;

/* Node Class */
class Node{
    public:
        Node(int r, int c, int weightValue);  // constructor
        int row;
        int column;
        int weight;
        int distance;
        bool visited;
        Node *backEdge;
};
Node::Node(int r, int c, int weightValue){  // constructor function
    row = r;
    column = c;
    weight = weightValue;
    distance = -1;
    visited = false;
    backEdge = NULL;
}


/* Main Execution */
int main(int argc, char *argv[]) {
    map<char, int> tileMap;
    int numOfDifferentTiles, tileCost, mapRows, mapColumns;
    char tileName;

    //read in the tile information (tile costs)
    cin >> numOfDifferentTiles;
    for(int i = 0; i < numOfDifferentTiles; i++){
        cin >> tileName >> tileCost;
        tileMap[tileName] = tileCost;
    }
    //read in map information and then create a node vector
    cin >> mapRows >> mapColumns;
    vector<vector<Node*> > nodeVec(mapRows);   // nodeVec will store node object
    for(int i = 0; i < nodeVec.size(); i++){
        nodeVec[i].resize(mapColumns);
    }
    //iterate through map and update nodeVec
    map<char,int>::iterator tileIter;
    for(int r = 0; r < mapRows; r++){
        for(int c = 0; c < mapColumns; c++){
            cin >> tileName;
            tileIter = tileMap.find(tileName);   // find tileName
            nodeVec[r][c] = new Node(r,c,tileIter->second);   // allocate a new node for every index
        }
    }
    
    /********** DIJKSTRA'S ALGORITHM **********/
    int totalDistance, row, col, startRow, startCol;
    Node *node, *leftNode, *rightNode, *topNode, *bottomNode;
    multimap<int, Node*> nodeMap;
    multimap<int, Node*>::iterator iter;

    //insert starting node into nodeMap & set distance to 0
    cin >> startRow >> startCol;
    node = nodeVec[startRow][startCol];
    node->distance = 0;
    nodeMap.insert( pair<int, Node*>(0, node) );
    iter = nodeMap.begin();
    
    //iterate through nodeMap
    while(iter != nodeMap.end()){
        node = iter->second;

        if(node->visited == false){   // visit a node that hasn't been visited yet
            node->visited = true;
            totalDistance = (iter->first)+(node->weight);   // compute distance
            row = node->row;
            col = node->column;

            //find adjacent nodes
            /* LEFT */
            if(col-1 >= 0){
                leftNode = nodeVec[row][col-1];
                if(leftNode->distance == -1){  // if leftNode hasn't been visited add to map
                    leftNode->distance = totalDistance;
                    leftNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, leftNode) );
                    iter = nodeMap.begin();
                }
                else if(totalDistance < leftNode->distance){  // if distance is less than leftNode's distance 
                    leftNode->distance = totalDistance;
                    leftNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, leftNode) );
                    iter = nodeMap.begin();
                }
            }
            /* RIGHT */
            if(col+1 < mapColumns){
                rightNode = nodeVec[row][col+1];
                if(rightNode->distance == -1){  // if rightNode hasn't been visited add to map
                    rightNode->distance = totalDistance;
                    rightNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, rightNode) );
                    iter = nodeMap.begin();
                }
                else if(totalDistance < rightNode->distance){  // if distance is less than rightNode's distance 
                    rightNode->distance = totalDistance;
                    rightNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, rightNode) );
                    iter = nodeMap.begin();
                }                
            }
            /* TOP */
            if(row-1 >= 0){
                topNode = nodeVec[row-1][col];
                if(topNode->distance == -1){  // if topNode hasn't been visited add to map
                    topNode->distance = totalDistance;
                    topNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, topNode) );
                    iter = nodeMap.begin();
                }
                else if(totalDistance < topNode->distance){  // if distance is less than topNode's distance 
                    topNode->distance = totalDistance;
                    topNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, topNode) );
                    iter = nodeMap.begin();
                }                
            }
            /* BOTTOM */
            if(row+1 < mapRows){
                bottomNode = nodeVec[row+1][col];
                if(bottomNode->distance == -1){  // if bottomNode hasn't been visited add to map
                    bottomNode->distance = totalDistance;
                    bottomNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, bottomNode) );
                    iter = nodeMap.begin();
                }
                else if(totalDistance < bottomNode->distance){  // if distance is less than bottomNode's distance 
                    bottomNode->distance = totalDistance;
                    bottomNode->backEdge = node;
                    nodeMap.insert( pair<int, Node*>(totalDistance, bottomNode) );
                    iter = nodeMap.begin();
                }                
            }
            else{
                iter++;
            }
        }
        else if(node->visited == true){  // if visited then iterate to next value
            iter++;
        }
    }

    //OUTPUT 
    int endRow, endCol;
    vector<int> rows;  // these vectors will hold final directions
    vector<int> cols;
    cin >> endRow >> endCol;   // read in destination coordinates
    cout << nodeVec[endRow][endCol]->distance << endl;
    node = nodeVec[endRow][endCol];

    while(node != NULL){
        rows.push_back(node->row);
        cols.push_back(node->column);
        node = node->backEdge;
    }
    for(int i = rows.size()-1; i >= 0; i--){
        cout << rows[i] << " " << cols[i] << endl;
    }


    for(int r = 0; r < mapRows; r++){
        for(int c = 0; c < mapColumns; c++){
            delete nodeVec[r][c];
        }
    }
    return 0;
}

