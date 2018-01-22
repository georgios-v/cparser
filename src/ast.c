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

_AST_terminal AST_terminal_int(_AST_int i) {
	_AST_terminal term = checked_malloc(sizeof (*term));
	term->int_ = i;
	return term;
}

_AST_terminal AST_terminal_float(_AST_float i) {
	_AST_terminal term = checked_malloc(sizeof (*term));
	term->float_ = i;
	return term;
}

_AST_terminal AST_terminal_string(_AST_string i) {
	_AST_terminal term = checked_malloc(sizeof (*term));
	term->string = i;
	return term;
}

_AST_ AST_primary_id(_AST_pos pos, _AST_string id) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_IDENTIFIER;
	cont->ast = AST_terminal_string(id);
	return cont;
}

_AST_ AST_primary_int(_AST_pos pos, _AST_int int_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_CONSTVAL_INT;
	cont->ast = AST_terminal_int(int_);
	return cont;
}

_AST_ AST_primary_float(_AST_pos pos, _AST_float float_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_CONSTVAL_FLO;
	cont->ast = AST_terminal_float(float_);
	return cont;
}

_AST_ AST_primary_hex(_AST_pos pos, _AST_string hex) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_CONSTVAL_HEX;
	cont->ast = AST_terminal_string(hex);
	return cont;
}

_AST_ AST_primary_string(_AST_pos pos, _AST_string string) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_STRING;
	cont->ast = AST_terminal_string(string);
	return cont;
}

_AST_ AST_primary_exp(_AST_pos pos, _AST_ left) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = EXP_BRACKETED;
	cont->ast = left;
	return cont;
}

_AST_ AST_subscript(_AST_pos pos, _AST_ left, _AST_ right) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->right = right;
	cont->pos = pos;
	cont->type = EXP_SUBSCRIPT;
	cont->ast = ast;
	return cont;
}

_AST_ AST_icall(_AST_pos pos, _AST_ left, _AST_ right) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->right = right;
	cont->pos = pos;
	cont->type = EXP_ICALL;
	cont->ast = ast;
	return cont;
}

_AST_ AST_structmbr(_AST_pos pos, _AST_ left, _AST_string id) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->constant = AST_terminal_string(id);
	cont->pos = pos;
	cont->type = EXP_STRUCTMBR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_ptrmbr(_AST_pos pos, _AST_ left, _AST_string id) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->constant = AST_terminal_string(id);
	;
	cont->pos = pos;
	cont->type = EXP_PTRMBR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_inc(_AST_pos pos, _AST_ left, _AST_int op) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->op = op;
	cont->pos = pos;
	cont->type = EXP_INC;
	cont->ast = ast;
	return cont;
}

_AST_ AST_cast(_AST_pos pos, _AST_ left, _AST_ right) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->right = right;
	cont->pos = pos;
	cont->type = EXP_CAST;
	cont->ast = ast;
	return cont;
}

_AST_ AST_unary(_AST_pos pos, _AST_ left, _AST_int op) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->op = op;
	cont->pos = pos;
	cont->type = EXP_UNARY;
	cont->ast = ast;
	return cont;
}

_AST_ AST_binary(_AST_pos pos, _AST_ left, _AST_int op, _AST_ right) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->right = right;
	ast->op = op;
	cont->pos = pos;
	cont->type = EXP_BINARY;
	cont->ast = ast;
	return cont;
}

_AST_ AST_condexp(_AST_pos pos, _AST_ left, _AST_ true_, _AST_ false_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_conditional ast = checked_malloc(sizeof (*ast));
	ast->condition = left;
	ast->then = true_;
	ast->else_ = false_;
	cont->pos = pos;
	cont->type = EXP_COND;
	cont->ast = ast;
	return cont;
}

_AST_ AST_arglist(_AST_pos pos, _AST_ list, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = exp;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = exp;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = exp;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = EXP_ARGS;
	cont->ast = seq;
	return cont;
}

_AST_ AST_explist(_AST_pos pos, _AST_ left, _AST_ right) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_exp ast = checked_malloc(sizeof (*ast));
	ast->left = left;
	ast->right = right;
	cont->pos = pos;
	cont->type = EXP_LIST;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declaration(_AST_pos pos, _AST_type type, _AST_ dspecs, _AST_ declarator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declaration ast = checked_malloc(sizeof (*ast));
	ast->extension = 0;
	ast->decl_specs = dspecs;
	ast->declarator = declarator;
	cont->pos = pos;
	cont->type = type;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declaration_extended(_AST_ ast) {
	_AST_declaration decl = (_AST_declaration) ast->ast;
	decl->extension = 1;
	return ast;
}

_AST_ AST_declaration_list(_AST_pos pos, _AST_ list, _AST_ declaration) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = declaration;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = declaration;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = declaration;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DECLARATION_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_declaration_struct(_AST_pos pos, _AST_ specs, _AST_ list, _AST_ struct_spec) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declaration ast = checked_malloc(sizeof (*ast));
	ast->extension = 0;
	ast->decl_specs = specs;
	ast->struct_list = list;
	ast->struct_spec = struct_spec;
	cont->pos = pos;
	cont->type = DECLARATION_STRUCT;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declaration_struct_list(_AST_pos pos, _AST_ list, _AST_ declaration) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = declaration;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = declaration;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = declaration;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DECLARATION_STRUCT_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_enumerator(_AST_pos pos, _AST_string id, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->id = id;
	ast->exp = exp;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->pos = pos;
	cont->type = ENUMERATOR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_enumerator_list(_AST_pos pos, _AST_ list, _AST_ enumerator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = enumerator;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = enumerator;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = enumerator;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = ENUMERATOR_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_declaration_specs(_AST_pos pos, _AST_ spec, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = spec;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = spec;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = spec;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DECLARATION_SPECIFIER_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_spec_store(_AST_pos pos, _AST_int spec) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = STORECLASS_SPECIFIER;
	cont->ast = AST_terminal_int(spec);
	return cont;
}

_AST_ AST_spec_type_int(_AST_pos pos, _AST_int spec) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = TYPE_SPECIFIER_INT;
	cont->ast = AST_terminal_int(spec);
	return cont;
}

_AST_ AST_spec_type_string(_AST_pos pos, _AST_string spec) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = TYPE_SPECIFIER_STRING;
	cont->ast = AST_terminal_string(spec);
	return cont;
}

_AST_ AST_spec_function(_AST_pos pos) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = FUNCTION_SPECIFIER;
	cont->ast = AST_terminal_int(INLINE);
	return cont;
}

_AST_ AST_spec_struct(_AST_pos pos, _AST_int type, _AST_string id, _AST_ list, _AST_ attrlist) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_struct ast = checked_malloc(sizeof (*ast));
	ast->type = type;
	ast->id = id;
	ast->list = list;
	ast->attrlist = attrlist;
	cont->pos = pos;
	cont->type = STRUCT_SPECIFIER;
	cont->ast = ast;
	return cont;
}

_AST_ AST_spec_enum(_AST_pos pos, _AST_string id, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_struct ast = checked_malloc(sizeof (*ast));
	ast->id = id;
	ast->list = list;
	ast->attrlist = NULL;
	cont->pos = pos;
	cont->type = ENUM_SPECIFIER;
	cont->ast = ast;
	return cont;
}

_AST_ AST_qual_type(_AST_pos pos, _AST_int qual) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = TYPE_QUALIFIER;
	cont->ast = AST_terminal_int(qual);
	return cont;
}

_AST_ AST_declarator_init(_AST_pos pos, _AST_ declarator, _AST_ init) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->init = init;
	cont->pos = pos;
	cont->type = DECLARATOR_INIT;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_init_list(_AST_pos pos, _AST_ list, _AST_ declarator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = declarator;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = declarator;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = declarator;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DECLARATOR_INIT_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_declarator(_AST_pos pos, _AST_ declarator, _AST_ pointer, _AST_string asm_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->pointer = pointer;
	ast->asm_ = asm_;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->pos = pos;
	cont->type = DECLARATOR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_struct(_AST_pos pos, _AST_ declarator, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->exp = exp;
	cont->pos = pos;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->type = DECLARATOR_STRUCT;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_struct_list(_AST_pos pos, _AST_ list, _AST_ declarator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = declarator;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = declarator;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = declarator;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DECLARATOR_STRUCT_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_declarator_direct(_AST_pos pos, _AST_type type, _AST_string id, _AST_ declarator, _AST_ list, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->id = id;
	ast->declarator = declarator;
	ast->list = list;
	ast->exp = exp;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->pos = pos;
	cont->type = type;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_abstract(_AST_pos pos, _AST_ declarator, _AST_ pointer) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->pointer = pointer;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->pos = pos;
	cont->type = DECLARATOR_ABSTRACT;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_direct_abstract(_AST_pos pos, _AST_type type, _AST_ declarator, _AST_ exp, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->list = list;
	ast->exp = exp;
	ast->attrlist_l = NULL;
	ast->attrlist_r = NULL;
	cont->pos = pos;
	cont->type = type;
	cont->ast = ast;
	return cont;
}

_AST_ AST_declarator_attributed(_AST_ declarator, _AST_ left, _AST_ right) {
	((_AST_declarator) declarator->ast)->attrlist_l = left;
	((_AST_declarator) declarator->ast)->attrlist_r = right;
	return declarator;
}

_AST_ AST_pointer(_AST_pos pos, _AST_ list, _AST_ pointer) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_pointer ast = checked_malloc(sizeof (*ast));
	ast->pointer = pointer;
	ast->list = list;
	cont->pos = pos;
	cont->type = POINTER;
	cont->ast = ast;
	return cont;
}

_AST_ AST_sqlist(_AST_pos pos, _AST_ list, _AST_ sq) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = sq;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = sq;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = sq;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = SPECQUAL_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_tqlist(_AST_pos pos, _AST_ list, _AST_ tq) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = tq;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = tq;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = tq;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = TYPE_QUALIFIER_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_ptlist(_AST_pos pos, _AST_ list, _AST_flag ellipsis) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq = checked_malloc(sizeof (*seq));
	seq->left = list;
	seq->right = NULL;
	if (ellipsis == 0) {
		seq->type = SEQ_AN;
	} else {
		seq->type = SEQ_AA;
	}
	cont->pos = pos;
	cont->type = PARAMETER_TYPE_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_parlist(_AST_pos pos, _AST_ list, _AST_ declaration) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = declaration;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = declaration;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = declaration;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = PARAMETER_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_idlist(_AST_pos pos, _AST_ list, _AST_string id) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_ astid = AST_primary_id(pos, id);
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = astid;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = astid;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = astid;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = IDENTIFIER_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_type_name(_AST_pos pos, _AST_ list, _AST_ declarator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_type_name ast = checked_malloc(sizeof (*ast));
	ast->declarator = declarator;
	ast->list = list;
	cont->pos = pos;
	cont->type = TYPE_NAME_;
	cont->ast = ast;
	return cont;
}

_AST_ AST_initializer(_AST_pos pos, _AST_ list, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->exp = exp;
	ast->list = list;
	cont->pos = pos;
	cont->type = INITIALIZER;
	cont->ast = ast;
	return cont;
}

_AST_ AST_initializer_list(_AST_pos pos, _AST_ list, _AST_ designation, _AST_ initializer) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->init = initializer;
	ast->list = list;
	ast->declarator = designation;
	cont->pos = pos;
	cont->type = INITIALIZER_LIST;
	cont->ast = ast;
	return cont;
}

_AST_ AST_designation(_AST_pos pos, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = DESIGNATION;
	cont->ast = list;
	return cont;
}

_AST_ AST_designator(_AST_pos pos, _AST_ exp, _AST_string id) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_declarator ast = checked_malloc(sizeof (*ast));
	ast->id = id;
	ast->exp = exp;
	cont->pos = pos;
	cont->type = DESIGNATOR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_designator_list(_AST_pos pos, _AST_ list, _AST_ designator) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = designator;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = designator;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = designator;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = DESIGNATOR_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_stmt_labeled(_AST_pos pos, _AST_string id, _AST_ exp, _AST_ stmt, _AST_ attrlist) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_flow ast = checked_malloc(sizeof (*ast));
	ast->id = id;
	ast->exp = exp;
	ast->stmt = stmt;
	ast->attrlist = attrlist;
	cont->pos = pos;
	cont->type = STMT_LABEL;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_compound(_AST_pos pos, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = STMT_COMPOUND;
	cont->ast = list;
	return cont;
}

_AST_ AST_stmt_exp(_AST_pos pos, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = STMT_EXP;
	cont->ast = exp;
	return cont;
}

_AST_ AST_stmt_if(_AST_pos pos, _AST_ if_, _AST_ then, _AST_ else_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_conditional ast = checked_malloc(sizeof (*ast));
	ast->condition = if_;
	ast->then = then;
	ast->else_ = else_;
	cont->pos = pos;
	cont->type = STMT_IF;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_switch(_AST_pos pos, _AST_ exp, _AST_ stmt) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_flow ast = checked_malloc(sizeof (*ast));
	ast->exp = exp;
	ast->stmt = stmt;
	cont->pos = pos;
	cont->type = STMT_SWITCH;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_while(_AST_pos pos, _AST_ exp, _AST_ stmt) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_flow ast = checked_malloc(sizeof (*ast));
	ast->exp = exp;
	ast->stmt = stmt;
	cont->pos = pos;
	cont->type = STMT_WHILE;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_do(_AST_pos pos, _AST_ stmt, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_flow ast = checked_malloc(sizeof (*ast));
	ast->exp = exp;
	ast->stmt = stmt;
	cont->pos = pos;
	cont->type = STMT_DO;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_for(_AST_pos pos, _AST_ init, _AST_ test, _AST_ step, _AST_ body) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_loop ast = checked_malloc(sizeof (*ast));
	ast->init = init;
	ast->test = test;
	ast->step = step;
	ast->body = body;
	cont->pos = pos;
	cont->type = STMT_FOR;
	cont->ast = ast;
	return cont;
}

_AST_ AST_stmt_jump(_AST_pos pos, _AST_int type, _AST_string id, _AST_ exp) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_flow ast = checked_malloc(sizeof (*ast));
	ast->type = type;
	ast->exp = exp;
	ast->id = id;
	cont->pos = pos;
	cont->type = STMT_JUMP;
	cont->ast = ast;
	return cont;
}

_AST_ AST_block_list(_AST_pos pos, _AST_ list, _AST_ block) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = block;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = block;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = block;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = BLOCK_ITEM_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_function(_AST_pos pos, _AST_ specs, _AST_ declarator, _AST_ decl_list, _AST_ stmt) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_function ast = checked_malloc(sizeof (*ast));
	ast->decl_specs = specs;
	ast->declarator = declarator;
	ast->decl_list = decl_list;
	ast->stmt = stmt;
	cont->pos = pos;
	cont->type = FUNCTION_DEFITION;
	cont->ast = ast;
	return cont;
}

_AST_root AST_unit(_AST_ declaration) {
	_AST_root root = checked_malloc(sizeof (*root));
	root->left = declaration;
	root->right = NULL;
	return root;
}

_AST_ AST_attribute(_AST_pos pos, _AST_string attribute, _AST_flag comma) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_attribute ast = checked_malloc(sizeof (*ast));
	ast->attribute = attribute;
	ast->comma = comma;
	cont->pos = pos;
	cont->type = ATTRIBUTE_SPEC;
	cont->ast = ast;
	return cont;
}

_AST_ AST_attribute_list(_AST_pos pos, _AST_ attribute, _AST_ list) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	_AST_seq seq;
	if (list == NULL) {
		seq = checked_malloc(sizeof (*seq));
		seq->left = attribute;
		seq->right = NULL;
		seq->type = SEQ_AN;
	} else if (((_AST_seq) list->ast)->right == NULL) {
		seq = (_AST_seq) list->ast;
		seq->right = attribute;
		seq->type = SEQ_AA;
	} else {
		seq = checked_malloc(sizeof (*seq));
		seq->left = list;
		seq->right = attribute;
		seq->type = SEQ_SA;
	}
	cont->pos = pos;
	cont->type = ATTRIBUTE_SPEC_LIST;
	cont->ast = seq;
	return cont;
}

_AST_ AST_include(_AST_pos pos, _AST_string inc) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = INC_DIRECTIVE;
	cont->ast = AST_terminal_string(inc);
	return cont;
}

_AST_ AST_asm(_AST_pos pos, _AST_string asm_) {
	_AST_ cont = checked_malloc(sizeof (*cont));
	cont->pos = pos;
	cont->type = STMT_ASM;
	cont->ast = AST_terminal_string(asm_);
	return cont;
}

