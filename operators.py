from datetime import datetime

from datatypes import(
    BoolType,
    #DataType,
    NumType,
    StrType,
    ListType
)


def add(left_arg, right_arg):
    try:
        return NumType(str(float(left_arg.value) + float(right_arg.value)))

    except:
       # print("Error: Invalid string format for conversion to double.")
        return "null"


def minus(left_arg, right_arg):
    try:
        return NumType(str(float(left_arg.value) - float(right_arg.value)))


    except:
      #  print("Invalid string format for conversion to double.")
        return "null"

def power(left_arg, right_arg):
  #  print("operator power")
   # print(left_arg)
    #print(right_arg)
    #print(NumType(str(float(left_arg.value) * float(right_arg.value))))
    return NumType(str(float(left_arg.value) * float(right_arg.value)))


def divide(left_arg, right_arg):
  #  print("operator divide")
  #  print(left_arg)
  #  print(right_arg)

   # print(str(float(left_arg) / float(right_arg.value)))

     return NumType(str(float(left_arg) / float(right_arg.value)))



def str_concat(left_arg, right_arg):
    return str(str(left_arg) + str(right_arg))

def count(arg: ListType) -> NumType:
   # print(len(arg.items))
    return NumType(len(arg.items))

def first(arg: ListType):
  #  print(len(arg.items))
    return arg.items[0]




def occurs_before(left_arg, right_arg):
    return BoolType((left_arg.value) <(right_arg.value))


def occured_after(left_arg, right_arg):
    return BoolType((left_arg.value) >(right_arg.value))

def less_than(left_arg, right_arg):
    return BoolType(float(left_arg.value) < float(right_arg.value))

def greater_than(left_arg, right_arg):
    return BoolType(float(left_arg.value) > float(right_arg.value))


def less_than_or_equal(left_arg, right_arg):
    return BoolType(float(left_arg.value) <= float(right_arg.value))

def greater_than_or_equal(left_arg, right_arg):

    try:
        return BoolType(float(left_arg.value) >= float(right_arg.value))
    except:
        return BoolType(float(left_arg) >= float(right_arg.value))


def is_within(arg1, argMin, argMax):
    try:
        return BoolType((float(arg1.value) >= float(argMin.value)) & (float(arg1.value) <= float(argMax.value)) )
    except:
        return BoolType((float(arg1) >= float(argMin.value)) & (float(arg1) <= float(argMax.value)) )










#def is_number(arg) -> BoolType: #nicht aktiv aktuell
#    var1 = arg["name"]
#    print(f"Der Datentyp von arg ist: {type(var1)}")
#    return BoolType(isinstance(arg["name"], NumType))


 
#def is_string(arg) -> BoolType: ##nicht aktiv aktuell
#    var1 = arg["name"]
#    #print(f"Der Datentyp von arg ist: {type(SymbolTable.get_variable_value(self, var1))}")
#    return BoolType(isinstance(var1, StrType))





