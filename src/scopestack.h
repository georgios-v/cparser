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
 * File:   scopestack.h
 * Author: Georgios Varisteas
 *
 * Created on April 15, 2010, 6:04 PM
 */

#ifndef __SCOPESTACK_H__
#define	__SCOPESTACK_H__

typedef struct scope {
	int braces;		// ++ forall '{', -- forall '}'. if a '}' decrements this to 0, the scope is finished.
	int state;		// the lexer state of the PREVIOUS scope, kept so that it can be restored upon termination.
	int xclusive;	// flag denoting if the scope is exclusive or not. A non exclusive scope allows backtracking lookups.
	int id;			// keeps the scope id number
} *scope;

void scope_init(stack);					/* initialize a scope stack */
void scope_clear(stack sk);				/* clear a scope stack */
scope scope_start(stack,int,int);		/* create and add a new scope */
void scope_new(stack, scope);			/* push a new scope onto the stack */
int scope_end(stack);					/* remove a scope for the stack */
scope scope_front(stack);				/* get the top scope */
scope scope_rear(stack);				/* get the last scope */

int scope_has_front(stack);				/* return true if the stack is not empty, false otherwise */
int scope_braces_inc(stack);			/* increment current scope's braces value */
int scope_braces_dec(stack);			/* decrement current scope's braces value */

#endif	/* __SCOPESTACK_H__ */

