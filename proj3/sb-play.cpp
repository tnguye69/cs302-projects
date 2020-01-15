#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include "disjoint.h"
using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s) {
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv){
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////
/*Function that finds a potential swap cell (either green or red)*/
int findSwapCell(char color, vector<int> &v, int row, int col){
  int returnVal = -1;
  if(color == 'g'){   // search for green cells on right side of grid
    for(int i = 0; i < row; i++){
      for(int j = 5; j < col; j++){
        if(v[i*col+j] == 'g'){
          returnVal = i*col+j;
        }  
      }
    }
  }
  else if(color == 'r'){  // search for red cells on left side of grid
    for(int i = 0; i < row; i++){
      for(int j = 0; j < (col/2); j++){
        if(v[i*col+j] == 'r'){
          returnVal = i*col+j;
        }
      }
    }
  }
  return returnVal;          
}

/*Function that returns adjacent cells with different color given index*/
int findAdjacentCell(char color, int index, vector<int> &v, int row, int col){
  int left, right, top, bottom;

  //LEFT
  left = index-1;
  if(index%col != 0){
    if(v[left]!=color && v[left]!='.' && v[left]!='*'){
      return left;
    }
  }
  //RIGHT
  right = index+1;
  if(right%col != 0){
    if(v[right]!=color && v[right]!='.' && v[right]!='*'){
      return right;
    }
  }
  //TOP
  top = index-col;
  if(top >= 0){
    if(v[top]!=color && v[top]!='.' && v[top]!='*'){
      return top;
    }
  }
  bottom = index+col;
  if(bottom < row*col){
    if(v[bottom]!=color && v[bottom]!='.' && v[bottom]!='*'){
      return bottom;
    }
  }
    return -1;
}

/*This function returns an index of a given color that is the RIGHTMOST cell*/
int findRightMostColor(char color, vector<int> &v, int row, int col){
  int returnVal = -1;
  for(int j = 0; j < col; j++){
    for(int i = 0; i < row; i++){
      if(v[i*col+j] == color){
        returnVal = i*col+j;
      }
    }
  }
  return returnVal;
}

/*This function returns an index of a given color that is the LEFTMOST cell*/
int findLeftMostColor(char color, vector<int> &v, int row, int col){
  int returnVal = -1;
  for(int j = col-1; j >= 0; j--){
    for(int i = 0; i < row; i++){
      if(v[i*col+j] == color){
        returnVal = i*col+j;
      }  
    }
  }
  return returnVal;
}

/******************************************************************/
/* MAIN FUNCTION */
/******************************************************************/
int main(int argc, char **argv){
  Superball *s;
  DisjointSet *d;
 
  s = new Superball(argc, argv);
  d = new DisjointSetByRankWPC(s->r * s->c);

  int cells = s->r * s->c;    // cells are row*columns
  int left, right, bottom, top, currParent, newParent;

  for(int curr = 0; curr < cells; curr++){  // iterate through grid
    /*check if RIGHT is valid*/
    right = curr+1;
    if(right % s->c != 0){
      if(s->board[curr] == s->board[right]){   // check if right is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(right);
        if(currParent != newParent){   // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }     
    }
    /*check if BOTTOM is valid*/
    bottom = curr + s->c;
    if(curr+s->c < cells){
      if(s->board[curr] == s->board[bottom]){   // check if bottom is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(bottom);
        if(currParent != newParent){   // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }
    }
  }
  for(int i = 0; i < cells; i++){  // path compress the sets
    d->Find(i);
  }

  vector<int> scores;  //scores vector will hold scores for each set
  scores.resize(cells);
  int findVal;
  for(int i = 0; i < cells; i++){ // iterate through grid and update scores vector based on findVal
    findVal = d->Find(i);
    if(findVal == -1){   // parent
      scores[i]++;
    }
    else{   // increment score by 1 at parent index
      scores[findVal]++;
    }
  }
//////////////////////////////////////////////////////////
/*SCORE IF YOU CAN*/
  for(int i = 0; i < cells; i++){
    findVal = d->Find(i);
      // if(s->goals[i]==1 && scores[findVal]>=s->mss && s->board[i]!='.' && s->board[i]!='*'){  // call score if size is >= findVal
      //   cout << "SCORE " << i/(s->c) << " " << i%(s->c) << endl;
      // }
      if(s->goals[i]==1 && scores[findVal]>=s->mss && s->board[i]!='.' && s->board[i]!='*' && s->empty <= 5){
        cout << "SCORE " << i/(s->c) << " " << i%(s->c) << endl;
      }
  }

//find green on right side
int greenSwap, adjacentGreen;
greenSwap = findRightMostColor('g', s->board, s->r, s->c);
if(greenSwap != -1){
  for(int j = 0; j < s->c/2; j++){   // move green to left side (goal post only)
    for(int i = 0; i < s->r; i++){
      if(s->goals[i*s->c+j]==1){
        if(s->board[i*s->c+j] == 'g'){  // find adjacent tiles of green that are diff color
          adjacentGreen = findAdjacentCell('g', i*s->c+j, s->board, s->r, s->c);
          if(adjacentGreen!=-1 && adjacentGreen!=greenSwap){  
            printf("SWAP %d %d %d %d\n", adjacentGreen/s->c, adjacentGreen%s->c, greenSwap/s->c, greenSwap%s->c);
          }
        }
        else if(s->board[i*s->c+j]!='*' && i*s->c+j != greenSwap){ // find open goal post to insert green
          printf("SWAP %d %d %d %d\n", i, j, greenSwap/s->c, greenSwap%s->c);
        }
      }
    }
  }
  for(int j = 0; j < 5; j++){   // move green to left side (non goal post & adjacency)
    for(int i = 0; i < s->r; i++){
      if(s->board[i*s->c+j] == 'g'){  // find adjacent tiles of green that are diff color
        adjacentGreen = findAdjacentCell('g', i*s->c+j, s->board, s->r, s->c);
        if(adjacentGreen!=-1 && adjacentGreen!=greenSwap){  
          printf("SWAP %d %d %d %d\n", adjacentGreen/s->c, adjacentGreen%s->c, greenSwap/s->c, greenSwap%s->c);
        }
      }
    }
  }
  for(int j = 0; j < 5; j++){   // move green to left side (non goal post)
    for(int i = 0; i < s->r; i++){
      if(s->board[i*s->c+j]!='*' && s->board[i*s->c+j]!='.' && s->board[i*s->c+j]!='g' && i*s->c+j!=greenSwap){
        printf("SWAP %d %d %d %d\n", i, j, greenSwap/s->c, greenSwap%s->c);
      }
    }
  }
}

//find red on left side
int redSwap, adjacentRed;
redSwap = findLeftMostColor('r', s->board, s->r, s->c);
if(redSwap != -1){
  for(int j = s->c-1; j > s->c/2; j--){   // move red to right side (goal post only)
    for(int i = 0; i < s->r; i++){
      if(s->goals[i*s->c+j]==1){
        if(s->board[i*s->c+j] == 'r'){  // find adjacent tiles of red that are diff color
          adjacentRed = findAdjacentCell('r', i*s->c+j, s->board, s->r, s->c);
          if(adjacentRed!=-1 && adjacentRed!=redSwap){  
            printf("SWAP %d %d %d %d\n", adjacentRed/s->c, adjacentRed%s->c, redSwap/s->c, redSwap%s->c);
          }
        }
        else if(s->board[i*s->c+j]!='*' && i*s->c+j != redSwap){ // find open goal post to insert red
          printf("SWAP %d %d %d %d\n", i, j, redSwap/s->c, redSwap%s->c);
        }
      }
    }
  }
  for(int j = s->c-1; j > s->c/2; j--){   // move red to right side (non goal post & adjacency)
    for(int i = 0; i < s->r; i++){
      if(s->board[i*s->c+j] == 'r'){  // find adjacent tiles of red that are diff color
        adjacentRed = findAdjacentCell('r', i*s->c+j, s->board, s->r, s->c);
        if(adjacentRed!=-1 && adjacentRed!=redSwap){  
          printf("SWAP %d %d %d %d\n", adjacentRed/s->c, adjacentRed%s->c, redSwap/s->c, redSwap%s->c);
        }
      }
    }
  }
  for(int j = s->c-1; j > s->c/2; j--){   // move red to right side (non goal post)
    for(int i = 0; i < s->r; i++){
      if(s->board[i*s->c+j]!='*' && s->board[i*s->c+j]!='.' && s->board[i*s->c+j]!='r' && i*s->c+j!=redSwap){
        printf("SWAP %d %d %d %d\n", i, j, redSwap/s->c, redSwap%s->c);
      }
    }
  }
}

//swap random
int swapCell1, swapCell2;
for(int i = 0; i < cells; i++){
  if(s->board[i]!='*' && s->board[i]!='.'){
    swapCell1 = i;
  }
}
for(int i = 0; i < cells; i++){
  if(s->board[i]!='*' && s->board[i]!='.' && i!=swapCell1){
    swapCell2 = i;
  }
}
printf("SWAP %d %d %d %d\n", swapCell1/s->c, swapCell1%s->c, swapCell2/s->c, swapCell2%s->c);

}
