#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int indexLine;
int charClass;
int lexLen;
int token;
int nextToken;
size_t expression_length = 0;
ssize_t read_exression;
char lexeme [100];
char nextChar;
char endCharacter;
char * expression = NULL;
FILE *in_fp, *fopen();

//List of functions
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

//Tokens
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

main(int argc, char* argv[]) { 
  if (argc != 2) {
      printf("NO FILE GIVEN!\n");
      exit(0);
  }
  if ((in_fp = fopen(argv[1], "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
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

int lookup(char ch) {
  switch (ch) {
    case '(':
      addChar();
      nextToken = LEFT_PAREN;
      break;
    case ')':
      addChar();
      nextToken = RIGHT_PAREN;
      break;
    case '+':
      addChar();
      nextToken = ADD_OP;
      break;
    case '-':
      addChar();
      nextToken = SUB_OP;
      break;
    case '*':
      addChar();
      nextToken = MULT_OP;
      break;
    case '/':
      addChar();
      nextToken = DIV_OP;
      break;
    default:
      addChar();
      nextToken = EOF;
      break;
    }
    return nextToken;
}

void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}

void getChar() {
  if (expression[indexLine] != '\n' && expression[indexLine] != '\0') {
      nextChar = expression[indexLine++];
    if (isalpha(nextChar)) {
      charClass = LETTER;
    }
    else if (isdigit(nextChar)) {
      charClass = DIGIT;
    }
    else if (nextChar == '\n')
      charClass = NEWLINE;
    else charClass = UNKNOWN;
  }
  else
  charClass = EOF;
}

void getNonBlank() {
  while (isspace(nextChar))
    getChar();
}

int lex() {
  lexLen = 0;
  getNonBlank();
  endCharacter = nextChar;
  switch (charClass) {
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
      addChar();
      getChar();
      }
      nextToken = INT_LIT;
      break;
    case UNKNOWN:
      lookup(nextChar);
      getChar();
      break;
    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
  }
    printf("Next token is: %d, Next lexeme is %s\n",
    nextToken, lexeme);
    return nextToken;
}

void expr() {
  printf("Enter <expr>\n");
  term();
  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    lex();
    term();
  }
  printf("Exit <expr>\n");
}

void term() {
  printf("Enter <term>\n");
  factor();
  while (nextToken == MULT_OP || nextToken == DIV_OP) {
    lex();
    factor();
  }
  printf("Exit <term>\n");
}

void factor() {
  printf("Enter <factor>\n");
  if (nextToken == IDENT || nextToken == INT_LIT)
    lex();
  else {
    if (nextToken == LEFT_PAREN) {
      lex();
      expr();
      if (nextToken == RIGHT_PAREN)
        lex();
      else
        error();
    } 
    else
      error();
  }
  printf("Exit <factor>\n");;
}

void error() {
  printf("!! Error has occurred and been caught at %c !! The expression is %s ****\n", endCharacter, expression);
}
