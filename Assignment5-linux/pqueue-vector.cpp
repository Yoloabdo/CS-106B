/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	
}

VectorPriorityQueue::~VectorPriorityQueue() {
	vec.clear();
}

int VectorPriorityQueue::size() {
	return vec.size();
}

bool VectorPriorityQueue::isEmpty() {
	return vec.isEmpty();
}

void VectorPriorityQueue::enqueue(string value) {
	vec.add(value);
}

string VectorPriorityQueue::peek() {
	if(vec.isEmpty()) error("That wich doesn't exist can't be dequeued.");
	int index;
	return queueHead(index);
}

string VectorPriorityQueue::dequeueMin() {
	if(vec.isEmpty()) error("That wich doesn't exist can't be dequeued.");
	int index;
	string result = queueHead(index);
	vec.remove(index);
	return result;
}

string VectorPriorityQueue::queueHead(int& index){
	string word = vec[0];
	index = 0;
	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i] < word)
		{
			word = vec[i];
			index = i;
		}
	}
	return word;
}
