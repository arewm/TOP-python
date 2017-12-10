"""
api.py
"""
from __future__ import absolute_import
import math

"""
 Structs and such
"""
class cluster():
    def __init__(self, center, count, count_new_add, count_new_sub, distChange, label):
        self.center = center
        self.count = count
        self.count_new_add = count_new_add
        self.count_new_sub = count_new_sub
        self.distChange = distChange
        self.label = label

class vertex():
    def __init__(self, point, best_cluster, best_cluster_old, changed, lowerbounds, upbound, outofdata):
        self.point = point
        self.best_cluster = best_cluster
        self.best_cluster_old = best_cluster_old
        self.changed = changed
        self.lowerbounds = lowerbounds
        self.upbound = upbound
        self.outofdata = outofdata

class fineCat():
    def __init__(self, catType, num_landmarks):
        self.catType = catType
        self.num_landmarks = num_landmarks



"""
 Helper Functions
"""

# function to calculate the Manhattan distance between two points
# which is the sum of the linear or horizontal distance between two points
def TOP_defDistance(S):
    point_set = S
    return sum(1 for i in point_set)

# not too sure whats the meaning of this, it was in top.h
def TOP_findClosestTargets(k):
    target = k
    print(str(target))
    return False

def TOP_distance_Euclidean(a, b):
    pt_a = a
    pt_b = b
    total = 0.0
    for i in range(0,(len(pt_a)-1)):
        distance = pt_a[i] - pt_b[i]
        total = total + distance * distance
    return math.sqrt(total)

# function to calculate Euclidean Distance between points
# to simplify Triangle Inequality Calcs
def TOP_calculate_distance(l):
    pts = l
    start=pts[0]
    dist=[]
    for i in range(0,len(pts)):
        distance_training=TOP_distance_Euclidean(pts[0],pts[i])
        dist.append(distance_training)
    return dist


# add two vectors (lists)
def plus_equal_vector(a, b):
    vec_a = a
    vec_b = b
    for i in vec_a:
        vec_a[i] = vec_a[i] + vec_b[i]
    return vec_a

# subtract two vectors (lists)
def subs_equal_vector(a, b):
    vec_a = a
    vec_b = b
    for i in vec_a:
        vec_a[i] = vec_a[i] - vec_b[i]
    return vec_a

# scale a vector A by value D
def scale_vector(a, d):
    vec_a = a
    scale = d
    for i in vec_a:
        vec_a[i] = vec_a[i] * scale
    return vec_a

"""
 Definitions:
"""
def maxLandmarks(Budget,cat,L,M,Tsqrt, Qsqrt):
    maxL = 100
    return maxL

def estimateSpaceCost(Tghost,T,Q,useTset):

    space = 1000
    return 1000

# Core function
# Find the distance for our P2P implementation
def TOP_P2P_distance(graph, start):
    # initializations
    S = set()

    # delta represents the length shortest distance paths from start -> v, for v in delta.
    # We initialize it so that every vertex has a path of infinity
    delta = dict.fromkeys(list(graph.vertices),float('Inf'))
    previous = dict.fromkeys(list(graph.vertices), None)

    # then we set the path length of the start vertex to 0
    delta[start] = 0

    # while there exists a vertex v not in S
    while S != graph.vertices:
        # let v be the closest vertex that has not been visited...it will begin at 'start'
        v = min((set(delta.keys()) - S), key=delta.get)

        # for each neighbor of v not in S
        for neighbor in set(graph.edges[v]) - S:
            new_path = delta[v] + graph.weights[v, neighbor]

            # is the new path from neighbor through
            if new_path < delta[neighbor]:
                # since it's optimal, update the shortest path for neighbor
                delta[neighbor] = new_path

                # set the previous vertex of neighbor to v
                previous[neighbor] = v

        S.add(v)

    return (delta, previous)

# Core function
# Select and configure landmark definitions
# determines which landmark is suitable for a problem instance
# Parameters: Query set q, Target set T, dimensions of data d, Budget, Category cat
# distributed_platform is a boolean variable
# Figure 7 in TOP Paper
def pickLandmarkDef(Budget,cat,L,M,T,Q,distributed_platform):
    T_len=T.count # put cluster here
    Q_len=Q.count  # put points here
    d=Q[0].point.size()   # dimension of data used
    Tsqrt=math.sqrt(T_len)
    Qsqrt=math.sqrt(Q_len)
    if (cat==1):
        L=1
        if distributed_platform:
            L=2
        M=1
        Nmax=maxLandmarks(Budget,cat,L,M,Tsqrt,Qsqrt)
        if (Nmax<Qsqrt):
            M=2
        if (cat==2 or cat==3):
            useTset=False
        if (Tsqrt<(0.01*Qsqrt)):
            useTset=True
        if (cat==2):
            spaceNeeds=1000 #estimateSpaceCost(Tghost,T,Q,useTset)
        else:
            spaceNeeds = 1000 #estimateSpaceCost(Qghost, T, Q, useTset)
        L=1
        if ((spaceNeeds>Budget) or (d<1000)):
            L=2
        if (cat == 4 or cat == 3):
            spaceNeeds = 1000 # estimateSpaceCost(TQghost, T, Q)
            L=1
            if ((spaceNeeds>Budget) or (d<1000)):
                L=2
        # do this later
        #configure(Budget,cat,L,M,T,Q)
