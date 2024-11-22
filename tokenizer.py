import re #für regular expression

class Token: #Klasse um die Infos eines Tocken zu speichern
    def __init__(self, line_number, token_type, value):
        self.line_number = line_number
        self.token_type = token_type
        self.value = value

    def __repr__(self):
        return f"[{self.line_number}, {self.token_type}, '{self.value}']"

class SimpleTokenizer: 
    def __init__(self, lines): #zum erhalten des Inputtexts
        self.lines = lines

    def tokenize(self):

        tokens = []
        line_number = 0

        reserved_list = ["NULL","FALSE", "TRUE", "TRACE", "EAD", "WRITE", "IF", "THEN", "ELSEIF", "ELSE", "ENDIF", "FOR","IN", "DO", "ENDDO", "D", "CURRENTTIME", "MINIMUM", "MAXIMUM","FIRST", "LAST", "SUM", "AVERAGE", "EARLIEST", "LATEST"]
        
        for line in self.lines:
            line_number += 1

            # parts = line.split()
            parts = re.findall(r"""\".*?\"  # Für Wörte rin Anführungszeichen
                     |\w+      # --> wörter ohne Anführungszeichen
                     |:=        
                     |;         
                     |\[
                     |\]       
                     |\*\*     
                     |\*       
                     |,       
                     |\(
                     |\)
                     |\&
                     |\+
                     |\-
                     |\<\>
                     |\<\=
                     |\>\=
                     |\>
                     |\=
                     |\<          
                                
                               '
                  """, line, re.VERBOSE) # re.verbose ist dafür, dass die Kommentar im code ignorniert werden

       
            for part in parts:


                if part == ':=':
                    tokens.append(Token(line_number, 'ASSIGN', part))
                elif re.match(r'\d{4}-\d{2}-\d{2}', part) or re.match(r'\d{4}-\d{2}-\d{2}T\d{2}:\d{2}(:\d{2})?', part):
                    tokens.append(Token(line_number, 'TIMETOKEN', part))
                elif part == ';':
                    tokens.append(Token(line_number, 'SEMICOLON', part))
                elif part == '[':
                    tokens.append(Token(line_number, 'LSPAR', part))
                elif part == ']':
                    tokens.append(Token(line_number, 'RSPAR', part))
                elif part == '(':
                    tokens.append(Token(line_number, 'LPAR', part))
                elif part == ')':
                    tokens.append(Token(line_number, 'RPAR', part))
                elif part == ',':
                    tokens.append(Token(line_number, 'COMMA', part))
                elif part == '+':
                    tokens.append(Token(line_number, 'PLUS', part))
                elif part == '-':
                    tokens.append(Token(line_number, 'MINUS', part))
                elif part == '**':
                    tokens.append(Token(line_number, 'POWER', part))
                elif part == '*':
                    tokens.append(Token(line_number, 'TIMES', part))
                elif part == '/':
                    tokens.append(Token(line_number, 'DIVIDE', part))
                elif part == '&':
                    tokens.append(Token(line_number, 'AMPERSAND', part))
                elif part == '<':
                    tokens.append(Token(line_number, 'LT', part))    
                elif part == '>':
                    tokens.append(Token(line_number, 'GT', part))  
                elif part == '<=':
                    tokens.append(Token(line_number, 'LTEQ', part))
                elif part == '>=':
                    tokens.append(Token(line_number, 'GTEQ', part))
                elif part == '=':
                    tokens.append(Token(line_number, 'EQ', part)) 
                elif part == '<>':
                    tokens.append(Token(line_number, 'NEQ', part))  
                elif re.match(r'^\d+(\.\d+)?$', part):
                    tokens.append(Token(line_number, 'NUMTOKEN', part))
                elif re.match(r'^".*"$', part):
                    tokens.append(Token(line_number, 'STRTOKEN', str(part).replace('"','')))
                elif str(part).upper() in reserved_list:
                     tokens.append(Token(line_number, str(part).upper(), part))
                elif re.match(r'^[A-Z]\w*$', part):
                    tokens.append(Token(line_number, 'IDENTIFIER', part))
                else:
                    tokens.append(Token(line_number, 'ERROR', part))
                    
        return tokens





# Datei einlesen
dateipfad = 'C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/input.txt'  # Geben Sie den Pfad zur Textdatei an

with open(dateipfad, 'r') as file:
    user_input = file.readlines()  # Liest alle Zeilen in eine Liste


# Tokenize
tokenizer = SimpleTokenizer(user_input)
tokens = tokenizer.tokenize()

# Ausgabe
for token in tokens:
    print(token)
    
# Ausgabe in eine Textdatei speichern
output_dateipfad = 'C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/token_output.txt'

with open(output_dateipfad, 'w') as output_file:
    output_file.write("[\n")
    for i, token in enumerate(tokens):
        line = f"    [\"{token.line_number}\", \"{token.token_type}\", \"{token.value}\"],\n"
        # Entferne das Komma bei der letzten Zeile
        if i == len(tokens) - 1:
            line = line.rstrip(",\n") + "\n"
        output_file.write(line)
    output_file.write("]")
