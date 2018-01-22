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

/* initialize a symboltable */
void symtab_init(ST_symboltable st) {
	st->scopes = checked_malloc(sizeof (*(st->scopes)));
	scope_init(st->scopes);
	st->symtab = checked_malloc(sizeof (*(st->symtab)));
	TAB_init(st->symtab);
	st->symind = checked_malloc(sizeof (*(st->symind)));
	TAB_init(st->symind);
	st->items = 0;
}

void symtab_clear(ST_symboltable st) {
	TAB_clear(st->symind);
	free(st->symind);
	TAB_clear(st->symtab);
	free(st->symtab);
	scope_clear(st->scopes);
	free(st->scopes);
}

int symtab_get_scope(ST_symboltable symtab) {
	int sp = 0;
	if (!StackIsEmpty(symtab->scopes))
		sp = ((scope)StackTop(symtab->scopes))->id;
	return sp;
}

scope symtab_new_scope(ST_symboltable symtab, int state, int xclusive) {
	return scope_start(symtab->scopes, state, xclusive);
}

int symtab_end_scope(ST_symboltable symtab) {
	// get the current scope id
	int sp = symtab_get_scope(symtab);
	// release all entries of that scope from the table
	symtab_release_scope(symtab, sp);
	// remove the scope from the stack
	int i = scope_end(symtab->scopes);
	return i;
}

/* release all entries in a given scope */
void symtab_release_scope(ST_symboltable symtab, int scope) {
	/* get a pointer to the symbol table */
	TAB_table st = symtab->symtab;
	/* get a pointer to the symbol index table */
	TAB_table si = symtab->symind;

	TAB_cell bi, b, li, l;
	/* get a pointer to the head of the list */
	li = si->row[scope];
	/* let's traverse the list, make sure there is a list though */
	while (li != NULL) {
		int ind = *((int *) (li->val));
		/* get the first symbol in this row */
		bi = st->row[ind];
		/* traverse this bucket */
		while (bi != NULL) {
			/* if it is in the same scope */
			if (((ST_symbol) bi->val)->scope == scope) {
				/* match found, should be removed */
				/* pop the cell */
				b = TAB_pop_cell(st, ind);
				symtab->items--;

				/* get the next cell */
				bi = st->row[ind];
				/* free current cell */
				if(((ST_symbol) b->val)->word != NULL)
					free(((ST_symbol) b->val)->word);
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
ST_symbol symtab_lookup(ST_symboltable symtab, char *word) {
	assert(symtab);
	/* get a pointer to the symbol table */
	TAB_table st = symtab->symtab;
	/* Calculate the row index */
	int index = TAB_get_index(word);
	/* Get the row head */
	TAB_cell bi = st->row[index];
	if (!bi) return NULL;
	ST_symbol sy;
	/* search the bucket */
	while (bi != NULL) {
		sy = bi->val;
		/* is this what we are looking for? */
		if (sy->word && !strcmp(sy->word, word)) return sy;
		/* get the next cell */
		bi = bi->next;
	}
	return NULL;
}

/* add an item to the symbol table */
ST_symbol symtab_new(ST_symboltable symtab, char *word, symboltype stp) {
	/* get a pointer to the current scope */
	/* first check that there is a scope */
	int sp = symtab_get_scope(symtab);

	/* get a pointer to the symbol table */
	TAB_table st = symtab->symtab;
	/* make sure the type is not already defined */
	/* first search for a match */
	/* Create the new Item */
	/* get a memory allocation */
	ST_symbol s = checked_malloc(sizeof (*s));
	/* set the symbol properties */
	s->word = strdup(word);
	s->scope = sp;
	s->type = stp;
	/* Add it to the table */
	TAB_new(st, word, s);
	symtab->items += 1;
	/* NOTE: It is important not to forget to update accordingly the symind table */
	symind_new(symtab, word);
	return s;
}

/* add a new entry in the symind table */
int symind_new(ST_symboltable symtab, char *word) {
	/* We need to make sure the the new entry is unique,
	 * So look it up in the symind table
	 * if a match is found a flag is set, else the new entry is added
	 */
	/* get the current scope id */
	int sp = symtab_get_scope(symtab);
	/* get a pointer to the symbol table */
	TAB_table si = symtab->symind;
	/* get the list for the current scope */
	int *val = checked_malloc(sizeof (int));
	*val = TAB_get_index(word);
	TAB_cell li = si->row[sp];
	int found = 0;
	while (li != NULL) {
		if ((int) li->val == *val) {
			found = 1;
			break;
		} else li = li->next;
	}
	if (!found) {
		TAB_append(si, sp, val);
	}
	return (found == 0) ? 1 : 0;
}