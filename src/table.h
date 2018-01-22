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
 * File:   table.h
 * Author: Georgios Varisteas
 *
 * Created on March 28, 2010, 7:31 AM
 */

#ifndef __TABLE_H__
#define	__TABLE_H__

#define MAX_TABLE_SIZE 9997

/* An actual cell. */
struct TAB_cell;
typedef struct TAB_cell *TAB_cell;
struct TAB_cell {
	void *val;			/* the cell's content data */
	TAB_cell next;		/* pointer to the next item in the list */
	int old_top;		/* pointer to the cell which was the table's top before this one was added */
						/* the old_top pointer is used to backtrack the top pointer in the case of cell removal */
};

/* The actual table. Each row entry represents a row of
 * same-key-mapped cells. Top is a pointer to the last cell entered */
typedef struct TAB_table{
	TAB_cell row[MAX_TABLE_SIZE];	/* array of rows. each row is a linked list of TAB_cell structs */
	int top;						/* pointer to the last cell added irrespective of row */
} *TAB_table;

/* Allocate and return a new table */
void TAB_init(TAB_table);

/* Clear all entries to a table */
void TAB_clear(TAB_table tbl);

/* Retrieve the value of the latest cell mapped to key in the table */
void *TAB_lookup(TAB_table tbl, char *key);

/* As TAB_lookup but with a custom comparison function upon the returned value */
void *TAB_lookup_custom(TAB_table tbl, char *key, int (*cmp)(void *l,void *r), void *subkey);

/* Pop and return the latest cell in the row at index */
TAB_cell TAB_pop_cell(TAB_table tbl, int index);

/* Pop and return the latest cell in the table */
TAB_cell TAB_pop(TAB_table tbl);

/* add a cell to the table, mapped to key */
void TAB_new(TAB_table tbl, char *key, void *val);

/* append a cell to a predefined row */
void TAB_append(TAB_table tbl, int index, void *val);

/* Helper function to recusrively free a linked list of cells */
void TAB_freerow(TAB_cell c);

/* wrapper function to get the hashed key of a string */
int TAB_get_index(char *key);

#endif	/* __TABLE_H__ */

