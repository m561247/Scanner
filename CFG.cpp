#include "CFG.h"

CFG::CFG(vector<Token> T,map<string,int> s)
{
	token_list = T;
	symbol_table = s;
}

vector<Token> CFG::gettoken_list(){
	return token_list;
}

string  CFG::Lower(string str)
{
    string ans = str;
    transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
    return ans;
}

void CFG::check_token(){
	eraceSpace();
	check_library();
	connectPointer();
	connectOperater();
	determine_const();
	final_erace(); 
}

void CFG::eraceSpace(){
	for(int i=0 ; i<token_list.size()-1 ; i++){
		if( token_list[i].category == Token::space ){
			int j=i+1;
			for( ; token_list[j].category == Token::space ;j++){
				token_list.erase(token_list.begin()+j);
				j--;
			}
			i=j-1;
		}
	}
	// 1.++ --  2.!= <= >=  3.== 4. - 1
	for(int i=1 ; i<token_list.size()-1 ; i++){
		if( token_list[i].category == Token::space && !( (token_list[i-1].category == Token::comparator && token_list[i+1].category == Token::comparator) || 
														 (token_list[i-1].category == Token::operate    && token_list[i+1].category == Token::operate)    ||
														 (token_list[i-1].category == Token::comparator && token_list[i+1].category == Token::operate)    ||
														 (token_list[i-1].category == Token::operate    && token_list[i+1].category == Token::constant))  ){
			token_list.erase(token_list.begin()+i);			
		}
	}
}

void CFG::connectPointer(){
	for(int i=1 ; i<token_list.size() ; i++){
		if(token_list[i].category == Token::none ||  token_list[i].category == Token::identifier){
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
		if(token_list[i].lexeme == "#"     		      && token_list[i].category   == Token::punctuation  && 
		   Lower(token_list[i+1].lexeme) == "include" && token_list[i+1].category == Token::reserved	 &&
		   token_list[i+2].lexeme == "<"	   		  && token_list[i+2].category == Token::comparator){
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
		}
	}
}

void CFG::connectOperater(){
	for(int i=0 ; i < token_list.size();i++){
		if((token_list[i].lexeme   == ">" && token_list[i].category   == Token::comparator &&
		    token_list[i+1].lexeme == "=" && token_list[i].category   == Token::operate)   ||
		   (token_list[i].lexeme   == "<" && token_list[i].category   == Token::comparator &&
		    token_list[i+1].lexeme == "=" && token_list[i].category   == Token::operate)   ||
		   (token_list[i].lexeme   == "!" && token_list[i].category   == Token::comparator &&
		    token_list[i+1].lexeme == "=" && token_list[i].category   == Token::operate)   ||
		   (token_list[i].lexeme   == "=" && token_list[i].category   == Token::operate    &&
		    token_list[i+1].lexeme == "=" && token_list[i].category   == Token::operate)   ){
			token_list[i].lexeme += token_list[i+1].lexeme;
			token_list[i].category = Token::comparator;
			token_list.erase(token_list.begin()+i+1);
		}
		else if(((token_list[i].lexeme     == "+" && token_list[i].category   == Token::operate    &&
			      token_list[i+1].lexeme   == "+" && token_list[i].category   == Token::operate)   ||
			     (token_list[i].lexeme     == "-" && token_list[i].category   == Token::operate    &&
			      token_list[i+1].lexeme   == "-" && token_list[i].category   == Token::operate    &&
				  token_list[i+2].category != Token::constant							       ))  &&
			     (token_list[i-1].category   == Token::identifier || token_list[i-1].category   == Token::address || token_list[i-1].category   == Token::pointer )) 
				{
			token_list[i].lexeme += token_list[i+1].lexeme;
			token_list[i].category = Token::operate;
			token_list.erase(token_list.begin()+i+1);
		}
	}
}

void CFG::determine_const(){
	for(int i=1 ; i<token_list.size()-1 ; i++){
		if( token_list[i].category == Token::space && !( (token_list[i-1].category == Token::operate  &&  token_list[i-1].lexeme == "-"  && token_list[i+1].category == Token::constant)) ){
			token_list.erase(token_list.begin()+i);			
		}
	}
	for(int i=0 ; i<token_list.size()-1 ; i++){
		if(token_list[i  ].lexeme   == "-" && token_list[i  ].category   == Token::operate   &&
		   token_list[i-1].lexeme   == "(" && token_list[i-1].category   == Token::bracket   &&
		   token_list[i+2].lexeme   == ")" && token_list[i+2].category   == Token::bracket   &&
		   token_list[i+1].category   == Token::constant)
		{
			token_list[i-1].lexeme += token_list[i].lexeme + token_list[i+1].lexeme + token_list[i+2].lexeme;
			token_list[i-1].category = Token::constant;
			token_list.erase(token_list.begin()+i,token_list.begin()+i+3);
		}
	}
	
	for(int i=0 ; i<token_list.size()-1 ; i++){
		if(token_list[i  ].lexeme   == "-" && token_list[i  ].category   == Token::operate   &&
		   token_list[i+1].category == Token::constant && token_list[i-1].category != Token::identifier  &&
		   token_list[i-1].category != Token::address  && token_list[i-1].category != Token::pointer	 &&
		   token_list[i-1].category != Token::constant)
		{
			token_list[i].lexeme += token_list[i+1].lexeme;
			token_list[i].category = Token::constant;
			token_list.erase(token_list.begin()+i+1);
		}
	}
}

void CFG::final_erace(){
	for(int i=0 ; i<token_list.size()-1 ; i++){
		if(token_list[i].category   == Token::space) {
			token_list.erase(token_list.begin()+i);
			i--; 
		}
	}
}
