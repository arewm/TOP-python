"""
api.py
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
