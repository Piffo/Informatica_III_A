/*
 ============================================================================
 Name        : Project_C_cyc.c
 Author      : Michele Piffari
 Version     :
 Copyright   : No copyright
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define USHRT_MAX  65535

#define SENDER 0
#define RECEIVER 1
#define DATE 2
#define PLACE 3

#define MAX_NAMES_LENGTH 30
#define MAX_PLACES_LENGTH 30
#define MAX_DATES_LENGTH 22

#define MAX_LINE_LENGTH 95
#define MAX_POSTCARDS_IN_FILE 500

#define FILE_NAME "C:\\Users\\miche\\OneDrive\\Desktop\\I anno Magistrale\\Informatica III - Modulo di programmazione\\WS_INFO_III_A\\Project_C_oneFile\\src\\PostcardsSimplified.txt"
#define BY_SENDER 1
#define BY_REC	  2
#define BY_PLACE  3
#define EXIT 	  -1

void outofmemory() {
	printf("[ERROR] - Out of memory error!\n");
	exit(EXIT_FAILURE);
}

typedef struct postcard {
	char* sender_name;
	char* receiver_name;
	char* place;
} postcard;

// ============== STRING UTILS ==============================
/* Destructively converts the given zero-terminated string to lowercase */
void str2lower(char* str) {
	while (*str) {
		if (*str >= 65 && *str <= 90) { // a-z
			*str += 32;
		}
		str++;
	}
}

/*Divides the given string into tokens, delimited in the original string
* by the given delimiter character.
* Returns an array of tokens (as a parameter) and the number of such
* substrings (as the return value). Delimiters are not part of the tokens;
* tokens may also be empty strings.
* If delimiter is '\0', the whole string is one single token.
*/
unsigned int tokenize(const char* str, char delimiter, char*** tokens) {
	const char* s = str;

	// counts occurrences of the tokens (= delimiters + 1)
	int tokenCount = 1;

	while (*s) {
		if (*s == delimiter)
			tokenCount++;
		s++;
	}

	// allocate space for tokens array
	char** t = malloc(tokenCount * sizeof(char*));
	if (!t)
		outofmemory();

	s = str;
	const char* p = str;
	tokenCount = 0;
	// scan the string, storing the start (p) and end (s) of
	// every token, and copying the token into the array

	while (1) {
		while (*s != delimiter && *s != '\0') {
			s++;
		}

		// s points to the delimiter right after the current token
		int len = s - p;
		t[tokenCount] = malloc((len + 1) * sizeof(char));
		if (!t[tokenCount])
			outofmemory();

		strncpy(t[tokenCount], p, len);
		t[tokenCount][len] = '\0';
		tokenCount++;

		// when the end of the string has been reached, end the loop
		if (*s == '\0')
			break;

		// otherwise, skip the delimiter
		s++;
		p = s; // now p points to the next token's first char
	}

	*tokens = t; // return the tokens array through the parameter
	return tokenCount;
}

//========================================================================

/*
//========================= DATE ==================================
 Given an array of 3 strings, try and parse it as day/month/year.
 * If the conversion is not successful, an invalid date will be returned.
date_ref tokens2date(const char** str) {
	// convert day/month/year, checking that they fit into an unsigned short int
	// on error, return an invalid date (which might not be {0,0,0})
	date_ref d = (date_ref) malloc(sizeof(date));
	d->day = 0;
	d->month = 0;
	d->year = 0;

	// day
	int day = atoi(str[0]);
	if (day == 0)
		return d;

	if (day < 0 || day > USHRT_MAX)
		return d;


	// month
	int month = atoi(str[1]);
	if (month == 0)
		return d;

	if (month < 0 || month > USHRT_MAX)
		return d;


	//year
	int year = atoi(str[2]);
	if(year==0)
		return d;
	if (year < 0 || year > USHRT_MAX)
		return d;

	d->day = day;
	d->month = month;
	d->year = year;

	return d;
}

Try and convert the given string into a date.
 * The string must be in one of the formats:
 * "<i><del><i><del><i>" (where i are suitable integers)
 * "<i><del><month><del><i>" (where i are suitable integers and month
 * 		is a string of the type "jan" or "january")
 * If the string is not a valid date, 0-0-0 is returned

date_ref string_to_date (char* str, char delimiter) {
	date_ref d = (date_ref) malloc(sizeof(date));
	d->day = 0;
	d->month = 0;
	d->year = 0;

	char** tokens = (char**) malloc(sizeof(char*) * (strlen(str) + 1));
	 get the first token

	int count = tokenize(str, delimiter, &tokens);

	if (count == 3) {
		date_ref tmp = (date_ref) malloc(sizeof(date));
		tmp = tokens2date((const char**) tokens);
		d = tmp;
	}

	int i;
	for (i = 0; i < count; i++) {
		free(tokens[i]);
	}
	free(tokens);
	tokens = NULL;

	return d;
}

 Try and parse the given string as a date.
 * Returns 0-0-0 if unsuccessful
date_ref parse_date(char* str) {
	char* tmp = malloc((strlen(str) + 1) * sizeof(char));
	if (!tmp) {
		outofmemory();
	}

	strcpy(tmp, str);
	str2lower(tmp);

	// try and figure out which delimiter was used
	char* p = tmp;
	while (*p != '\0' && *p >= '0' && *p <= '9') {
		p++;
	}

	date_ref d = (date_ref) malloc(sizeof(date));
	d = string_to_date(tmp, *p);
	free(tmp);
	return d;
}
//=================================================================
*/

/*
 * Make postcard.
 * If number of informations it's not enough, return NULL postcard
 */
postcard* make_postcard(char* line) {
	postcard* pc = (postcard*) malloc(sizeof(postcard));

	char** tokens = (char**) malloc(sizeof(char*) * (strlen(line) + 1));
	int numberOfToken = tokenize(line, '|', &tokens);

	// Complete postcard
	char* sender = (char*) malloc(sizeof(char) * (MAX_NAMES_LENGTH + 1));
	char* receiver = (char*) malloc(sizeof(char) * (MAX_NAMES_LENGTH + 1));
	char* place = (char*) malloc(sizeof(char) * (MAX_NAMES_LENGTH + 1));

	if(numberOfToken == 3) {
		for (int i = 0; i < numberOfToken; i++) {
			str2lower(tokens[i]);

			printf("%s\n", tokens[i]);
			switch (i) {
			case SENDER:
				strcpy(sender, tokens[i]);
				break;
			case RECEIVER:
				strcpy(receiver, tokens[i]);
				break;
			case PLACE:
				strcpy(place, tokens[i]);
				break;
			default:
				break;
			}
		}

		pc->sender_name = sender;
		pc->receiver_name = receiver;
		pc->place = place;

		for (int i = 0; i < numberOfToken; i++) {
			free(tokens[i]);
		}
		free(tokens);
		return pc;
	} else {
		return NULL;
	}
}

/* Read the file and create a list of postcard object, only if it's present
 * all the needed informations.
 * Returns the number of postcards (as a parameter) and the reference of such
 * postcard (as the return value).
 */
postcard** readFile(int** num_of_postcard) {
	postcard** postcards = (postcard**) malloc(sizeof(postcard*) * (MAX_POSTCARDS_IN_FILE + 1));
	FILE* stream;
	// Opening stream file for reading
	stream = fopen(FILE_NAME, "r");
	if(stream == NULL) {
		printf("Error opening file %s\n", FILE_NAME);
		return NULL;
	}

	char* line = (char*) malloc(sizeof(char) * (MAX_LINE_LENGTH + 1));
	int* index = (int*) calloc(1, sizeof(int)); // From 1 to (MAX_POSTCARDS_IN_FILE - 1)

	while(fgets(line, (MAX_LINE_LENGTH + 1), stream) != NULL) {
		//puts(line); // Print line

		postcard* pc = make_postcard(line);
		if(pc != NULL) {
			*(postcards + *index) = pc;
			*index = *index + 1;
		} else {
			printf("__ERROR ON POSTCARD ___\n");
		}
	}

	(*(postcards + *index + 1)) = NULL; // Terminator
	*(num_of_postcard) = index;
	fclose(stream);
	free(line);
	return postcards;
}

/*
 * Search postcards and return them.
 * If number of informations it's not enough, return NULL postcard
 */
postcard** findBy(postcard** postcards, char* str_src, int* nop, int findBy) {
	postcard** res = (postcard**) malloc(sizeof(postcard*) * (*nop));
	int nop_searched = 0;
	/*date_ref d = (date_ref) malloc(sizeof(date));
	if(findBy == BY_DATE) {
		d = parse_date(str_src);
		if(d->day == 0 && d->month == 0 && d->year == 0) {
			printf("INVALID DATE SEARCH\n");
			return NULL;
		}
	}*/

	int temp = *nop;
	for(int i = 0; i < temp; i++) {
		if(findBy == BY_SENDER &&
				strcmp((*(postcards+i))->sender_name, str_src) == 0) {

			*(res + nop_searched) = *(postcards + i);
			nop_searched = nop_searched + 1;
		} else if(findBy == BY_REC &&
				strcmp((*(postcards+i))->receiver_name, str_src) == 0) {

			*(res + nop_searched) = *(postcards + i);
			nop_searched = nop_searched + 1;
		} else if(findBy == BY_PLACE &&
				strcmp((*(postcards+i))->place, str_src) == 0) {

			*(res + nop_searched) = *(postcards + i);
			nop_searched = nop_searched + 1;
		} /*else if(findBy == BY_DATE) {

			date_ref dd = (date_ref) malloc(sizeof(date));
			dd = (*(postcards + i))->sending_date;
			if(d ->day == dd->day && d->month == dd->month && d->year == dd->year ) {
				*(res + nop_searched) = *(postcards + i);
				nop_searched = nop_searched + 1;
			}
			free(dd);
		}*/
	}
	//free(d);
	*nop = nop_searched;
	return res;
}

void printPostcard (postcard* p) {
	printf("	----------------\n");
	printf("	from: %s\n", p->sender_name);
	printf("	to: %s\n", p->receiver_name);
	printf("	place: %s\n", p->place);
	printf("	----------------\n");
}

int main( int argc, char *argv[] ){
	// Console reading
	char *command = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);
	char *param = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);
	int* nop = 0; // Number of postcard
	postcard** postcards = (postcard**) malloc(sizeof(postcard*) * (MAX_POSTCARDS_IN_FILE + 1));
	postcards = readFile(&nop);
	printf("Number of postcards = %i\n", *nop);

	postcard** res = (postcard**) malloc(sizeof(postcard*) * (MAX_POSTCARDS_IN_FILE + 1));
	printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]"
			"\n- EXIT [-1]\n");
	while (atoi(command) != EXIT) {

		gets(command);
		if(atoi(command) == BY_SENDER) {
			printf("BY SENDER --> ");
			gets(param);
			int* nop_searched = nop;
			str2lower(param);
			res = findBy(postcards, param, nop_searched, BY_SENDER);
			printf("Postcard search: \n");
			if(*nop_searched == 0)  {
				printf("----> No postcard match <----\n");
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			} else {
				for(int i = 0; i < *nop_searched; i++) {
					printPostcard(*(res + i));
				}
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			}
		} else if(atoi(command) == BY_REC) {
			printf("BY REC --> ");
			gets(param);
			int* nop_searched = nop;
			str2lower(param);
			res = findBy(postcards, param, nop_searched, BY_REC);
			printf("Postcard search: \n");
			if(*nop_searched == 0)  {
				printf("----> No postcard match <----\n");
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			} else {
				for(int i = 0; i < *nop_searched; i++) {
					printPostcard(*(res + i));
				}
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			}
		} else if(atoi(command) == BY_PLACE) {
			printf("BY PLACE --> ");
			gets(param);
			int* nop_searched = nop;
			str2lower(param);
			res = findBy(postcards, param, nop_searched, BY_PLACE);
			printf("Postcard search: \n");
			if(*nop_searched == 0)  {
				printf("----> No postcard match <----\n");
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			} else {
				for(int i = 0; i < *nop_searched; i++) {
					printPostcard(*(res + i));
				}
				printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- EXIT [-1]\n");
			}
		}/* else if(atoi(command) == BY_DATE) {
			printf("BY DATE --> ");

			gets(param);
			int* nop_searched = nop;
			res = findBy(postcards, param, nop_searched, BY_DATE);
			if(res == NULL) {
				printf("NO Postcard found\n");
			} else {
				printf("Postcard search: \n");
				if(*nop_searched == 0)  {
					printf("----> No postcard match <----\n");
					printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- DATE [4]\n- EXIT [-1]\n");
				} else {
					for(int i = 0; i < *nop_searched; i++) {
						printPostcard(*(res + i));
					}
					printf("FIND POSTCARD BY: \n- SENDER [1]\n- RECEIVER [2]\n- PLACE [3]\n- DATE [4]\n- EXIT [-1]\n");
				}
			}
		}*/ else if(atoi(command) == EXIT) {
			printf("EXIT");
		}
	}

	free(command);
	return EXIT_SUCCESS;
}
