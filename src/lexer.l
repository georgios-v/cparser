/*
 *	Licensed to the Apache Software Foundation (ASF) under one
 *	or more contributor license agreements.  See the NOTICE file
 *	distributed with this work for additional information
 *	regarding copyright ownership.  The ASF licenses this file
 *	to you under the Apache License, Version 2.0 (the
 *	"License"); you may not use this file except in compliance
 *	with the License.  You may obtain a copy of the License at
 *	
 *	     http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing,
 *	software distributed under the License is distributed on an
 *	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *	KIND, either express or implied.  See the License for the
 *	specific language governing permissions and limitations
 *	under the License.
 */


/* some complex named patterns */
/* Universal Character Name */
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
/* float exponent */
EXP ([Ee][-+]?[0-9]+)
/* integer length */
ILEN ([Uu](L|l|LL|ll)?|(L|l|LL|ll)[Uu]?)


/*
%option reentrant bison-bridge
*/
%option noyywrap
%option yylineno
%x SC_COMMENT
%x SC_ASM

%{
	#include "config.h"

	extern YYSTYPE yylval;
	extern char yychar;
	extern int yylloc;
//	extern ST_symboltable symtab;

	int prevtok = 0;				/* keeps a reference of the previous token */
	int previous_sc = 0;			/* keeps the previous start condition to return to at the end of a scope */
	int instruct = 0;
	int defining = 0;

	char *linebuf;					/* buffer to keep a copy of the current line for error printing */
	int debug_parse = 0;			/* a flag activating debug mode for the parser */

%}
%%
	/* next line */
\n.* 	{
			if(linebuf != NULL) free(linebuf);				/* free the previous line */
			linebuf = strdup(yytext);	 					/* save the next line */
			if(debug_parse)
				fprintf(stderr, "\n %4d: ",yylineno);		/* makes easier to map tokens to source */
			yyless(1); 										/* give back all but the <NEW_LINE> to rescan */
		}

	/* comments */
"/*" 					{ previous_sc = YY_START; BEGIN(SC_COMMENT); }
<SC_COMMENT>"*/" 		{ BEGIN(previous_sc); }
<SC_COMMENT>([^*]|\n)+|.
<SC_COMMENT><<EOF>> 	{ yyerror("error: Unterminated comment"); abort(); }
"//".*\n


	/* GCC extensions */
__extension__					{ return token(EXTENSION); }
__attribute__[ \t]*"((".*"))"	{ yylval.strval = strdup(yytext); return token(ATTRIBUTE); }

(asm|__asm__)([ \t]+(volatile|goto))?[ \t]*\([ \t]*.*\)		{ yylval.strval = strdup(yytext);return token(ASM); }

__const 		{ return token(_CONST);}
__const__ 		{ return token(_CONST_);}
__restrict 		{ return token(_RESTRICT);}
__restrict__	{ return token(_RESTRICT_);}


	/* declaration keywords */
_Bool 		{ defining = (defining != 2?1:2); return token(BOOL);}
_Complex 	{ defining = (defining != 2?1:2); return token(COMPLEX);}
_Imaginary 	{ defining = (defining != 2?1:2); return token(IMAGINARY);}
char 		{ defining = (defining != 2?1:2); return token(CHAR);}
double 		{ defining = (defining != 2?1:2); return token(DOUBLE);}
float 		{ defining = (defining != 2?1:2); return token(FLOAT);}
int 		{ defining = (defining != 2?1:2); return token(INT);}
long 		{ defining = (defining != 2?1:2); return token(LONG);}
short 		{ defining = (defining != 2?1:2); return token(SHORT);}
signed 		{ defining = (defining != 2?1:2); return token(SIGNED);}
unsigned 	{ defining = (defining != 2?1:2); return token(UNSIGNED);}
void 		{ defining = (defining != 2?1:2); return token(VOID);}
enum 		{ defining = (defining != 2?1:2); instruct = 1; return token(ENUM); }
struct 		{ defining = (defining != 2?1:2); instruct = 1; return token(STRUCT); }
union 		{ defining = (defining != 2?1:2); instruct = 1; return token(UNION); }
typedef 	{ defining = 2; return token(TYPEDEF);}
extern 		{ defining = 2; return token(EXTERN);}
static 		{ defining = 2; return token(STATIC);}
auto 		{ defining = 2; return token(AUTO);}
register 	{ defining = 2; return token(REGISTER);}
inline 		{ return token(INLINE);}
const 		{ return token(CONST);}
restrict 	{ return token(RESTRICT);}
volatile 	{ return token(VOLATILE);}


	/* keywords */
break		{ return token(BREAK);}
case		{ return token(CASE);}
continue	{ return token(CONTINUE);}
default		{ return token(DEFAULT);}
goto		{ return token(GOTO);}
return		{ return token(RETURN);}
sizeof		{ return token(SIZEOF);}
switch		{ return token(SWITCH);}
do			{ return token(DO);}
else		{ return token(ELSE);}
for			{ return token(FOR);}
if			{ return token(IF);}
while		{ return token(WHILE);}

	/* integers */
0[0-7]*{ILEN}?				{ return setconstval(CONSTANT_INT); }
[1-9][0-9]*{ILEN}?			{ return setconstval(CONSTANT_INT); }
0[Xx][0-9a-fA-F]+{ILEN}?	{ return setconstval(CONSTANT_INT); }

	/* decimal float */
([0-9]*\.[0-9]+|[0-9]+\.){EXP}?[flFL]?	{ return setconstval(CONSTANT_FLO); }
[0-9]+{EXP}[flFL]?						{ return setconstval(CONSTANT_FLO); }

	/* hex float */
0[Xx]([0-9a-fA-F]*\.[0-9a-fA-F]+|[0-9a-fA-F]+\.?)[Pp][-+]?[0-9]+[flFL]?	{ return setconstval(CONSTANT_STR); }

	/* char const */
\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})+\'	{ return setconstval(CONSTANT_STR); }
		

	/* string literal */
L?\"([^"\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9a-fA-F]+|{UCN})*\"	{ return setconstval(CONSTANT_STR); }


	/* punctuators */
"..."			{ return token(ELLIPSIS); }
">>="			{ return token(RIGHT_ASSIGN); }
"<<="			{ return token(LEFT_ASSIGN); }
"+="			{ return token(ADD_ASSIGN); }
"-="			{ return token(SUB_ASSIGN); }
"*="			{ return token(MUL_ASSIGN); }
"/="			{ return token(DIV_ASSIGN); }
"%="			{ return token(MOD_ASSIGN); }
"&="			{ return token(AND_ASSIGN); }
"^="			{ return token(XOR_ASSIGN); }
"|="			{ return token(OR_ASSIGN); }
">>"			{ return token(RIGHT_OP); }
"<<"			{ return token(LEFT_OP); }
"++"			{ return token(INC_OP); }
"--"			{ return token(DEC_OP); }
"->"			{ return token(PTR_OP); }
"&&"			{ return token(AND_OP); }
"||"			{ return token(OR_OP); }
"<="			{ return token(LE_OP); }
">="			{ return token(GE_OP); }
"=="			{ return token(EQ_OP); }
"!="			{ return token(NE_OP); }
";"				{ if((!instruct && defining == 2) || defining) defining = 0; return token(';'); }
("{"|"<%")		{ instruct = 0; return token('{'); }
("}"|"%>")		{ return token('}'); }
","				{ defining = 0; return token(','); }
":"				{ return token(':'); }
"="				{ return token('='); }
"("				{ return token('('); }
")"				{ return token(')'); }
("["|"<:")		{ return token('['); }
("]"|":>")		{ return token(']'); }
"."				{ return token('.'); }
"&"				{ return token('&'); }
"!"				{ return token('!'); }
"~"				{ return token('~'); }
"-"				{ return token('-'); }
"+"				{ return token('+'); }
"*"				{ return token('*'); }
"/"				{ return token('/'); }
"%"				{ return token('%'); }
"<"				{ return token('<'); }
">"				{ return token('>'); }
"^"				{ return token('^'); }
"|"				{ return token('|'); }
"?"				{ return token('?'); }

	/* new identifier */
([_a-zA-Z]|{UCN})([_a-zA-Z0-9]|{UCN})* 	{ return ret_symbol();}

	/* whitespace */
[ \t]+

	/* continued line */
\\$

	/* some preprocessor stuff */
[ \t]*"#"[ \t]*include[ \t]+((\".*\")|(\<.*\>))	{ yylval.strval = strdup(yytext); return token(INCLUDE); }
"#".*
[ \t]*"#"[ \t]*(if|else|endif|define|undef|line|pragma).*

	/* Unmatched characters */
.			{ unmatched(); }

%%

void unmatched(void){
	yyerror("error: found unmatched character");
	char c = 0;
	while ((c = input()) != 0)      /* (EOF maps to 0) */
	{
		if (c == '\n')
			break;
	}
	abort();
}

void yyerror(char *s){
	if(debug_parse) {
		if(yylval.intval < 1000) fprintf(stderr, "\ntoken %d\n", yylval.intval);
		else fprintf(stderr, "\ntoken %d string %s\n", prevtok, yylval.strval);
		fprintf(stderr, "\n defining %d\n", defining);
		fprintf(stderr, "\n instruct %d\n", instruct);
	}
	fprintf(stderr, "\n%s:%d: %s at %s in this line:  \n\t\t%s\n", curfilename, yylineno, s, yytext, linebuf);
	abort();
}

int token(int tok){
	if(debug_parse) fprintf(stderr, "%d ", tok);
	if(tok != IDENTIFIER && tok != CONSTANT_INT && tok != CONSTANT_FLO &&
		tok != CONSTANT_STR && tok != TYPE_NAME && tok != STRING_LITERAL &&
		tok != ATTRIBUTE && tok != ASM && tok != INCLUDE)
		yylval.intval = tok;
	prevtok = tok;
	return tok;
}

int ret_symbol(){
	/* save the matched string for parser retrieval */
	yylval.strval = strdup(yytext);
	/* if the previous token was a structure member access operator, this is an IDENTIFIER */
	if(prevtok == '.' || prevtok == PTR_OP){
		return token(IDENTIFIER);
	}
	/* if we are in the structure declaration (before first brace or
		semicolon) this is the structure tag */
	if(instruct == 1){
		// fprintf(stderr, "\n instruct %d\n", instruct);
		instruct = 0;
		return token(IDENTIFIER);
	}
	/* if we have met a TYPE_NAME this is an identifier */
	if(defining == 1 && !instruct){
		// fprintf(stderr, "\n defining %d\n", defining);
		defining = 0;
		return token(IDENTIFIER);
	}
	if(defining == 1) defining = 0;
	/* search the symbol table */
	char *word = strdup(yytext);
	ST_symbol s = symtab_lookup(symtab, word);
	free(word);
	/* examine the retrieved symbol's type */
	if(s != NULL){
		if(s->type == TYPE_NAME){
			defining = 1;
			return token(TYPE_NAME);
		}else return token(IDENTIFIER);
	}
	/* in all other cases this is an IDENTIFIER */
	return token(IDENTIFIER);
}

int setconstval(int type){
	switch(type){
		case CONSTANT_INT:
			yylval.intval = atoi(yytext);
			break;
		case CONSTANT_FLO:
			yylval.floval = atof(yytext);
			break;
		case CONSTANT_STR:
			yylval.strval = strdup(yytext);
			break;
		default:
			assert(0);
			return 0;
	}
	return token(type);
}

/* Wrapper to initialize a new scope but keep the same state */
void scope_enter(int xclusive){
	int oldscope = 0;
	int olditems = 0;
	if(debug_parse){
		oldscope = symtab_get_scope(symtab);
		olditems = symtab->items;
	}
	symtab_new_scope(symtab, YY_START, xclusive);
	if(debug_parse)
		fprintf(stderr, " { SCOPE: in %d(%d), has %d(%d) } ",symtab_get_scope(symtab),oldscope,symtab->items,olditems);
}

/* Wrapper to release current scope and start the previous state */
void scope_leave(){
	int oldscope = 0;
	int olditems = 0;
	if(debug_parse){
		oldscope = symtab_get_scope(symtab);
		olditems = symtab->items;
	}
	int state = symtab_end_scope(symtab);
	if(debug_parse)
		fprintf(stderr, " { SCOPE: out %d(%d) has %d(%d) } ",oldscope,symtab_get_scope(symtab),olditems,symtab->items);
	/* re-activate that start condition */
	BEGIN(state);
}

/* Wrapper procedure to add a symbol to the symboltable */
void symboladd(char * id,int type){
	if(debug_parse)
		fprintf(stderr, " { SYMTAB: %s %s } ",(type == IDENTIFIER?"IDENTIFIER":"TYPE_NAME"),id);
	symtab_new(symtab, id, type);
}

void struct_enter(){
	instruct = 2;
}

void struct_leave(){
	instruct = 0;
}