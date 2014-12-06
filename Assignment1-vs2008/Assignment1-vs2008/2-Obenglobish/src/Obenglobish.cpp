/*
 * File: Obenglobish.java
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Obenglobish problem.
 * 
 */

#include <iostream>
#include <string>
#include "simpio.h"
#include "strlib.h"
using namespace std;


string obenglobish(string word);
bool isVowel(char ch);
/* Main program */

int main() {
	while(true){
		string word = getLine("Enter a word: ");
		if (word =="") break;
		string trans = obenglobish(word);
		cout << word << " -> " << trans << endl;
	}

   return 0;
}


string obenglobish(string word){
	string pre = "ob";
	string res = word;
	// to count correct insert pos
	int c = 0;
	int len = res.length();
	for(int i = 0; i < len; i++)
		if(isVowel(word[i])){
			if(i == len -1) return res;
			if(isVowel(word[i+1])) continue;
			else {
				res.insert(i+(2*c), pre);
				c++;
				}
			}
		return res;
	}

bool isVowel(char ch){
	switch(ch){
		case 'A' : case 'E': case 'I': case'O': case 'U':
		case 'a': case 'e': case 'i': case 'o': case 'u':
			return true;
		default:
			return false;
	}
}