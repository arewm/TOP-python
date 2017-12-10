"""""
Converted Test Case 3 for TOP
K Means
Clustering query points into K groups
Test Case picked from
source: https://gist.github.com/iandanforth/5862470
http://pandoricweb.tumblr.com/post/8646701677/python-implementation-of-the-k-means-clustering
"""""

import math
import random
import time
from TOP_api.api import TOP_distance_Euclidean
from TOP_api.api import TOP_calculate_distance

def main():
    timeList = []
    for index in range(1,200):
        t1 = time.time()
        num_points = 200
        dimensions = 2

        lower = 0
        upper = 200

        num_clusters = 5
        cutoff = 0.2

        # Generate some points to cluster
        points = [
            makeRandomPoint(dimensions, lower, upper) for i in xrange(num_points)
        ]
        clusters = kmeans(points, num_clusters, cutoff)
        t2 = time.time()
        timeList.append(t2 - t1)

    totalTime = 0
    for times in timeList:
        totalTime = totalTime + times
    averageTime = totalTime / 200
    print("average completion time: " + str(averageTime))

class Point(object):
    '''
    A point in n dimensional space
    '''
    def __init__(self, coords):
        '''
        coords - A list of values, one per dimension
        '''
        self.coords = coords
        self.n = len(coords)

    def __repr__(self):
        return str(self.coords)

class Cluster(object):
    '''
    A set of points and their centroid
    '''
    def __init__(self, points):
        '''
        points - A list of point objects
        '''
        if len(points) == 0:
            raise Exception("ERROR: empty cluster")

        self.points = points
        self.n = points[0].n

        # Assert that all points are of the same dimensionality
        for p in points:
            if p.n != self.n:
                raise Exception("ERROR: inconsistent dimensions")

        # Set up the initial centroid (this is usually based off one point)
        self.centroid = self.calculateCentroid()

    def __repr__(self):
        '''
        String representation of this object
        '''
        return str(self.points)

    def update(self, points):
        '''
        Returns the distance between the previous centroid and the new after
        recalculating and storing the new centroid.
        Note: Initially we expect centroids to shift around a lot and then
        gradually settle down.
        '''
        old_centroid = self.centroid
        self.points = points
        self.centroid = self.calculateCentroid()
        # TODO: Replace with TOP call
        # shift = getDistance(old_centroid, self.centroid)
        shift = TOP_distance_Euclidean(old_centroid.coords, self.centroid.coords)
        return shift

    def calculateCentroid(self):
        '''
        Finds a virtual center point for a group of n-dimensional points
        '''
        numPoints = len(self.points)
        # Get a list of all coordinates in this cluster
        coords = [p.coords for p in self.points]
        # Reformat that so all x's are together, all y'z etc.
        unzipped = zip(*coords)
        # Calculate the mean for each dimension
        # TODO: Replace with TOP function, or not? its just mean
        centroid_coords = [math.fsum(dList)/numPoints for dList in unzipped]

        return Point(centroid_coords)

def kmeans(points, k, cutoff):
    initial = random.sample(points, k)

    # Create k clusters using those centroids
    # Note: Cluster takes lists, so we wrap each point in a list here.
    clusters = [Cluster([p]) for p in initial]

    # Loop through the dataset until the clusters stabilize
    loopCounter = 0
    while True:
        # Create a list of lists to hold the points in each cluster
        lists = [[] for _ in clusters]
        clusterCount = len(clusters)

        loopCounter += 1
        # goes through every point and assigns closest centroid
        for p in points:
            # Get the distance between that point and the centroid of the first cluster.
            # TODO: replace with TOP Call
            # old: smallest_distance = getDistance(p, clusters[0].centroid)
            smallest_distance = TOP_distance_Euclidean(p.coords, clusters[0].centroid.coords)

            clusterIndex = 0
            for i in range(clusterCount - 1):
                # calculate the distance of that point to each other cluster's
                # centroid.
                # TODO: replace with TOP function call
                # old: distance = getDistance(p, clusters[i+1].centroid)
                distance = TOP_distance_Euclidean(p.coords, clusters[i+1].centroid.coords)

                if distance < smallest_distance:
                    smallest_distance = distance
                    clusterIndex = i+1
            lists[clusterIndex].append(p)

        biggest_shift = 0.0
        for i in range(clusterCount):
            # Calculate how far the centroid moved in this iteration
            shift = clusters[i].update(lists[i])
            biggest_shift = max(biggest_shift, shift)

        if biggest_shift < cutoff:
            print "Converged after %s iterations" % loopCounter
            break
    return clusters

# TODO: This will all be replaces by TOP calls
def getDistance(a, b):
    if a.n != b.n:
        raise Exception("ERROR: non comparable points")

    accumulatedDifference = 0.0
    for i in range(a.n):
        squareDifference = pow((a.coords[i]-b.coords[i]), 2)
        accumulatedDifference += squareDifference
    distance = math.sqrt(accumulatedDifference)

    return distance

def makeRandomPoint(n, lower, upper):
    p = Point([random.uniform(lower, upper) for _ in range(n)])
    return p

if __name__ == "__main__":
    main()
