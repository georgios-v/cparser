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
 *File:   ast_f.h
 *Author: Georgios Varisteas
 *
 *Created on March 22, 2010, 5:13 PM
 */

#ifndef __AST_F_H__
#define	__AST_F_H__

/*
 *Forward Declarations
 */
typedef enum _AST_seq_type _AST_seq_type;
typedef enum _AST_type _AST_type;

typedef int _AST_pos;		/*Holds the position for each structure in the source */

typedef char *_AST_string;		/*simple alias for IDENTIFIER objects */

typedef int _AST_int;

typedef double _AST_float;

typedef char _AST_flag;

struct _AST_root;
typedef struct _AST_root *_AST_root;

struct _AST_;
typedef struct _AST_ *_AST_;

struct _AST_seq;
typedef struct _AST_seq *_AST_seq;

union _AST_terminal;
typedef union _AST_terminal *_AST_terminal;

struct _AST_exp;
typedef struct _AST_exp *_AST_exp;

struct _AST_loop;
typedef struct _AST_loop *_AST_loop;

struct _AST_conditional;
typedef struct _AST_conditional *_AST_conditional;

struct _AST_flow;
typedef struct _AST_flow *_AST_flow;

union _AST_specifier;
typedef union _AST_specifier *_AST_specifier;

struct _AST_declaration;
typedef struct _AST_declaration *_AST_declaration;

struct _AST_declarator;
typedef struct _AST_declarator *_AST_declarator;

struct _AST_type_name;
typedef struct _AST_type_name *_AST_type_name;

struct _AST_pointer;
typedef struct _AST_pointer *_AST_pointer;

struct _AST_struct;
typedef struct _AST_struct *_AST_struct;

struct _AST_function;
typedef struct _AST_function *_AST_function;

struct _AST_attribute;
typedef struct _AST_attribute *_AST_attribute;

#endif	/*__AST_F_H__ */

