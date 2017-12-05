"""
api.py
"""
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

# function to calculate the Manhattan distance between two points
# which is the sum of the linear or horizontal distance between two points
def TOP_defDistance(S):
    return sum(1 for i in S)

# not too sure whats the meaning of this, it was in top.h
def TOP_findClosestTargets(k):
    print(str(k))
    return false

"""
 Helper Functions
"""
# a, b are lists of doubles
def distance_Euclidean(a, b):
    total = 0.0
    for i in a:
        distance = a[i] - b[i]
        total = total + distance * distance
    return math.sqrt(total)

# add two vectors (lists)
def plus_equal_vector(a, b):
    for i in a:
        a[i] = a[i] + b[i]
    return a

# subtract two vectors (lists)
def subs_equal_vector(a, b):
    for i in a:
        a[i] = a[i] - b[i]
    return a

# scale a vector A by value D
def scale_vector(a, d):
    for i in a:
        a[i] = a[i] * d
    return a

"""
 Definitions:
"""
def maxLandmarks(Budget,cat,L,M,Tsqrt, int Qsqrt):
    maxL = 100
    return maxL

def estimateSpaceCost(Tghost,T,Q,useTset):
    space = 1000
    return 1000

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
