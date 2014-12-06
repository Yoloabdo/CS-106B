/*
 * File: NumericConversion.cpp
 * ---------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the numeric-conversion problem
 * in which you implement the functions intToString and stringToInt.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include "simpio.h"
#include <cmath>
using namespace std;

/* Function prototypes */

string intToString(int n);
int stringToInt(string str);
string intToStringW(int n);
/* Main program */

int main() {

	
	int entered = getInteger("Enter an integer: ");
	cout << "your string is: " << intToStringW(entered) << endl;
	string test = getLine("Enter your string numbers: ");
	cout << stringToInt(test) << endl;
}

string intToString(int n){
		if (n == 0)return string();
		else return intToString(n/10) + char(n%10 + '0');
}


string intToStringW(int n){
	if(n == 0) return "0";
	else if(n < 0) return "-" + intToString(-n);
	else return intToString(n);
}

int stringToInt(string n){
	if(n.length() == 1) return n[0] - '0';
	else return (n[0] - '0') * pow(10.0, int(n.length()-1))+ stringToInt(n.substr(1));
}