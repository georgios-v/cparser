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

void *checked_malloc(int len) {
	void *p = malloc(len);
	if (!p) {
		fprintf(stderr, "\nRan out of memory!\n");
		exit(1);
	}
	return p;
}

/* hash a symbol */
unsigned int hash(char *str) {
	if(str == NULL) return 0;
	unsigned int hsh = 0;
	char *c;
	for(c = str; *c; c++){
		hsh = hsh*65599 + *c;
	}
	return hsh;
}

/* error printing helper */
void asterror(char *msg, int pos, char *fl){
	char out[1024];
	out[0] = '\0';
	strcat(out, fl);
	strcat(out, ":");
	sprintf(out, "%s%d", out, pos);
//	strcpy(out, pos);
	strcat(out, ": ");
	strcat(out, msg);
	fprintf(stderr, "%s\n", out);
}


/*
**  TRIM.C - Remove leading, trailing, & excess embedded spaces
**
**  public domain by Bob Stout
*/

#include <ctype.h>
#include <string.h>

#define NUL '\0'

char *trim(char *str)
{
      char *ibuf = str, *obuf = str;
      int i = 0, cnt = 0;

      /*
      **  Trap NULL
      */

      if (str)
      {
            /*
            **  Remove leading spaces (from RMLEAD.C)
            */

            for (ibuf = str; *ibuf && isspace(*ibuf); ++ibuf)
                  ;
            if (str != ibuf)
                  memmove(str, ibuf, ibuf - str);

            /*
            **  Collapse embedded spaces (from LV1WS.C)
            */

            while (*ibuf)
            {
                  if (isspace(*ibuf) && cnt)
                        ibuf++;
                  else
                  {
                        if (!isspace(*ibuf))
                              cnt = 0;
                        else
                        {
                              *ibuf = ' ';
                              cnt = 1;
                        }
                        obuf[i++] = *ibuf++;
                  }
            }
            obuf[i] = NUL;

            /*
            **  Remove trailing spaces (from RMTRAIL.C)
            */

            while (--i >= 0)
            {
                  if (!isspace(obuf[i]))
                        break;
            }
            obuf[++i] = NUL;
      }
      return str;
}










/*

void trim_copy(char *input, char *output)
{
	char *end = output;
	char c;

	// skip spaces at start
	while(*input && isspace(*input))
	++input;

	// copy the rest while remembering the last non-whitespace
	while(*input)
	{
		// copy character
		c = *(output++) = *(input++);

		// if its not a whitespace, this *could* be the last character
		if( !isspace(c) )
			end = output;
	}

	// white the terminating zero after last non-whitespace
	*end = 0;
}

void trim_inplace(char *s)
{
	trim_copy(s, s);
}

char *trim_nocopy(char *s)
{
	char *start = s;

	// skip spaces at start
	while(*start && isspace(*start))
		++start;

	char *i = start;
	// iterate over the rest remebering last non-whitespace
	while(*i)
	{
		if( !isspace(*(i++)) )
			end = i;
	}

	// white the terminating zero after last non-whitespace
	*end = 0;

	return start;
}
*/
