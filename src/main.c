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

/* main.c */
#include "config.h"

#define FILE_NEW_SUFFIX ".new.c"

extern int yylineno;
extern int yydebug;
extern int debug_parse;			/* a flag activating debug mode for the parser */
extern int debug_ast;			/* a flag activating debug mode for the AST traversal */
extern int debug_print;			/* a flag activating debug mode for the printer */
int __embedded;					/* a flag disabling all ptining not explicitly activated */

/* concordance main routine */
main(int argc,char **argv)
{
	/* go through the arguments and execute parser */
	int i;
	char files[argc][256];
	int f = 0;
	if(argc < 2) { /* just read stdin */
		curfilename = "(stdin)";
		yylineno = 1;
		yyparse();
	} else {
		for(i = 1; i < argc; i++) {
			if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")){
				/* activate embedded mode,
				 * disables all ouput, not explicitly activated */
				usage(argv[0]);
				exit(0);
			} else if(!strcmp(argv[i],"-e") || !strcmp(argv[i],"--embedded")){
				/* activate embedded mode,
				 * disables all ouput, not explicitly activated */
				__embedded = 1;
			} else if(!strcmp(argv[i],"-t") || !strcmp(argv[i],"--debug")){
				/* activate the parser's debug mode,
				 * prints an step-by-step guide to
				 * parsing as it is performed */
				yydebug = 1;
			} else if(!strcmp(argv[i],"-s") || !strcmp(argv[i],"--parser")){
				/* activate debug mode in the parser
				 * debugs the parser, plus printing
				 * the tokenized version of the files*/
				debug_parse = 1;
			} else if(!strcmp(argv[i],"-a") || !strcmp(argv[i],"--ast")){
				/* activate debug mode in the AST traversal,
				 * prints debug messages in the traversal engine */
				debug_ast = 1;
			} else if(!strcmp(argv[i],"-r") || !strcmp(argv[i],"--printer")){
				/* activate debug mode in the printer,
				 * prints debug messages in the printer engine */
				debug_print = 1;
			} else {
				strcpy(files[f], argv[i]);
				f++;
			}
		}
		char newfile[256];
		int ret = 0;
		for(i = 0; i < f; i++) {
			printf("\nparsing: %d: %s\n", i + 1, files[i]);
			ret = parse_file(files[i], newfile);
			if(ret)	printf("\ndone: %d: %s\n" , i + 1, newfile);
		}
	}
	printf("\n");
	return 0;
}


int parse_file(char *file, char newfile[]){
	FILE *fl = fopen(file, "r");
	if(!fl) {
		perror(file);
		return 0;
	}
	/* get the file's size */
	fseek(fl, 0, SEEK_END); // seek to end of file
	long flsz = ftell(fl);	// get current file pointer
	fseek(fl, 0, SEEK_SET); // seek back to beginning of file
	/* get the new file's name */
//	char newfile[256];
	newfile[0] = '\0';
	int i;
	for(i = strlen(file) - 1; i >= 0; i--){
		if(file[i] == '.'){
			int k;
			for(k = 0; k < i; k++){
				newfile[k] = file[k];
			}
			newfile[i] = '\0';
			break;
		}
	}
	strcat(newfile, FILE_NEW_SUFFIX);

	/* initialize the parsing symbol table */
	symtab = checked_malloc(sizeof(*symtab));
	symtab_init(symtab);
	/* append builtin constructs */
	symtab_new(symtab, "__builtin_va_list", SYMTAB_TYPENAME);

	/* initialize the declaration table */
	dtable = checked_malloc(sizeof(*dtable));
	decltab_init(dtable);
	// set the filename variable
	curfilename = file;
	// make sure that the file curret to the start
	yyrestart(fl);
	// set the line variable
	yylineno = 1;
	// the first line number is not going to be printed
	// cause the line matching expression searches for ^\n
	if(debug_parse) fprintf(stderr, "\n %4d: ",yylineno);
	// recursively parse the whole file
	yyparse();
	// parsing complete, close the file
	fclose(fl);
	// maintenance
	symtab_clear(symtab);
	if(!__embedded) printf("\nanalyzing... \n");
	// move on to semantics analysis
	asttraverse(dtable, ast_root);
	if(!__embedded) printf("\nprinting to %s ... \n",newfile);
	// open the output file
	FILE *n = fopen(newfile, "w");
	// print to ouput
	astprint(n, flsz, dtable, ast_root, 0);
	// close output file
	fclose(n);
	// maintenance
	decltab_clear(dtable);
	return 1;
}

void usage(char *app){
	char *args = "[-hetsar] <file1>[,<file2>[,...]]";
	fprintf(stderr, "Usage: %s %s \nOptions:\n", app, args);
	fprintf(stderr, "\t-e,\t--embedded\tDisable all output.\n");
	fprintf(stderr, "\t-t,\t--debug\t\tEnable general debug output.\n");
	fprintf(stderr, "\t-s,\t--parser\tEnable parser debug output.\n");
	fprintf(stderr, "\t-a,\t--ast\t\tEnable AST debug output.\n");
	fprintf(stderr, "\t-r,\t--printer\tEnable printer debug output.\n");
	fprintf(stderr, "\t-h,\t--help\t\tPrint this message.\n");
}