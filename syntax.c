#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int indexLine;
//
int lexLen;
int charClass;
//
int token;
int nextToken;
size_t expression_length = 0;
ssize_t read_exression;
char lexeme [100];
char nextChar;
char endCharacter;
char * expression = NULL;
FILE *in_fp, *fopen();

//Functions
void addChar();
void getChar();
void getNonBlank();
int lex();
void getLine();
void addLine();
void expr();
void term();
void factor();
void error();

//Characters
#define LETTER 0
#define DIGIT 1
#define NEWLINE 2
#define UNKNOWN 99

//Token Codes
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

main(int argco, char* argv[]) { 
  if (argc != 2) {
      printf("ERROR! NO FILE GIVEN!\n");
      exit(0);
  }
  if ((in_fp = fopen(argv[1], "r")) == NULL)
    printf("ERROR - Cannot open front.in \n");
  else {
    while ((read_exression = getline(&expression, &expression_length, in_fp)) != EOF) {
      indexLine = 0;
      getChar();
      if (expression != NULL) {
        do {
          lex();
          expr();
        } while (nextToken != EOF);
      }
    }
  }
}