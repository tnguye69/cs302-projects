#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

typedef enum {SOURCE, SINK, DICE, WORD} Node_Type;

/* Node Class */
class Node{
    public:
        int id;
        string name;
        Node_Type type;
        vector <class Edge*> adj;
        class Edge *backEdge;
        int visited;  
};
/* Edge Class */
class Edge{
    public:
        Node *from;
        Node *to;
        Edge *reverse;
        int original;
        int residual;
};
/* Graph Class */
class Graph{
    public:    
        vector <Node*> nodes;
        vector<int> spellingIds;
        int minNodes; //number of dice
        int BFS();
        int canISpell();
        void deleteHalfGraph();
        Node *getNode(string &s);
};

/* Main Execution */
int main(int argc, char* argv[]){
    // if(argc > 2){
    //     return 0;
    // }
    
    Graph *nodeGraph = new Graph;  // make sure to delete

    //set up source nodes
    Node *source;
    source = new Node;  // make sure to delete
    source->id = 0;
    source->name = "SOURCE";
    source->type = SOURCE;
    source->backEdge = NULL;
    nodeGraph->nodes.push_back(source);
    
    //read in dice
    string dice;
    Node *diceNode;
    ifstream diceFile;
    diceFile.open(argv[1]);
    int id = 1;
    int diceAmount = 0;
    while(diceFile >> dice){
        diceNode = new Node;  // delete
        diceNode->name = dice;
        diceNode->type = DICE;
        diceNode->id = id;
        nodeGraph->nodes.push_back(diceNode);
        id++;
        diceAmount++;
    }
    nodeGraph->minNodes = id;  // minNodes is number of dice + 1

    //link source to dice
    Edge *edgeVar; 
    Edge *edgeReverse;
    for(int i = 0; i < nodeGraph->nodes.size(); i++){
        if(nodeGraph->nodes[i]->type == DICE){
            edgeVar = new Edge; // delete
            edgeReverse = new Edge; // delete

            edgeVar->from = source;
            edgeVar->to = nodeGraph->nodes[i];
            edgeVar->original = 1;
            edgeVar->residual = 0;

            edgeReverse->from = nodeGraph->nodes[i];
            edgeReverse->to = source;
            edgeReverse->original = 0;
            edgeReverse->residual = 1;

            edgeVar->reverse = edgeReverse;
            edgeReverse->reverse = edgeVar;

            // nodeGraph->nodes[i]->backEdge = edgeReverse;
            
            source->adj.push_back(edgeVar);
        }
    }

    //read in word
    ifstream wordFile;
    wordFile.open(argv[2]);
    string word;
    Node *wordNode;
    while(wordFile >> word){
        for(int i = 0; i < word.size(); i++){
            wordNode = new Node;  // delete
            wordNode->name = word[i];
            wordNode->type = WORD;
            wordNode->id = id;
            nodeGraph->nodes.push_back(wordNode);
            id++;
        }
        
        //create sink node
        Node *sink = new Node;
        sink->name = "SINK";
        sink->type = SINK;
        sink->id = id;
        nodeGraph->nodes.push_back(sink);

        //create edges  
        string diceWord;
        for(int i = 0; i < nodeGraph->nodes.size(); i++){

            if(nodeGraph->nodes[i]->type == DICE){  // create edges from dice to letters of word
                diceWord = nodeGraph->nodes[i]->name;
                for(int j = 0; j < word.size(); j++){
                    for (int k = 0; k < diceWord.size(); k++){
                        if (diceWord[k] == word[j]){
                            Node * matchNode = nodeGraph->nodes[1+diceAmount+j];
                            edgeVar = new Edge;
                            edgeVar->from = nodeGraph->nodes[i];
                            edgeVar->to = matchNode;
                            edgeVar->original = 1;
                            edgeVar->residual = 0;

                            edgeReverse = new Edge;
                            edgeReverse->from = matchNode;
                            edgeReverse->to = nodeGraph->nodes[i];
                            edgeReverse->original = 0;
                            edgeReverse->residual = 1;

                            edgeVar->reverse = edgeReverse;
                            edgeReverse->reverse = edgeVar;
                            
                            if(nodeGraph->nodes[i]->adj.size() > 0){  // make sure there's no repeated edges
                                bool check = 0;
                                for (int l = 0; l < nodeGraph->nodes[i]->adj.size(); l++){
                                    if (edgeVar->to->id == nodeGraph->nodes[i]->adj[l]->to->id){
                                        check = 1;
                                        delete edgeVar;
                                    }
                                }
                                if (check == 0){
                                    nodeGraph->nodes[i]->adj.push_back(edgeVar);
                                    matchNode->adj.push_back(edgeReverse);
                                }
                            }
                            else {
                                nodeGraph->nodes[i]->adj.push_back(edgeVar);
                                matchNode->adj.push_back(edgeReverse);
                            }  
                        }
                    }
                }
            }
            
            if(nodeGraph->nodes[i]->type == WORD){  // create edges from word to sink
                edgeVar = new Edge;
                edgeVar->from = nodeGraph->nodes[i];
                edgeVar->to = sink;
                edgeVar->original = 1;
                edgeVar->residual = 0;

                edgeReverse = new Edge;
                edgeReverse->from = sink;
                edgeReverse->to = nodeGraph->nodes[i];
                edgeReverse->residual = 1;
                edgeReverse->original = 0;

                edgeVar->reverse = edgeReverse;
                edgeReverse->reverse = edgeVar;

                nodeGraph->nodes[i]->adj.push_back(edgeVar);
            }
        }


        for(int i = 0; i < source->adj.size(); i++){
            if (source->adj[i]->original == 0){
                //cout << "SOURCE EDGES TO NODE " << source->adj[i]->to->id << endl;
            }
        }

        // for (int i = 0; i < nodeGraph->nodes.size(); i++){
        //     if (nodeGraph->nodes[i]->type == DICE){
        //         //cout << nodeGraph->nodes[i]->name << " EDGES TO ";
        //         for (int j = 0; j < nodeGraph->nodes[i]->adj.size(); j++){
        //             if (nodeGraph->nodes[i]->adj[j]->original == 0){
        //                 //cout << nodeGraph->nodes[i]->adj[j]->to->id  << " ";
        //             }
        //         }
        //         cout << endl;
        //     }
        //     if(nodeGraph->nodes[i]->type == WORD){
        //         //cout << nodeGraph->nodes[i]->name << " edges to ";
        //         for (int j = 0; j < nodeGraph->nodes[i]->adj.size(); j++){
        //             if (nodeGraph->nodes[i]->adj[j]->original == 0){
        //                 //cout << nodeGraph->nodes[i]->adj[j]->to->id  << " ";
        //             }
        //         }
        //         cout << endl; 
        //     }
        // }
        int count = 0;
        while (nodeGraph->BFS() == 1){
            count++;
        }
        
        //cout << "Count = " << count << endl;
        bool checker = 1;

        for (int k = 0; k < nodeGraph->nodes.size(); k++){
            if (nodeGraph->nodes[k]->type == WORD){
                for(int l = 0; l < nodeGraph->nodes[k]->adj.size(); l++){
                    if(nodeGraph->nodes[k]->adj[l]->to->type == SINK){
                        if(nodeGraph->nodes[k]->adj[l]->residual == 1){

                        }else {
                            checker = 0;  // once any of the residuals = 0, you cannot spell the word

                        }
                    }
                }
            }
        }

        if (checker == 1){
            for (int k = 0; k < nodeGraph->nodes.size(); k++){
                if (nodeGraph->nodes[k]->type == WORD){
                    for (int l = 0; l < nodeGraph->nodes[k]->adj.size(); l++){
                        if (nodeGraph->nodes[k]->adj[l]->to->type == DICE && nodeGraph->nodes[k]->adj[l]->original == 1){
                            nodeGraph->spellingIds.push_back(nodeGraph->nodes[k]->adj[l]->to->id);
                        }
                    }
                }
            }

            cout << nodeGraph->spellingIds[0]-1;
            for (int k = 1; k < nodeGraph->spellingIds.size(); k++){
                cout << ", " << nodeGraph->spellingIds[k]-1;
            }
            cout << ": " << word << endl;
            //cout << "Can spell " << word << endl;
        }
        else {
            cout << "Cannot spell " << word << endl;
        }

        //cout << endl; // remove
        nodeGraph->deleteHalfGraph();
        
        

        // Graph *residualGraph = new Graph;

        // Node * sourceResidual;
        // Node * sinkResidual;

        // sourceResidual = new Node;
        // sinkResidual = new Node;
        // sourceResidual->id = 0;
        // sourceResidual->name = "SOURCE";
        // sourceResidual->type = SOURCE;
        
        // sinkResidual->id = id;
        // sinkResidual->type = SINK;
        // sinkResidual->name = "SINK";

        id = nodeGraph->minNodes;

    }

    diceFile.close();
    wordFile.close();
    return 0;
}

//function to remove dice nodes and sink nodes
void Graph::deleteHalfGraph(){
    for(int i = 0; i < nodes.size(); i++){
        if(nodes[i]->type == SOURCE){
            for(int j = 0; j < nodes[i]->adj.size(); j++){ // 
                nodes[i]->adj[j]->original = 1;
                nodes[i]->adj[j]->residual = 0;
            }
        }
        if(nodes[i]->type == DICE){  // remove edges from dice
            for(int j = 0; j < nodes[i]->adj.size(); j++){ // iterate through adj list at node
                delete nodes[i]->adj[j];
            }
            nodes[i]->adj.resize(0); 
        }
        if(nodes[i]->type == WORD){  // remove edges from word
            for(int j = 0; j < nodes[i]->adj.size(); j++){ // iterate through adj list at node
                delete nodes[i]->adj[j];
            }
            nodes[i]->adj.resize(0);
            delete nodes[i]; 
        }
        if(nodes[i]->type == SINK){  // delete sink
            delete nodes[i];
        }

        nodes.resize(minNodes);
        spellingIds.resize(0);
    }
}


int Graph::BFS(){
    Node * reverseNode;
    queue<Node*> q;
    Node *poppedNode;

    for(int i = 0; i < nodes.size(); i++){  // set all nodes visited to 0 and backEdge to NULL
        nodes[i]->visited = 0;
        nodes[i]->backEdge = NULL;
    }

    q.push(nodes[0]); // push source to queue
    //set source to visited
    nodes[0]->visited = 1;

    while(!q.empty()){
        poppedNode = q.front();  // retrieve first element from queue
        q.pop();
        poppedNode->visited = 1;

        for(int i = 0; i < poppedNode->adj.size(); i++){  //visit node's children
            if (poppedNode->adj[i]->to->visited == 0 && poppedNode->adj[i]->original == 1){
                q.push(poppedNode->adj[i]->to);
                poppedNode->adj[i]->to->backEdge = poppedNode->adj[i]->reverse;
                if (poppedNode->adj[i]->to->type == SINK){
                    
                    reverseNode = poppedNode->adj[i]->to;

                    while(reverseNode->backEdge != NULL){
                        // cout << reverseNode->id << endl;
                        // cout << "Residual: " << reverseNode->backEdge->reverse->residual << endl;
                        // cout << "Original: " << reverseNode->backEdge->reverse->original << endl;
                        // cout << endl;

                        reverseNode->backEdge->residual = 0;
                        reverseNode->backEdge->original = 1;
                        
                        reverseNode->backEdge->reverse->residual = 1;
                        reverseNode->backEdge->reverse->original = 0;

                        reverseNode = reverseNode->backEdge->to;
                        
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}