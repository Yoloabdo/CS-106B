/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

HeapPriorityQueue::HeapPriorityQueue() {
	capacity = INITIAL_SIZE;
	array = new string[capacity];
	count = 0;
	array[0] = "emptyCell";
	for (int i = 1; i < capacity; ++i)
	{
		array[i] = "";
	}
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete [] array;
}

int HeapPriorityQueue::size() {
	return count;
}

bool HeapPriorityQueue::isEmpty() {
	return count == 0;
}

void HeapPriorityQueue::enqueue(string value) {
	if(++count == capacity) expandCapacity(); // expand array
	array[count] = value;
	bubbleUp(count);
}

string HeapPriorityQueue::peek() {
	if (isEmpty()) error("Can't peek an empty queue");
	// string result;
	// for (int i = 0; i < capacity; ++i)
	// {
	// 	result += array[i] + " , ";
	// }
	//return the top of the tree.
	return array[1];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Can't dequeue an empty queue");
	// minimum element of the binary heap is atop the heap
	string result = array[1];
	// swap the top of the heap for the very last node in the heap
	swapNodes(1, count);
	// remove the item from the heap
	array[count] = "";
	// reduce count bubble-down step and repeatedly swap the node 
	// with its smaller child until it comes to rest.
	count--;

	bubbleDown(1);
	
	return result;
}

void HeapPriorityQueue::bubbleUp(int n){
	while(n/2 > 0) {
		if (array[n/2] > array[n])
		{
			swapNodes(n/2, n);
			n = n/2;
		}else{
			break;
		}
	}
}

void HeapPriorityQueue::bubbleDown(int n){
	if (n * 2 > size()) return;
	int firstChild = 2 * n;
	int secondChild = 2 * n + 1;
	if (array[firstChild] < array[n])
	    {
	    	if (array[firstChild] != "")
	    	{
	    		swapNodes(firstChild, n);
	    		bubbleDown(firstChild);
	    	}
    }
    if (array[secondChild] < array[n])
    {
    	if (array[secondChild] != "")
    	{
    		swapNodes(secondChild, n);
    		bubbleDown(secondChild);
    	}
    }
}

void HeapPriorityQueue::swapNodes(int first, int second){
	string tmp = array[first];
	array[first] = array[second];
	array[second] = tmp;
}

void HeapPriorityQueue::expandCapacity(){
	string *oldArray = array;
	capacity = 2 * capacity;
	array = new string[capacity];

	//intializing the empty array
	for (int i = 0; i < capacity; ++i)
	{
		array[i] = "";
	}
	//copying the old one
	for (int i = 0; i < count; ++i)
	{
		array[i] = oldArray[i];
	}
	delete[] oldArray;
}