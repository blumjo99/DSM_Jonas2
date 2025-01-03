import json
from symbol_table import SymbolTable

from datetime import datetime


from operators import (
    add,
    minus,
    str_concat,
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
               return self.symbol_table.get_variable_value(variable_name)

            
           

            case "PLUS":
                #print("plus")
                return str(self.run_operator(add, node))
            

            case "MINUS":
                #print("minus")
                return str(self.run_operator(minus, node))
            
            case "POWER":
             #   print("case power")
                return str(self.run_operator(power, node))
            
            case "DIVIDE":
            #    print("case DIVIDE")
                return str(self.run_operator(divide, node))

            case "STR_CONCAT":
                #print("concat")
                return str(self.run_operator(str_concat, node))

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
                return str(self.run_operator(count, node))
            
            case "FIRST":
              #  print("case first")
                return str(self.run_operator(first, node))
            
            

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
                varname = node["varname"]  
                timeValue = self.interpreterFunction(node["arg"])
                self.symbol_table.set_variable_timestamp(varname, timeValue)

            case "NOW":
                return TimeType.now()
            
            case "LESS_THAN":
                return str(self.run_operator(less_than, node))
            
            case "LESS_OR_EQUAL":
                return str(self.run_operator(less_than_or_equal, node))

            case "GREATER_THAN":
                return str(self.run_operator(greater_than, node))
            
            case "GREATER_OR_EQUAL":
                return str(self.run_operator(greater_than_or_equal, node))
            
            case "IS_WITHIN":
                return str(self.run_operator(is_within, node))
            


            
            case "IT":
                return self.it
            
            case "WHERE":
                list_values = self.interpreterFunction(node["arg"][0]) #value: [100, 200, 50, 120, 150, 90]
                new_list: list[list] = []

                for i in range(list_values.length()):
                   
                   self.it = list_values[i]
                   var1 = self.interpreterFunction(node["opt"][0])

                   if isinstance(var1, ListType):
                       if str(var1[i]) == "TRUE":
                           new_list.append(list_values[i])
                   else:
                       if str(var1) == "true":
                           new_list.append(list_values[i])
#               

                return ListType(new_list)
            

        
            
            case "TIME_READ":
                value = self.interpreterFunction(node["arg"][0])
                return value
            

            case "LIST_ASSIGN":
                value = self.interpreterFunction(node["arg"])
                index = self.interpreterFunction(node["list_index"])

                variable_name = node["varname"]
                list =  self.symbol_table.get_variable_value(variable_name)

                list.changeEntry(index, value)

                variable_name = node["varname"]
                list =  self.symbol_table.get_variable_value(variable_name)

            case "OCCURS_BEFORE":
                return str(self.run_operator(occurs_before, node))

    

            case "OCCURRED_AFTER":
                return str(self.run_operator(occured_after, node))
            

            case "FOR_LOOP":

                variable_name = StrType(node["varname"])

                expression = node["expression"]
                expression2 = node["expression2"]

                loop_start = int(str(self.interpreterFunction(expression)))
                loop_end = int(str(self.interpreterFunction(expression2)))                
             
                for i in range(loop_start, loop_end+1):
                   
                    self.symbol_table.set_variable_value(str(variable_name), NumType(i))
                    self.interpreterFunction(node["statements"])

            case "IF":

                condition = node["condition"]        

                if self.interpreterFunction(condition) == "true":
                    then = node["thenbranch"]
                    self.interpreterFunction(then)
                else:
                    elseVar = node["elsebranch"]
                    self.interpreterFunction(elseVar)

              



    def run_operator(self, operator_func: Callable, node: dict) -> Any:
        
        interpreted_args = [self.interpreterFunction(arg) for arg in node["arg"]]

        # Sicherstellen, dass die Anzahl der Argumente passt
        func_signature = signature(operator_func)
        expected_param_length = len(func_signature.parameters)

        if len(interpreted_args) != expected_param_length:
            print(f"'{operator_func.__name__}' erwartet {expected_param_length} Argument(e), aber {len(args)} wurden übergeben.")
        else:
            # Operatorfunktion ausführen
            result = operator_func(*interpreted_args)
            return result




with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt', 'r') as file:
    node = json.load(file)  


interpreter = Interpreter()
interpreter.interpreterFunction(node)




