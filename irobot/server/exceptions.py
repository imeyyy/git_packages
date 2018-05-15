class Error(Exception):
    """Base class for exceptions in this module."""
    pass


class RobotCommandError(Error):
    """Exception raised if robot init failed.

    Attributes:
        expression -- input expression in which the error occurred
        message -- explanation of the error
    """

    def __init__(self, message):
        Exception.__init__(self, message)
