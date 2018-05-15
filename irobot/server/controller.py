# library imports
import subprocess
import pickle
import sys
from time import sleep

# our imports
from cfg import cfg
from exceptions import RobotCommandError


########################################################################

cmd_common = 'gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String'


class Controller:
    """ controls the robot and all sensors"""

    def __init__(self):
        self.state = {}
        try:
            self.load_state()
        except (OSError, IOError):
            print("started with no state")

    # ----------------------------------

    def do(self, command, args=None):
        if not cfg['is_ros_available']:
            return '{"command": "' + command + '", "status": "ros is not available"}'
        try:
            if command == 'turn_on':
                status = self.turn_on()
            elif command == 'turn_off':
                status = self.turn_off()
            elif command == 'stop':
                status = self.stop()
            elif command == 'forward':
                status = self.forward()
            elif command == 'back':
                status = self.back()
            elif command == 'left':
                status = self.left()
            elif command == 'right':
                status = self.right()
            elif command == 'rotate_left':
                status = self.rotate_left()
            elif command == 'rotate_right':
                status = self.rotate_right()
            else:
                raise ValueError("command: " + command + " not implemented")
        except RobotCommandError:
            raise NotImplementedError   # TODO: Handle failure

        if status:
            return '{"command": "' + command + '", "status": "success"}'
        else:
            return '{"command": "' + command + '", "status": "fail"}'

    # ----------------------------------

    def turn_on(self):
        res = subprocess.call(['gnome-terminal', '-x', 'roscore'])
        if res == 0:
            print("roscore successful")
            self.state['initalized'] = True
            #return True
        else:
            raise RobotCommandError('roscore failed')
        res = subprocess.call(['gnome-terminal', '-x', 'rosrun', 'rosserial_python', 'serial_node.py', '/dev/ttyACM0'])
        if res == 0:
            print("serial_node successful")
            self.state['initalized'] = True
            return True
        else:
            raise RobotCommandError('serial_node failed')

    # ----------------------------------

    def turn_off(self):

        raise NotImplementedError  # TODO: kill the robot

    # ----------------------------------

    def stop(self):
        res = subprocess.call(['gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String', 'stop', '--once'])
        if res == 0:
            print("stop successful")
            return True
        else:
            raise RobotCommandError('stop failed')

    # ----------------------------------

    def forward(self):
        res = subprocess.call(['gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String', 'forward', '--once'])
        if res == 0:
            print("forward successful")
            return True
        else:
            raise RobotCommandError('forward failed')

    # ----------------------------------

    def back(self):
        res = subprocess.call(['gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String', 'back', '--once'])
        if res == 0:
            print("back successful")
            return True
        else:
            raise RobotCommandError('back failed')

    # ----------------------------------

    def left(self):
        res = subprocess.call(['gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String', 'left', '--once'])
        if res == 0:
            print("left successful")
            return True
        else:
            raise RobotCommandError('left failed')

        # ----------------------------------

    def right(self):
        res = subprocess.call(['gnome-terminal', '-x', 'rostopic', 'pub', 'robot_cmd', 'std_msgs/String', 'right', '--once'])
        if res == 0:
            print("right successful")
            return True
        else:
            raise RobotCommandError('right failed')

    # ----------------------------------

    def rotate_left(self):

        raise NotImplementedError  # TODO: rotate left

        # ----------------------------------

    def rotate_right(self):

        raise NotImplementedError  # TODO: rotate right

    # ----------------------------------

    def load_state(self):
        with open('state.pickle', 'rb') as handle:
            self.state = pickle.load(handle)

    # ----------------------------------

    def save_state(self):
        with open('state.pickle', 'wb') as handle:
            pickle.dump(self.state, handle, protocol=pickle.HIGHEST_PROTOCOL)

    # ----------------------------------

    def finish(self):
        self.save_state()
        exit()

########################################################################
