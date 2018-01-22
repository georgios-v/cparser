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

/* Allocate and return a new table */
void TAB_init(TAB_table tbl){
	tbl->top = 0;
	int i;
	for (i = 0; i < MAX_TABLE_SIZE; i++)
		tbl->row[i] = NULL;
}

/* Clear all entries to a table */
void TAB_clear(TAB_table tbl){
	int i;
	for (i = 0; i < MAX_TABLE_SIZE; i++) {
		TAB_freerow(tbl->row[i]);
		tbl->row[i] = NULL;
	}
	tbl->top = 0;
}

/* Retrieve the value of the latest cell mapped to key in the table */
void *TAB_lookup(TAB_table tbl, char *key){
	assert(tbl && key);
	// get the key hash
	int index = TAB_get_index(key);
	// return the latest cell's value for that hash
	return tbl->row[index]->val;
}

/* As TAB_lookup but with a custom comparison function upon the returned value */
void *TAB_lookup_custom(TAB_table tbl, char *key, int (*cmp)(void *l,void *r), void *subkey){
	assert(tbl && key);
	TAB_cell c;
	// get the key hash
	int index = TAB_get_index(key);
	// Iterate the list to find a match to the custom function
	for(c = tbl->row[index]; c; c = c->next)
		if((*cmp)(subkey,c->val) > 0) return c->val;
	return NULL;
}

/* Pop and return the latest cell in the row at index */
TAB_cell TAB_pop_cell(TAB_table tbl, int index){
	assert(tbl);
	// get the cell
	TAB_cell c = tbl->row[index];
	// remove it from the row
	tbl->row[index] = c->next;
	// check the table's top reference
	if(tbl->top == index) tbl->top = c->old_top;
	// return the cell
	return c;
}

/* Pop and return the latest cell in the table */
TAB_cell TAB_pop(TAB_table tbl){
	assert(tbl);
	// get the cell
	TAB_cell c = tbl->row[tbl->top];
	// remove it from the table
	tbl->row[tbl->top] = c->next;
	// exchange 'top' reference
	tbl->top = c->old_top;
	// return the cell
	return c;
}

/* add a cell to the table, mapped to key */
void TAB_new(TAB_table tbl, char *key, void *val){
	assert(tbl && key);
	// get the key hash
	int index = TAB_get_index(key);
	// allocate the new cell
	TAB_cell c = checked_malloc(sizeof(*c));
	// assigne value
	c->val = val;
	// keep a reference to the old top
	c->old_top = tbl->top;
	// assign new top
	tbl->top = index;
	// push into the list
	c->next = tbl->row[index];
	tbl->row[index] = c;
}

/* append a cell to a predefined row */
void TAB_append(TAB_table tbl, int index, void *val){
	assert(tbl);
	// allocate the new cell
	TAB_cell c = checked_malloc(sizeof(*c));
	// assigne value
	c->val = val;
	// keep a reference to the old top
	c->old_top = tbl->top;
	// assign new top
	tbl->top = index;
	// push into the list
	c->next = tbl->row[index];
	tbl->row[index] = c;
}

/* Helper function to recursively free a linked list of cells */
void TAB_freerow(TAB_cell c){
	if(c == NULL) return;
	TAB_cell n = c->next;
	free(c->val);
	free(c);
	if(n != NULL) TAB_freerow(n);
}

/* wrapper function to get the hashed key of a string */
int TAB_get_index(char *key){
	unsigned int h = hash(key);
//	printf("\n%d\n",h % MAX_TABLE_SIZE);
	return h % MAX_TABLE_SIZE;
}