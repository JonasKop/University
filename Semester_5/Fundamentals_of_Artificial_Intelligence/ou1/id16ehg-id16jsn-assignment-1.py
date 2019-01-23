# robot.py
# Authors: Emil Hallberg & Jonas Sjödin
# Date: September 2018
# Course: Fundamentals of Artificial Intelligence
#
# Description: This python program controlls a lokarria robot by sending
# different http-requests through its interface. It takes a path as an argument
# and follows the path with a look ahead distance using an interpretation of
# the "Follow the carrot algorithm". It records its lap time and prints it on finish.


import http.client, json, time, sys
from math import pi, atan2, sqrt, pow

MRDS_URL = 'localhost:50000'
HEADERS = {"Content-type": "application/json", "Accept": "text/json"}


class UnexpectedResponse(Exception):
    pass


def get_laser():
    """
    returns the laser of the robot at this moment
    :return: the laser of the robot at this moment
    """
    mrds = http.client.HTTPConnection(MRDS_URL)
    mrds.request('GET', '/lokarria/laser/echoes')
    response = mrds.getresponse()
    if response.status == 200:
        laser_data = response.read()
        response.close()
        return json.loads(laser_data.decode())
    else:
        return response


def rotate(q, v):
    """
    rotation taking a q and a v
    :param q: the q
    :param v: the v
    :return: the rotation
    """
    return vector(qmult(qmult(q, quaternion(v)), conjugate(q)))


def quaternion(v):
    """
    Returns the quaternion of v
    :param v: the v that should become a queaternion
    :return: the quaternion of v
    """
    q = v.copy()
    q['W'] = 0.0
    return q


def vector(q):
    """
    returns a dict version of q
    :param q: the q
    :return: dict version of q
    """
    return {"X": q["X"], "Y": q["Y"], "Z": q["Z"]}


def conjugate(q):
    """
    returns the conjugate of q
    :param q: the q
    :return: the q copy
    """
    qc = q.copy()
    qc["X"] = -q["X"]
    qc["Y"] = -q["Y"]
    qc["Z"] = -q["Z"]
    return qc


def qmult(q1, q2):
    """
    Returns the qmult
    :param q1: value 1
    :param q2: value 2
    :return: the qmult
    """
    return {
        "W": q1["W"] * q2["W"] - q1["X"] * q2["X"] - q1["Y"] * q2["Y"] - q1["Z"] * q2["Z"],
        "X": q1["W"] * q2["X"] + q1["X"] * q2["W"] + q1["Y"] * q2["Z"] - q1["Z"] * q2["Y"],
        "Y": q1["W"] * q2["Y"] - q1["X"] * q2["Z"] + q1["Y"] * q2["W"] + q1["Z"] * q2["X"],
        "Z": q1["W"] * q2["Z"] + q1["X"] * q2["Y"] - q1["Y"] * q2["X"] + q1["Z"] * q2["W"]
    }


def get_heading():
    """Returns the XY Orientation as a heading unit vector"""
    return rotate(get_pose()['Pose']['Orientation'], {'X': 1.0, 'Y': 0.0, "Z": 0.0})


class Path:
    """
    Initializes the Path class and reads a path file
    """

    def __init__(self):
        self.path = []
        self.load_path(sys.argv[1])
        self.vecPath = self.vectorize_path()

    def load_path(self, file_name):
        """
        Loads the path taken as an argument and saves it as json data
        :param file_name: the file that should be read.
        :return: the path as json data
        """
        with open(file_name) as path_file:
            data = json.load(path_file)
        self.path = data

    def vectorize_path(self):
        """
        Makes the path into a vector
        :return: the path as an vector
        """
        return [{'X': p['Pose']['Position']['X'],
                 'Y': p['Pose']['Position']['Y'],
                 'Z': p['Pose']['Position']['Z']}
                for p in self.path]


def post_speed(angular_speed, linear_speed):
    """
    Sets the speed of the robot by taking an angular speed and a linear speed
    as arguments
    :param angular_speed: the rotational speed
    :param linear_speed: the speed in the robots direction
    :return: the response status of the robot
    """
    mrds = http.client.HTTPConnection(MRDS_URL)
    params = json.dumps({'TargetAngularSpeed': angular_speed, 'TargetLinearSpeed': linear_speed})
    mrds.request('POST', '/lokarria/differentialdrive', params, HEADERS)
    response = mrds.getresponse()
    status = response.status
    if status == 204:
        return response
    else:
        raise UnexpectedResponse(response)


def get_pose():
    """
    Gets the pose of the robot by sending a http request
    :return: the pose if eveything went right
    """
    mrds = http.client.HTTPConnection(MRDS_URL)
    mrds.request('GET', '/lokarria/localization')
    response = mrds.getresponse()
    if response.status == 200:
        pose_data = response.read()
        response.close()
        return json.loads(pose_data.decode())
    else:
        return UnexpectedResponse(response)


def get_dist(robot_t, goal):
    """
    Returns the distance between two positions
    :param robot_t: The robots position
    :param goal: The goal position
    :return: the distandce between the two positions
    """
    return sqrt(pow(robot_t['X'] - goal['X'], 2) + pow(robot_t['Y'] - goal['Y'], 2))


def get_next_goal(path_t, ind_t, robot_t, look_dist_t):
    """
    Gets the next goal on the given path that the robot should aim for
    :param path_t: The given path that the robot should follow
    :param ind_t: The current index that the robot last looked for
    :param robot_t: The robots position
    :param look_dist_t: the look ahead distance of the robot
    :return: index of the position the robot should look at
    """
    try:
        while get_dist(path_t[ind_t + 1], robot_t) < look_dist_t:
            ind_t += 1
    except IndexError:
        return ind_t - 1

    return ind_t


def set_heading(goal, robot_t):
    """
    Sets the heading direction of the robot
    :param goal: the goal position in this state
    :param robot_t: the robots position in this state
    :return: Returns if the robot should rotate to the right or to the left
    """
    robot_head = get_heading()
    robot_angle = -1 * atan2(robot_head['Y'], robot_head['X'])

    dest_degree = -1 * atan2(goal['Y'] - robot_t['Y'], goal['X'] - robot_t['X'])

    if dest_degree < 0:
        dest_degree += 2 * pi
    if robot_angle < 0:
        robot_angle += 2 * pi

    if abs(robot_angle - dest_degree) > 0.2:
        if robot_angle > dest_degree:
            if abs(robot_angle - dest_degree) > pi:
                return -1
            return 1
        if abs(robot_angle - dest_degree) > pi:
            return 1
        return -1
    return 0


def set_speed(laser_t):
    """
    Sets the next speed, look ahead distance and rotation speed.
    :param laser_t: the given laser containing info about the surroundings
    :return: speed, look ahead distance and rotation speed
    """
    smallest = min(laser_t['Echoes'][95:270 - 95])
    if smallest < 0.3:
        return [0.1, 0.4, 1]
    if smallest < 0.4:
        return [0.3, 0.6, 1]
    if smallest < 0.5:
        return [0.5, 0.6, 1]
    if smallest < 0.6:
        return [0.7, 0.6, 1]
    if smallest < 0.7:
        return [0.8, 0.6, 1]
    if smallest < 0.8:
        return [1, 0.6, 1]
    if smallest < 2:
        return [1, 1, 1]
    return [1, 2, 0.8]


def get_passed_nodes(robot_t, path_t, ind_t):
    """
    Returns the index of the last position in the path that the robot has passed
    :param robot_t: The robots position containig a x-pos and a y-pos
    :param path_t: The given path
    :param ind_t: The last passed index of the path
    :return: the index of the last position passed.
    """

    nearest = get_dist(robot_t, path_t[ind_t])
    near_ind = ind_t

    for i in range(ind_t, len(path_t)):
        dist2 = get_dist(robot_t, path_t[ind_t])
        if dist2 > nearest:
            nearest = dist2
            near_ind = i
        elif ind_t + 400 < i:
            break
    return near_ind


if __name__ == '__main__':
    """
    Main function for the robot controller program, which runs everything.
    """
    print("Starting robot")
    path = list(Path().vecPath)
    ind = 0
    look_dist = 1.4
    robot = dict
    laser = dict
    print("Running")
    t0 = time.time()
    while 1:

        try:
            robot = get_pose()['Pose']['Position']
        except UnexpectedResponse as ex:
            print('Unexpected response from server when reading position:', ex)

        try:
            laser = get_laser()
        except UnexpectedResponse as ex:
            print('Unexpected response from server when reading laser data:', ex)

        [speed, look_dist, rot] = set_speed(laser)

        if get_dist(robot, path[len(path) - 1]) < 0.2:
            try:
                post_speed(0, 0)
            except UnexpectedResponse as ex:
                print('Unexpected response from server when sending speed commands:', ex)
            break

        ind = get_passed_nodes(robot, path, ind)
        if ind == -1:
            break
        ind = get_next_goal(path, ind, robot, look_dist)

        direction = set_heading(path[ind], robot)

        try:
            post_speed(direction * rot, speed)
        except UnexpectedResponse as ex:
            print('Unexpected response from server when sending speed commands:', ex)

        time.sleep(0.05)
    t1 = time.time()
    print("Finished! Lap time:", "{:.4f}s".format(t1 - t0))
