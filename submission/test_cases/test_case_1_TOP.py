"""""
Converted Test Case 1 for TOP
Point to Point - calculated the shortest distance between two points in a directed graph
Original Test Case picked from
source: https://github.com/alexhwoods/alexhwoods.com/blob/master/Algorithms/Dijkstra/dijkstra.py
author: Alex H Woods
Djikstra implements Triangle Inequality
"""""

# graph class inspired by https://gist.github.com/econchick/4666413
# only run this in python 3!!! (because 'math.inf' is used)
import collections
import math
import time

from TOP_api.api import TOP_P2P_distance

class Graph:
    def __init__(self):
        self.vertices = set()

        # makes the default value for all vertices an empty list
        self.edges = collections.defaultdict(list)
        self.weights = {}

    def add_vertex(self, value):
        self.vertices.add(value)

    def add_edge(self, from_vertex, to_vertex, distance):
        if from_vertex == to_vertex: pass  # no cycles allowed
        self.edges[from_vertex].append(to_vertex)
        self.weights[(from_vertex, to_vertex)] = distance

    def __str__(self):
        string = "Vertices: " + str(self.vertices) + "\n"
        string += "Edges: " + str(self.edges) + "\n"
        string += "Weights: " + str(self.weights)
        return string

def shortest_path(graph, start, end):
    # rename the Djikstra to TOP
    #Djikstra implements Triangle Inequality
    delta, previous = TOP_P2P_distance(graph, start)

    path = []
    vertex = end

    while vertex is not None:
        path.append(vertex)
        vertex = previous[vertex]

    path.reverse()
    return path

def main():
    G = Graph()
    G.add_vertex('a')
    G.add_vertex('b')
    G.add_vertex('c')
    G.add_vertex('d')
    G.add_vertex('e')

    G.add_edge('a', 'b', 2)
    G.add_edge('a', 'c', 8)
    G.add_edge('a', 'd', 5)
    G.add_edge('b', 'c', 1)
    G.add_edge('c', 'e', 3)
    G.add_edge('d', 'e', 4)

    timeList = []
    for i in range(1,200):
        t1 = time.time()
        print(shortest_path(G, 'a', 'e'))
        t2 = time.time()
        timeList.append(t2 - t1)

    totalTime = 0
    for times in timeList:
        totalTime = totalTime + times
    averageTime = totalTime / 200
    print("average completion time: " + str(averageTime))

main()
