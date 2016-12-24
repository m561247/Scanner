#include "CFG.h"

CFG::CFG(vector<Token> T,map<string,int> s)
{
	token_list = T;
	symbol_table = s;
}

vector<Token> CFG::gettoken_list(){
	return token_list;
}

void CFG::check_token(){
	check_library();
	connectPointer();
}

void CFG::connectPointer(){
	for(int i=1 ; i<token_list.size() ; i++){
		if(token_list[i].category == Token::none){
			if(token_list[i-1].lexeme == "*" && token_list[i-1].category == Token::operate){
				string temp = "*"+ token_list[i].lexeme;
				if(symbol_table.find(temp) != symbol_table.end() ){
					token_list[i-1].lexeme = temp;
					token_list[i-1].category = Token::pointer;
					token_list.erase(token_list.begin()+i);
				}
			}
			else if(token_list[i-1].lexeme == "&" && token_list[i-1].category == Token::operate){
				string temp = "&"+ token_list[i].lexeme;
				if(symbol_table.find(temp) != symbol_table.end() ){
					token_list[i-1].lexeme = temp;
					token_list[i-1].category = Token::address;
					token_list.erase(token_list.begin()+i);
				}
			}
		}
	}
}

void CFG::check_library(){
	for(int i=0 ; i<token_list.size()-4 ; i++){
		if(token_list[i].lexeme == "#"         && token_list[i].category   == Token::punctuation && 
		   token_list[i+1].lexeme == "include" && token_list[i+1].category == Token::reserved	 &&
		   token_list[i+2].lexeme == "<"	   && token_list[i+2].category == Token::comparator){
		   	state.push('<');
		   	int j=i+2;
			for(; token_list[j].category != Token::newline ;j++){
				if(token_list[j].lexeme == ">" && token_list[j].category == Token::comparator){
					state.pop();
					break;
				}
			}
			if(state.empty()){
				string temp;
				for(int merge=i+2; merge<=j ;merge++){
					temp = temp+token_list[merge].lexeme;
				}
				token_list.erase (token_list.begin()+i+3,token_list.begin()+j+1);
				token_list[i+2].lexeme=temp;
				token_list[i+2].category = Token::library;
			}
			else
				token_list[j-1].category = Token::none;
		}
	}
}
