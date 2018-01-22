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
 * File:   astprint.h
 * Author: Georgios Varisteas
 *
 * Created on March 23, 2010, 9:00 PM
 */

#ifndef __ASTPRINT_H__
#define	__ASTPRINT_H__

#define MAXPARMS 99

typedef struct hdrparms{
	int tk[MAXPARMS];	// TASK
	int tktop;			// last entry for task
	int vt[MAXPARMS];	// VOID_TASK
	int vttop;			// last entry for void_task
	int lp[MAXPARMS];	// LOOP_BODY
	int lptop;			// last entry for loop_body
} *hdrparms;

void astprint(FILE *out, long sz, DC_table dt, _AST_root ast, _AST_flag safe);

void pr_list(char out[], _AST_ ast, _AST_flag safe);
void pr_declarator(char out[], _AST_ ast, _AST_flag safe);
void pr_primary_exp(char out[], _AST_ ast, _AST_flag safe);

#endif	/* __ASTPRINT_H__ */

