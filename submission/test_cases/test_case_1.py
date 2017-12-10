"""""
Test Case 1 for TOP
Point to Point - calculated the shortest distance between two points in a directed graph
Test Case picked from
source: https://www.python.org/doc/essays/graphs/
author: Guido van Rossum
Assumption : No Negative edges in Graph
             Using Directed Acyclic Graph

Changes made : es001 - Remove in built python len function
"""""
import time
# adding function for distance calculation --es001
def len_path(S):
    return sum(1 for i in S)


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
                if not shortest or len_path(newpath) < len_path(shortest): #es001
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
    
    timeList = []
    for i in range(1,200):
        t1 = time.time()
        print(find_shortest_path(graph,'a','d'))
        t2 = time.time()
        timeList.append(t2 - t1)

    totalTime = 0
    for times in timeList:
        totalTime = totalTime + times
    averageTime = totalTime / 200
    print("average completion time: " + str(averageTime))

    #print(len(find_shortest_path(graph,'a','d')))
