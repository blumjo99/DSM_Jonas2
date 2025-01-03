import json
from symbol_table import SymbolTable

from datetime import datetime


from operators import (
    add,
    minus,
    str_concat,
    #is_number,
    #is_string,
    count,
    first,
    less_than,
    less_than_or_equal,
    greater_than,
    greater_than_or_equal,
    is_within,
    power,
    divide,
    occurs_before,
    occured_after
)

from datatypes import(
#    DataType,
    TimeType,
    ListType,
    NumType,
    StrType
)

from typing import Callable, Any
from inspect import signature

class Interpreter:

    symbol_table: SymbolTable #attribut der Klasse, das eine Insatz der Smyboltabele enthält. Speichert variablen und die Werte darin
    itVar: NumType |None


    def __init__(self) -> None:
        self.symbol_table = SymbolTable()

        self.itVar = None# erklärung


    def interpreterFunction(self, node):

       # print("Nodevalue: " + str(node))
        match node["type"]:
            case "STATEMENTBLOCK":
                #print("case statementblock")
                for statement in node["statements"]:
                    self.interpreterFunction(statement)

            case "WRITE":
                #print("write")
                print(self.interpreterFunction(node["arg"]))

            case "TRACE":
            #    print("case trace")
                value = self.interpreterFunction(node["arg"])
                print("Line " + str(node["line"]) + ": " + str(value))


            case "VARIABLE":
              # print("case variable")
               variable_name = node["name"]
               #print(variable_name)
               #print(self.symbol_table.get_variable_value(variable_name).value)

               return self.symbol_table.get_variable_value(variable_name)

            
           

            case "PLUS":
                #print("plus")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(add, interpreted_args))
            

            case "MINUS":
                #print("minus")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(minus, interpreted_args))
            
            case "POWER":
             #   print("case power")


                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(power, interpreted_args))
            

            case "DIVIDE":
            #    print("case DIVIDE")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(divide, interpreted_args))


            
            case "STR_CONCAT":
                #print("concat")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(str_concat, interpreted_args))

            case "NUMTOKEN":
                #print("numtoken")
                return NumType(node["value"])
            
            case "STRTOKEN":
                #print("case strtoken")
                return StrType(node["value"])
            
            case "NUMBER":
                #print("case NUMBER")
                return NumType(node["value"])
            
            case "TIMETOKEN":
                return TimeType(node["value"])

            case "TRUE":
                return node["type"]   
            
            case "FALSE":
                return node["type"]   
            
            case "IS_NUMBER":
                #print("is_number")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])
                return(isinstance(self.symbol_table.get_variable_value(var2), NumType))


            case "IS_STRING":
                #print("case is_string")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])
                return(isinstance(self.symbol_table.get_variable_value(var2), StrType))

                
            case "IS_LIST":
                #print("case is_string")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])
                return(isinstance(self.symbol_table.get_variable_value(var2), ListType))
            
            case "IS_NOT_LIST":
                #print("case is_not_list")
                var1 = node["arg"]
                var2 = str(var1[0]['name'])

                return not(isinstance(self.symbol_table.get_variable_value(var2), ListType))
            

            case "COUNT":
              #  print("case count")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(count, interpreted_args))
            
            case "FIRST":
              #  print("case first")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(first, interpreted_args))
            
            

            case "LIST":
             #   print("case list")
                items = node["items"]
                list_items = []
                for item in items:
                    list_items.append(self.interpreterFunction(item))
                
                return ListType(list_items)   
            
            case "VARIABLE_ASSIGN":
                #print("case variable_assign")
                varname = node["varname"]   
                value = self.interpreterFunction(node["arg"])
                self.symbol_table.set_variable_value(varname, value)

            case "TIME_ASSIGN":
                #print("case time_assign")

                varname = node["varname"]  
                value = self.interpreterFunction(node["arg"])
                self.symbol_table.set_variable_value(varname, value)

            case "TIME_OF":
              #  print(" case time of")

                varname = node["varname"]  
              #  print(varname)
                timeValue = self.interpreterFunction(node["arg"])
              #  print(timeValue)
                self.symbol_table.set_variable_timestamp(varname, timeValue)

            case "NOW":
                #print("case now")
                return TimeType.now()
            
            case "LESS_THAN":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(less_than, interpreted_args))
            
            case "LESS_OR_EQUAL":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(less_than_or_equal, interpreted_args))
            

            case "GREATER_THAN":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(greater_than, interpreted_args))
            
            case "GREATER_OR_EQUAL":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(greater_than_or_equal, interpreted_args))
            
            case "IS_WITHIN":
             #   print("case is within")
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]

                return str(self.run_operator(is_within, interpreted_args))
            


            
            case "IT":
             #   print("case it")
               # if self.__it is None:
                #    return NullType()
               #print(self.it)
                return self.it
            
            case "WHERE":
              #  print("case where")
                list_values = self.interpreterFunction(node["arg"][0]) #value: [100, 200, 50, 120, 150, 90]

                #bool_values = self.interpreterFunction(node["opt"][0]) # value: [TRUE, FALSE, FALSE, TRUE, TRUE, FALSE]

                new_list: list[list] = []

                for i in range(list_values.length()):
                   
                  # print("value:")
                 #  print(list_values[i])
                 #  print("self:")
                 #  print(str(list_values[i]))
                   self.it = list_values[i]

                 #  print("operator:")
                 #  print(self.interpreterFunction(node["opt"][0])) #~berechnugsoperator
                  # print("get true or false")
                   var1 = self.interpreterFunction(node["opt"][0])

                   #print("var1")
                  # print(str(var1))
                   # if str(bool_values[i]) == "TRUE":
                  # if str(self.interpreterFunction(node["opt"][0]) == "true"):
                   #if str(var1) == "true":
                  #    print("add " + str(list_values[i]))
                    #  new_list.append(list_values[i])


                   if isinstance(var1, ListType):
                     #  print("list")
                    #   print(var1[i])
                       if str(var1[i]) == "TRUE":
                           new_list.append(list_values[i])
                   else:
                      # print("hello2")
                       if str(var1) == "true":
                           new_list.append(list_values[i])
#               

                return ListType(new_list)
            

        
            
            case "TIME_READ":
                value = self.interpreterFunction(node["arg"][0])
                return value
            

            case "LIST_ASSIGN":
              #  print("case list_assign")

            
                value = self.interpreterFunction(node["arg"])
                index = self.interpreterFunction(node["list_index"])

                variable_name = node["varname"]
                list =  self.symbol_table.get_variable_value(variable_name)

                list.changeEntry(index, value)

                variable_name = node["varname"]
                list =  self.symbol_table.get_variable_value(variable_name)

            case "OCCURS_BEFORE":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(occurs_before, interpreted_args))

    

            case "OCCURRED_AFTER":
                interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]
                return str(self.run_operator(occured_after, interpreted_args))
            

            case "FOR_LOOP":
                variable_name = StrType(node["varname"])
                #print(variable_name)

                 # Extract and evaluate the loop expression
                expression = node["expression"]
                expression2 = node["expression2"]

                loop_start = int(str(self.interpreterFunction(expression)))
                loop_end = int(str(self.interpreterFunction(expression2)))                
             
                for i in range(loop_start, loop_end+1):
                   
                    self.symbol_table.set_variable_value(str(variable_name), NumType(i))
                    self.interpreterFunction(node["statements"])

            case "IF":
                #print("case if")

                condition = node["condition"]
              #  print(condition)
               # print(condition["type"])
                #print(self.interpreterFunction(condition))

                if self.interpreterFunction(condition) == "true":
                    then = node["thenbranch"]
                    self.interpreterFunction(then)
                else:
                    elseVar = node["elsebranch"]
                    self.interpreterFunction(elseVar)

              


          

                
            


            

            








    def run_operator(self, operator_func: Callable, args: list[Any]) -> Any:
        

        
        # Sicherstellen, dass die Anzahl der Argumente passt
        func_signature = signature(operator_func)
        expected_param_length = len(func_signature.parameters)

        if len(args) != expected_param_length:
            print(f"'{operator_func.__name__}' erwartet {expected_param_length} Argument(e), aber {len(args)} wurden übergeben.")
        else:
            # Operatorfunktion ausführen
            result = operator_func(*args)
            return result





#einlesen der Parser Datei
with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt', 'r') as file:
    node = json.load(file)  


interpreter = Interpreter()
interpreter.interpreterFunction(node)




