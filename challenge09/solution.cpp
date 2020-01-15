// Challenge 9: Sequence alignment

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//scoring system variables
int match = 1;
int mismatch = -1;
int gap = -1; 

// Main Execution
int main(int argc, char *argv[]) {
  
  vector<vector<int> > matrix;
  int topVal, leftVal, diagonalVal, finalScore;
  
  string sequence1, sequence2;    // read in sequences
  cin >> sequence1 >> sequence2;  // sequence1 is first row & sequence2 is first column
  
  matrix.resize(sequence2.size()+2);  // resize matrix
  for(int i = 0; i < matrix.size(); i++){
    matrix[i].resize(sequence1.size()+2);
  }

  for(int col = 2; col < matrix[0].size(); col++){  // read in seq1 as first row
    matrix[0][col] = sequence1[col-2];
  }
  for(int row = 2; row < matrix.size(); row++){   // read in seq2 as first column
    matrix[row][0] = sequence2[row-2];
  }

  for(int col = 2; col < matrix[0].size(); col++){  // read in values for second row
    matrix[1][col] = matrix[1][col-1] + gap;   
  }
  for(int row = 2; row < matrix.size(); row++){  // read in values for second column
    matrix[row][1] = matrix[row-1][1] + gap;
  }

  for(int row = 2; row < matrix.size(); row++){   // read in values for rest of the matrix
    for(int col = 2; col < matrix[row].size(); col++){
      
      //FROM THE TOP
      topVal = matrix[row-1][col] + gap;
      
      //FROM THE LEFT
      leftVal = matrix[row][col-1] + gap;

      //FROM THE DIAGONAL
      if(matrix[0][col] == matrix[row][0]){  // if the values match
        diagonalVal = matrix[row-1][col-1] + match;
      }
      else{    // if the values dont match
        diagonalVal = matrix[row-1][col-1] - 1;
      }

      finalScore = max(topVal, leftVal);    // find the max of the top, left, and diagonal 
      matrix[row][col] = max(finalScore, diagonalVal);
    }
  }

  cout << matrix.back().back() << endl;

  // for(int i=0; i < matrix.size(); i++){
  //   for(int j=0; j < matrix[i].size(); j++){
  //     cout << matrix[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  
  return 0;

}

