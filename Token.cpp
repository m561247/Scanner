#include "Token.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

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

string  Token::Lower(string str)
{
    string ans = str;
    transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
    return ans;
}

void Token::assign_type(Mode m){
	
	if( m!= letter_block && m!= string_mode && isfolat(lexeme) )
		category = constant;
	else if(lexeme.length()==1 &&(lexeme[0]=='#'  || lexeme[0]==',' || 
			lexeme[0]==';' 	   || lexeme[0]==':') && (m != string_mode && m != letter_block) )
		category = punctuation;
	else if( lexeme.length()==1 &&(lexeme[0]=='\"'|| lexeme[0]=='\'') )
		category = punctuation;
	else if((Lower(lexeme).compare("include") == 0 || Lower(lexeme).compare("main")   == 0 ||
			 Lower(lexeme).compare("if")      == 0 || Lower(lexeme).compare("else")   == 0 ||
			 Lower(lexeme).compare("elseif")  == 0 || Lower(lexeme).compare("for")    == 0 ||
			 Lower(lexeme).compare("while")   == 0 || Lower(lexeme).compare("do")     == 0 ||
			 Lower(lexeme).compare("return")  == 0 || Lower(lexeme).compare("switch") == 0 ||
			 Lower(lexeme).compare("case")    == 0 || Lower(lexeme).compare("void")   == 0)&& (m != string_mode && m != letter_block))
		category = reserved;
	else if((lexeme.compare("+")  == 0 || lexeme.compare("-")  == 0 ||
			 lexeme.compare("*")  == 0 || lexeme.compare("/")  == 0 ||
			 lexeme.compare("++") == 0 || lexeme.compare("--") == 0 ||
			 lexeme.compare("=")  == 0 || lexeme.compare("&")  == 0 ||
			 lexeme.compare("|")  == 0 || lexeme.compare("%")  == 0 ||
			lexeme.compare("^")  == 0 )&& (m != string_mode && m != letter_block))
		category = operate;
	else if((Lower(lexeme).compare("char")  == 0   || Lower(lexeme).compare("int") == 0 ||
			 Lower(lexeme).compare("float") == 0 ) && (m != string_mode && m != letter_block)){
			category = reserved;
			mode=define_mode;
	}
	else if((lexeme.compare(">")  == 0 || lexeme.compare(">=") == 0 ||
			 lexeme.compare("<")  == 0 || lexeme.compare("<=") == 0 ||
			 lexeme.compare("!=") == 0 || lexeme.compare("!") == 0 )&& (m != string_mode && m != letter_block))
			category = comparator;
	else if((Lower(lexeme).compare("printf")  == 0 || Lower(lexeme).compare("scanf") == 0) && (m != string_mode && m != letter_block)){
		category = reserved;
		mode=string_mode;
	}
	else if(lexeme.length() == 1 && (lexeme[0] =='(' || lexeme[0] ==')'  ||
									 lexeme[0] =='[' || lexeme[0] ==']'  ||
									 lexeme[0] =='{' || lexeme[0] =='}') && (m != string_mode && m != letter_block)){
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
		if(lexeme == "%d" || lexeme == "%f" || lexeme == "%c" || lexeme[0] == '\\')
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
	else if( m == newline_token ){
		category = newline;	
	}
	else if( m == space_token ){
		category = space;
	}
	else 
		category = none;	
} 

void Token::print_token(ofstream& output){
	if(category == format){
		output << "token " << setw(12) << left << lexeme << " with length  1 belongs to ";
	}
	else if(category == comment){
		output << "token " << lexeme << " with length -1 belongs to ";
	}
	else{
		output << "token " << setw(12) << left << lexeme << " with length ";
		output << setw(2) << right << lexeme.length()  << " belongs to ";
	}
	switch(category){
		case library	:	output <<  "library name"		;break; 
		case punctuation: 	output <<  "punctuation"		;break;
		case reserved	:	output <<  "reserved word"	;break;
		case identifier	:	output <<  "identifier"		;break;
		case pointer	:	output <<  "pointer"			;break;
		case address	:	output <<  "address"			;break;
		case comment	:	output <<  "comment"			;break;
		case printed	:	output <<  "printed token"	;break;
		case format		:	output <<  "format specifier"	;break;
		case bracket	:	output <<  "bracket" 			;break;
		case none		: 	output <<  "undefine token"	;break;
		case newline	:	output <<  "newline"			;break;
		case operate	:	output <<  "operator"			;break;
		case constant	:	output <<  "constant"			;break;
		case letter		:	output <<  "letter"			;break;
		case comparator	:	output <<  "comparator"		;break;
		case space		:	output <<  "space"			;break;
		default :break;
	}
	output << endl;
	return;
}
