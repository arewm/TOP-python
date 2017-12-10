"""""
Test Case 1 for TOP 
Point to Point - calculated the shortest distance between two points in a directed graph 
Original Test Case picked from 
original source: https://github.com/alexhwoods/alexhwoods.com/blob/master/Algorithms/Dijkstra/dijkstra.py
Brute Force Algorithm implemented 


"""""

import collections
import math
import time
from api import TOP_distance_Euclidean # Adding TOP function
from api import TOP_defDistance
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

def shortest_path(graph, start, end,path=[],weight=0):
    path=path=[start]
    weights = [float('inf')]
    #vertices = dict.fromkeys(list(graph.vertices), None)
    edges = (graph.edges)
    Weights = graph.weights

    if start==end:
        return min(weights)

    if not edges.has_key(start):
        return 0


    for node in edges[start]:
        if (start==node):
            return min(weights)
        if (Weights.has_key((start, node)) and node !=end):
            weight = weight + Weights[start, node]

        if node==end:
             weight = weight + Weights[start, node]

             weights.append(weight)

             return(min(weights))

        else:
            weight = weight + shortest_path(graph,node,end,weight)
        weights.append(weight)

    return min(weights)



          #print("found")
          #if (node not in path ):
           # newpath = shortest_path(graph, node, end, path,weight)
           # if (weights.has_key((node,end))):
           #    weight=weight+weights[node,end]
          #  if newpath:
           #     if not shortest or len(newpath) < len(shortest):
           #         shortest = newpath


def main():
    start_time =  time.time()
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
    print("--- %s seconds ---" % ((time.time() - start_time)))
    #print(G)
    print(shortest_path(G, 'a', 'e'))



main()
