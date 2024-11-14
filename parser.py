import subprocess
import json


# Textdatei öfnnen und auslesen
with open('C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/token_output.txt', 'r') as file:

   
    tokenized_input = json.load(file)  

# erstelle Json
json_input = json.dumps(tokenized_input)

#stdin = Standardeingabekanal
try:
    process = subprocess.Popen( #popen = starten vom externen parser
        ['C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/dsm_lemon/parse'],  # externer parser
        stdin=subprocess.PIPE,  # Schreiben in stdin
        stdout=subprocess.PIPE,  # lesen vom Rückgabewert
        stderr=subprocess.PIPE,  # error output
        text=True  # wenn false, error
    )

    # JSON werte an parser senden
    output = process.communicate(input=json_input)
    #print(output)

    stdout = output[0]  # Ausgabe der Executable
    stderr = output[1]  # Fehler der Executable


  
    if stderr: #falls error nicht leer
        print("Fehler:", stderr)

    # Ausgabe des Programms
    print("Ausgabe Executable:", stdout)

    output_dateipfad = 'C:/Users/Jonas/Documents/Masterstudium/Masterstudiengang/3. Semester/DSM/GIT/files/parser_output.txt'
    with open(output_dateipfad, 'w') as output_file:
        output_file.write(stdout)

except Exception as e:
    print(f"Es gab einen Fehler: {e}")
