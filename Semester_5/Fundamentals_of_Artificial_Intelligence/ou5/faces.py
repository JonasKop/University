# faces.py
# -----------------------------------------------------------------------------
# Facial recognision program which takes a given set of images, answers to the
# images and a test as arguments. It then uses Back-propagation with 4 synapses
# to traing on a given set of images. It does this with a sigmoid function
# which it later on used to test the image and compares it with a given answer
# and returns the error. Then it runs the testing and prints the answers.
# -----------------------------------------------------------------------------
# Usage: python faces.py <training.txt> <training_keys.txt> <test.txt>
# -----------------------------------------------------------------------------
# INPUT: training.txt: file with n nr of images following this format: First
#           Image name, then a 20x20 image with numbers ranging from 1 to 32.
#        training_keys.txt: file with n nr of answers to the images of the
#           format: Image name then answer
#        test.txt: same as training.txt
# -----------------------------------------------------------------------------
# OUTPUT: Prints to STDOUT in this format
#    Image1 2
#    Image2 3
#    Image3 1
# -----------------------------------------------------------------------------
# Authors: Emil Hallberg (id16ehg) & Jonas Sjödin (id16jsn)
# Date: October 2018
# Course: Fundamentals of Artificiall intelligence
# Assignment: 5

import numpy as np
from random import uniform
from math import exp, pow
from sys import argv


def get_part_of_image_sum(image, x0, xn, y0, yn):
    """
    Returns the sum value of a given part of an image.
    :param image: The image
    :param x0: Start-x
    :param xn: End-x
    :param y0: Start-y
    :param yn: End-y
    :return: The sum of the np.array part
    """
    return np.sum([[image[i][j] for j in range(y0, yn)] for i in range(x0, xn)])


def rotate_image(image):
    """
    Rotates an image if necessary so all images should be rotated the right way.
    :param image: The image that might be rotated
    :return: If necessary, a rotated image, else the image
    """
    half_sum = [0, 0, 0, 0]

    half_sum[0] = get_part_of_image_sum(image, 0, 20, 0, 10)
    half_sum[1] = get_part_of_image_sum(image, 0, 10, 0, 20)
    half_sum[2] = get_part_of_image_sum(image, 0, 20, 10, 20)
    half_sum[3] = get_part_of_image_sum(image, 10, 20, 0, 20)

    ind = 0
    largest_val = 0
    for i in range(4):
        if half_sum[i] > largest_val:
            ind = i
            largest_val = half_sum[i]

    return image if ind == 0 else np.rot90(image, ind)


def read_images(file_name):
    """
    Reads all images from a given file. The images should be of size 20x20.
    Then it returns them as a np.array containing another 2d np.array.
    :param file_name: the file name of the image file
    :return: The images in a np.array conatining a 2d np.array
    """
    f = open(file_name, "r")
    if f.mode != 'r':
        print("Could not open file: " + file_name)
        exit(1)

    file_content = list(f)
    images = list()
    i = 0
    while i < len(file_content):
        if file_content[i].startswith("Image"):
            i += 1
            image = list()
            for j in range(20):
                row = file_content[i].split()
                row_one = list()
                for nr in row:
                    row_one.append(int(nr) / 32)

                image.append(np.asarray(row_one))
                i += 1

            images.append(rotate_image(np.asarray(image)).flatten())
        else:
            i += 1
    return np.asarray(images)


def read_training_answers(file_name):
    """
    Reads all answers from a given file. The answers should be of 2x1. First
    the image name, then the answer. Then it returns them as a np.array
    containing only the answers.
    :param file_name: the file name of the answers file
    :return: The answers in a np.array
    """
    f = open(file_name, "r")
    if f.mode != 'r':
        print("Could not open file: " + file_name)
        exit(1)

    file_content = list(f)
    answers = list()

    for i in range(len(file_content)):
        if file_content[i].startswith("Image"):
            answers.append(int(file_content[i].split()[1]))

    return np.asarray(answers)


def init_synapses():
    """
    Initiates 4 synapses with random numbers ranging from 0 to 1.
    :return: An array with 4 synapses of size 400 (20x20) containing random
             numbers ranging from 0 to 1.
    """
    return np.asarray([np.asarray([uniform(0, 1) for _ in range(400)]) for _ in range(4)])


def sigmoid(image, synapse):
    """
    Calculates a sigmoid function and returns it on a given image using a synapse.
    :param image: the image which should be calculated on
    :param synapse: the synapse which should be calculated on
    :return: value of sigmoid function
    """
    return 1 / (1 + exp(-(np.dot(image, synapse))))


def train(constant, image, answer, synapses):
    """
    Trains all synapses on a a given image using a sigmoid function.
    :param constant: A constant which will be be multiplied with the result
    :param image: The image which should be trained on
    :param answer: The correct answer to the image
    :param synapses: The synapses that should be used in the training
    """
    for i in range(len(synapses)):
        synapses[i] += constant * image * ((answer == i + 1) - sigmoid(image, synapses[i]))


def test_training(image, answer, synapses):
    """
    Tests the training by doing the following: For all synapses calculate the
    power of two by ((correct/incorrect answer) - (sigmoid))^2 and summarizes
    the result which should result in a sum of all the error which is returned.
    :param image: The image which should be tested on
    :param answer: The answer to the image
    :param synapses: The synapses that should be used in the testing
    :return: the error sum
    """
    return sum([pow((answer == i + 1) - sigmoid(image, synapses[i]), 2) for i in range(len(synapses))])


def test(image, synapses):
    """
    Tests the image by using a given set of synapses.
    :param image: The image which should be tested on
    :param synapses: The synapses which should be used for testing
    :return: The guess at which face it is (1, 2, 3 or 4)
    """
    return np.argmax([sigmoid(image, synapses[i]) for i in range(len(synapses))]) + 1


if __name__ == '__main__':
    if len(argv) != 4:
        print("Usage: " + argv[0] + " <training.txt> <training_keys.txt> <test.txt>")
        exit(1)

    training_images = read_images(argv[1])
    training_answers = read_training_answers(argv[2])
    test_images = read_images(argv[3])

    error = 32
    synapses = init_synapses()

    for i in range(100):
        if error < 0.09:
            break

        error = 0
        for j in range(len(training_images)):
            if (j % 3) != 0:
                train(0.05, training_images[j], training_answers[j], synapses)
            else:
                error += test_training(training_images[j], training_answers[j], synapses)

        error = error / len(training_images)

    for i in range(len(test_images)):
        print("Image" + str(i + 1) + ": " + str(test(test_images[i], synapses)))
