#ifndef TOKEN_H
#define TOKEN_H
#include<string>
#include<iostream>
using namespace std;

class Token
{
	public:
		enum token_type {none,reserved,library,comment,identifier,constant,newline,letter
						,operate,comparator,bracket,format,pointer,address,punctuation,printed};
		enum Mode{defaul,string_mode,define_mode,command_mode,print_id,undefine,inblock,letter_block};
		token_type category;
		Mode mode;
		string lexeme;
		Token();
		void set_lexeme(string);
		void assign_type(Mode m);
		void print_token();
		void reset();
		bool isfolat(string);
	private:
		
};

#endif
