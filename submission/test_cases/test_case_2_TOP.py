"""""
Converted Test Case 2 for TOP
KNN: K nearest Neighbours
Finding the K nearest neighbors for a point
Original Test Case picked from
source: https://machinelearningmastery.com/tutorial-to-implement-k-nearest-neighbors-in-python-from-scratch/
source: https://machinelearningmastery.com/machine-learning-in-python-step-by-step/
"""""

# Example of kNN implemented from Scratch in Python

import csv
import random
import math
import time
import operator
from TOP_api.api import TOP_calculate_distance
from TOP_api.api import TOP_getNeighbors

def loadDataset(filename, split, trainingSet=[], testSet=[]):
    with open(filename, 'rb') as csvfile:
        lines = csv.reader(csvfile)

        dataset = list(lines)
        for x in range(len(dataset) - 1):
            for y in range(4):
                dataset[x][y] = float(dataset[x][y])
            if random.random() < split:
                trainingSet.append(dataset[x])
            else:
                testSet.append(dataset[x])

def getResponse(neighbors):
    classVotes = {}
    for x in range(len(neighbors)):
        response = neighbors[x][-1]
        if response in classVotes:
            classVotes[response] += 1
        else:
            classVotes[response] = 1
    sortedVotes = sorted(classVotes.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedVotes[0][0]


def getAccuracy(testSet, predictions):
    correct = 0
    for x in range(len(testSet)):
        if testSet[x][-1] == predictions[x]:
            correct += 1
    return (correct / float(len(testSet))) * 100.0


def main():
    # prepare data
    trainingSet = []
    testSet = []
    split = 0.67
    loadDataset('./iris.data', split, trainingSet, testSet)
    print 'Train set: ' + repr(len(trainingSet))
    print 'Test set: ' + repr(len(testSet))

    timeList = []
    for index in range(1,200):
        t1 = time.time()
        # generate predictions
        predictions = []
        k = 3
        distance_training_top=TOP_calculate_distance(trainingSet)
        #print(distance_training)
        for x in range(len(testSet)):
            neighbors = TOP_getNeighbors(trainingSet, testSet[x], distance_training_top, k)
            result = getResponse(neighbors)
            predictions.append(result)
            print('> predicted=' + repr(result) + ', actual=' + repr(testSet[x][-1]))
        accuracy = getAccuracy(testSet, predictions)
        #print('Accuracy: ' + repr(accuracy) + '%')
        t2 = time.time()
        timeList.append(t2 - t1)

    totalTime = 0
    for times in timeList:
        totalTime = totalTime + times
    averageTime = totalTime / 200
    print("average completion time: " + str(averageTime))

main()
