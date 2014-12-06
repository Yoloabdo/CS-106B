/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "hashmap.h"
#include "random.h"
#include "hashset.h"
using namespace std;


// helpers
Vector<Loc> getNeighbours(Grid<double>& world, Loc loc);
void getEdges(int width, int height, Set<Edge> & result , Vector<Set<Loc> > & clusters);
int findLocIndex(Vector<Set<Loc> > & clusters, Loc loc);

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc>
	 shortestPath(Loc start, Loc end,
					 Grid<double>& world,
					 double costFunction(Loc one, Loc two, Grid<double>& world),
					 double heuristic(Loc start, Loc end, Grid<double>& world)){
	TrailblazerPQueue<Loc> pq;
	HashMap<Loc, double> distance;
	HashMap<Loc, Color> colors;
	HashMap<Loc, Loc> parent;

	colorCell(world, start, YELLOW);
    colors.put(start, YELLOW);
	pq.enqueue(start, heuristic(start, end, world));
	distance[start] = 0;

	while(!pq.isEmpty()) {
		// The algorithm is finished, reversing the vector into a stack then returning the result.
	    Loc curr = pq.dequeueMin();
	    colorCell(world, curr, GREEN);
	    colors.put(curr, GREEN);

	    if (curr == end) {
	    	Stack<Loc> reverse;
			while(curr != start) {
			    reverse.push(curr);
			    curr = parent[curr];
			}

			Vector<Loc> res;
			res.add(curr);
			while(!reverse.isEmpty()) {
			    res.add(reverse.pop());
			}
			return res;
	    }
	    
	    foreach(Loc neigbour in getNeighbours(world, curr)){
	    	if (!colors.containsKey(neigbour))
	    	{
	    		double dist = distance[curr] + costFunction(curr, neigbour, world);
	    		colorCell(world, neigbour, YELLOW);
	    		colors.put(neigbour, YELLOW);
	    		distance.put(neigbour, dist);
	    		parent[neigbour] = curr;

	    		pq.enqueue(neigbour, dist + heuristic(neigbour, end, world));
	    	} else{
	    		if (colors[neigbour] == YELLOW)
	    		{
	    			double dist = distance[curr] + costFunction(curr, neigbour, world);
		    		if (distance[neigbour] > dist)
		    		{
		    			distance[neigbour] = dist;
		    			parent[neigbour] = curr;
		    			pq.decreaseKey(neigbour, dist + heuristic(neigbour, end, world));
		    		}
	    		}
	    	}
	    }
	}
}

/* 
* Fucnction: getNeighbours 
* --------------------------
* going through the graph in order to return the close bloks within 1 move
* same method has been applied in Boggle assignment.
*/
Vector<Loc> getNeighbours(Grid<double>& world, Loc loc){
	Vector<Loc> result;
	for (int i = loc.row - 1; i < loc.row + 2; ++i)
	{
		for (int j = loc.col - 1; j < loc.col + 2 ; ++j)
		{
			if (world.inBounds(i , j))
			{
				if (i != loc.row || j != loc.col)
				{
					result.add(makeLoc(i, j));
				}
			}
		}
	}
	return result;
}

/* 
* Fucnction: createMaze
* --------------------------
* final task in this assignment is to implement a maze generator using Kruskal's algorithm. 
* more info about it in the assignment handout
*/

Set<Edge> createMaze(int numRows, int numCols) {
	// creating the graph
	TrailblazerPQueue<Edge> pq;
	Vector<Set<Loc> > clusters;
	Set<Edge> edges;
	Set<Edge> result;

	getEdges(numRows, numCols, edges, clusters);

	// Queueuin the random cost of edges.
	foreach(Edge edge in edges){
		pq.enqueue(edge, randomReal(0, 1));
	}

	while(clusters.size() >= 2) {
	    Edge ed = pq.dequeueMin();
	    Loc start = ed.start;
	    Loc end = ed.end;
	    int headSet = findLocIndex(clusters, start);
	    cout << headSet << endl;
	    if (!clusters[headSet].contains(end))
	    {

	    	int tailSet = findLocIndex(clusters, end);
	    	cout << tailSet << endl;
	    	// merging thw two sets and clearing the second as removing it would shift items.

	    	clusters[headSet] += clusters[tailSet];
	    	clusters.remove(tailSet);

	    	// adding e to the resulting tree
	    	result.add(ed);
	    }
	}

	return result;

}

/* 
* Fucnction: findLocIndex
* --------------------------
* a simple function to find the element in vector of clusters or sets and to return it's index.
*/

int findLocIndex(Vector<Set<Loc> > & clusters, Loc loc){
	for (int i = 0; i < clusters.size(); ++i)
	{
		if (clusters[i].contains(loc))
		{
			return i;
		}
	}
}

/* 
* Fucnction: getEdges
* --------------------------
* initializing the grid and edges also nodes, using previous function used to get neighbours 
* in previous algorithm. kinda slow but yet nessecary for initializing and dealing with the Grid.
*/

void getEdges(int width, int height, Set<Edge> & result , Vector<Set<Loc> > & clusters){

	Grid<double> world;
	world.resize(width, height);
	int x = 0;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height ; ++j)
		{
			Loc startPoint = makeLoc(i, j);
			Set<Loc> set; 
			set.add(startPoint);
			clusters += set;
			foreach(Loc endPoint in getNeighbours(world, startPoint)){
				// if case here to remove digonal edges as the cost is infinite for it.
				if (endPoint.col == startPoint.col || endPoint.row == startPoint.row)
				{
					result += makeEdge(startPoint, endPoint);
				}
			}
		}
	}
}
