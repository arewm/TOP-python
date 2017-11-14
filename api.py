"""
api.py
"""
import math

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

# not too sure whats the meaning of this, it was in top.h
def TOP_defDistance(defDist):
    print(str(defDist))

# not too sure whats the meaning of this, it was in top.h
def TOP_findClosestTargets(k):
    print(str(k))
    return false

# a, b are lists of doubles
def distance_Euclidean(a, b):
    total = 0.0
    for i in range(0, a):
        distance = a[i] - b[i]
        total = total + distance * distance
    return math.sqrt(total)
