import math

from symbol_table import SymbolTable


from datatypes import(
    BoolType,
    DataType,
    NumType,
    StrType
)


def add(left_arg, right_arg):
    try:
        double_number1 = NumType(left_arg)
        double_number2 = NumType(right_arg)
        return NumType(double_number1.value+ double_number2.value)
    except:
       # print("Error: Invalid string format for conversion to double.")
        return "null"


def minus(left_arg, right_arg):
    try:
        double_number1 = float(left_arg)
        double_number2 = float(right_arg)
        return double_number1 - double_number2
    except:
      #  print("Invalid string format for conversion to double.")
        return "null"
    
def str_concat(left_arg, right_arg):
    return str(str(left_arg) + str(right_arg))

def count(arg: DataType) -> DataType:
   # print(len(arg.items))
    return NumType(len(arg.items))

def first(arg: DataType) -> DataType:
  #  print(len(arg.items))
    return arg.items[0]




def is_number(arg: DataType) -> BoolType: #nicht aktiv aktuell
    print("is number?")
    print(arg)
    print(arg["name"])
    var1 = arg["name"]
    print(var1)
    print(f"Der Datentyp von arg ist: {type(var1)}")

    return BoolType(isinstance(arg["name"], NumType))
    #return BoolType(isinstance(arg["name"], str))


 
def is_string(arg: DataType) -> BoolType: ##nicht aktiv aktuell
    #print("is string?")
    #print(arg)
    var1 = arg["name"]
    print(var1)
    #print(f"Der Datentyp1 von arg ist: {type(arg["name"])}")
    #print(f"Der Datentyp von arg ist: {type(SymbolTable.get_variable_value(self, var1))}")
    return BoolType(isinstance(var1, StrType))
    #return BoolType(isinstance(var1, str))





