#include <ctype.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include "disjoint.h"
using namespace std;

#define talloc(type, num) (type *)malloc(sizeof(type) * (num))

class Superball {
 public:
  Superball(int argc, char **argv);
  int r;
  int c;
  int mss;
  int empty;
  vector<int> board;
  vector<int> goals;
  vector<int> colors;
};

void usage(const char *s) {
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv) {
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
    colors[argv[4][i]] = 2 + i;
    colors[toupper(argv[4][i])] = 2 + i;
  }

  board.resize(r * c);
  goals.resize(r * c, 0);

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
      board[i * c + j] = s[j];
      if (board[i * c + j] == '.') empty++;
      if (board[i * c + j] == '*') empty++;
      if (isupper(board[i * c + j]) || board[i * c + j] == '*') {
        goals[i * c + j] = 1;
        board[i * c + j] = tolower(board[i * c + j]);
      }
    }
  }
}

int main(int argc, char **argv) {
  Superball *s;
  DisjointSet *d;
  s = new Superball(argc, argv);
  d = new DisjointSetByRankWPC(s->r * s->c);

  int cells = s->r * s->c;  // cells are row*columns
  int left, right, bottom, top, currParent, newParent;

  for (int curr = 0; curr < cells; curr++) {  // iterate through grid
    /*check if LEFT is valid*/
    left = curr - 1;
    if (curr % s->c != 0) {
      if (s->board[curr] ==  s->board[left]) {  // check if left is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(left);
        if (currParent != newParent) {  // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }
    }
    /*check if RIGHT is valid*/
    right = curr + 1;
    if (right % s->c != 0) {
      if (s->board[curr] == s->board[right]) {  // check if right is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(right);
        if (currParent != newParent) {  // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }
    }
    /*check if BOTTOM is valid*/
    bottom = curr + s->c;
    if (curr + s->c < cells) {
      if (s->board[curr] == s->board[bottom]) {  // check if bottom is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(bottom);
        if (currParent != newParent) {  // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }
    }
    /*check if TOP is valid*/
    top = curr - s->c;
    if (curr - s->c >= 0) {
      if (s->board[curr] == s->board[top]) {  // check if top is same color as current
        currParent = d->Find(curr);
        newParent = d->Find(top);
        if (currParent != newParent) {  // if parent's are different combine sets
          d->Union(currParent, newParent);
        }
      }
    }
  }

  for (int i = 0; i < cells; i++) {  // path compress the sets
    d->Find(i);
  }

  vector<int> scores;  // scores vector will hold scores for each set
  scores.resize(cells);
  int findVal;
  for (int i = 0; i < cells; i++) {  // iterate through grid and update scores vector based on findVal
    findVal = d->Find(i);
    if (findVal == -1) {  // parent
      scores[i]++;
    } else {  // increment score by 1 at parent index
      scores[findVal]++;
    }
  }

  /******PRINT******/
  cout << "Scoring sets:" << endl;
  vector<int> finalScores;  // finalScores vector will make sure repeated sets aren't printed again
  vector<int>::iterator it;
  int row, col;
  for (int i = 0; i < cells; i++) {  // iterate through grid but this time only for goal cells
    if (s->goals[i] == 1 && s->board[i] != '.' && s->board[i] != '*') {
      findVal = d->Find(i);
      if (scores[findVal] >= s->mss) {  // print score if size is >= mss
        for (it = finalScores.begin(); it != finalScores.end(); it++) {  // check if same set has already been printed
          if (*it == findVal) {
            break;
          }
        }
        if (it == finalScores.end()) {  // print if a different set has been found
          finalScores.push_back(findVal);
          row = i / s->c;
          col = i % s->c;
          printf("  Size:%*d  Char:%*c  Scoring Cell: %d,%d\n", 3, scores[findVal], 2, (char)(s->board[i]), row, col);
        }
      }
    }
  }
}
