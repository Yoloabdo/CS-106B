/*
 * File: Combinations.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the recursive combinations problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include "simpio.h"
#include <iomanip>
using namespace std;



int pascalTri(int n, int k);
int printTri();
void space(int x);

int main() {
	cout << "Hello, this application do recursive to draw pascal triangle" << endl;
	int n = getInteger("Enter n: ");
	int k = getInteger("Enter k: ");
	cout << "Your result is: "<<pascalTri(n,k) << endl << "now check the triangle" << endl;
	cout << string( 50, '-') << endl;
	printTri();
   return 0;
}

int pascalTri(int n, int k){
	if (k == 0 || n == k){
		return 1;
		}
	else return pascalTri(n-1, k-1) + pascalTri(n-1, k);


	}

int printTri(){
	for(int i =0; i <10; i++){
		space(9-i);
		for(int j = 0; j <= i; j++){
			cout << setw(3) <<pascalTri(i, j) << "   ";
			}
		cout << endl;
		}

	return 0;
	}
void space(int x){
	for (int i = 0; i <= x; i++)
		cout << setw(3) << "";
	}
