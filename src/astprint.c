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
 * astprint.c - Print Abstract Syntax data structures.
 *	astprint() is the initial function. Arguments are
 *  the file handler and the structure root.
 *  There is one function per family of
 *  ast structures.
 */

#include "config.h"

int debug_print = 0;	/* a flag activating debug mode for the printer */
hdrparms hdrp;

/* local function prototypes */

void pr_root(char out[], DC_table dt, _AST_root ast, _AST_flag safe);

void pr_primary_id(char out[], _AST_string ast, _AST_flag safe);
void pr_primary_int(char out[], _AST_int ast);
void pr_primary_float(char out[], _AST_float ast);
void pr_primary_string(char out[], _AST_string ast, _AST_flag safe);
void pr_operator(char out[], _AST_int op);


void pr_declaration(char out[], _AST_ ast, _AST_flag safe);

void pr_spec(char out[], _AST_ ast, _AST_flag safe);

void pr_pointer(char out[], _AST_ ast, _AST_flag safe);

void pr_type_name(char out[], _AST_ ast, _AST_flag safe);

void pr_designation(char out[], _AST_ ast, _AST_flag safe);

void pr_stmt(char out[], _AST_ ast, _AST_flag safe);
void pr_stmt_flow(char out[], _AST_ ast, _AST_flag safe);

void pr_block(char out[], _AST_ ast, _AST_flag safe);

void pr_function(char out[], _AST_ ast, _AST_flag safe);

void pr_attribute(char out[], _AST_ ast, _AST_flag safe);
void pr_include(char out[], _AST_string ast, _AST_flag safe);
void pr_asm(char out[], _AST_string ast, _AST_flag safe);


/* Function declarations */


void astprint(FILE *out, long sz, DC_table dt, _AST_root ast, _AST_flag safe){
	/* create a big enough buffer for the printed file */
	char str[2*sz];
	/* base null termination */
	str[0] = '\0';
	/* initialiaze recursive tree parsing */
	pr_root(str, dt, ast, safe);
	/* tail null termination */
	str[strlen(str)] = '\0';
	/* print to file */
	fprintf(out, "%s\n", str);
}

void pr_root(char out[],  DC_table dt, _AST_root ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nEXTERNAL: entering type: %d\n", ast->left->type);

	switch(ast->left->type){
		case FUNCTION_DEFITION:
			pr_function(out, ast->left, safe);
			break;
		case DECLARATION:
			pr_declaration(out, ast->left, safe);
			break;
		case INC_DIRECTIVE:
			pr_include(out, ((_AST_terminal)ast->left->ast)->string, safe);
			break;
		default:
			assert(0);
			return;
	}
	if(ast->right != NULL){
		pr_root(out, dt, ast->right, safe);
	}
	if(!safe) free(ast);
}

void pr_primary_id(char out[], _AST_string ast, _AST_flag safe){
	if(ast == NULL) return;
	strcat(out, ast);
	if(!safe) free(ast);
}

void pr_primary_int(char out[], _AST_int ast){
	sprintf(out,"%s%d", out, ast);
}

void pr_primary_float(char out[], _AST_float ast){
	sprintf(out,"%s%f", out, ast);
}

void pr_primary_string(char out[], _AST_string ast, _AST_flag safe){
	if(ast == NULL) return;
	strcat(out, " ");
	strcat(out, ast);
	strcat(out, " ");
	if(!safe) free(ast);
}

void pr_primary_exp(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	int freeast = 1;
	if(debug_print) fprintf(stderr, "\nEXPRESSION: entering type: %d\n", ast->type);
	switch (ast->type) {
		case EXP_IDENTIFIER:
			pr_primary_id(out,((_AST_terminal)ast->ast)->string, safe);
			freeast = 0;
			break;
		case EXP_CONSTVAL_INT:
			pr_primary_int(out,((_AST_terminal)ast->ast)->int_);
			break;
		case EXP_CONSTVAL_FLO:
			pr_primary_float(out,((_AST_terminal)ast->ast)->float_);
			break;
		case EXP_CONSTVAL_HEX:
			pr_primary_string(out,((_AST_terminal)ast->ast)->string, safe);
			freeast = 0;
			break;
		case EXP_STRING:
			pr_primary_string(out,((_AST_terminal)ast->ast)->string, safe);
			freeast = 0;
			break;
		case EXP_BRACKETED:
			strcat(out,"(");
			pr_primary_exp(out,((_AST_)ast->ast), safe);
			strcat(out,")");
			freeast = 0;
			break;
		case EXP_SUBSCRIPT:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,"[");
			pr_primary_exp(out,((_AST_exp)ast->ast)->right, safe);
			strcat(out,"]");
			break;
		case EXP_ICALL:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,"(");
			if(((_AST_exp)ast->ast)->right != NULL) {
				if(((_AST_)((_AST_exp)ast->ast)->right)->type == EXP_ARGS)
					pr_list(out,((_AST_exp)ast->ast)->right, safe);
				else
					pr_primary_exp(out,((_AST_exp)ast->ast)->right, safe);
			}
			strcat(out,")");
			break;
		case EXP_STRUCTMBR:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,".");
			pr_primary_id(out,((_AST_exp)ast->ast)->constant->string, safe);
			break;
		case EXP_PTRMBR:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,"->");
			pr_primary_id(out,((_AST_exp)ast->ast)->constant->string, safe);
			break;
		case EXP_INC:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			pr_operator(out,((_AST_exp)ast->ast)->op);
			break;
		case EXP_CAST:
			strcat(out,"(");
			pr_type_name(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,")");
			if(((_AST_exp)ast->ast)->right != NULL) {
				if(((_AST_exp)ast->ast)->right->type == INITIALIZER_LIST){
					strcat(out,"{");
					pr_declarator(out,((_AST_exp)ast->ast)->right, safe);
					strcat(out,"}");
				} else
					pr_primary_exp(out,((_AST_exp)ast->ast)->right, safe);
			}
			break;
		case EXP_UNARY:
			pr_operator(out,((_AST_exp)ast->ast)->op);
			if(((_AST_exp)ast->ast)->left != NULL) {
				if(((_AST_exp)ast->ast)->left->type == TYPE_NAME_){
					strcat(out,"(");
					pr_type_name(out,((_AST_exp)ast->ast)->left, safe);
					strcat(out,")");
				} else {
					pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
				}
			}
			break;
		case EXP_BINARY:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			pr_operator(out, ((_AST_exp)ast->ast)->op);
			pr_primary_exp(out,((_AST_exp)ast->ast)->right, safe);
			break;
		case EXP_COND:
			pr_primary_exp(out,((_AST_conditional)ast->ast)->condition, safe);
			strcat(out,"?");
			pr_primary_exp(out,((_AST_conditional)ast->ast)->then, safe);
			strcat(out,":");
			pr_primary_exp(out,((_AST_conditional)ast->ast)->else_, safe);
			break;
		case EXP_LIST:
			pr_primary_exp(out,((_AST_exp)ast->ast)->left, safe);
			strcat(out,", ");
			pr_primary_exp(out,((_AST_exp)ast->ast)->right, safe);
			break;
		default:
			if(debug_print) fprintf(stderr, "\nEXP: type not found: %d\n", ast->type);
			assert(0);
			return;
	}
	if(freeast && !safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_operator(char out[], _AST_int op){
	if(op == 0) return;
	if(debug_print) fprintf(stderr, "\nOP: operator: %c\n", op);
	switch (op) {
		case '+':
			strcat(out, " + ");
			break;
		case '-':
			strcat(out, " - ");
			break;
		case '*':
			strcat(out, " * ");
			break;
		case '/':
			strcat(out, " / ");
			break;
		case '%':
			strcat(out, " % ");
			break;
		case '<':
			strcat(out, " < ");
			break;
		case '>':
			strcat(out, " > ");
			break;
		case LE_OP:
			strcat(out, " <= ");
			break;
		case GE_OP:
			strcat(out, " >= ");
			break;
		case EQ_OP:
			strcat(out, " == ");
			break;
		case NE_OP:
			strcat(out, " != ");
			break;
		case LEFT_OP:
			strcat(out, " << ");
			break;
		case RIGHT_OP:
			strcat(out, " >> ");
			break;
		case INC_OP:
			strcat(out, "++");
			break;
		case DEC_OP:
			strcat(out, "--");
			break;
		case SIZEOF:
			strcat(out, " sizeof ");
			break;
		case '&':
			strcat(out, "&");
			break;
		case '~':
			strcat(out, "~");
			break;
		case '!':
			strcat(out, "!");
			break;
		case '^':
			strcat(out, " ^ ");
			break;
		case '|':
			strcat(out, " | ");
			break;
		case AND_OP:
			strcat(out, " && ");
			break;
		case OR_OP:
			strcat(out, " || ");
			break;
		case '=':
			strcat(out, " = ");
			break;
		case MUL_ASSIGN:
			strcat(out, " *= ");
			break;
		case DIV_ASSIGN:
			strcat(out, " /= ");
			break;
		case MOD_ASSIGN:
			strcat(out, " %= ");
			break;
		case ADD_ASSIGN:
			strcat(out, " += ");
			break;
		case SUB_ASSIGN:
			strcat(out, " -= ");
			break;
		case LEFT_ASSIGN:
			strcat(out, " <<= ");
			break;
		case RIGHT_ASSIGN:
			strcat(out, " >>= ");
			break;
		case AND_ASSIGN:
			strcat(out, " &= ");
			break;
		case XOR_ASSIGN:
			strcat(out, " ^= ");
			break;
		case OR_ASSIGN:
			strcat(out, " |= ");
			break;
		default:
			if(debug_print) fprintf(stderr, "\nOP: %d\n", op);
			assert(0);
			return;
	}
}

void pr_list(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	int sa = 0;
	if(debug_print) fprintf(stderr, "\nLIST: entering type: %d\n", ast->type);
	_AST_seq seq = (_AST_seq)ast->ast;
	if(seq->type == SEQ_SA){
		sa = 1;
	}
	if(ast->type == DECLARATION_LIST || ast->type == DECLARATION_STRUCT_LIST || ast->type == PARAMETER_LIST){
		if(!sa) pr_declaration(out, seq->left, safe);
		else pr_list(out, seq->left, safe);
		if(ast->type == PARAMETER_LIST && seq->right != NULL) strcat(out,", ");
		pr_declaration(out, seq->right, safe);
	} else if(ast->type == ENUMERATOR_LIST || ast->type == DECLARATOR_INIT_LIST || ast->type == DECLARATOR_STRUCT_LIST || ast->type == DESIGNATOR_LIST){
		if(!sa) pr_declarator(out, seq->left, safe);
		else pr_list(out, seq->left, safe);
		if(ast->type != DESIGNATOR_LIST && seq->right != NULL) strcat(out,", ");
		pr_declarator(out, seq->right, safe);
	} else if(ast->type == DECLARATION_SPECIFIER_LIST || ast->type == SPECQUAL_LIST){
		pr_spec(out, seq->right, safe);
		if(!sa) pr_spec(out, seq->left, safe);
		else pr_list(out, seq->left, safe);
	} else if(ast->type == TYPE_QUALIFIER_LIST){
		if(!sa) pr_spec(out, seq->left, safe);
		else pr_list(out, seq->left, safe);
		pr_spec(out, seq->right, safe);
	} else {
		switch (ast->type) {
			case EXP_ARGS:
				if(!sa) pr_primary_exp(out, seq->left, safe);
				else pr_list(out, seq->left, safe);
				if(seq->right != NULL) strcat(out,", ");
				pr_primary_exp(out, seq->right, safe);
				break;
			case PARAMETER_TYPE_LIST:
				if(!sa) pr_list(out, seq->left, safe);
				else pr_list(out, seq->left, safe);
				if(seq->type == SEQ_AA) strcat(out, ", ... ");
				break;
			case IDENTIFIER_LIST:
				if(!sa) {
					pr_primary_id(out, ((_AST_terminal)seq->left->ast)->string, safe);
					if(!safe) free(seq->left->ast);
				}
				else pr_list(out, seq->left, safe);
				pr_primary_id(out, ((_AST_terminal)seq->right->ast)->string, safe);
				if(seq->right->ast != NULL && !safe) free(seq->right->ast);
				break;
			case BLOCK_ITEM_LIST:
				if(!sa) pr_block(out, seq->left, safe);
				else pr_list(out, seq->left, safe);
				pr_block(out, seq->right, safe);
				break;
			case ATTRIBUTE_SPEC_LIST:
				if(!sa) pr_attribute(out, seq->left, safe);
				else pr_list(out, seq->left, safe);
				pr_attribute(out, seq->right, safe);
				break;
			default:
				assert(0);
				return;
		}
	}
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_declaration(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nDECLARATION: entering type: %d\n", ast->type);
	_AST_declaration d = (_AST_declaration)ast->ast;
	if(d->extension){
		strcat(out, " __extension__ ");
	}
	pr_list(out, d->decl_specs, safe);
	switch(ast->type){
		case DECLARATION:
			// this is an init-declarator-list here
			pr_list(out, d->declarator, safe);
			strcat(out, ";\n ");
			break;
		case DECLARATION_PARAMETER:
			pr_declarator(out, d->declarator, safe);
			break;
		case DECLARATION_STRUCT:
			pr_list(out, d->struct_list, safe);
			pr_spec(out, d->struct_spec, safe);
			strcat(out,";\n ");
			break;
		default:
			assert(0);
			return;
	}
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}


void pr_spec(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nSPECIFIER: entering type: %d\n", ast->type);
	if(ast->type == STORECLASS_SPECIFIER || ast->type == TYPE_SPECIFIER_INT || ast->type == TYPE_QUALIFIER || ast->type == FUNCTION_SPECIFIER) {
		switch(((_AST_terminal)ast->ast)->int_){
			case INLINE:
				strcat(out," inline ");
				break;
			case CONST:
				strcat(out," const ");
				break;
			case _CONST:
				strcat(out," __const ");
				break;
			case _CONST_:
				strcat(out," __const__ ");
				break;
			case RESTRICT:
				strcat(out," restrict ");
				break;
			case _RESTRICT:
				strcat(out," __restrict ");
				break;
			case _RESTRICT_:
				strcat(out," __restrict__ ");
				break;
			case VOLATILE:
				strcat(out," volatile ");
				break;
			case TYPEDEF:
				strcat(out, " typedef ");
				break;
			case EXTERN:
				strcat(out, " extern ");
				break;
			case STATIC:
				strcat(out, " static ");
				break;
			case AUTO:
				strcat(out, " auto ");
				break;
			case REGISTER:
				strcat(out, " register ");
				break;
			case CHAR:
				strcat(out, " char ");
				break;
			case SHORT:
				strcat(out, " short ");
				break;
			case INT:
				strcat(out, " int ");
				break;
			case LONG:
				strcat(out, " long ");
				break;
			case SIGNED:
				strcat(out, " signed ");
				break;
			case UNSIGNED:
				strcat(out, " unsigned ");
				break;
			case FLOAT:
				strcat(out, " float ");
				break;
			case DOUBLE:
				strcat(out, " double ");
				break;
			case VOID:
				strcat(out, " void ");
				break;
			case BOOL:
				strcat(out, " _Bool ");
				break;
			case COMPLEX:
				strcat(out, " _Complex ");
				break;
			case IMAGINARY:
				strcat(out, " _Imaginary ");
				break;
			default:
				assert(0);
				return;
		}
	} else if(ast->type == TYPE_SPECIFIER_STRING){
		strcat(out, " ");
		pr_primary_id(out, ((_AST_terminal)ast->ast)->string, safe);
		strcat(out, " ");
	} else {
		_AST_struct str = (_AST_struct)ast->ast;
		switch(ast->type){
			case STRUCT_SPECIFIER:
				if(str->type == STRUCT)	strcat(out, " struct ");
				else strcat(out, " union ");
				break;
			case ENUM_SPECIFIER:
				strcat(out, " enum ");
				break;
			default:
				assert(0);
				return;
		}
		pr_primary_id(out, str->id, safe);
		strcat(out, " ");
		pr_list(out, str->attrlist, safe);
		if(str->list != NULL){
			strcat(out, "{");
			pr_list(out, str->list, safe);
			strcat(out, "}");
		}
	}
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_declarator(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nDECLARATOR: entering type: %d\n", ast->type);
	_AST_declarator dr = (_AST_declarator)ast->ast;
	switch (ast->type) {
		case DECLARATOR_INIT:
			pr_declarator(out, dr->declarator, safe);
			if(dr->init != NULL){
				strcat(out, " = ");
				pr_declarator(out, dr->init, safe);
			}
			break;
		case DECLARATOR:
			pr_list(out, dr->attrlist_l, safe);
			pr_pointer(out, dr->pointer, safe);
			pr_declarator(out, dr->declarator, safe);
			pr_asm(out, dr->asm_, safe);
			pr_list(out, dr->attrlist_r, safe);
			break;
		case DECLARATOR_STRUCT:
			pr_declarator(out, dr->declarator, safe);
			pr_list(out, dr->attrlist_r, safe);
			if(dr->exp != NULL){
				strcat(out, " : ");
				pr_primary_exp(out, dr->exp, safe);
			}
			break;
		case ENUMERATOR:
			pr_primary_id(out, dr->id, safe);
			if(dr->exp != NULL){
				strcat(out, " = ");
				pr_primary_exp(out, dr->exp, safe);
			}
			break;
		case DECLARATOR_DIRECT_1:
//			strcat(out, " ");
			if(safe != 1) pr_primary_id(out, dr->id, safe);
			break;
		case DECLARATOR_DIRECT_2:
			strcat(out, "(");
			pr_declarator(out, dr->declarator, safe);
			strcat(out, ")");
			break;
		case DECLARATOR_DIRECT_3:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[");
			pr_list(out, dr->list, safe);
			pr_primary_exp(out, dr->exp, safe);
			strcat(out, "]");
			break;
		case DECLARATOR_DIRECT_4:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[");
			strcat(out, " static ");
			pr_list(out, dr->list, safe);
			pr_primary_exp(out, dr->exp, safe);
			strcat(out, "]");
			break;
		case DECLARATOR_DIRECT_5:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[");
			pr_list(out, dr->list, safe);
			strcat(out, " static ");
			pr_primary_exp(out, dr->exp, safe);
			strcat(out, "]");
			break;
		case DECLARATOR_DIRECT_6:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[");
			pr_list(out, dr->list, safe);
			strcat(out, "*");
			strcat(out, "]");
			break;
		case DECLARATOR_DIRECT_7:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[]");
			break;
		case DECLARATOR_DIRECT_8:
			pr_declarator(out, dr->declarator, safe);
			if(safe != 1) {
				strcat(out, "(");
				pr_list(out, dr->list, safe);
				strcat(out, ")");
			}
			break;
		case DECLARATOR_ABSTRACT:
			pr_pointer(out, dr->pointer, safe);
			pr_declarator(out, dr->declarator, safe);
			break;
		case DECLARATOR_ABSTRACT_DIRECT_1:
			strcat(out, "(");
			pr_declarator(out, dr->declarator, safe);
			strcat(out, ")");
			break;
		case DECLARATOR_ABSTRACT_DIRECT_2:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[");
			pr_primary_exp(out, dr->exp, safe);
			strcat(out, "]");
			break;
		case DECLARATOR_ABSTRACT_DIRECT_3:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "[*]");
			break;
		case DECLARATOR_ABSTRACT_DIRECT_4:
			pr_declarator(out, dr->declarator, safe);
			strcat(out, "(");
			pr_list(out, dr->list, safe);
			strcat(out, ")");
			break;
		case INITIALIZER:
			pr_primary_exp(out, dr->exp, safe);
			if(dr->list != NULL){
				strcat(out, "{");
				pr_declarator(out, dr->list, safe);
				strcat(out, "}");
			}
			break;
		case INITIALIZER_LIST:
			if(dr->list != NULL){
				pr_declarator(out, dr->list, safe);
				strcat(out, ", ");
			}
			pr_designation(out, dr->declarator, safe);
			pr_declarator(out, dr->init, safe);
			break;
		case DESIGNATOR:
			if(dr->exp != NULL){
				strcat(out, "[");
				pr_primary_exp(out, dr->exp, safe);
				strcat(out, "]");
			} else {
				strcat(out, ".");
				pr_primary_id(out, dr->id, safe);
			}
			break;
		default:
			assert(0);
			return;
	}
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_pointer(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nPOINTER: entering type: %d\n", ast->type);
	_AST_pointer p = (_AST_pointer) ast->ast;
	strcat(out, "*");
	pr_list(out, p->list, safe);
	pr_pointer(out, p->pointer, safe);
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_type_name(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nTYPE_NAME: entering type: %d\n",ast->type);
	_AST_type_name t = (_AST_type_name) ast->ast;
	pr_list(out, t->list, safe);
	pr_declarator(out, t->declarator, safe);
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_designation(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	pr_list(out, ast->ast, safe);
	strcat(out, " = ");
	if(!safe) free(ast);
}

void pr_stmt(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nSTATEMENT: entering type: %d\n",ast->type);
	if(ast->type == STMT_LABEL || ast->type == STMT_SWITCH || ast->type == STMT_WHILE || ast->type == STMT_DO || ast->type == STMT_JUMP)
		pr_stmt_flow(out, ast, safe);
	else {
		switch (ast->type) {
			case STMT_COMPOUND:
				strcat(out, "{\n");
				pr_list(out, (_AST_)ast->ast, safe);
				strcat(out, "\n}\n");
				break;
			case STMT_EXP:
				pr_primary_exp(out, (_AST_)ast->ast, safe);
				strcat(out, " ; ");
				break;
			case STMT_IF:
				strcat(out, " if ");
				strcat(out, "(");
				_AST_conditional c = (_AST_conditional)ast->ast;
				pr_primary_exp(out, c->condition, safe);
				strcat(out, ") ");
				pr_stmt(out, c->then, safe);
				if(c->else_ != NULL){
					strcat(out, " else ");
					pr_stmt(out, c->else_, safe);
				}
				if(!safe) free(ast->ast);
				break;
			case STMT_FOR:
				strcat(out, " for ");
				strcat(out, "(");
				_AST_loop lp = (_AST_loop)ast->ast;
				if(lp->init->type == DECLARATION)
					pr_declaration(out, lp->init, safe);
				else pr_stmt(out, lp->init, safe);
				pr_stmt(out, lp->test, safe);
				pr_primary_exp(out, lp->step, safe);
				strcat(out, ") ");
				pr_stmt(out, lp->body, safe);
				if(!safe) free(ast->ast);
				break;
			case STMT_ASM:
				pr_asm(out, ((_AST_terminal)ast->ast)->string, safe);
				strcat(out, "; ");
				if(!safe) free(ast->ast);
				break;
			default:
				assert(0);
				return;
		}
	}
	if(!safe) free(ast);
}

void pr_stmt_flow(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nSTATEMENT_FLOW: entering type: %d\n",ast->type);
	_AST_flow fl = (_AST_flow) ast->ast;
	switch (ast->type) {
		case STMT_LABEL:
			if(fl->id != NULL){
				pr_primary_id(out, fl->id, safe);
			} else if(fl->exp != NULL){
				strcat(out, " case ");
				pr_primary_exp(out, fl->exp, safe);
			} else {
				strcat(out, " default ");
			}
			strcat(out, ": ");
			pr_list(out, fl->attrlist, safe);
			pr_stmt(out, fl->stmt, safe);
			break;
		case STMT_SWITCH:
			strcat(out, " switch ");
			strcat(out, "(");
			pr_primary_exp(out, fl->exp, safe);
			strcat(out, ")");
			pr_stmt(out, fl->stmt, safe);
			break;
		case STMT_WHILE:
			strcat(out, " while ");
			strcat(out, "(");
			pr_primary_exp(out, fl->exp, safe);
			strcat(out, ")");
			pr_stmt(out, fl->stmt, safe);
			break;
		case STMT_DO:
			strcat(out, " do ");
			pr_stmt(out, fl->stmt, safe);
			strcat(out, "  while ");
			strcat(out, "(");
			pr_primary_exp(out, fl->exp, safe);
			strcat(out, ")");
			strcat(out, "; ");
			break;
		case STMT_JUMP:
			switch (fl->type){
				case GOTO:
					strcat(out, " goto ");
					break;
				case CONTINUE:
					strcat(out, " continue ");
					break;
				case BREAK:
					strcat(out, " break ");
					break;
				case RETURN:
					strcat(out, " return ");
					break;
				default:
					assert(0);
					return;
			}
			pr_primary_id(out, fl->id, safe);
			pr_primary_exp(out, fl->exp, safe);
			strcat(out, "; ");
			break;
		default:
			assert(0);
			return;
	}
	if(!safe) free(ast->ast);
}

void pr_block(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(ast->type == DECLARATION) pr_declaration(out, ast, safe);
	else pr_stmt(out, ast, safe);
}


void pr_function(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nFUNCTION: entering type: %d\n", ast->type);
	_AST_function fn = (_AST_function) ast->ast;
	pr_list(out, fn->decl_specs, safe);
	pr_declarator(out, fn->declarator, safe);
	pr_list(out, fn->decl_list, safe);
	pr_stmt(out, fn->stmt, safe);
	if(!safe) free(ast);
}

void pr_attribute(char out[], _AST_ ast, _AST_flag safe){
	if(ast == NULL) return;
	if(debug_print) fprintf(stderr, "\nATTRIBUTE: entering type: %d\n", ast->type);
	_AST_attribute at = (_AST_attribute) ast->ast;
	strcat(out, " ");
	pr_primary_string(out, at->attribute, safe);
	strcat(out, " ");
	if(at->comma) strcat(out, ", ");
	if(!safe) free(ast->ast);
	if(!safe) free(ast);
}

void pr_include(char out[], _AST_string ast, _AST_flag safe){
	if(ast == NULL) return;
	pr_primary_string(out, ast, safe);
	strcat(out, "\n");
}

void pr_asm(char out[], _AST_string ast, _AST_flag safe){
	if(ast == NULL) return;
	pr_primary_string(out, ast, safe);
}
