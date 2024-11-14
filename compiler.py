import json

def compilerFunction(node):

    print("Nodevalue: " + str(node))

    match node["type"]:
        case "STATEMENTBLOCK":
          #  print("case statementblock")
            for statement in node["statements"]:
                compilerFunction(statement)

        case "WRITE":
           # print("write")
            print(compilerFunction(node["arg"]))

        case "TRACE":
            #print("trace")
            print("Line " + str(node["line"]) + ": " + compilerFunction(node["arg"]))

        case "STRTOKEN":
            #print("strtoken")
            return node["value"]


        case "VARIABLE":
            #print("variable")
            name = node["value"]
            return variables(node["value"])

        case "NUMTOKEN":
            #print("numtoken")
            return float(node["value"])
        
        case "ASSIGN":
            #print("assign")
            varname = node["varname"]    
            value = compilerFunction(node["arg"]) 
            variables[varname] = value   

        case "PLUS":
            #print("plus")
            arg1 = compilerFunction(node["arg"][0])
            arg2 = compilerFunction(node["arg"][1])
            return arg1 + arg2
        
        case "STR_CONCAT":
            #print("concat")
            arg1 = compilerFunction(node["arg"][0])
            arg2 = compilerFunction(node["arg"][1])
            return arg1 + arg2
        

variables = {}

with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt', 'r') as file:
    node = json.load(file)  


compilerFunction(node)
