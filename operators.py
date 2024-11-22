import math


def add(left_arg, right_arg):
    #print("add_operator")


    try:
        double_number1 = float(left_arg)
        double_number2 = float(right_arg)
        return double_number1 + double_number2
    except ValueError:
      #  print("Invalid string format for conversion to double.")
        return "null"


def minus(left_arg, right_arg):
    try:
        double_number1 = float(left_arg)
        double_number2 = float(right_arg)
        return double_number1 - double_number2
    except ValueError:
      #  print("Invalid string format for conversion to double.")
        return "null"
    
def str_concat(left_arg, right_arg):

    return str(str(left_arg) + str(right_arg))
 

    
