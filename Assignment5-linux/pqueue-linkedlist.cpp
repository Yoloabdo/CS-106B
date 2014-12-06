/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	start = NULL;
	count = 0;

}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	while(count > 0) {
	    dequeueMin();
	}
}

int LinkedListPriorityQueue::size() {
	return count;
}

bool LinkedListPriorityQueue::isEmpty() {
	return count == 0;
}

void LinkedListPriorityQueue::enqueue(string value) {
	Node *node = new Node;
	node->value = value;
	if (isEmpty())
	{
		node->next = NULL;
		start = node;
	}else if(start->value >= value){
		node->next = start;
		start = node;
	}else{
		Node *location, *prevLocation;
		location = start;
		while(location->value < value ) {
			prevLocation = location;
		    location = location->next;
		    if(location == NULL) break;
		}
		node->next = location ;
		prevLocation->next = node;
	}
	++count;
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Can't peek an empty queue");
	return start->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Can't dequeue an empty queue");
	Node *deleteNode = start;
	start = start->next;
	string value = deleteNode->value;
	delete deleteNode;
	count--;
	return value;
}

