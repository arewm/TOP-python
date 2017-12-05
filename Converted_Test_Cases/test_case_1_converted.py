"""""
Converted Test Case 1 for TOP 
Point to Point - calculated the shortest distance between two points in a directed graph 
Original Test Case picked from 
source: https://www.python.org/doc/essays/graphs/
author: Guido van Rossum
"""""

from api import TOP_defDistance

def find_shortest_path(graph, start, end, path=[]):
    """
    re='https://www.python.org/doc/essays/graphs/'
    __author__='Guido van Rossum'
    """
    path = path + [start]
    if start == end:
        return path
    if not graph.has_key(start):
        return None
    shortest = None
    for node in graph[start]:
        if node not in path:
            newpath = find_shortest_path(graph, node, end, path)
            if newpath:
                if not shortest or TOP_defDistance(newpath) < TOP_defDistance(shortest):
                    shortest = newpath
    return shortest

if __name__=='__main__':
    # Defining the Graph
    graph = {
        'a': {'b', 'c'},
        'b': {'d'},
        'c': {'e'},
        'd': {'e', 'c','f'}

    }
    print(find_shortest_path(graph,'a','d'))

    print(len(find_shortest_path(graph,'a','d')))

