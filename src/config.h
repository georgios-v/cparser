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

#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define YYDEBUG 1

#define LINE_BUFFER_SIZE 999

#define CONSTVAL_INT 401
#define CONSTVAL_FLO 402
#define CONSTVAL_STR 403

#define EXP_IDENTIFIER 501
#define EXP_CONSTVAL_INT 502
#define EXP_CONSTVAL_FLO 503
#define EXP_CONSTVAL_HEX 504
#define EXP_STRING 505
#define EXP_BRACKETED 506

#define EXP_SUBSCRIPT 507
#define EXP_ICALL 508
#define EXP_STRUCTMBR 509
#define EXP_PTRMBR 510
#define EXP_INC 511
#define EXP_CAST 512

#define EXP_ARGS 513

#define EXP_UNARY 514

#define EXP_BINARY 516

#define EXP_COND 517

#define EXP_LIST 519

#define DECLARATION 531
#define DECLARATION_LIST 532
#define DECLARATION_PARAMETER 533

#define DECLARATION_STRUCT 541
#define DECLARATION_STRUCT_LIST 542

#define SPECQUAL_LIST 551

#define STRUCT_SPECIFIER 561
#define ENUM_SPECIFIER 562

#define DECLARATOR_STRUCT 571
#define DECLARATOR_STRUCT_LIST 572

#define ENUMERATOR 581
#define ENUMERATOR_LIST 582


#define DECLARATION_SPECIFIER_LIST 591
#define STORECLASS_SPECIFIER 592
#define TYPE_SPECIFIER_INT 593
#define TYPE_SPECIFIER_STRING 594
#define TYPE_SPECIFIER_STRUCT 595
#define TYPE_SPECIFIER_ENUM 596
#define FUNCTION_SPECIFIER 597

#define TYPE_QUALIFIER_LIST 601
#define TYPE_QUALIFIER 602

#define DECLARATOR_INIT_LIST 611
#define DECLARATOR_INIT 612
#define DECLARATOR_ATTRIBUTED 613
#define DECLARATOR 614
#define DECLARATOR_DIRECT_1 615
#define DECLARATOR_DIRECT_2 616
#define DECLARATOR_DIRECT_3 617
#define DECLARATOR_DIRECT_4 618
#define DECLARATOR_DIRECT_5 619
#define DECLARATOR_DIRECT_6 620
#define DECLARATOR_DIRECT_7 621
#define DECLARATOR_DIRECT_8 622
#define DECLARATOR_ABSTRACT 623
#define DECLARATOR_ABSTRACT_DIRECT_1 624
#define DECLARATOR_ABSTRACT_DIRECT_2 625
#define DECLARATOR_ABSTRACT_DIRECT_3 626
#define DECLARATOR_ABSTRACT_DIRECT_4 627

#define POINTER 629

#define PARAMETER_TYPE_LIST 631
#define PARAMETER_LIST 632
#define IDENTIFIER_LIST 633

#define TYPE_NAME_ 641

#define INITIALIZER 651
#define INITIALIZER_LIST 652

#define DESIGNATION 661
#define DESIGNATOR 662
#define DESIGNATOR_LIST 663

#define STMT_LABEL 671
#define STMT_COMPOUND 672
#define STMT_EXP 673
#define STMT_IF 674
#define STMT_SWITCH 675
#define STMT_WHILE 676
#define STMT_DO 677
#define STMT_FOR 678
#define STMT_JUMP 679
#define STMT_ASM 680

#define BLOCK_ITEM_LIST 690

#define TRANSLATION_UNIT 701
#define DECLARATION_EXTERNAL 702
#define FUNCTION_DEFITION 703

#define INC_DIRECTIVE 749

#define ATTRIBUTE_SPEC 751
#define ATTRIBUTE_SPEC_LIST 752

#define WOOL_TASK 770
#define WOOL_FOR 771
#define WOOL_FOR_INIT_ASSN 772
#define WOOL_FOR_INIT_DECL 773
#define WOOL_FOR_TEST 774
#define WOOL_FOR_STEP 775
#define WOOL_FOR_STEP_01 776
#define WOOL_FOR_STEP_02 777
#define WOOL_FOR_STEP_03 778
#define WOOL_SPAWN 779
#define WOOL_CALL 780
#define WOOL_RCALL 781
#define WOOL_SYNC 782

#define WOOL_TASK_PROTO 790

#define SEQ_AN 801
#define SEQ_AA 802
#define SEQ_SA 803

#define W_FOR_INIT_ASSGN 1101
#define W_FOR_INIT_DECLR 1102

#define _W_EXP 1111

#define DC_TYPE 801
#define DC_VAR 802
#define DC_FUNC 803
#define DC_ARRAY 804
#define DC_STRUCT 805
#define DC_UNION 806
#define DC_ENUM 807
#define DC_TASK 808
#define DC_TASK_PROTO 809
#define DC_WOOL_FOR 810


#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define KEY_SPAWN "SPAWN"
#define KEY_CALL "CALL"
#define KEY_RCALL "ROOT_CALL"
#define KEY_SYNC "SYNC"
#define KEY_TASK "TASK_"
#define KEY_VTASK "VOID_TASK_"
#define KEY_FOR "FOR"
#define KEY_RFOR "ROOT_FOR"
#define KEY_FOR_STEP "FOR_STEP"
#define KEY_LOOP "LOOP_BODY_"

#define KEY_PROTO_SPAWN "SPAWN_"
#define KEY_PROTO_CALL "CALL_"
#define KEY_PROTO_SYNC "SYNC_"
#define KEY_PROTO_DQTOP "Task *__dq_top"

#define WFOR_NAME "wool_for_"
#define WFOR_COST "LARGE_BODY"

#include "util.h"
#include "ast_f.h"
#include "tokens.h"
#include "stack.h"
#include "table.h"
#include "scopestack.h"
#include "symboltable.h"
#include "decltable.h"
#include "ast.h"
#include "asttraverse.h"
#include "astprint.h"


char *curfilename; /* name of current input file */

ST_symboltable symtab;
DC_table dtable;
_AST_root ast_root;

int yyparse(); /* prototype of bison-generated parser function */
int parse_file(char *, char []); /* parser wrapper function */
void usage(char *app);	/* print usage information */
void unmatched(void); /* default action for all characters not matched by lexical specification */

int ret_token(int); /* action that returns a token and sets up the subtok member of yylval */
int ret_symbol(); /* action to check and return appropriately on the IDENTIFIER issue */
int setconstval(int); /* action to setup the constval structure appropriately */
int eval_identifier(void); /* action to distinquish identifiers and type_names */
int eval_identifier_typedef(void); /* action to distinquish identifiers and type_names during a typedef */

void scope_enter(int); /* Wrapper to initialize a new scope but keep the same state */
void scope_leave(); /* Wrapper to release current scope and start the previous state */

void symboladd(char *, int); /* Wrapper procedure to add a symbol to the symboltable */

void struct_enter();	/* Wrapper procedure to set the start of a structure declaration list*/
void struct_leave();	/* Wrapper procedure to set the end of a structure declaration list */

void yyerror(char *); /* ouput a formated error message in case of error */

#endif
