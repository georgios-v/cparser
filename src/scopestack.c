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

/* initialize a scope stack */
void scope_init(stack sk) {
	StackInitialize(sk);
}

/* create and add a new scope */
scope scope_start(stack sk, int state, int xclusive) {
	scope scp = checked_malloc(sizeof (*scp));
	scp->braces = 0;
	if (StackIsEmpty(sk)) scp->id = 1;
	else scp->id = ((scope)StackTop(sk))->id + 1;
	scp->state = state;
	scp->xclusive = xclusive;
	scope_new(sk, scp);
	return scp;
}

void scope_clear(stack sk) {
	while (!StackIsEmpty(sk)) {
		scope_end(sk);
	}
}

/* push a new scope onto the stack */
void scope_new(stack sk, scope sp) {
	/* push new scope onto the stack */
	push(sp, sk);
}

/* remove a scope for the stack */
int scope_end(stack sk) {
	scope scp = pop(sk);
	/* keep the state */
	int state = scp->state;
	free(scp);
	return state;
}

/* get the top scope */
scope scope_front(stack sk) {
	if (StackIsEmpty(sk)) return NULL;
	return (scope) StackTop(sk);
}

/* increment current scope's braces value */
int scope_braces_inc(stack sk) {
	if (!StackIsEmpty(sk)) {
		return ++((scope) StackTop(sk))->braces;
	}
	return 0;
}

/* decrement current scope's braces value */
int scope_braces_dec(stack sk) {
	if (!StackIsEmpty(sk)) {
		return --((scope) StackTop(sk))->braces;
	}
	return 0;
}
