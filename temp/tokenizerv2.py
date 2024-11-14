from ply import lex

# Definiere Tokens
tokens = (
    'WRITE',
    'NUMTOKEN',
    'SEMICOLON',
)

# Reguläre Ausdrücke für einfache Tokens
t_WRITE = r'WRITE'
t_NUMTOKEN = r'\d+'
t_SEMICOLON = r';'

# Ignoriere Leerzeichen
t_ignore = ' \t'

# Fehlerbehandlung
def t_error(t):
    print(f"Illegal character '{t.value[0]}' at line {t.lineno}")
    t.lexer.skip(1)

# Lexer-Initialisierung
lexer = lex.lex()

# Beispiel-Code
#code = "WRITE 1 1 5 ;"
# Lese den Beispielcode aus der Datei "input.txt"
with open("C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/input.txt", "r") as file:
    code = file.read()



lexer.input(code)

# Ausgabe aller Tokens
tokens_output = []
for token in lexer:
    tokens_output.append([token.lineno, token.type, token.value])

# Speichern der Ausgabe in einer Textdatei
output_format = "[\n"
for token in tokens_output:
    output_format += f"    {token},\n"
output_format = output_format.rstrip(",\n") + "\n]"

# In eine Textdatei schreiben
#with open("tokens_output.txt", "w") as file:
 #   file.write(output_format)


with open("C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/token_output.txt", "w") as file:
    file.write(output_format)


print(tokens_output)
