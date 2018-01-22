%{
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

	#include "config.h"

	extern _AST_root ast_root;
	extern int yylineno;
	int is_typedef = 0;		/* flag to keep track of when inside a typedef */
	int in_par_list = 0;	/* flag to keep track of when inside a parameter_list */
	int in_function = 0;	/* flag to keep track of when inside a function body */
	int in_funcpar = 0;		/* flag to keep track of when inside a function parameter_list */
	char *type_id;			/* when in a typedef, save the identifier of the declared type */
%}

%union {
	_AST_ ast;
	_AST_root ast_root;
	_AST_int intval;
	_AST_float floval;
	_AST_string strval;
}

%token IDENTIFIER STRING_LITERAL CONSTANT_INT CONSTANT_FLO CONSTANT_STR
%token SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT
%token _RESTRICT _RESTRICT_ _CONST _CONST_
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token BOOL COMPLEX IMAGINARY
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token ATTRIBUTE ASM EXTENSION INCLUDE

%type <intval> CONSTANT_INT
%type <floval> CONSTANT_FLO
%type <strval> CONSTANT_STR
%type <strval> IDENTIFIER
%type <strval> STRING_LITERAL
%type <strval> TYPE_NAME

%type <strval> ATTRIBUTE
%type <strval> ASM asm_expression
%type <intval> EXTENSION
%type <strval> INCLUDE

%type <ast> attribute_specifier attribute_specifier_list asm_statement
%type <ast> struct_declaration_extended declaration_extended
%type <ast> include_directive

%type <intval> SIZEOF
%type <intval> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%type <intval> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%type <intval> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%type <intval> XOR_ASSIGN OR_ASSIGN
%type <intval> ';' '{' '}' ',' ':' '=' '(' ')' '[' ']' '.' '&' '!' '~' '-' '+' '*' '/' '%' '<' '>' '^' '|' '?'
%type <intval> TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT
%type <intval> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%type <intval> _RESTRICT _RESTRICT_ _CONST _CONST_
%type <intval> BOOL COMPLEX IMAGINARY
%type <intval> STRUCT UNION ENUM ELLIPSIS
%type <intval> GOTO CONTINUE BREAK RETURN
%type <intval> assignment_operator unary_operator struct_or_union

%type <ast> pointer
%type <ast> init_declarator init_declarator_list
%type <ast> initializer initializer_list
%type <ast> designator designator_list designation
%type <ast> identifier_list
%type <ast> direct_declarator declarator declarator_attributed direct_abstract_declarator abstract_declarator
%type <ast> declaration_specifiers specifier_qualifier_list storage_class_specifier type_qualifier type_qualifier_list type_specifier function_specifier
%type <ast> statement labeled_statement compound_statement expression_statement if_statement selection_statement iteration_statement jump_statement block_item
%type <ast> block_item_list
%type <ast> expression unary_expression constant_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression primary_expression postfix_expression argument_expression_list
%type <ast> type_name
%type <ast> struct_or_union_specifier struct_declaration struct_declaration_list struct_declarator struct_declarator_list
%type <ast> declaration parameter_declaration parameter_list parameter_type_list declaration_list
%type <ast> enum_specifier enumerator enumerator_list
%type <ast> function_definition external_declaration
%type <ast_root> translation_unit



%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE DUMMY
%nonassoc ','

%start translation_unit

%%

primary_expression
	: IDENTIFIER			{ $$ = AST_primary_id(yylineno,$1); }
	| CONSTANT_INT			{ $$ = AST_primary_int(yylineno,$1); }
	| CONSTANT_FLO			{ $$ = AST_primary_float(yylineno,$1); }
	| CONSTANT_STR			{ $$ = AST_primary_string(yylineno,$1); }
	| STRING_LITERAL		{ $$ = AST_primary_string(yylineno,$1); }
	| '(' expression ')'	{ $$ = AST_primary_exp(yylineno,$2); }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'					{ $$ = AST_subscript(yylineno,$1,$3); }
	| postfix_expression '(' ')'							{ $$ = AST_icall(yylineno,$1,NULL); }
	| postfix_expression '(' argument_expression_list ')'	{ $$ = AST_icall(yylineno,$1,$3); }
	| postfix_expression '.' IDENTIFIER						{ $$ = AST_structmbr(yylineno,$1,$3); }
	| postfix_expression PTR_OP IDENTIFIER					{ $$ = AST_ptrmbr(yylineno,$1,$3); }
	| postfix_expression INC_OP								{ $$ = AST_inc(yylineno,$1,$2); }
	| postfix_expression DEC_OP								{ $$ = AST_inc(yylineno,$1,$2); }
	| '(' type_name ')' '{' initializer_list '}'			{ $$ = AST_cast(yylineno,$2,$5); }
	| '(' type_name ')' '{' initializer_list ',' '}'		{ $$ = AST_cast(yylineno,$2,$5); }
	;

argument_expression_list
	: assignment_expression									{ $$ = AST_arglist(yylineno,NULL,$1); }
	| argument_expression_list ',' assignment_expression	{ $$ = AST_arglist(yylineno,$1,$3); }
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression			{ $$ = AST_unary(yylineno,$2,$1); }
	| DEC_OP unary_expression			{ $$ = AST_unary(yylineno,$2,$1); }
	| unary_operator cast_expression	{ $$ = AST_unary(yylineno,$2,$1); }
	| SIZEOF unary_expression			{ $$ = AST_unary(yylineno,$2,$1); }
	| SIZEOF '(' type_name ')'			{ $$ = AST_unary(yylineno,$3,$1); }
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression	{ $$ = AST_cast(yylineno,$2,$4);; }
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| multiplicative_expression '/' cast_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| multiplicative_expression '%' cast_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| additive_expression '-' multiplicative_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| relational_expression '>' shift_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| relational_expression LE_OP shift_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| relational_expression GE_OP shift_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	| equality_expression NE_OP relational_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = AST_condexp(yylineno,$1,$3,$5); }
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression	{ $$ = AST_binary(yylineno,$1,$2,$3); }
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression				{ $$ = AST_explist(yylineno,$1,$3); }
	;

constant_expression
	: conditional_expression
	;

declaration_extended
	: EXTENSION declaration								{ $$ = AST_declaration_extended($2); }
	| declaration
	;

declaration
	: declaration_specifiers init_declarator_list ';'	{
															if(in_funcpar && !in_function){
																in_funcpar = 0;
																scope_leave();
															}
															if(is_typedef && type_id) symboladd(type_id,TYPE_NAME);
															is_typedef = 0;
															type_id = NULL;
															$$ = AST_declaration(yylineno,DECLARATION,$1,$2);
														}
	| declaration_specifiers ';'						{ is_typedef = 0; $$ = AST_declaration(yylineno,DECLARATION,$1,NULL); }
	;

declaration_specifiers
	: storage_class_specifier							{ $$ = AST_declaration_specs(yylineno,$1,NULL); }
	| storage_class_specifier declaration_specifiers	{ $$ = AST_declaration_specs(yylineno,$1,$2); }
	| type_specifier declaration_specifiers				{ $$ = AST_declaration_specs(yylineno,$1,$2); }
	| type_specifier									{ $$ = AST_declaration_specs(yylineno,$1,NULL); }
	| type_qualifier declaration_specifiers				{ $$ = AST_declaration_specs(yylineno,$1,$2); }
	| type_qualifier									{ $$ = AST_declaration_specs(yylineno,$1,NULL); }
	| function_specifier								{ $$ = AST_declaration_specs(yylineno,$1,NULL); }
	| function_specifier declaration_specifiers			{ $$ = AST_declaration_specs(yylineno,$1,$2); }
	;

init_declarator_list
	: init_declarator										{ $$ = AST_declarator_init_list(yylineno,NULL,$1); }
	| init_declarator_list ',' init_declarator				{ $$ = AST_declarator_init_list(yylineno,$1,$3); }
	;

init_declarator
	: declarator_attributed					{ $$ = AST_declarator_init(yylineno,$1,NULL); }
	| declarator_attributed '=' initializer	{ $$ = AST_declarator_init(yylineno,$1,$3); }
	;

declarator_attributed
	: declarator							
	| attribute_specifier_list declarator	{ $$ = AST_declarator_attributed($2,$1,NULL); }
	| declarator attribute_specifier_list	{ $$ = AST_declarator_attributed($1,NULL,$2); }
	;

storage_class_specifier
	: TYPEDEF	{ is_typedef = 1; $$ = AST_spec_store(yylineno,$1); }
	| EXTERN	{ $$ = AST_spec_store(yylineno,$1); }
	| STATIC	{ $$ = AST_spec_store(yylineno,$1); }
	| AUTO		{ $$ = AST_spec_store(yylineno,$1); }
	| REGISTER	{ $$ = AST_spec_store(yylineno,$1); }
	;

type_specifier
	: VOID							{ $$ = AST_spec_type_int(yylineno,$1); }
	| CHAR							{ $$ = AST_spec_type_int(yylineno,$1); }
	| SHORT							{ $$ = AST_spec_type_int(yylineno,$1); }
	| INT							{ $$ = AST_spec_type_int(yylineno,$1); }
	| LONG							{ $$ = AST_spec_type_int(yylineno,$1); }
	| FLOAT							{ $$ = AST_spec_type_int(yylineno,$1); }
	| DOUBLE						{ $$ = AST_spec_type_int(yylineno,$1); }
	| SIGNED						{ $$ = AST_spec_type_int(yylineno,$1); }
	| UNSIGNED						{ $$ = AST_spec_type_int(yylineno,$1); }
	| BOOL							{ $$ = AST_spec_type_int(yylineno,$1); }
	| COMPLEX						{ $$ = AST_spec_type_int(yylineno,$1); }
	| IMAGINARY						{ $$ = AST_spec_type_int(yylineno,$1); }
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME						{ $$ = AST_spec_type_string(yylineno,$1);}
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_enter struct_declaration_list struct_leave '}'							{ $$ = AST_spec_struct(yylineno,$1,$2,$5,NULL); }
	| struct_or_union attribute_specifier_list IDENTIFIER '{' struct_enter struct_declaration_list struct_leave '}'	{ $$ = AST_spec_struct(yylineno,$1,$3,$6,$2); }
	| struct_or_union '{' struct_enter struct_declaration_list struct_leave '}'										{ $$ = AST_spec_struct(yylineno,$1,NULL,$4,NULL); }
	| struct_or_union attribute_specifier_list '{' struct_enter struct_declaration_list struct_leave '}'			{ $$ = AST_spec_struct(yylineno,$1,NULL,$5,$2); }
	| struct_or_union IDENTIFIER																					{ struct_leave(); $$ = AST_spec_struct(yylineno,$1,$2,NULL,NULL); }
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration_extended							{ $$ = AST_declaration_struct_list(yylineno,NULL,$1); }
	| struct_declaration_list struct_declaration_extended	{ $$ = AST_declaration_struct_list(yylineno,$1,$2); }
	;

struct_declaration_extended
	: EXTENSION struct_declaration							{ $$ = AST_declaration_extended($2); }
	| struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'	{ $$ = AST_declaration_struct(yylineno,$1,$2,NULL); }
	| struct_or_union_specifier ';'							{ $$ = AST_declaration_struct(yylineno,NULL,NULL,$1); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list				{ $$ = AST_sqlist(yylineno,$2,$1); }
	| type_specifier										{ $$ = AST_sqlist(yylineno,NULL,$1); }
	| type_qualifier specifier_qualifier_list				{ $$ = AST_sqlist(yylineno,$2,$1); }
	| type_qualifier										{ $$ = AST_sqlist(yylineno,NULL,$1); }
	;

struct_declarator_list
	: struct_declarator										{ $$ = AST_declarator_struct_list(yylineno,NULL,$1); }
	| struct_declarator_list ',' struct_declarator			{ $$ = AST_declarator_struct_list(yylineno,$1,$3); }
	;

struct_declarator
	: declarator											{ $$ = AST_declarator_struct(yylineno,$1,NULL); }
	| ':' constant_expression								{ $$ = AST_declarator_struct(yylineno,NULL,$2); }
	| declarator ':' constant_expression					{ $$ = AST_declarator_struct(yylineno,$1,$3); }
	;

enum_specifier
	: ENUM '{' struct_enter enumerator_list struct_leave '}'					{ $$ = AST_spec_enum(yylineno,NULL,$4); }
	| ENUM IDENTIFIER '{' struct_enter enumerator_list struct_leave '}'			{ $$ = AST_spec_enum(yylineno,$2,$5); }
	| ENUM '{' struct_enter enumerator_list ',' struct_leave '}' 				{ $$ = AST_spec_enum(yylineno,NULL,$4); }
	| ENUM IDENTIFIER '{' struct_enter enumerator_list ',' struct_leave '}' 	{ $$ = AST_spec_enum(yylineno,$2,$5); }
	| ENUM IDENTIFIER															{ struct_leave(); $$ = AST_spec_enum(yylineno,$2,NULL); }
	;

enumerator_list
	: enumerator						{ $$ = AST_enumerator_list(yylineno,NULL,$1); }
	| enumerator_list ',' enumerator	{ $$ = AST_enumerator_list(yylineno,$1,$3); }
	;

enumerator
	: IDENTIFIER							{ $$ = AST_enumerator(yylineno,$1,NULL); }
	| IDENTIFIER '=' constant_expression	{ $$ = AST_enumerator(yylineno,$1,$3); }
	;

type_qualifier
	: CONST			{ $$ = AST_qual_type(yylineno,$1); }
	| _CONST		{ $$ = AST_qual_type(yylineno,$1); }
	| _CONST_		{ $$ = AST_qual_type(yylineno,$1); }
	| RESTRICT		{ $$ = AST_qual_type(yylineno,$1); }
	| _RESTRICT		{ $$ = AST_qual_type(yylineno,$1); }
	| _RESTRICT_	{ $$ = AST_qual_type(yylineno,$1); }
	| VOLATILE		{ $$ = AST_qual_type(yylineno,$1); }
	;

function_specifier
	: INLINE		{ $$ = AST_spec_function(yylineno); }
	;

declarator
	: pointer direct_declarator asm_expression		{ $$ = AST_declarator(yylineno,$2,$1,$3); }
	| pointer direct_declarator						{ $$ = AST_declarator(yylineno,$2,$1,NULL); }
	| direct_declarator asm_expression				{ $$ = AST_declarator(yylineno,$1,NULL,$2); }
	| direct_declarator								{ $$ = AST_declarator(yylineno,$1,NULL,NULL); }
	;


direct_declarator
	: IDENTIFIER																	{
																						if(is_typedef && !in_par_list) type_id = $1;
																						else symboladd($1, IDENTIFIER);
																						$$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_1,$1,NULL,NULL,NULL);
																					}
	| '(' declarator ')'															{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_2,NULL,$2,NULL,NULL); }
	| direct_declarator '[' type_qualifier_list assignment_expression ']'			{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_3,NULL,$1,$3,$4); }
	| direct_declarator '[' type_qualifier_list ']'									{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_3,NULL,$1,$3,NULL); }
	| direct_declarator '[' assignment_expression ']'								{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_3,NULL,$1,NULL,$3); }
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'	{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_4,NULL,$1,$4,$5); }
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'	{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_5,NULL,$1,$3,$5); }
	| direct_declarator '[' type_qualifier_list '*' ']'								{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_6,NULL,$1,$3,NULL); }
	| direct_declarator '[' '*' ']'													{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_6,NULL,$1,NULL,NULL); }
	| direct_declarator '[' ']'														{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_7,NULL,$1,NULL,NULL); }
	| direct_declarator '(' funcpar_enter parameter_type_list ')'					{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_8,NULL,$1,$4,NULL); }
	| direct_declarator '(' identifier_list ')'										{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_8,NULL,$1,$3,NULL); }
	| direct_declarator '('  ')'													{ $$ = AST_declarator_direct(yylineno,DECLARATOR_DIRECT_8,NULL,$1,NULL,NULL); }
	;

pointer
	: '*'								{ $$ = AST_pointer(yylineno,NULL,NULL); }
	| '*' type_qualifier_list			{ $$ = AST_pointer(yylineno,$2,NULL); }
	| '*' pointer						{ $$ = AST_pointer(yylineno,NULL,$2); }
	| '*' type_qualifier_list pointer	{ $$ = AST_pointer(yylineno,$2,$3); }
	;

type_qualifier_list
	: type_qualifier						{ $$ = AST_tqlist(yylineno,NULL,$1); }
	| type_qualifier_list type_qualifier	{ $$ = AST_tqlist(yylineno,$1,$2); }
	;


parameter_type_list
	: parameter_list				{ $$ = AST_ptlist(yylineno,$1,0); }
	| parameter_list ',' ELLIPSIS	{ $$ = AST_ptlist(yylineno,$1,1); }
	;

parameter_list
	: parameter_declaration						{ $$ = AST_parlist(yylineno,NULL,$1); }
	| parameter_list ',' parameter_declaration	{ $$ = AST_parlist(yylineno,$1,$3); }
	;

parameter_declaration
	: declaration_specifiers declarator				{ $$ = AST_declaration(yylineno,DECLARATION_PARAMETER,$1,$2); }
	| declaration_specifiers abstract_declarator	{ $$ = AST_declaration(yylineno,DECLARATION_PARAMETER,$1,$2); }
	| declaration_specifiers						{ $$ = AST_declaration(yylineno,DECLARATION_PARAMETER,$1,NULL); }
	;

identifier_list
	: IDENTIFIER						{ $$ = AST_idlist(yylineno,NULL,$1); }
	| identifier_list ',' IDENTIFIER	{ $$ = AST_idlist(yylineno,$1,$3); }
	;

type_name
	: specifier_qualifier_list						{ $$ = AST_type_name(yylineno,$1,NULL); }
	| specifier_qualifier_list abstract_declarator	{ $$ = AST_type_name(yylineno,$1,$2); }
	;

abstract_declarator
	: pointer								{ $$ = AST_declarator_abstract(yylineno,NULL,$1); }
	| direct_abstract_declarator			{ $$ = AST_declarator_abstract(yylineno,$1,NULL); }
	| pointer direct_abstract_declarator	{ $$ = AST_declarator_abstract(yylineno,$2,$1); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'								{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_1,$2,NULL,NULL); }
	| '[' ']'													{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_2,NULL,NULL,NULL); }
	| '[' assignment_expression ']'								{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_2,NULL,$2,NULL); }
	| direct_abstract_declarator '[' ']'						{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_2,$1,NULL,NULL); }
	| direct_abstract_declarator '[' assignment_expression ']'	{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_2,$1,$3,NULL); }
	| '[' '*' ']'												{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_3,NULL,NULL,NULL); }
	| direct_abstract_declarator '[' '*' ']'					{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_3,$1,NULL,NULL); }
	| '(' ')'													{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_4,NULL,NULL,NULL); }
	| '(' scope_enter parameter_type_list scope_leave ')'		{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_4,NULL,NULL,$3); }
	| direct_abstract_declarator '(' ')'						{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_4,$1,NULL,NULL); }
	| direct_abstract_declarator '(' parlist_enter parameter_type_list parlist_leave ')' 	{ $$ = AST_declarator_direct_abstract(yylineno,DECLARATOR_ABSTRACT_DIRECT_4,$1,NULL,$4); }
	;

initializer
	: assignment_expression			{ $$ = AST_initializer(yylineno,NULL,$1); }
	| '{' initializer_list '}'		{ $$ = AST_initializer(yylineno,$2,NULL); }
	| '{' initializer_list ',' '}'	{ $$ = AST_initializer(yylineno,$2,NULL); }
	;

initializer_list
	: initializer									{ $$ = AST_initializer_list(yylineno,NULL,NULL,$1); }
	| designation initializer						{ $$ = AST_initializer_list(yylineno,NULL,$1,$2); }
	| initializer_list ',' initializer				{ $$ = AST_initializer_list(yylineno,$1,NULL,$3); }
	| initializer_list ',' designation initializer	{ $$ = AST_initializer_list(yylineno,$1,$3,$4); }
	;

designation
	: designator_list '='	{ $$ = AST_designation(yylineno,$1); }
	;

designator_list
	: designator					{ $$ = AST_designator_list(yylineno,NULL,$1); }
	| designator_list designator	{ $$ = AST_designator_list(yylineno,$1,$2); }
	;

designator
	: '[' constant_expression ']'	{ $$ = AST_designator(yylineno,$2,NULL);}
	| '.' IDENTIFIER				{ $$ = AST_designator(yylineno,NULL,$2);}
	;

statement
	: labeled_statement
	| scope_enter compound_statement scope_leave	{ $$ = $2; }
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| asm_statement
	;

labeled_statement
	: IDENTIFIER ':' statement							{ $$ = AST_stmt_labeled(yylineno,$1,NULL,$3,NULL); }
	| IDENTIFIER ':' attribute_specifier_list statement	{ $$ = AST_stmt_labeled(yylineno,$1,NULL,$4,$3); }
	| CASE constant_expression ':' statement			{ $$ = AST_stmt_labeled(yylineno,NULL,$2,$4,NULL); }
	| DEFAULT ':' statement								{ $$ = AST_stmt_labeled(yylineno,NULL,NULL,$3,NULL); }
	;

compound_statement
	: '{' '}'					{ $$ = AST_stmt_compound(yylineno,NULL); }
	| '{' block_item_list '}'	{ $$ = AST_stmt_compound(yylineno,$2); }
	;

block_item_list
	: block_item					{ $$ = AST_block_list(yylineno,NULL,$1); }
	| block_item_list block_item	{ $$ = AST_block_list(yylineno,$1,$2); }
	;

block_item
	: declaration_extended
	| statement
	;

expression_statement
	: ';'				{ $$ = AST_stmt_exp(yylineno,NULL); }
	| expression ';'	{ $$ = AST_stmt_exp(yylineno,$1); }
	;

if_statement
	: IF '(' expression ')' statement %prec LOWER_THAN_ELSE	{ $$ = AST_stmt_if(yylineno,$3,$5,NULL); }
	| IF '(' expression ')' statement ELSE statement		{ $$ = AST_stmt_if(yylineno,$3,$5,$7); }
	;

selection_statement
	: if_statement
	| SWITCH '(' expression ')' statement	{ $$ = AST_stmt_switch(yylineno,$3,$5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement 											{ $$ = AST_stmt_while(yylineno,$3,$5); }
	| DO statement WHILE '(' expression ')' ';'										{ $$ = AST_stmt_do(yylineno,$2,$5); }
	| FOR '(' expression_statement expression_statement ')' statement				{ $$ = AST_stmt_for(yylineno,$3,$4,NULL,$6); }
	| FOR '(' expression_statement expression_statement expression ')' statement	{ $$ = AST_stmt_for(yylineno,$3,$4,$5,$7); }
	| FOR '(' declaration expression_statement ')' statement						{ $$ = AST_stmt_for(yylineno,$3,$4,NULL,$6); }
	| FOR '(' declaration expression_statement expression ')' statement				{ $$ = AST_stmt_for(yylineno,$3,$4,$5,$7); }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{ $$ = AST_stmt_jump(yylineno,$1,$2,NULL); }
	| CONTINUE ';'			{ $$ = AST_stmt_jump(yylineno,$1,NULL,NULL); }
	| BREAK ';'				{ $$ = AST_stmt_jump(yylineno,$1,NULL,NULL); }
	| RETURN ';'			{ $$ = AST_stmt_jump(yylineno,$1,NULL,NULL); }
	| RETURN expression ';'	{ $$ = AST_stmt_jump(yylineno,$1,NULL,$2); }
	;

translation_unit
	: external_declaration					{ $$ = AST_unit($1); ast_root = $$;}
	| translation_unit external_declaration	{ $1->right = AST_unit($2); $$ = $1->right;}
	;

external_declaration
	: function_definition
	| declaration_extended
	| include_directive
	;

function_definition
	: declaration_specifiers declarator declaration_list function_enter compound_statement function_leave	{ $$ = AST_function(yylineno,$1,$2,$3,$5); }
	| declaration_specifiers declarator function_enter compound_statement function_leave					{ $$ = AST_function(yylineno,$1,$2,NULL,$4); }
	;

declaration_list
	: declaration					{ $$ = AST_declaration_list(yylineno,NULL,$1); }
	| declaration_list declaration	{ $$ = AST_declaration_list(yylineno,$1,$2); }
	;

attribute_specifier
	: ATTRIBUTE %prec DUMMY		{ $$ = AST_attribute(yylineno,$1,0); }
	| ATTRIBUTE ','				{ $$ = AST_attribute(yylineno,$1,1); }
	;

attribute_specifier_list
	: attribute_specifier								{ $$ = AST_attribute_list(yylineno,$1,NULL); }
	| attribute_specifier_list attribute_specifier 		{ $$ = AST_attribute_list(yylineno,$2,$1); }
	;

asm_expression
	: ASM
	;

asm_statement
	: asm_expression ';'	{ $$ = AST_asm(yylineno,$1); }
	;

include_directive
	: INCLUDE				{ $$ = AST_include(yylineno,$1); }
	;

funcpar_enter
	: scope_enter	{ in_funcpar = 1; }
	;

function_enter
	: scope_enter	{ in_function = 1; }
	;

function_leave
	: scope_leave	{
						if(in_funcpar){
							in_funcpar = 0;
							scope_leave();
						}
						in_function = 0;
					}
	;

struct_enter
	: scope_enter	{ struct_enter(); }
	;

struct_leave
	: scope_leave	{ struct_leave(); }
	;

parlist_enter
	: scope_enter	{ in_par_list = 1; }
	;

parlist_leave
	: scope_leave	{ in_par_list = 0; }
	;

scope_enter
	: { scope_enter(1); }
	;

scope_leave
	: { scope_leave(); }
	;

%%
