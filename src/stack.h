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

#ifndef __STACK_H__
#define __STACK_H__

#define MAX_STACK 150


// the stack structure
typedef struct stack {
	void *list[MAX_STACK];
	int top;
} *stack;

void StackInitialize (stack);		// Initialize stack boundaries

int StackIsFull (const stack);		// Check if the stack is full

int StackIsEmpty (const stack);		// Check if the stack is empty

int StackItemCount (const stack);	// Return the count of items on the stack

int push (void *, stack);			// Push a new item onto the stack

void *pop (stack);					// Remove the top of the stack and save it to the first parameter

void *StackTop(stack sk);			// return he top element of the stack without poping

void StackEmpty (stack);			// Clear the stack

#endif
