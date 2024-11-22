import json

from operators import (
    add,
    minus,
    str_concat
)

from typing import Callable, Any
from inspect import signature

def interpreterFunction(node):

   # print("Nodevalue: " + str(node))

    match node["type"]:
        case "STATEMENTBLOCK":
            print("case statementblock")
            for statement in node["statements"]:
                interpreterFunction(statement)

        case "WRITE":
           # print("write")
            print(interpreterFunction(node["arg"]))

        case "TRACE":
            #print("trace")
            print("Line " + str(node["line"]) + ": " + interpreterFunction(node["arg"]))

        case "STRTOKEN":
            #print("strtoken")
            return node["value"]


        case "VARIABLE":
          #  print("variable")
            return(variables[node["name"]])

        case "NUMTOKEN":
            #print("numtoken")
            return float(node["value"])
        
        case "VARIABLE_ASSIGN":
          #  print("assign")
            varname = node["varname"]   
          #  print("Varname: " + varname) 
            value = interpreterFunction(node["arg"]) 
         #   print("value: " +value)
            variables[varname] = value   

        case "PLUS":
            #print("plus")
            interpreted_args = [interpreterFunction(arg) for arg in node["arg"]]
            return str(run_operator(add, interpreted_args))


          

        case "MINUS":
           # print("minus")
            interpreted_args = [interpreterFunction(arg) for arg in node["arg"]]
            return str(run_operator(minus, interpreted_args))

        
        case "STR_CONCAT":
            #print("concat")
            interpreted_args = [interpreterFunction(arg) for arg in node["arg"]]
            return str(run_operator(str_concat, interpreted_args))


        
        case "NUMBER":
            #print("NUMBER")
            #print(node)
            #print(node["value"])
            return node["value"] 

        case "TRUE":
            return node["type"]        



def run_operator(operator_func: Callable, args: list[Any]) -> Any:

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


interpreterFunction(node)


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