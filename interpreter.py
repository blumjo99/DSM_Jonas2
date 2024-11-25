import json

from symbol_table import SymbolTable



from operators import (
    add,
    minus,
    str_concat,
    is_number,
    is_string,
    count,
    first
)

from datatypes import(
    DataType,
    TimeType,
    ListType,
    NumType,
    StrType
)

from typing import Callable, Any
from inspect import signature

class Interpreter:

    __symbol_table: SymbolTable #attribut der Klasse, das eine INsatz der Smyboltabele enthält. Speichert variablen und die Werte darin
    __start_time: TimeType #startzeitpunkt des Interpreters
    __it: DataType | None # 

    def __init__(self) -> None:
        self.__symbol_table = SymbolTable()
        self.__start_time = TimeType.now()
        self.__it = None

    def interpreterFunction(self, node):

    # print("Nodevalue: " + str(node))

        match node["type"]:
            case "STATEMENTBLOCK":
                #print("case statementblock")
                for statement in node["statements"]:
                    self.interpreterFunction(statement)

            case "WRITE":
            # print("write")
                print(self.interpreterFunction(node["arg"]))

            case "TRACE":
               # print("case trace")
               # print(node["arg"])
                value = self.interpreterFunction(node["arg"])
 
                print("Line " + str(node["line"]) + ": " + str(value))

            case "STRTOKEN":
               # print("case strtoken")
               # print(StrType(node["value"]))
                return StrType(node["value"])


            case "VARIABLE":
               #print("case variable")
               #print(node["name"])
               variable_name = node["name"]
               return self.__symbol_table.get_variable_value(variable_name)

            case "NUMTOKEN":
                #print("numtoken")
                return float(node["value"])
            
            case "VARIABLE_ASSIGN":
               # print("case variable_assign")
                varname = node["varname"]   
                value = self.interpreterFunction(node["arg"]) 
                #variables[varname] = value   #old
                #print(f"Der Datentyp von arg ist: {type(value)}")

                self.__symbol_table.set_variable_value(varname, value)

            case "PLUS":
                #print("plus")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(add, interpreted_args))


            

            case "MINUS":
            # print("minus")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(minus, interpreted_args))

            
            case "STR_CONCAT":
                #print("concat")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(str_concat, interpreted_args))


            
            case "NUMBER":
               # print("case NUMBER")
                #print(node)
                #print(node["value"])
                
                #return node["value"] --old but functional
                return NumType(node["value"])

            case "TRUE":
                return node["type"]   
            
            case "FALSE":
                return node["type"]   
            
            case "IS_NUMBER":
              #  print("is_number")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])

                return(isinstance(self.__symbol_table.get_variable_value(var2), NumType))
                #interpreted_args = node["arg"]
               # print(interpreted_args)
                #return self.run_operator(is_number, interpreted_args)

            case "IS_STRING":
             #   print("case is_string")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])

                return(isinstance(self.__symbol_table.get_variable_value(var2), StrType))

                #var2=str(node["arg"][0]['name'])
               # interpreted_args = node["arg"]
                #print(f"Der Datentyp von arg ist: {type(self.__symbol_table.get_variable_value(var2))}")
                
            case "IS_LIST":
             #   print("case is_string")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])

                return(isinstance(self.__symbol_table.get_variable_value(var2), ListType))
            
            case "IS_NOT_LIST":
                # print("case is_not_list")

                var1 = node["arg"]
                var2 = str(var1[0]['name'])

                return not(isinstance(self.__symbol_table.get_variable_value(var2), ListType))
            

            case "COUNT":
              #  print("case count")

                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(count, interpreted_args))
            
            case "FIRST":
              #  print("case first")

                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(first, interpreted_args))
            
            






            

            case "LIST":
                #print("case list")

                items = node["items"]
                list_items = []
                for item in items:
                    list_items.append(self.interpreterFunction(item))
                 #   print(self.interpreterFunction(item))
                
                return ListType(list_items)   



    def run_operator(self, operator_func: Callable, args: list[Any]) -> Any:

        # print("run_operator")
    
        # Sicherstellen, dass die Anzahl der Argumente passt
        func_signature = signature(operator_func)
        expected_param_length = len(func_signature.parameters)

        if len(args) != expected_param_length:
            print(f"'{operator_func.__name__}' erwartet {expected_param_length} Argument(e), aber {len(args)} wurden übergeben.")
        else:
            # Operatorfunktion ausführen
            result = operator_func(*args)
            return result





variables = {}


with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt', 'r') as file:
    node = json.load(file)  


#Interpreter.interpreterFunction(node) --old

interpreter = Interpreter()
interpreter.interpreterFunction(node)


###old code


##case plus
# arg1 = interpreterFunction(node["arg"][0])
           # print("break")
            # arg2 = interpreterFunction(node["arg"][1])
            #arg3 = interpreterFunction(node["arg"][2])
 # return str(run_operator(add, [arg1,arg2]))

           

# if isinstance(arg1,int) and isinstance(arg2,int) : #and isinstance(arg2,int):
#     result = int(arg1) + int(arg2)
#     return str(result)
# else:
#     return "null"

##case STR_CONCAT
            #arg1 = interpreterFunction(node["arg"][0])
            #arg2 = interpreterFunction(node["arg"][1])

            #return str(run_operator(str_concat, [arg1,arg2]))
        


           # arg1 = interpreterFunction(node["arg"][0])
            #arg2 = interpreterFunction(node["arg"][1])
            #return str(arg1) + str(arg2)


######


 # case "IS_STRING":
 #               print("case is_string")

                #print(f"Der Datentyp von arg ist: {type(self.__symbol_table.get_variable_value(variable_name))}")
                
               # print(type(self.__symbol_table.get_variable_value(variable_name)))

                #print(node)
                #print(node["arg"])

                #var1 = node["arg"]
                #var2 = str(var1[0]['name'])
 
#                interpreted_args = node["arg"]

 #               print(f"Der Datentyp von arg ist: {type(self.__symbol_table.get_variable_value(var2))}")
  #              print("JO")
                
   #             return(isinstance(self.__symbol_table.get_variable_value(var2), NumType))


               # if type(self.__symbol_table.get_variable_value(var2)) == datatypes.StrType:
                #    return True
               # else:
    #            return False
#


               # return self.run_operator(is_string, interpreted_args)