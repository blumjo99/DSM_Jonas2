import json

def compilerFunction(node):
    match node["type"]:
        case "STATEMENTBLOCK":
           # print("case statementblock")
            for statement in node["statements"]:
                compilerFunction(statement)

        case "WRITE":
           # print("case write")
            print(compilerFunction(node["arg"]))

        case "TRACE":
           # print("case write")
            print(compilerFunction(node["arg"]))

        case "STRTOKEN":
            return node["value"]


        case "VARIABLE":
            name = node["value"]
            return variables(node["value"])

        case "NUMTOKEN":
            return float(node["value"])
        
        case "ASSIGN":
            varname = node["varname"]    
            value = compilerFunction(node["arg"]) 
            variables[varname] = value   

        case "PLUS":
            print("plus")
            arg1 = compilerFunction(node["arg"][0])
            arg2 = compilerFunction(node["arg"][1])
            return arg1 + arg2
        

variables = {}

with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt', 'r') as file:

   
    node = json.load(file)  


compilerFunction(node)
