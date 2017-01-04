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

