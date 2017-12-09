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
import operator
from api import TOP_distance_Euclidean # Adding TOP function
from api import TOP_calculate_distance
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


def getNeighbors(trainingSet, testInstance,distance_training_top, k):
    length = len(testInstance) - 1
    first_dist = TOP_distance_Euclidean(testInstance, trainingSet[0])
    second_dist = TOP_distance_Euclidean(testInstance, trainingSet[1])
    lowest_dist = TOP_distance_Euclidean(testInstance, trainingSet[2])
    # default case : Need atleast 3 distance values 
    distances = [(trainingSet[0],first_dist),(trainingSet[1],second_dist),(trainingSet[2],lowest_dist)]
    for x in range(3,len(trainingSet)):
        # apply triangle inequality here
        if (first_dist>(distance_training_top[x])):
            #print("j")
            lowest_dist = TOP_distance_Euclidean(testInstance, trainingSet[x]) # Adding TOP function
            distances.append((trainingSet[x], lowest_dist))
    distances.sort(key=operator.itemgetter(1))
    neighbors = []
    for x in range(k):
        neighbors.append(distances[x][0])
    return neighbors


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
    loadDataset('/Users/esha/PycharmProjects/Compiler_Project_CC/Test_cases/iris.data', split, trainingSet, testSet)
    print 'Train set: ' + repr(len(trainingSet))
    print 'Test set: ' + repr(len(testSet))
    # generate predictions
    predictions = []
    k = 3
    distance_training_top=TOP_calculate_distance(trainingSet)
    #print(distance_training)
    for x in range(len(testSet)):
        neighbors = getNeighbors(trainingSet, testSet[x], distance_training_top, k)
        result = getResponse(neighbors)
        predictions.append(result)
        print('> predicted=' + repr(result) + ', actual=' + repr(testSet[x][-1]))
    accuracy = getAccuracy(testSet, predictions)
    print('Accuracy: ' + repr(accuracy) + '%')


main()
