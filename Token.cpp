#include "Token.h"
#include <iomanip>
#include <sstream>

Token::Token(){
	reset();
}

void Token::reset(){
	lexeme="";
	category = none;
	mode=defaul;
}

void Token::set_lexeme(string l){
	lexeme=l;
	return;
}

bool Token::isfolat(string myString){
    istringstream iss(myString);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail(); 
}

void Token::assign_type(Mode m){
	
	if( m!= inblock && isfolat(lexeme) )
		category = constant;
	else if(lexeme.length()==1 &&(lexeme[0]=='#' || lexeme[0]==',' || 
			lexeme[0]==';' 	   || lexeme[0]==':' ||	lexeme[0]=='\"'|| lexeme[0]=='\'') )
		category = punctuation;
	else if(lexeme.compare("include") == 0 || lexeme.compare("main")   == 0 ||
			lexeme.compare("if")      == 0 || lexeme.compare("else")   == 0 ||
			lexeme.compare("elseif")  == 0 || lexeme.compare("for")    == 0 ||
			lexeme.compare("while")   == 0 || lexeme.compare("do")     == 0 ||
			lexeme.compare("return")  == 0 || lexeme.compare("switch") == 0 ||
			lexeme.compare("case")    == 0 || lexeme.compare("void") == 0)
		category = reserved;
	else if(lexeme.compare("+")  == 0 || lexeme.compare("-")  == 0 ||
			lexeme.compare("*")  == 0 || lexeme.compare("/")  == 0 ||
			lexeme.compare("++") == 0 || lexeme.compare("--") == 0 ||
			lexeme.compare("=")  == 0 || lexeme.compare("&")  == 0 ||
			lexeme.compare("|")  == 0 || lexeme.compare("%")  == 0 ||
			lexeme.compare("^")  == 0)
		category = operate;
	else if(lexeme.compare("char")  == 0 || lexeme.compare("int") == 0 ||
			lexeme.compare("float") == 0 ){
			category = reserved;
			mode=define_mode;
	}
	else if(lexeme.compare(">")  == 0 || lexeme.compare(">=") == 0 ||
			lexeme.compare("<")  == 0 || lexeme.compare("<=") == 0 ||
			lexeme.compare("!=") == 0 || lexeme.compare("!") == 0 )
			category = comparator;
	else if(lexeme.compare("printf")  == 0 || lexeme.compare("scanf") == 0){
		category = reserved;
		mode=string_mode;
	}
	else if(lexeme.length() == 1 && (lexeme[0] =='(' || lexeme[0] ==')' ||
									 lexeme[0] =='[' || lexeme[0] ==']' ||
									 lexeme[0] =='{' || lexeme[0] =='}')){
		category = bracket;
	} 
	else if(m ==  define_mode) {
		if(lexeme[0]=='*')
		category = pointer;
		else if(lexeme[0]=='&')
		category = address;
		else
		category = identifier;
	}
	else if(m == command_mode){
		category = comment;
	}
	else if(m == string_mode ){
		if(lexeme[0] == '%' || lexeme[0] == '\\')
			category = format;
		else
			category = printed;
	}
	else if( m == letter_block){
		if(lexeme[0] == '\\')
			category = format;
		else
		category = letter;
	}
	else if(lexeme == "newline"){
		category = newline;	
	}
	else 
		category = none;	
} 

void Token::print_token(){
	if(category == format){
		cout << "token " << setw(12) << left << lexeme << " with length  1 belongs to ";
	}
	else if(category == comment){
		cout << "token " << lexeme << " with length -1 belongs to ";
	}
	else{
		cout << "token " << setw(12) << left << lexeme << " with length ";
		cout << setw(2) << right << lexeme.length()  << " belongs to ";
	}
	switch(category){
		case library	:	cout <<  "library name"		;break; 
		case punctuation: 	cout <<  "punctuation"		;break;
		case reserved	:	cout <<  "reserved word"	;break;
		case identifier	:	cout <<  "identifier"		;break;
		case pointer	:	cout <<  "pointer"			;break;
		case address	:	cout <<  "address"			;break;
		case comment	:	cout <<  "comment"			;break;
		case printed	:	cout <<  "printed token"	;break;
		case format		:	cout <<  "format specifier"	;break;
		case bracket	:	cout <<  "bracket" 			;break;
		case none		: 	cout <<  "undefine token"	;break;
		case newline	:	cout <<  "newline"			;break;
		case operate	:	cout <<  "operator"			;break;
		case constant	:	cout <<  "constant"			;break;
		case letter		:	cout <<  "letter"			;break;
		case comparator	:	cout <<  "comparator"		;break;
		default :break;
	}
	cout << endl;
}
