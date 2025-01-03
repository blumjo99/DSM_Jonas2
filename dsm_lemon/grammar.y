%include { 

#include "grammar.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <iostream>
#include "cJSON.h"


using namespace std;
int get_token_id (char*);
const char *getValue (cJSON *token);
const char *getLine (cJSON *token);
cJSON *unary (char *fname, cJSON *a); 
cJSON *binary (char *fname, cJSON *a, cJSON *b); 
cJSON *ternary (char *fname, cJSON *a, cJSON *b, cJSON *c); 
char *linenumber;
char *curtoken;
char *curtype;
}  
   
   
%code {

using namespace std;
typedef struct {char *value; int line;} token;

token* create_token (char *value, int line) {
	token *t = (token*) malloc (sizeof (token));
	t->value = strdup (value);
	t->line = line;
	return t;
}

const char * getValue (cJSON* token) {
	return cJSON_GetObjectItem (token, "value")->valuestring;
}


const char * getLine (cJSON* token) {
	return cJSON_GetObjectItem (token, "line")->valuestring;
}


int main(int argc, char* argv[]) {
	char *result;
	std::string line;
	std::string input = "";
	while (std::getline(std::cin, line)) {
      input += line + "\n";
    }
	if (input == "") {
		cout << "Empty input";
		exit(0);
	}
	
	cJSON *root = cJSON_Parse(input.c_str());
	
	if (!root) {
		cout << "JSON invalid\n";
		exit(0);
	}
	
	void* pParser = ParseAlloc (malloc);
	int num = cJSON_GetArraySize (root);
	
	for (int i = 0; i < num; i++ ) {
	
		// Knoten im Token-Stream auslesen
		cJSON *node = cJSON_GetArrayItem(root,i);
		
		char *line = cJSON_GetArrayItem(node,0)->valuestring;
		char *type = cJSON_GetArrayItem(node,1)->valuestring;
		char *value = cJSON_GetArrayItem(node,2)->valuestring;
		
		cJSON *tok = cJSON_CreateObject();
		cJSON_AddStringToObject(tok, "value", value);
		cJSON_AddStringToObject(tok, "line", line);

		linenumber = line;
		curtoken = value;
		curtype = type;
		// THE und Kommentare werden ueberlesen
		if (strcmp(type, "THE") == 0) continue;
		if (strcmp(type, "COMMENT") == 0) continue;
		if (strcmp(type, "MCOMMENT") == 0) continue;
		
		int tokenid = get_token_id (type);
		Parse (pParser, tokenid, tok);
		
	}
	Parse (pParser, 0, 0);
    ParseFree(pParser, free );
}




 
/////////////////////// 
// TOKENS
///////////////////////
 

int get_token_id (char *token) {
	if (strcmp(token, "DIVIDE") == 0) return DIVIDE;
	if (strcmp(token, "IDENTIFIER") == 0) return IDENTIFIER;
	if (strcmp(token, "LPAR") == 0) return LPAR; 
	if (strcmp(token, "RPAR") == 0) return RPAR;
	if (strcmp(token, "MINUS") == 0) return MINUS;
	if (strcmp(token, "NUMTOKEN") == 0) return NUMTOKEN;
	if (strcmp(token, "PLUS") == 0) return PLUS;
	if (strcmp(token, "POWER") == 0) return POWER;
	if (strcmp(token, "SEMICOLON") == 0) return SEMICOLON;
	if (strcmp(token, "STRTOKEN") == 0) return STRTOKEN;
	if (strcmp(token, "WRITE") == 0) return WRITE;

	if (strcmp(token, "TRACE") == 0) return TRACE; 
	if (strcmp(token, "AMPERSAND") == 0) return AMPERSAND;
	if (strcmp(token, "ASSIGN") == 0) return ASSIGN;
	if (strcmp(token, "TRUE") == 0) return TRUE;
	if (strcmp(token, "FALSE") == 0) return FALSE;
	if (strcmp(token, "NULL") == 0) return NULL1;

	if (strcmp(token, "RSPAR") == 0) return RSPAR;
	if (strcmp(token, "LSPAR") == 0) return LSPAR;
	if (strcmp(token, "COMMA") == 0) return COMMA;

	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "NUMBER") == 0) return NUMBER;
	if (strcmp(token, "STRING") == 0) return STRING;

	if (strcmp(token, "LIST") == 0) return LIST;
	if (strcmp(token, "COUNT") == 0) return COUNT;
	if (strcmp(token, "FIRST") == 0) return FIRST;

	if (strcmp(token, "NOT") == 0) return NOT;

	if (strcmp(token, "TIMETOKEN") == 0) return TIMETOKEN;
	if (strcmp(token, "TIME") == 0) return TIME;
	if (strcmp(token, "TIMES") == 0) return TIMES;
	if (strcmp(token, "CURRENTTIME") == 0) return CURRENTTIME;

	if (strcmp(token, "NOW") == 0) return NOW;

	if (strcmp(token, "WHERE") == 0) return WHERE;
	if (strcmp(token, "LESS") == 0) return LESS;
	if (strcmp(token, "GREATER") == 0) return GREATER;


	if (strcmp(token, "IS") == 0) return IS;
	if (strcmp(token, "IT") == 0) return IT;

	if (strcmp(token, "THAN") == 0) return THAN;

	if (strcmp(token, "EQUAL") == 0) return EQUAL;
	if (strcmp(token, "OR") == 0) return OR;

	if (strcmp(token, "TO") == 0) return TO;
	if (strcmp(token, "WITHIN") == 0) return WITHIN;

	if (strcmp(token, "OF") == 0) return OF;

	if (strcmp(token, "OCCURS") == 0) return OCCURS;
	if (strcmp(token, "OCCURRED") == 0) return OCCURRED;
	if (strcmp(token, "AFTER") == 0) return AFTER;


	if (strcmp(token, "BEFORE") == 0) return BEFORE;

	if (strcmp(token, "THEY") == 0) return IT;


	if (strcmp(token, "FOR") == 0) return FOR;
	if (strcmp(token, "IN") == 0) return IN;
	if (strcmp(token, "DO") == 0) return DO;
	if (strcmp(token, "ENDDO") == 0) return ENDDO;
	if (strcmp(token, "FORRANGE") == 0) return FORRANGE;

	if (strcmp(token, "IF") == 0) return IF;
	if (strcmp(token, "THEN") == 0) return THEN;
	if (strcmp(token, "ELSEIF") == 0) return ELSEIF;
	if (strcmp(token, "ELSE") == 0) return ELSE;
	if (strcmp(token, "ENDIF") == 0) return ENDIF;

	
	printf ("{\"error\" : true, \"message\": \"UNKNOWN TOKEN TYPE %s\"}\n", token);
	exit(0);
} 
  


cJSON* unary (char* fname, cJSON* a) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a);
	cJSON_AddStringToObject(res, "type", fname);
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
} 



cJSON* binary (char *fname, cJSON *a, cJSON *b) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



cJSON* ternary (char *fname, cJSON *a, cJSON *b, cJSON *c) 
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a); 
	cJSON_AddItemToArray(arg, b); 
	cJSON_AddItemToArray(arg, c);
	cJSON_AddStringToObject(res, "type", fname); 
	cJSON_AddItemToObject(res, "arg", arg);
	return res;
}



}
   
%syntax_error {
  printf ("{\"error\" : true, \"message\": \"Syntax Error: Compiler reports unexpected token \\\"%s\\\" of type \\\"%s\\\" in line %s\"}\n", curtoken, curtype, linenumber);
  exit(0);
} 
  
%token_type {cJSON *}
%default_type {cJSON *} 

/////////////////////// 
/////////////////////// 
// PRECEDENCE
///////////////////////
/////////////////////// 

%right 	   WHERE.
%right 	   NOT .

%left 	   PLUS MINUS EQUAL LESS THAN .
%right	   		TIME .

%left 	   IS .
%left 	   TIMES DIVIDE  .
%right     POWER .
%right		IT .

%nonassoc LSPAR .


/////////////////////// 
// CODE
///////////////////////


 
code ::= statementblock(sb) . 
{
	printf (cJSON_Print(sb)); 
}  



/////////////////////// 
// STATEMENTBLOCK
///////////////////////


statementblock(sb) ::= .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENTBLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg); 
	sb = res;
}


statementblock(sb) ::= statementblock(a) statement(b) .
{
	cJSON_AddItemToArray(cJSON_GetObjectItem ( a, "statements"), b);
	sb = a;
}




///////////////////////////
// STATEMENTS
///////////////////////////

statement(r) ::= WRITE ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "WRITE"); 
	cJSON_AddItemToObject(res, "arg", e); 
	r = res; 
}


statement(r) ::= TRACE(t) ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRACE");
	cJSON_AddStringToObject(res, "line", getLine(t));
	cJSON_AddItemToObject(res, "arg", e);
	r = res;
}


statement(r) ::= IDENTIFIER(i) LSPAR ex(index) RSPAR ASSIGN ex(e) SEMICOLON .
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "type", "LIST_ASSIGN");
    cJSON_AddStringToObject(res, "varname", getValue(i)); 
    cJSON_AddItemToObject(res, "list_index", index);     
    cJSON_AddItemToObject(res, "arg", e); 
    r = res;
}

statement(r) ::= IDENTIFIER(i) ASSIGN ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "VARIABLE_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(i));
	cJSON_AddItemToObject(res, "arg", e);
	r = res;
}

statement(r) ::= TIME IDENTIFIER(i) ASSIGN ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_ASSIGN");
	cJSON_AddStringToObject(res, "varname", getValue(i));
	cJSON_AddItemToObject(res, "arg", e);
	r = res;
}

statement(r) ::= TIME OF IDENTIFIER(i) ASSIGN ex(e) SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIME_OF");
	cJSON_AddStringToObject(res, "varname", getValue(i));
	cJSON_AddItemToObject(res, "arg", e);
	r = res;
}

statement(r) ::= FOR IDENTIFIER(i) IN ex(e) FORRANGE ex(e2) DO statementblock(sb) ENDDO SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FOR_LOOP");
	cJSON_AddStringToObject(res, "varname", getValue(i));
	cJSON_AddItemToObject(res, "expression", e);
	cJSON_AddItemToObject(res, "expression2", e2);

	cJSON_AddItemToObject(res, "statements", sb);
	r = res;
}


statement(r) ::= IF if_then_else(a) .
{ r = a; }

if_then_else(r) ::= ex(a) THEN statementblock(b) elseif(c) .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IF");
	cJSON_AddItemToObject(res, "condition", a);
	cJSON_AddItemToObject(res, "thenbranch", (b));
	cJSON_AddItemToObject(res, "elsebranch", (c));
	r = res;
}

elseif(r) ::= ENDIF SEMICOLON .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STATEMENT_BLOCK");
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToObject(res, "statements", arg);
	r = res;
}

elseif(r) ::= ELSE statementblock(a) ENDIF SEMICOLON .
{ r = a; }

elseif(r) ::= ELSEIF if_then_else(a) .
{ r = a; }





///////////////////////////
// EXPRESSION
///////////////////////////



ex(r) ::= NUMTOKEN (a).        
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NUMBER"); 
	cJSON_AddStringToObject(res, "value", getValue(a)); 
	r = res; 
} 

ex(r) ::= TIMETOKEN (a).
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TIMETOKEN");
	cJSON_AddStringToObject(res, "value", getValue(a));
	r = res;
}

ex(r) ::= STRTOKEN (a).        
{ 
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "STRTOKEN"); 
	cJSON_AddStringToObject(res, "value", getValue(a)); 
	r = res; 
}

ex(r) ::= IDENTIFIER(a) .      
{ 
	cJSON *res = cJSON_CreateObject(); 
	cJSON_AddStringToObject(res, "type", "VARIABLE"); 
	cJSON_AddStringToObject(res, "name", getValue(a)); 
	cJSON_AddStringToObject(res, "line", getLine(a)); 
	r = res; 
}

ex(r) ::= TRUE .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "TRUE");
	r = res;
}

ex(r) ::= FALSE .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "FALSE");
	r = res;
}

ex(r) ::= CURRENTTIME .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "CURRENT_TIME");
	r = res;
}

ex(r) ::= NOW .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NOW");
	r = res;
}

ex(r) ::= NULL1 .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "NULL");
	r = res;
}

of::= .
of::= OF.

ex(r) ::= TIME of ex(a) .
{ r = unary ("TIME_READ", a); }

ex(r) ::= ex(a) PLUS ex(b) .                                
{r = binary ("PLUS", a, b); }

ex(r) ::= ex(a) MINUS ex(b) .                               
{r = binary ("MINUS", a, b); }

ex(r) ::= ex(a) TIMES ex(b) .                               
{r = binary ("TIMES", a, b); }

ex(r) ::= ex(a) DIVIDE ex(b) .                              
{r = binary ("DIVIDE", a, b); }

ex(r) ::= ex(a) POWER ex(b) .                               
{r = binary ("POWER", a, b); }

ex(r) ::= ex(a) AMPERSAND ex(b) .
{ r = binary ("STR_CONCAT", a, b); }

ex(r) ::= ex(a) IS NUMBER .
{ r = unary ("IS_NUMBER", a); }

ex(r) ::= ex(a) IS STRING .
{ r = unary ("IS_STRING", a); }

ex(r) ::= ex(a) IS LIST .
{ r = unary ("IS_LIST", a); }

ex(r) ::= ex(a) IS NOT LIST .
{ r = unary ("IS_NOT_LIST", a); }

ex(r) ::= LPAR ex(a) RPAR .    
{ 
	r = a; 
}





ex(r) ::= jsonarray (a) .
{ r = a; }

jsonarray(r) ::= LSPAR RSPAR .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "EMPTY_LIST");
	r = res;
}

jsonarray(r) ::= LSPAR exlist(a) RSPAR .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "LIST");
	cJSON_AddItemToObject(res, "items", a);
	cJSON_AddNumberToObject(res,"code", 1016);
	r = res;
}

exlist(r) ::= ex(a) .
{
	cJSON *arg = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a);
	r = arg;
}

exlist(r) ::= exlist(a) COMMA ex(b) .
{
	cJSON_AddItemToArray(a,b);
	r = a;
}

ex(r) ::= COUNT ex(a) .
{ r = unary ("COUNT", a); }

ex(r) ::= FIRST ex(a) .
{ r = unary ("FIRST", a); }

ex(r) ::= ex(a) WHERE ex(b) .
{
	cJSON *res = cJSON_CreateObject();
	cJSON *arg = cJSON_CreateArray();
	cJSON *opt = cJSON_CreateArray();
	cJSON_AddItemToArray(arg, a);
	cJSON_AddStringToObject(res, "type", "WHERE");
	cJSON_AddItemToObject(res, "arg", arg);
	cJSON_AddItemToArray(opt, b);
	cJSON_AddItemToObject(res, "opt", opt);
	r = res;
}

ex(r) ::= ex(a) IS LESS THAN ex(b) .
{ 
	r = binary ("LESS_THAN", a, b); 
}

ex(r) ::= ex(a) IS LESS THAN OR EQUAL ex(b) .
{ 
	r = binary ("LESS_OR_EQUAL", a, b); 
}

ex(r) ::= ex(a) IS GREATER THAN ex(b) .
{ 
	r = binary ("GREATER_THAN", a, b); 
}

ex(r) ::= ex(a) IS GREATER THAN OR EQUAL ex(b) .
{ 
	r = binary ("GREATER_OR_EQUAL", a, b); 
}

ex(r) ::= ex(a) IS WITHIN ex(b) TO ex(c)  .
{ 
	r = ternary ("IS_WITHIN", a, b, c); 
}

ex(r) ::= IT .
{
	cJSON *res = cJSON_CreateObject();
	cJSON_AddStringToObject(res, "type", "IT");
	r = res;
}

ex(r) ::= ex(a) OCCURS BEFORE ex(b) .
{ r = binary ("OCCURS_BEFORE", a, b); }

ex(r) ::= ex(a) OCCURRED AFTER ex(b) .
{ r = binary ("OCCURRED_AFTER", a, b); }