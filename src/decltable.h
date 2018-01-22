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
 * File:   idtable.h
 * Author: Georgios Varisteas
 *
 * Created on May 10, 2010, 3:25 PM
 */

#ifndef __DECLTABLE_H__
#define	__DECLTABLE_H__

/* when visiting a decl node a new TR_decl struct is created;
 * its "old" member is set to the "current" member of the table;
 * its decl member is set to the ast argument.
 * Before leaving the visitor, "current" is restored to the
 * "old".
 * If no declaration was active upon entry, "current" will
 * be null, thus "old" will become null and eventually "current"
 * will be restored to null upon return.
 */

typedef enum DC_decl_type {
	DC_decl_tpdef = DC_TYPE,
	DC_decl_var = DC_VAR,
	DC_decl_func = DC_FUNC,
	DC_decl_array = DC_ARRAY,
	DC_decl_struct = DC_STRUCT,
	DC_decl_union = DC_UNION,
	DC_decl_enum = DC_ENUM,
} DC_decl_type;

/* the wrapper structure of a declaration, encapsulating a pointer to the AST declaration root
 * includes additional info for type and scope as well as an initialization flag.
 * the initialization flag is used to handle prototypes. An unitialized entry means that it is not
 * finished. But this has a different context to tha traversal pause. If a declaration is met and a
 * non-initialized id exists of the same type in the same scope, then this declaration is treated to be
 * a late declaration of a prototype and simply replaces the decl member of the pre existing entry. */
struct DC_decl;
typedef struct DC_decl *DC_decl;

struct DC_decl {
	char *id; /* the IDENTIFIER (symbol) to be stored */
	_AST_ specs; /* pointer to the corresponding AST node of the declaration specs for quick re-visiting */
	_AST_ declarator; /* pointer to the corresponding AST node of the declarator for quick re-visiting */
	_AST_ body; /* pointer to the corresponding AST node of the declaration body for quick re-visiting */
	int scope; /* the scope in which the symbol was defined */
	DC_decl old; /* a pointer to the decl that was 'current' before this one was created */
	DC_decl_type type; /* a flag identifying the type of declaration */
	char initialized; /* flag denoting the initialization status of the declaration */
};

/* the wrapper structure for the declaration table data */
typedef struct DC_table {
	TAB_table decltab; /* the table of the declarations */
	TAB_table declind; /* a table of shortcuts for the lists that were updated in each scope
						 * mitigates the need to exhaustively search decltab for scope-specific
						 * entries when releashing a scope */
	stack temp; /* temporary stack of declarations not yet finalized, a.k.a. entered into the table */
	int scope; /* a stack of the scope objects */
	int items; /* counter of the number of items in the table */
	DC_decl current;
} *DC_table;


void decltab_init(DC_table); /* initialize a symboltable */
void decltab_clear(DC_table); /* clear and free a symboltable */

DC_decl decltab_current(DC_table); /* get the current active declaration */
void decltab_replace_cur(DC_table, DC_decl); /* replace the first decl object with the current, eventually freeing the later */

void decltab_setid(DC_table, _AST_string); /* set the id member of a DC_decl structure */
void decltab_setspecs(DC_table, _AST_); /* set the specs member of a DC_decl structure */
void decltab_setdeclarator(DC_table, _AST_); /* set the decalrator member of a DC_decl structure */
void decltab_setbody(DC_table, _AST_); /* set the body member of a DC_decl structure */
_AST_int decltab_hastype(DC_table); /* returns 1 if the current declaration has a set type, 0 if not */
void decltab_settypedef(DC_table); /* set the current declaration as a typedef declaration */
void decltab_setvar(DC_table); /* set the current declaration as a variable declaration */
void decltab_setfunction(DC_table); /* set the current declaration as a function declaration */
void decltab_setarray(DC_table); /* set the current declaration as an array declaration */
void decltab_setstruct(DC_table); /* set the current declaration as an struct declaration */
void decltab_setunion(DC_table); /* set the current declaration as an union declaration */
void decltab_setenum(DC_table); /* set the current declaration as an enum declaration */

_AST_int decltab_scope_get(DC_table);	/* return the current scope id */
_AST_int decltab_scope_new(DC_table);	/* initialize a new scope */
_AST_int decltab_scope_end(DC_table);

DC_decl decltab_lookup(DC_table, _AST_string, DC_decl_type);	/* lookup a word in the symbol table and
																 * return the first symbol that matches */

DC_decl decltab_new(DC_table, _AST_string, _AST_, _AST_, _AST_);/* enter an identifier in the symbol table */
DC_decl decltab_finalize(DC_table);								/* Add a declaration to the table */

#endif	/* __DECLTABLE_H__ */

