# eulerian_tour.py by cubohan
# circa 2017
#
# Problem statement: Given a list of edges, output a list of vertices followed in an eulerian tour
#
# complexity analysis: O(E + V) LINEAR


def find_eulerian_tour(graph):
    edges = graph
    graph = {}
    degree = {}
    start = edges[0][0]
    count_e = 0
    for e in edges:
        if not e[0] in graph:
            graph[e[0]] = {}
        if not e[0] in degree:
            degree[e[0]] = 0
        if not e[1] in graph:
            graph[e[1]] = {}
        if not e[1] in degree:
            degree[e[1]] = 0
        graph[e[0]][e[1]] = 1
        graph[e[1]][e[0]] = 1
        degree[e[0]] += 1
        degree[e[1]] += 1
        count_e += 1
    max_d = 0
    this_ = 0
    for v, d in degree.items():
        if not d%2 == 0:
            # Eulerian tour not possible as odd degree found!
            return False 
        if d>max_d:
            this_ = v
            max_d = d
    visited_e = {}
    def is_visited(i, j):
        key = str(sorted([i,j]))
        if key in visited_e:
            return True
        else:
            visited_e[key] = True
            return False
    start = this_
    route = [start]
    indexof = {}
    indexof[start] = 0
    while count_e>0:
        flag = False
        for to_v in graph[this_]:
            if not is_visited(to_v, this_):
                route.append([to_v])
                indexof[to_v] = len(route)-1
                degree[to_v] -= 1
                if degree[to_v] == 0:
                    del degree[to_v]
                degree[this_] -= 1
                if degree[this_] == 0:
                    del degree[this_]
                this_ = to_v
                flag = True
                count_e -= 1
                break
        if not flag:
            break
    for key, v in degree.items():
        if v <=0:
            continue
        try:
            ind = indexof[key]
        except Exception as e:
            continue
        this_ = key
        while count_e>0:
            flag = False
            for to_v in graph[this_]:
                if not is_visited(to_v, this_):
                    route[ind].append(to_v)
                    degree[to_v] -= 1
                    degree[this_] -= 1
                    this_ = to_v
                    flag = True
                    count_e -= 1
                    break
            if not flag:
                break
    route_ref = []
    for r in route:
        if type(r) == list:
            for _r in r:
                route_ref.append(_r)
        else:
            route_ref.append(r)
    return route_ref

if __name__ == "__main__":
    print find_eulerian_tour([(0, 1), (1, 5), (1, 7), (4, 5),(4, 8), (1, 6), (3, 7), (5, 9),(2, 4), (0, 4), (2, 5), (3, 6), (8, 9)])
"""
This is a bit long but its worth it, trust me.

I should mention how my solution is different from that on stackoverflow, 
It's optimized for O(V+E) complexity i.e. linear in number of Edges and Vertices in the graph

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
4. You maintain a count of degrees remaining of the current vertex (This will prove useful later)
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

"""
