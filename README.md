# Algorithms in python

### Graphs:
    #### 1. eulerian_tour.py:
                ##### Problem: 
                        - Given a list of edges in graph, output the list of vertices to be traversed in an  Eulerian tour or else ouput False
                ##### Complexity: 
                        - O(V+E) LINEAR


This solution is optimized for O(V+E) complexity i.e. linear in number of Edges and Vertices in the graph

Those directly wishing to see the code, go here:
https://github.com/cubohan/py-algos/blob/master/eulerian_tour.py

First the problem can be divided into these subtasks: 
    1. Build the graph into a more friendly structure than a list of edges for easier processing 
        i.e (Adjacency Lists)
    2. Find the degrees of each vertex to first check if an Eulerian Tour is possible 
        (Are there only even degrees? Also, STORE these values in a dict with vertex as key => to be used later) 
    3. Build the Eulerian tour

The concept behind my solution is simple. 

1. You choose the vertex with the highest degree as a starting point and set it as the current vertex. 
    (Note: You accomplish this at the same time while calculating degrees of every vertex. You store all these degrees in a dictionary.)
2. You insert current vertex in the route list which is your answer
    (Note: also make a dictionary of vertices and their indexes in the route list. This will be used later.)
3. You visit the first edge of the current vertex if it's not already visited. 
    (Note: A dictionary of visited edges is maintained, the key to this dict is a sorted tuple of the pair of vertices constituting the edge. After visiting an edge, mark it visited by inserting it into the dict.)
4. You maintain a count of degrees remaining of the current vertex and the visited vertex (This will prove useful later)
    (Note: you only need to subtract 1 from the dict of degrees you generate before each time you choose an edge)
5. You switch current vertex to the vertex on the other end of the edge you decided to visit.
6. Repeat steps 2-5 until you can't find an unvisited edge in the current vertex. 
    (Note: this means you have returned to your starting vertex)

Now consider this:
Notice that any unvisited edges/vertices will constitute subgraphs in the main graph which have the same properties as the main graph i.e. an Eulerian tour is possible from any of the vertices in the subgraph starting and ending at the same vertex.  
All unvisted edges can be thus visited by taking Eulerian tours in these subgraphs
You just need to merge these sub-tours with the first tour.

Next:

7. You loop through all the vertices of the graph and build a subtour in the same process as listed for the main tour if and only if the reduced degree of this vertex is non-zero

8. The way these tours will merge with the route list computed previously is that you replace the position of the vertex you're considering to start a subtour from in the route list with the subtour output list and later flattening this route list

We're still not done!

Can you guess what's wrong above?

What happens when you get a non-zero degree vertex which HASN'T BEEN VISITED and isn't present in the route list?!

A caveat:
This is an exceptional case: You might encounter vertices not visited before and thus they will not be present
in the main route list. IGNORE THESE while looping! One of the vertices you have already visited with non-zero reduced degree is GUARANTEED to lead to these vertices in the subtour you will create starting from those.

How is that possible??!!
Draw a graph from the test case given in the link to the code and you will understand. Trace out what your algo is doing every step of the process. Draw it! An image is log(N) complexity to understanding and words O(n2).

Ah, and note that this guarantee holds only if all the edges in the input list form a single graph and not two separate disjointed graphs.

