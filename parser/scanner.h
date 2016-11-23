/* scanner .h*/
#ifndef __SCANNER__H
#define __SCANNER__H

#include<string>
#include"fd.h"
#include<queue>
using namespace std;
typedef enum
{
	/* Literals */
	lx_identifier, lx_integer, lx_string,lx_float,
	/* Keywords */
	kw_program,
	kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
	kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
	kw_while, kw_do, kw_od,
	kw_and, kw_or, 
	kw_read, kw_write,
	kw_for, kw_from, kw_to, kw_by,
	kw_function, kw_procedure,kw_return, kw_not, kw_begin, kw_end,
	/* Operators */
	lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
	lx_colon, lx_dot, lx_semicolon, lx_comma,lx_eq,
	lx_plus, lx_minus, lx_star, lx_slash,
	 lx_neq, lx_lt,lx_gt, lx_le,  lx_ge,lx_colon_eq,lx_eof,lx_not
}
LEXEME_TYPE;

//Token types enumeration

// Definition of TOKEN, you can use a clase if you  want 
class	TOKEN
{
public:
	LEXEME_TYPE type;
	int value;  // can be used instead of the str_ptr for IDs and strings
	float float_value;
	string str;
	//char *str_ptr; // points to strings or Identifiers, can use value
	              //instead but with type casting
};

class SCANNER{
   private:

   public:
   FileDescriptor *file;
	   char c;
	   TOKEN * currt;
   bool isSpace(char c);
   bool islegalIdStart(char c);
   bool isNumber(char c);
   bool isIdDelimiter(char c);
   bool isLegalIdChar(char c);
   TOKEN * getId(FileDescriptor &f);
   string getInteger(FileDescriptor &f);
   TOKEN * getNumber(FileDescriptor &f);
   TOKEN * getString(FileDescriptor &f);
   bool isOperator(char c);
   bool isNextEqualSign(FileDescriptor &f);
   void toKeyWord(TOKEN * t);
   void eatComment(FileDescriptor & f);
   TOKEN * Token();
   TOKEN * nextToken();
	SCANNER(string filename);
	//SCANNER (FileDescriptor *fd);
	TOKEN*  Scan(); 
    	//TOKEN *Scan(FileDescriptor *fd);

};

// It is helpful to define the following arrays in your .cpp file
// not in this header file

/* The keyword list is and must be kept in alphabetical order. To
add a keyword, insert it at the appropriate place in keyword[],
insert its type in the appropriate place in key type[], and
increment the var keys. Order is needed for binary search
*/

#endif

