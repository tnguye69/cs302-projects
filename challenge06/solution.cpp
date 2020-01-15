// Challenge 06: Palindrome Permutation
#include <iostream>
#include <string>
#include <unordered_set>
#include <stdio.h>
using namespace std;

//function to check if a string is a palindrome permutation
bool is_palindrome(string s){
  unordered_set<char> palindromeSet;
  unordered_set<char>::iterator setIter;

  for(int i = 0; i < s.size(); i++){   // iterate through every character
    if(s[i]>=65){  // make sure a char is a letter
      setIter = palindromeSet.find(s[i]);   
      if(setIter != palindromeSet.end()){  // if same character is found then erase   
        palindromeSet.erase(s[i]);
      }
      else{   // else insert character
        palindromeSet.insert(s[i]);
      }
    }
  }

  if(palindromeSet.size() <= 1){  // if no more than 1 char appears odd number of times
    return true;
  }
  else{   // else it's false
    return false;
  }
}

/* Main Function */
int main(int argc, char *argv[]) {
  bool check;
  string input;

  while(getline(cin, input)){  // read in strings
    check = is_palindrome(input);
    if(check == true){  // string is a palindrome
       printf("\"%s\" is a palindrome permutation\n", input.c_str());
    }
    else{   // string is not a palindrome
      printf("\"%s\" is not a palindrome permutation\n", input.c_str());
    }
  }
  return 0;
}

