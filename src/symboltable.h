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

/*
 * File:   symboltable.h
 * Author: Georgios Varisteas
 *
 * Created on March 28, 2010, 7:31 AM
 */

#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#define SYMTAB_NOTHING 0
#define SYMTAB_IDENTIFIER IDENTIFIER
#define SYMTAB_TYPENAME TYPE_NAME

/* enumeration of predefined values for symbol taxonomy */
typedef enum symboltype {
	symboltype_no = SYMTAB_NOTHING,		// unidentified type, entered as a safety measure
	symboltype_id = SYMTAB_IDENTIFIER,	// symbol identified as IDENTIFIER
	symboltype_tn = SYMTAB_TYPENAME		// symbol identified as TYPE_NAME
} symboltype;

/* the structure wrapper of a symbol, including the additional info of the symbol's type and scope */
typedef struct ST_symbol {
	symboltype type;	/* the type of the symbol, according to symboltype */
	char *word;			/* the string (symbol) to be stored */
	char scope;			/* the scope in which the symbol was defined */
} *ST_symbol;

/* the wrapper structure for the symbol table data */
struct ST_symboltable {
	TAB_table symtab;	/* the table of the symbols */
	TAB_table symind;	/* a table of shortcuts for the lists that were updated in each scope
						 * mitigates the need to exhaustively search symtab for scope-specific
						 * entries when releashing a scope */
	stack scopes;		/* a stack of the scope objects */
	int items;
};
typedef struct ST_symboltable *ST_symboltable;

void symtab_init(ST_symboltable);							/* initialize a symboltable */
void symtab_clear(ST_symboltable st);						/* clear and free a symboltable */

int symtab_get_scope(ST_symboltable symtab);
scope symtab_new_scope (ST_symboltable symtab, int state, int xclusive);
int symtab_end_scope (ST_symboltable symtab);

void symtab_release_scope(ST_symboltable, int);				/* release all entries in a given scope */
ST_symbol symtab_lookup(ST_symboltable, char *);			/* lookup a word in the symbol table and
															 * return the first symbol that matches */
ST_symbol symtab_new(ST_symboltable, char *, symboltype);	/* enter an identifier in the symbol table */
int symind_new(ST_symboltable, char *word);					/* add a new entry in the symind table */

#endif