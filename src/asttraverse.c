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
 * astprint.c - Print Abstract Syntax data ures.
 *	astprint() is the initial function that accepts the
 *  output file handler and the root ure.
 *  Subsequently there is one function per family of
 *  ast ures.
 */

#include "config.h"

int debug_ast = 0;			/* a flag activating debug mode for the AST traversal */

/* local function prototypes */

void tr_primary_exp(DC_table dt, const _AST_ ast);
void tr_list(DC_table dt, const _AST_ ast);
void tr_declaration(DC_table dt, const _AST_ ast);
void tr_spec(DC_table dt, const _AST_ ast);
void tr_declarator(DC_table dt, const _AST_ ast);
void tr_pointer(DC_table dt, const _AST_ ast);
void tr_type_name(DC_table dt, const _AST_ ast);
void tr_designation(DC_table dt, const _AST_ ast);
void tr_stmt(DC_table dt, const _AST_ ast);
void tr_stmt_flow(DC_table dt, const _AST_ ast);
void tr_block(DC_table dt, const _AST_ ast);
void tr_function(DC_table dt, const _AST_ ast);

DC_decl declaration_create(DC_table, const _AST_string, _AST_, _AST_ , _AST_);
//DC_decl declaration_create(DC_table, _AST_string, _AST_, _AST_, _AST_);
void declaration_finalize(DC_table);

/* Function declarations */

void asttraverse(DC_table dt, const _AST_root ast) {
	if (ast == NULL) return;
	if (ast->left != NULL) {
		if (debug_ast) fprintf(stderr, "\nLINE %d -- parsing %d\n", ast->left->pos, ast->left->type);
		switch (ast->left->type) {
			case FUNCTION_DEFITION:
				tr_function(dt, ast->left);
				break;
			case DECLARATION:
				tr_declaration(dt, ast->left);
				break;
			case INC_DIRECTIVE:
				break;
			default:
				assert(0);
				return;
		}
	}
	if (ast->right != NULL) asttraverse(dt, ast->right);
}

void tr_declaration(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	_AST_declaration d = (_AST_declaration) ast->ast;
	if (debug_ast) fprintf(stderr, "\t ** DECLARATION %d **\n", ast->type);
	switch (ast->type) {
		case DECLARATION:
			// create a new declaration
			// declarator is set to NULL cause at this point
			// we have a list of possibly multiple declarators
			// that need to be parsed one by one
			if (d->declarator != NULL) {
				declaration_create(dt, NULL, d->decl_specs, NULL, NULL);
				if (debug_ast) fprintf(stderr, " 1 **\n");
			}
			// parse the declaration for nested declarations
			tr_list(dt, d->decl_specs);
			// parse the init-declarator-list to extract the identifier
			tr_list(dt, d->declarator);
			// terminate the declaration's declaration
			if (d->declarator != NULL) declaration_finalize(dt);
			break;
		case DECLARATION_PARAMETER:
		{
			// create a new declaration
			// we don't want to store a parameter of a prototype
			// they have zero scope of effect
			int declare = 0;
			if (decltab_current(dt)->body != NULL && d->declarator != NULL)
				if (d->declarator->type != DECLARATOR_ABSTRACT)
					declare = 1;
			if (declare) {
				declaration_create(dt, NULL, d->decl_specs, d->declarator, NULL);
				if (debug_ast) fprintf(stderr, " 2 **\n");
			}
			// parse the declaration for nested declarations
			tr_list(dt, d->decl_specs);
			// parse the init-declarator-list to extract the identifier
			tr_declarator(dt, d->declarator);
			// terminate the declaration's declaration
			if (declare) declaration_finalize(dt);
			break;
		}
		case DECLARATION_STRUCT:
			// we are inside the body of a struct declaration
			// we only care for a declaration of a nested structure
			// so all other members are disregarded
			//			tr_list(dt, d->decl_specs);
			//			tr_list(dt, d->struct_list);
			tr_spec(dt, d->struct_spec);
			break;
		default:
			assert(0);
			return;
	}
}

void tr_spec(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	if (debug_ast) fprintf(stderr, "\t ** SPEC %d **\n", ast->type);
	if (ast->type == STORECLASS_SPECIFIER || ast->type == TYPE_SPECIFIER_INT || ast->type == TYPE_QUALIFIER || ast->type == FUNCTION_SPECIFIER) {
		if (((_AST_terminal) ast->ast)->int_ == TYPEDEF)
			decltab_settypedef(dt);
	} else if (ast->type != TYPE_SPECIFIER_STRING) {
		/* this is a stuct, union or enum specifier
		 * so IF an identifier exists it consists of
		 * a declaration on its own
		 */

		_AST_struct str = (_AST_struct) ast->ast;
		if (str->id != NULL && str->list != NULL) {
			declaration_create(dt, str->id, NULL, NULL, str->list);
			if (debug_ast) fprintf(stderr, " new STRUCT **\n");
			// create a new declaration
			switch (ast->type) {
				case STRUCT_SPECIFIER:
					if (str->type == STRUCT)
						decltab_setstruct(dt);
					else decltab_setunion(dt);
					break;
				case ENUM_SPECIFIER:
					decltab_setenum(dt);
					break;
				default:
					if (debug_ast) fprintf(stderr, "\nthe type is: %d\n", ast->type);
					assert(0);
					return;
			}
			declaration_finalize(dt);
		}
		if (str->list != NULL) {
			decltab_scope_new(dt);
			if (debug_ast) fprintf(stderr, " ** in STRUCT body **\n");
			tr_list(dt, str->list);
			decltab_scope_end(dt);
		}
	}
}

void tr_declarator(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	_AST_declarator dr = (_AST_declarator) ast->ast;
	switch (ast->type) {
		case DECLARATOR_INIT:
			decltab_setdeclarator(dt, dr->declarator);
			tr_declarator(dt, dr->declarator);
			if (dr->init != NULL) {
				decltab_setbody(dt, dr->init);
				tr_declarator(dt, dr->init);
			}
			break;
		case DECLARATOR:
			tr_pointer(dt, dr->pointer);
			tr_declarator(dt, dr->declarator);
			break;
		case DECLARATOR_STRUCT:
			tr_declarator(dt, dr->declarator);
			if (dr->exp != NULL) {
				tr_primary_exp(dt, dr->exp);
			}
			break;
		case ENUMERATOR:

			break;
		case DECLARATOR_DIRECT_1:
			/* a declarator declares at most one identifier */
			if (decltab_current(dt)->id == NULL) {
				decltab_setid(dt, dr->id);
				if (decltab_hastype(dt) == DC_FUNC || decltab_hastype(dt) == DC_TASK || decltab_hastype(dt) == DC_TASK_PROTO) {
					decltab_scope_new(dt);
					if (debug_ast) fprintf(stderr, " ** DIRECT_DECLARATOR **\n");
				}
			}
			break;
		case DECLARATOR_DIRECT_2:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			break;
		case DECLARATOR_DIRECT_3:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			tr_primary_exp(dt, dr->exp);
			break;
		case DECLARATOR_DIRECT_4:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			tr_primary_exp(dt, dr->exp);
			break;
		case DECLARATOR_DIRECT_5:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			tr_primary_exp(dt, dr->exp);
			break;
		case DECLARATOR_DIRECT_6:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			break;
		case DECLARATOR_DIRECT_7:
			if (!decltab_hastype(dt)) decltab_setarray(dt);
			tr_declarator(dt, dr->declarator);
			break;
		case DECLARATOR_DIRECT_8:
		{
			/* if NOT inside a function definition
			 * this is a function prototype.
			 */
			int proto = 0;
			if (!decltab_hastype(dt) && dr->list != NULL && dr->list->type != IDENTIFIER_LIST) {
				decltab_setfunction(dt);
				proto = 1;
			}
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			if (proto) decltab_scope_end(dt);
			break;
		}
		case DECLARATOR_ABSTRACT:
			/* type-name or parameter-type-list.
			 * the later is part of either direct or direct-abstract
			 */
			tr_pointer(dt, dr->pointer);
			tr_declarator(dt, dr->declarator);
			break;
			/* all of the following are part only of an abstract declarator */
		case DECLARATOR_ABSTRACT_DIRECT_1:
			tr_declarator(dt, dr->declarator);
			break;
		case DECLARATOR_ABSTRACT_DIRECT_2:
			tr_declarator(dt, dr->declarator);
			tr_primary_exp(dt, dr->exp);
			break;
		case DECLARATOR_ABSTRACT_DIRECT_3:
			tr_declarator(dt, dr->declarator);
			break;
		case DECLARATOR_ABSTRACT_DIRECT_4:
			tr_declarator(dt, dr->declarator);
			tr_list(dt, dr->list);
			break;
		case INITIALIZER:
			tr_primary_exp(dt, dr->exp);
			if (dr->list != NULL) {
				tr_declarator(dt, dr->list);
			}
			break;
		case INITIALIZER_LIST:
			if (dr->list != NULL) {
				tr_declarator(dt, dr->list);
			}
			tr_designation(dt, dr->declarator);
			tr_declarator(dt, dr->init);
			break;
		case DESIGNATOR:

			break;
		default:
			if (debug_ast) fprintf(stderr, "\nthe type is: %d\n", ast->type);
			assert(0);
			return;
	}
}

void tr_designation(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	tr_list(dt, ast->ast);
}

void tr_pointer(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	_AST_pointer p = (_AST_pointer) ast->ast;
	tr_list(dt, p->list);
	tr_pointer(dt, p->pointer);
}

void tr_type_name(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	_AST_type_name t = (_AST_type_name) ast->ast;
	tr_list(dt, t->list);
	tr_declarator(dt, t->declarator);
}

void tr_function(DC_table dt, const _AST_ ast) {
	_AST_function f = (_AST_function) ast->ast;
	declaration_create(dt, NULL, f->decl_specs, f->declarator, f->stmt);
	decltab_setfunction(dt);
	tr_list(dt, f->decl_specs);
	tr_declarator(dt, f->declarator);
	tr_list(dt, f->decl_list);
	tr_stmt(dt, f->stmt);
	decltab_scope_end(dt);
}

void tr_primary_exp(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	switch (ast->type) {
		case EXP_IDENTIFIER:
		case EXP_CONSTVAL_INT:
		case EXP_CONSTVAL_FLO:
		case EXP_CONSTVAL_HEX:
		case EXP_STRING:
			break;
		case EXP_BRACKETED:
			tr_primary_exp(dt, ((_AST_) ast->ast));
			break;
		case EXP_SUBSCRIPT:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->right);
			break;
		case EXP_ICALL:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			if (((_AST_exp) ast->ast)->right != NULL) {
				if (((_AST_) ((_AST_exp) ast->ast)->right)->type == EXP_ARGS)
					tr_list(dt, ((_AST_exp) ast->ast)->right);
				else
					tr_primary_exp(dt, ((_AST_exp) ast->ast)->right);
			}
			break;
		case EXP_STRUCTMBR:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			break;
		case EXP_PTRMBR:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			break;
		case EXP_INC:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			break;
		case EXP_CAST:
			tr_type_name(dt, ((_AST_exp) ast->ast)->left);
			if (((_AST_exp) ast->ast)->right != NULL) {
				if (((_AST_exp) ast->ast)->right->type == INITIALIZER_LIST) {
					tr_declarator(dt, ((_AST_exp) ast->ast)->right);
				} else
					tr_primary_exp(dt, ((_AST_exp) ast->ast)->right);
			}
			break;
		case EXP_UNARY:
			if (((_AST_exp) ast->ast)->left != NULL) {
				if (((_AST_exp) ast->ast)->left->type == TYPE_NAME_) {
					tr_type_name(dt, ((_AST_exp) ast->ast)->left);
				} else {
					tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
				}
			}
			break;
		case EXP_BINARY:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->right);
			break;
		case EXP_COND:
			tr_primary_exp(dt, ((_AST_conditional) ast->ast)->condition);
			tr_primary_exp(dt, ((_AST_conditional) ast->ast)->then);
			tr_primary_exp(dt, ((_AST_conditional) ast->ast)->else_);
			break;
		case EXP_LIST:
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->left);
			tr_primary_exp(dt, ((_AST_exp) ast->ast)->right);
			break;
		default:
			if (debug_ast) fprintf(stderr, "\nthe type is: %d\n", ast->type);
			assert(0);
			return;
	}
}

void tr_stmt(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	if (ast->type == STMT_LABEL || ast->type == STMT_SWITCH || ast->type == STMT_WHILE || ast->type == STMT_DO || ast->type == STMT_JUMP)
		tr_stmt_flow(dt, ast);
	else {
		switch (ast->type) {
			case STMT_COMPOUND:
				decltab_scope_new(dt);
				if (debug_ast) fprintf(stderr, " ** COMPOUND STMT **\n");
				tr_list(dt, (_AST_) ast->ast);
				decltab_scope_end(dt);
				break;
			case STMT_EXP:
				tr_primary_exp(dt, (_AST_) ast->ast);
				break;
			case STMT_IF:
			{
				_AST_conditional c = (_AST_conditional) ast->ast;
				tr_primary_exp(dt, c->condition);
				decltab_scope_new(dt);
				if (debug_ast) fprintf(stderr, " ** IF STMT **\n");
				tr_stmt(dt, c->then);
				decltab_scope_end(dt);
				if (c->else_ != NULL) {
					decltab_scope_new(dt);
					if (debug_ast) fprintf(stderr, " ** ELSE STMT **\n");
					tr_stmt(dt, c->else_);
					decltab_scope_end(dt);
				}
				break;
			}
			case STMT_FOR:
				decltab_scope_new(dt);
				if (debug_ast) fprintf(stderr, " ** FOR STMT **\n");
				_AST_loop lp = (_AST_loop) ast->ast;
				if(lp->init != NULL){
					if (lp->init->type == DECLARATION){
						tr_declaration(dt, lp->init);
					}else {
						tr_stmt(dt, lp->init);
					}
				}
				tr_stmt(dt, lp->test);
				tr_primary_exp(dt, lp->step);
				tr_stmt(dt, lp->body);
				decltab_scope_end(dt);
				break;
			case STMT_ASM:
				break;
			default:
				assert(0);
				return;
		}
	}
}

void tr_stmt_flow(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	_AST_flow fl = (_AST_flow) ast->ast;
	switch (ast->type) {
		case STMT_LABEL:
			decltab_scope_new(dt);
			if (debug_ast) fprintf(stderr, " ** LABEL STMT **\n");
			tr_stmt(dt, fl->stmt);
			decltab_scope_end(dt);
			break;
		case STMT_SWITCH:
			decltab_scope_new(dt);
			if (debug_ast) fprintf(stderr, " ** SWITCH STMT **\n");
			tr_primary_exp(dt, fl->exp);
			tr_stmt(dt, fl->stmt);
			decltab_scope_end(dt);
			break;
		case STMT_WHILE:
			decltab_scope_new(dt);
			if (debug_ast) fprintf(stderr, " ** WHILE STMT **\n");
			tr_primary_exp(dt, fl->exp);
			tr_stmt(dt, fl->stmt);
			decltab_scope_end(dt);
			break;
		case STMT_DO:
			decltab_scope_new(dt);
			if (debug_ast) fprintf(stderr, " ** DO STMT **\n");
			tr_stmt(dt, fl->stmt);
			tr_primary_exp(dt, fl->exp);
			decltab_scope_end(dt);
			break;
		case STMT_JUMP:
			break;
		default:
			assert(0);
			return;
	}
}

void tr_block(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	if (ast->type == DECLARATION) tr_declaration(dt, ast);
	else tr_stmt(dt, ast);
}

void tr_list(DC_table dt, const _AST_ ast) {
	if (ast == NULL) return;
	int sa = 0;
	_AST_seq seq = (_AST_seq) ast->ast;
	if (seq->type == SEQ_SA) {
		sa = 1;
	}
	if (debug_ast) fprintf(stderr, "\t ** LIST %d **\n", ast->type);
	if (ast->type == DECLARATION_LIST || ast->type == DECLARATION_STRUCT_LIST || ast->type == PARAMETER_LIST) {
		if (!sa) tr_declaration(dt, seq->left);
		else tr_list(dt, seq->left);
		tr_declaration(dt, seq->right);
	} else if (ast->type == ENUMERATOR_LIST || ast->type == DECLARATOR_STRUCT_LIST || ast->type == DESIGNATOR_LIST) {
		if (!sa) tr_declarator(dt, seq->left);
		else tr_list(dt, seq->left);
		tr_declarator(dt, seq->right);
	} else if (ast->type == DECLARATOR_INIT_LIST) {
		if (!sa) tr_declarator(dt, seq->left);
		else tr_list(dt, seq->left);
		// check if we have a new declarator and create a new declaration for it
		if (seq->right != NULL) {
			// a new declarator means that the current declaration has ended and a new one
			// has to be initialized with the same specifiers
			_AST_ specs = decltab_current(dt)->specs;
			declaration_finalize(dt);
			declaration_create(dt, NULL, specs, NULL, NULL);
		}
		tr_declarator(dt, seq->right);
	} else if (ast->type == DECLARATION_SPECIFIER_LIST || ast->type == SPECQUAL_LIST) {
		tr_spec(dt, seq->right);
		if (!sa) tr_spec(dt, seq->left);
		else tr_list(dt, seq->left);
	} else if (ast->type == TYPE_QUALIFIER_LIST) {
		if (!sa) tr_spec(dt, seq->left);
		else tr_list(dt, seq->left);
		tr_spec(dt, seq->right);
	} else {
		switch (ast->type) {
			case EXP_ARGS:
				if (!sa) tr_primary_exp(dt, seq->left);
				else tr_list(dt, seq->left);
				tr_primary_exp(dt, seq->right);
				break;
			case PARAMETER_TYPE_LIST:
				if (!sa) tr_list(dt, seq->left);
				else tr_list(dt, seq->left);
				if (seq->type == SEQ_AA && decltab_hastype(dt) == DC_TASK) {
					// Ellipsis IS NOT ALLOWED as part of task parameters
					asterror("error: ellipsis found as part of a task definition", decltab_current(dt)->declarator->pos, curfilename);
				}
				break;
			case BLOCK_ITEM_LIST:
				if (!sa) tr_block(dt, seq->left);
				else tr_list(dt, seq->left);
				tr_block(dt, seq->right);
				break;
			case IDENTIFIER_LIST:
			case ATTRIBUTE_SPEC_LIST:
				// intentionally left empty
				break;
			default:
				assert(0);
				return;
		}
	}
}

DC_decl declaration_create(DC_table dt, const _AST_string word, _AST_ specs, _AST_ declarator, _AST_ body) {
	if (debug_ast) fprintf(stderr, "\n\t start ** %s **", word);
	return decltab_new(dt, word, specs, declarator, body);
}

/* finalize a declaration entry into the table */
void declaration_finalize(DC_table dt) {
	if (debug_ast) fprintf(stderr, "\n\t stop ** \n");
	DC_decl dcl = decltab_current(dt);
	// if at this point no type has been set then this is a simple variable declaration
	if (!decltab_hastype(dt)) decltab_setvar(dt);
	// traversal finished, set to initialized
	dcl->initialized = 1;
	DC_decl pdcl = decltab_lookup(dt, dcl->id, dcl->type);
	if (pdcl != NULL) {
		// replace pdcl with current
		decltab_replace_cur(dt, pdcl);
		// pop and free current
		free(pop(dt->temp));
		// pdcl is finalized so fallback to dcl's parent
		dt->current = pdcl->old;
	} else decltab_finalize(dt);
	// deactivate and fallback to the previous, if any

}

