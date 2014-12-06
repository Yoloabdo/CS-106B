/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	start = target = NULL;
	count = 0;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	Cell *cp = start;
	while(cp != NULL) {
	    Cell *temp = cp;
	    cp = cp->next;
	    delete temp;
	}
	count = 0;
	start = target = NULL;
}

int DoublyLinkedListPriorityQueue::size() {
	
	return count;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	
	return count == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	Cell *cell = new Cell;
	cell->value = value;
	cell->prev = NULL;
	cell->next = start;

	if(start != NULL) start->prev = cell;
	start = cell;
	count++;
}

string DoublyLinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Can't peek an empty queue");
	findTarget();
	return target->value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Can't dequeue an empty queue");

	findTarget();
	// removing the target from the list
	string result = target->value;
	Cell *prevCell = target->prev;
	Cell *nextCell = target->next;
	// swapping and deleting , by checking for two special cases
	// start cell and the end
	if (target == start)
	{
		if(nextCell != NULL) nextCell->prev = NULL;
		start = nextCell;
	}else if(nextCell == NULL){
		if(prevCell != NULL) prevCell->next = NULL;
	}else{
		prevCell->next = nextCell;
		nextCell->prev = prevCell;
	}

	delete target;
	count--;

	return result;
}

void DoublyLinkedListPriorityQueue::findTarget(){
	Cell *cp = start;
	target = start;
	while(cp != NULL) {
	    if(target->value > cp->value){
	    	target = cp;
	    }
	    cp = cp->next;
	}
}