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

#ifndef __AST_H__
#define __AST_H__

#include "ast_f.h"


#include "config.h"

/*
 * Structure multiplicity enumerations
 */

enum _AST_seq_type {
	_AST_seq_AN = SEQ_AN, /* left AST, right NULL */
	_AST_seq_AA = SEQ_AA, /* left AST, right AST */
	_AST_seq_SA = SEQ_SA /* left SEQ, right AST */

};

enum _AST_type {
	AST_CONSTVAL_INT = CONSTVAL_INT,
	AST_CONSTVAL_FLO = CONSTVAL_FLO,
	AST_CONSTVAL_HEX = CONSTVAL_STR,

	AST_EXP_IDENTIFIER = EXP_IDENTIFIER,
	AST_EXP_CONSTVAL_INT = EXP_CONSTVAL_INT,
	AST_EXP_CONSTVAL_FLO = EXP_CONSTVAL_FLO,
	AST_EXP_CONSTVAL_HEX = EXP_CONSTVAL_HEX,
	AST_EXP_STRING = EXP_STRING,
	AST_EXP_BRACKETED = EXP_BRACKETED,

	AST_EXP_SUBSCRIPT = EXP_SUBSCRIPT,
	AST_EXP_ICALL = EXP_ICALL,
	AST_EXP_STRUCTMBR = EXP_STRUCTMBR,
	AST_EXP_PTRMBR = EXP_PTRMBR,
	AST_EXP_INC = EXP_INC,
	AST_EXP_CAST = EXP_CAST,

	AST_EXP_ARGS = EXP_ARGS,

	AST_EXP_UNARY = EXP_UNARY,

	AST_EXP_BINARY = EXP_BINARY,

	AST_EXP_COND = EXP_COND,

	AST_EXP_LIST = EXP_LIST,

	AST_DECLARATION = DECLARATION,
	AST_DECLARATION_LIST = DECLARATION_LIST,
	AST_DECLARATION_PARAMETER = DECLARATION_PARAMETER,

	AST_DECLARATION_STRUCT = DECLARATION_STRUCT,
	AST_DECLARATION_STRUCT_LIST = DECLARATION_STRUCT_LIST,

	AST_SPECQUAL_LIST = SPECQUAL_LIST,

	AST_STRUCT_SPECIFIER = STRUCT_SPECIFIER,
	AST_ENUM_SPECIFIER = ENUM_SPECIFIER,

	AST_DECLARATOR_STRUCT = DECLARATOR_STRUCT,
	AST_DECLARATOR_STRUCT_LIST = DECLARATOR_STRUCT_LIST,

	AST_ENUMERATOR = ENUMERATOR,
	AST_ENUMERATOR_LIST = ENUMERATOR_LIST,


	AST_DECLARATION_SPECIFIERS = DECLARATION_SPECIFIER_LIST,
	AST_STORECLASS_SPECIFIER = STORECLASS_SPECIFIER,
	AST_TYPE_SPECIFIER_INT = TYPE_SPECIFIER_INT,
	AST_TYPE_SPECIFIER_STRING = TYPE_SPECIFIER_STRING,
	AST_TYPE_SPECIFIER_STRUCT = TYPE_SPECIFIER_STRUCT,
	AST_TYPE_SPECIFIER_ENUM = TYPE_SPECIFIER_ENUM,
	AST_FUNCTION_SPECIFIER = FUNCTION_SPECIFIER,

	AST_TYPE_QUALIFIER_LIST = TYPE_QUALIFIER_LIST,
	AST_TYPE_QUALIFIER = TYPE_QUALIFIER,

	AST_DECLARATOR_INIT_LIST = DECLARATOR_INIT_LIST,
	AST_DECLARATOR_INIT = DECLARATOR_INIT,
	AST_DECLARATOR_ATTRIBUTED = DECLARATOR_ATTRIBUTED,
	AST_DECLARATOR = DECLARATOR,
	AST_DECLARATOR_DIRECT_1 = DECLARATOR_DIRECT_1, // IDENTIFIER
	AST_DECLARATOR_DIRECT_2 = DECLARATOR_DIRECT_2, // Bracketed
	AST_DECLARATOR_DIRECT_3 = DECLARATOR_DIRECT_3, // [ ... ]
	AST_DECLARATOR_DIRECT_4 = DECLARATOR_DIRECT_4, // [ STATIC ... ]
	AST_DECLARATOR_DIRECT_5 = DECLARATOR_DIRECT_5, // [ ... STATIC ... ]
	AST_DECLARATOR_DIRECT_6 = DECLARATOR_DIRECT_6, // [ ... * ]
	AST_DECLARATOR_DIRECT_7 = DECLARATOR_DIRECT_7, // [ ]
	AST_DECLARATOR_DIRECT_8 = DECLARATOR_DIRECT_8, // ( ... )
	AST_DECLARATOR_ABSTRACT = DECLARATOR_ABSTRACT,
	AST_DECLARATOR_ABSTRACT_DIRECT_1 = DECLARATOR_ABSTRACT_DIRECT_1, // ( declarator )
	AST_DECLARATOR_ABSTRACT_DIRECT_2 = DECLARATOR_ABSTRACT_DIRECT_2, // ... [ ... ]
	AST_DECLARATOR_ABSTRACT_DIRECT_3 = DECLARATOR_ABSTRACT_DIRECT_3, // ... [ * ]
	AST_DECLARATOR_ABSTRACT_DIRECT_4 = DECLARATOR_ABSTRACT_DIRECT_4, // ... ( ... )

	AST_POINTER = POINTER,

	AST_PARAMETER_TYPE_LIST = PARAMETER_TYPE_LIST,
	AST_PARAMETER_LIST = PARAMETER_LIST,
	AST_IDENTIFIER_LIST = IDENTIFIER_LIST,

	AST_TYPE_NAME = TYPE_NAME,

	AST_INITIALIZER = INITIALIZER,
	AST_INITIALIZER_LIST = INITIALIZER_LIST,

	AST_DESIGNATION = DESIGNATION,
	AST_DESIGNATOR = DESIGNATOR,
	AST_DESIGNATOR_LIST = DESIGNATOR_LIST,

	AST_STMT_LABEL = STMT_LABEL,
	AST_STMT_COMPOUND = STMT_COMPOUND,
	AST_STMT_EXP = STMT_EXP,
	AST_STMT_IF = STMT_IF,
	AST_STMT_SWITCH = STMT_SWITCH,
	AST_STMT_WHILE = STMT_WHILE,
	AST_STMT_DO = STMT_DO,
	AST_STMT_FOR = STMT_FOR,
	AST_STMT_JUMP = STMT_JUMP,
	AST_STMT_ASM = STMT_ASM,

	AST_BLOCK_ITEM_LIST = BLOCK_ITEM_LIST,

	AST_TRANSLATION_UNIT = TRANSLATION_UNIT,
	AST_DECLARATION_EXTERNAL = DECLARATION_EXTERNAL,
	AST_FUNCTION_DEFITION = FUNCTION_DEFITION,

	AST_INC_DIRECTIVE = INC_DIRECTIVE,

	AST_ATTRIBUTE_SPEC = ATTRIBUTE_SPEC,
	AST_ATTRIBUTE_SPEC_LIST = ATTRIBUTE_SPEC_LIST,

};

/*
 * Structure Declarations
 */

struct _AST_root {
	_AST_ left;
	_AST_root right;
};

struct _AST_ {
	_AST_pos pos;
	_AST_type type;
	void *ast;
};

struct _AST_seq {
	_AST_seq_type type; // exp, block, dspec
	_AST_ left;
	_AST_ right;
};

union _AST_terminal {
	_AST_int int_;
	_AST_float float_;
	_AST_string string;
};

struct _AST_exp {
	_AST_int op;
	_AST_terminal constant;
	_AST_ left;
	_AST_ right;
	_AST_seq seq;
};

struct _AST_loop {
	_AST_ init;
	_AST_ test;
	_AST_ step;
	_AST_ body;
};

struct _AST_conditional {
	_AST_ condition;
	_AST_ then;
	_AST_ else_;
};

struct _AST_flow {
	_AST_string id;
	_AST_ exp;
	_AST_ stmt;
	_AST_int type;
	_AST_ attrlist;
};

union _AST_specifier {
	_AST_int spec;
	_AST_string type;
	_AST_struct sspc;
};

struct _AST_declaration {
	_AST_flag extension;
	_AST_ decl_specs;
	_AST_ declarator;
	_AST_ struct_list;
	_AST_ struct_spec;
};

struct _AST_declarator {
	_AST_ pointer;
	_AST_string id;
	_AST_ list;
	_AST_ exp;
	_AST_ declarator;
	_AST_ init;
	_AST_string asm_;
	_AST_ attrlist_l;
	_AST_ attrlist_r;
};

struct _AST_type_name {
	_AST_ list;
	_AST_ declarator;
};

struct _AST_pointer {
	_AST_ list;
	_AST_ pointer;
};

struct _AST_struct {
	_AST_int type;
	_AST_string id;
	_AST_ list;
	_AST_ attrlist;
};

struct _AST_function {
	_AST_ decl_specs;
	_AST_ declarator;
	_AST_ decl_list;
	_AST_ stmt;
};

struct _AST_attribute {
	_AST_string attribute;
	_AST_flag comma;
};


/**
 * Function Prototypes
 */

_AST_terminal AST_terminal_int(_AST_int i);
_AST_terminal AST_terminal_float(_AST_float i);
_AST_terminal AST_terminal_string(_AST_string i);

_AST_ AST_primary_id(_AST_pos pos, _AST_string id);
_AST_ AST_primary_int(_AST_pos pos, _AST_int int_);
_AST_ AST_primary_float(_AST_pos pos, _AST_float float_);
_AST_ AST_primary_string(_AST_pos pos, _AST_string string);
_AST_ AST_primary_exp(_AST_pos pos, _AST_ left);

_AST_ AST_subscript(_AST_pos pos, _AST_ left, _AST_ right);
_AST_ AST_icall(_AST_pos pos, _AST_ left, _AST_ right);
_AST_ AST_structmbr(_AST_pos pos, _AST_ left, _AST_string id);
_AST_ AST_ptrmbr(_AST_pos pos, _AST_ left, _AST_string id);
_AST_ AST_inc(_AST_pos pos, _AST_ left, _AST_int op);
_AST_ AST_cast(_AST_pos pos, _AST_ left, _AST_ right);

_AST_ AST_unary(_AST_pos pos, _AST_ left, _AST_int op);
_AST_ AST_binary(_AST_pos pos, _AST_ left, _AST_int op, _AST_ right);

_AST_ AST_condexp(_AST_pos pos, _AST_ left, _AST_ true_, _AST_ false_);

_AST_ AST_arglist(_AST_pos pos, _AST_ list, _AST_ exp);
_AST_ AST_explist(_AST_pos pos, _AST_ left, _AST_ right);

_AST_ AST_declaration(_AST_pos pos, _AST_type type, _AST_ dspecs, _AST_ declarator);
_AST_ AST_declaration_extended(_AST_ ast);
_AST_ AST_declaration_list(_AST_pos pos, _AST_ list, _AST_ declaration);

_AST_ AST_declaration_struct(_AST_pos pos, _AST_ specs, _AST_ list, _AST_ struct_spec);
_AST_ AST_declaration_struct_list(_AST_pos pos, _AST_ list, _AST_ declaration);

_AST_ AST_enumerator(_AST_pos pos, _AST_string id, _AST_ exp);
_AST_ AST_enumerator_list(_AST_pos pos, _AST_ list, _AST_ enumerator);

_AST_ AST_declaration_specs(_AST_pos pos, _AST_ spec, _AST_ list);
_AST_ AST_spec_store(_AST_pos pos, _AST_int spec);
_AST_ AST_spec_type_int(_AST_pos pos, _AST_int spec);
_AST_ AST_spec_type_string(_AST_pos pos, _AST_string spec);
_AST_ AST_spec_function(_AST_pos pos);
_AST_ AST_spec_struct(_AST_pos pos, _AST_int type, _AST_string id, _AST_ list, _AST_ attrlist);
_AST_ AST_spec_enum(_AST_pos pos, _AST_string id, _AST_ list);
_AST_ AST_qual_type(_AST_pos pos, _AST_int qual);

_AST_ AST_declarator_init(_AST_pos pos, _AST_ declarator, _AST_ init);
_AST_ AST_declarator_init_list(_AST_pos pos, _AST_ list, _AST_ declarator);
_AST_ AST_declarator(_AST_pos pos, _AST_ declarator, _AST_ pointer, _AST_string asm_);
_AST_ AST_declarator_struct(_AST_pos pos, _AST_ declarator, _AST_ exp);
_AST_ AST_declarator_struct_list(_AST_pos pos, _AST_ list, _AST_ declarator);
_AST_ AST_declarator_direct(_AST_pos pos, _AST_type type, _AST_string id, _AST_ declarator, _AST_ list, _AST_ exp);
_AST_ AST_declarator_abstract(_AST_pos pos, _AST_ declarator, _AST_ pointer);
_AST_ AST_declarator_direct_abstract(_AST_pos pos, _AST_type type, _AST_ declarator, _AST_ exp, _AST_ list);
_AST_ AST_declarator_attributed(_AST_ declarator, _AST_ left, _AST_ right);

_AST_ AST_pointer(_AST_pos pos, _AST_ list, _AST_ pointer);

_AST_ AST_sqlist(_AST_pos pos, _AST_ list, _AST_ sq);
_AST_ AST_tqlist(_AST_pos pos, _AST_ list, _AST_ tq);
_AST_ AST_ptlist(_AST_pos pos, _AST_ list, _AST_flag ellipsis);
_AST_ AST_parlist(_AST_pos pos, _AST_ list, _AST_ declaration);
_AST_ AST_idlist(_AST_pos pos, _AST_ list, _AST_string id);

_AST_ AST_type_name(_AST_pos pos, _AST_ list, _AST_ declarator);

_AST_ AST_initializer(_AST_pos pos, _AST_ list, _AST_ exp);
_AST_ AST_initializer_list(_AST_pos pos, _AST_ list, _AST_ designation, _AST_ initializer);

_AST_ AST_designation(_AST_pos pos, _AST_ list);
_AST_ AST_designator(_AST_pos pos, _AST_ exp, _AST_string id);
_AST_ AST_designator_list(_AST_pos pos, _AST_ list, _AST_ designator);

_AST_ AST_stmt_labeled(_AST_pos pos, _AST_string id, _AST_ exp, _AST_ stmt, _AST_ attrlist);
_AST_ AST_stmt_compound(_AST_pos pos, _AST_ list);
_AST_ AST_stmt_exp(_AST_pos pos, _AST_ exp);
_AST_ AST_stmt_if(_AST_pos pos, _AST_ if_, _AST_ then, _AST_ else_);
_AST_ AST_stmt_switch(_AST_pos pos, _AST_ exp, _AST_ stmt);
_AST_ AST_stmt_while(_AST_pos pos, _AST_ exp, _AST_ stmt);
_AST_ AST_stmt_do(_AST_pos pos, _AST_ stmt, _AST_ exp);
_AST_ AST_stmt_for(_AST_pos pos, _AST_ init, _AST_ test, _AST_ step, _AST_ body);
_AST_ AST_stmt_jump(_AST_pos pos, _AST_int type, _AST_string id, _AST_ exp);

_AST_ AST_block_list(_AST_pos pos, _AST_ list, _AST_ block);

_AST_ AST_function(_AST_pos pos, _AST_ specs, _AST_ declarator, _AST_ decl_list, _AST_ stmt);
_AST_root AST_unit(_AST_ declaration);

_AST_ AST_include(_AST_pos pos, _AST_string inc);
_AST_ AST_attribute(_AST_pos pos, _AST_string attribute, _AST_flag comma);
_AST_ AST_attribute_list(_AST_pos pos, _AST_ attribute, _AST_ list);
_AST_ AST_asm(_AST_pos pos, _AST_string asm_);


#endif
