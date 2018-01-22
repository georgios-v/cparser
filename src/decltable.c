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

/* get the previous active declaration */
DC_decl decltab_previous(DC_table);
/* make a declaration the active declaration */
void decltab_makecurrent(DC_table, DC_decl);
/* deactivate the current declaration and return the previous or NULL */
DC_decl decltab_deactivate(DC_table);
/* reposition the latest cell at index */
void decltab_reposition_current(DC_table, int, char *);
/* add a new entry in the symind table */
_AST_int declind_new(DC_table, _AST_string);
/* release all entries in a given scope */
void decltab_scope_release(DC_table, _AST_int);

extern int debug_ast;			/* a flag activating debug mode for the AST traversal */

/* initialize a symboltable */
void decltab_init(DC_table dt) {
	dt->scope = 0;
	dt->items = 0;
	dt->current = NULL;
	dt->decltab = checked_malloc(sizeof (*(dt->decltab)));
	TAB_init(dt->decltab);
	dt->declind = checked_malloc(sizeof (*(dt->declind)));
	TAB_init(dt->declind);
	dt->temp = checked_malloc(sizeof (*(dt->temp)));
	StackInitialize(dt->temp);
}

void decltab_clear(DC_table dt) {
	TAB_clear(dt->declind);
	free(dt->declind);
	TAB_clear(dt->decltab);
	free(dt->decltab);
	scope_clear(dt->temp);
	free(dt->temp);
	dt->scope = 0;
	dt->items = 0;
	dt->current = NULL;
}

/* get the last entry to the table */
DC_decl decltab_current(DC_table dt) {
	return dt->current;
}

/* get the previous active declaration */
DC_decl decltab_previous(DC_table dt) {
	return decltab_current(dt)->old;
}

/* make a NEW declaration the active declaration */
void decltab_makecurrent(DC_table dt, DC_decl dcl) {
	dcl->old = decltab_current(dt);
	dt->current = dcl;
}

/* deactivate the current declaration and return the previous (or NULL) */
DC_decl decltab_deactivate(DC_table dt) {
	dt->current = decltab_previous(dt);
	return decltab_current(dt);
}

/* replace the first decl object with the current, eventually freeing the second also */
void decltab_replace_cur(DC_table dt, DC_decl pd) {
	DC_decl cd = decltab_current(dt);
	/* what actually happens is transfer all the data from cd to pd
	 * this helps keep consistensy in the list chaining */
	pd->specs = cd->specs;
	pd->declarator = cd->declarator;
	pd->body = cd->body;
	pd->initialized = 1;
	pd->id = cd->id;
	pd->scope = cd->scope;
	pd->type = cd->type;
	pd->old = cd->old; // if the old member is changed then the chain will break
}

/* set the id member of a DC_decl structure */
void decltab_setid(DC_table dt, _AST_string id) {
	//	int index = TAB_get_index(decltab_current(dt)->id);
	if (debug_ast) fprintf(stderr, "\n\t SETID %s \n", id);
	decltab_current(dt)->id = strdup(id);
	//	decltab_reposition_current(dt, index, id);
}

/* set the specs member of a DC_decl structure */
void decltab_setspecs(DC_table dt, _AST_ ast) {
	decltab_current(dt)->specs = ast;
}

/* set the decalrator member of a DC_decl structure */
void decltab_setdeclarator(DC_table dt, _AST_ ast) {
	decltab_current(dt)->declarator = ast;
}

/* set the body member of a DC_decl structure */
void decltab_setbody(DC_table dt, _AST_ ast) {
	decltab_current(dt)->body = ast;
}

/* returns 1 if the current declaration has a set type, 0 if not */
_AST_int decltab_hastype(DC_table dt) {
	return decltab_current(dt)->type;
}

/* wrapper to automatically set the current declaration as a typedef declaration */
void decltab_settypedef(DC_table dt) {
	decltab_current(dt)->type = DC_TYPE;
}

/* wrapper to automatically set the current declaration as a variable declaration */
void decltab_setvar(DC_table dt) {
	decltab_current(dt)->type = DC_VAR;
}

/* wrapper to automatically set the current declaration as a function declaration */
void decltab_setfunction(DC_table dt) {
	decltab_current(dt)->type = DC_FUNC;
}

/* wrapper to automatically set the current declaration as an array declaration */
void decltab_setarray(DC_table dt) {
	decltab_current(dt)->type = DC_ARRAY;
}

/* set the current declaration as an struct declaration */
void decltab_setstruct(DC_table dt) {
	decltab_current(dt)->type = DC_STRUCT;
}

/* set the current declaration as an union declaration */
void decltab_setunion(DC_table dt) {
	decltab_current(dt)->type = DC_UNION;
}

/* set the current declaration as an enum declaration */
void decltab_setenum(DC_table dt) {
	decltab_current(dt)->type = DC_ENUM;
}


_AST_int decltab_scope_get(DC_table decltab) {
	return decltab->scope;
}

_AST_int decltab_scope_new(DC_table dt) {
	if (debug_ast) fprintf(stderr, "\nentering scope %d", dt->scope + 1);
	return ++dt->scope;
}

_AST_int decltab_scope_end(DC_table dt) {
	// get the current scope id
	_AST_int sc = decltab_scope_get(dt);
	// release all entries of that scope from the table
	decltab_scope_release(dt, sc);
	// remove the scope from the stack
	if (debug_ast) fprintf(stderr, "\nleaving scope %d\n", dt->scope);
	return --dt->scope;
}

/* release all entries in a given scope */
void decltab_scope_release(DC_table dt, _AST_int scope) {
	/* get a pointer to the symbol table */
	TAB_table st = dt->decltab;
	/* get a pointer to the symbol index table */
	TAB_table si = dt->declind;

	TAB_cell bi, b, li, l;
	/* get a pointer to the head of the list */
	li = si->row[scope];
	/* let's traverse the list, make sure there is a list though */
	while (li != NULL) {
		_AST_int ind = *((_AST_int *) (li->val));
		/* get the first symbol in this row */
		bi = st->row[ind];
		/* traverse this bucket */
		while (bi != NULL) {
			/* if it is in the same scope */
			if (((DC_decl) bi->val)->scope == scope) {
				/* match found, should be removed */
				/* pop the cell */
				b = TAB_pop_cell(st, ind);
				dt->items--;

				/* get the next cell */
				bi = st->row[ind];
				/* free current cell */
				if (((DC_decl) b->val)->id != NULL)
					free(((DC_decl) b->val)->id);
				free(b->val);
				free(b);
			} else break; /* we have gone out of scope */
		}
		/* continue to the next index */
		li = li->next;
		/* current row finished, pop it from the table */
		l = TAB_pop_cell(si, scope);
		free(l->val);
		free(l);
	}
}

/* lookup a word in the symbol table and return the first symbol that matches */
DC_decl decltab_lookup(DC_table dt, _AST_string word, DC_decl_type type) {
	assert(dt);
	/* get a pointer to the symbol table */
	TAB_table tab = dt->decltab;
	/* Calculate the row index */
	_AST_int index = TAB_get_index(word);
	/* Get the row head */
	TAB_cell cell = tab->row[index];
	if (!cell) return NULL;
	DC_decl dcl;
	/* search the bucket */
	while (cell != NULL) {
		dcl = (DC_decl) cell->val;
		assert(dcl->id);
		/* is this what we are looking for? */
		if (!strcmp(dcl->id, word) && dcl->type == type)
			return dcl;
		/* get the next cell */
		cell = cell->next;
	}
	return NULL;
}

/* add an item to the symbol table */
DC_decl decltab_new(DC_table dt, _AST_string word, _AST_ specs, _AST_ declarator, _AST_ body) {
	/* Create the new Item */
	/* get a memory allocation */
	DC_decl dcl = checked_malloc(sizeof (*dcl));
	/* set the symbol properties */
	if (word != NULL) dcl->id = strdup(word);
	else dcl->id = NULL;
	dcl->specs = specs;
	dcl->declarator = declarator;
	dcl->body = body;

	dcl->scope = decltab_scope_get(dt);
	dcl->type = 0;
	dcl->initialized = 0;
	/* push the new declaration into the temp stack */
	push(dcl, dt->temp);
	/* activate the current declaration */
	decltab_makecurrent(dt, dcl);
	return dcl;
}

/* Add a declaration to the table */
DC_decl decltab_finalize(DC_table dt) {
	// remove it from the temporary stack
	DC_decl dcl = pop(dt->temp);
	if (debug_ast) if (dcl->id == NULL) fprintf(stderr, "\ntype %d, scope: %d, line: %d\n", dcl->type, dcl->scope, dcl->specs->pos);
	assert(dcl->id);
	//	printf("\n%d\n",TAB_get_index(dcl->id));
	TAB_new(dt->decltab, dcl->id, dcl);
	dt->items++;
	/* NOTE: It is important not to forget to update accordingly the declind table */
	declind_new(dt, dcl->id);
	/* deactivate and fallback to the previous declaration, if any */
	decltab_deactivate(dt);
	return dcl;
}

void decltab_reposition_current(DC_table dt, int index, char *word) {
	TAB_cell c = TAB_pop_cell(dt->decltab, index);
	TAB_new(dt->decltab, word, c->val);
	free(c);
}

/* add a new entry in the declind table */
_AST_int declind_new(DC_table dt, _AST_string word) {
	/* We need to make sure the the new entry is unique,
	 * So look it up in the declind table
	 * if a match is found a flag is set, else the new entry is added
	 */
	/* get the current scope id */
	_AST_int sp = decltab_scope_get(dt);
	/* get a pointer to the symbol table */
	TAB_table si = dt->declind;
	/* get the list for the current scope */
	_AST_int *val = checked_malloc(sizeof (_AST_int));
	*val = TAB_get_index(word);
	TAB_cell li = si->row[sp];
	_AST_int found = 0;
	while (li != NULL) {
		if ((_AST_int) li->val == *val) {
			found = 1;
			break;
		} else li = li->next;
	}
	if (!found) TAB_append(si, sp, val);
	return (found == 0) ? 1 : 0;
}