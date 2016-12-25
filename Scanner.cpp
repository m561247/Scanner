#include "Scanner.h"
#include <iomanip>
	void Scanner::setinput(string in){
		input = in;
		return;
	}
	
	void Scanner::scan(){
		cout << input;
		string lexeme="";
		
		int i=0;
		char next_char;
		char_type next_type;
		Token next_token;
		stall=0;
		now_mode=Token::defaul;
		while(i < input.length()){
			next_char = input[i];
			next_type = get_type(next_char);
			if(next_type == white_space || next_type == tab){
				//if(now_mode == Token::string_mode){
					next_token.set_lexeme(lexeme);
					next_token.assign_type(Token::space_token);
					token_list.push_back(next_token);
					next_token.reset();
					lexeme = "";
				//}
				i++;
			}
		    else if(!state.empty() && state.top() =='\"' && next_char!='\\' && next_char!='\"' && next_type!=new_line && now_mode!=Token::string_mode){
				while( next_type != white_space  && next_type != tab && next_char!='\\' && next_type!=new_line && next_char!='\"'){
					lexeme = lexeme + next_char;
					i++;
					next_char = input[i];
					next_type = get_type(next_char);
				}
				next_token.set_lexeme(lexeme);
				next_token.assign_type(now_mode);
				token_list.push_back(next_token);
				next_token.reset();
				lexeme = "";
			}
			else if(!state.empty() && state.top() =='\"' && next_char!='\\' && next_char!='\"' && next_char!='%' && next_type!=new_line && now_mode==Token::string_mode){
				while( next_type != white_space  && next_type != tab && next_char!='\\' && next_char!='%' && next_type!=new_line && next_char!='\"'){
					lexeme = lexeme + next_char;
					i++;
					next_char = input[i];
					next_type = get_type(next_char);
				}
				next_token.set_lexeme(lexeme);
				next_token.assign_type(now_mode);
				token_list.push_back(next_token);
				next_token.reset();
				lexeme = "";
			}
			else if( next_type == digit){
				while( next_type == digit  || next_char =='.'){
					lexeme = lexeme + next_char;
					i++;
					next_char = input[i];
					next_type = get_type(next_char);
				}
				next_token.set_lexeme(lexeme);
				next_token.assign_type(now_mode);
				token_list.push_back(next_token);
				next_token.reset();
				lexeme = "";
			}
			else if( next_type == letter){
				while( next_type == letter || next_type == digit ){
					lexeme = lexeme + next_char;
					i++;
					next_char = input[i];
					next_type = get_type(next_char);
				}
				if( now_mode == Token::define_mode ){
					if(lexeme[0] != '&' || lexeme[0] !='*')
						symbol_table.insert(pair<string,int>(lexeme,1));
					next_token.set_lexeme(lexeme);
					next_token.assign_type(Token::define_mode); 
				}
				else if( !symbol_table.empty() && symbol_table.find(lexeme) != symbol_table.end() ){
						next_token.set_lexeme(lexeme);
						next_token.assign_type(Token::define_mode);
				}
				else{
					next_token.set_lexeme(lexeme);
					next_token.assign_type(now_mode);
				}
				token_list.push_back(next_token);
				next_token.reset();
				lexeme = "";
			}
			else if( next_type == symbol ){
				if(next_char == '\"'){
					if(!state.empty() && state.top()=='\"')
					{
						state.pop();
						now_mode = Token::defaul;
					}
					else
					{
						state.push(next_char);
						if(now_mode != Token::string_mode)
							now_mode = Token::letter_block;
					}
				}
				else if(next_char =='\\' && !state.empty() && state.top() == '\"' ){
					lexeme = lexeme + next_char;
					i++;
					next_char = input[i];
					next_type = get_type(next_char);
				}
			   	lexeme = lexeme + next_char;
				next_token.set_lexeme(lexeme);
				next_token.assign_type(now_mode);
			  	token_list.push_back(next_token);
			  	next_token.reset();
			  	lexeme = "";
		  		i++;
      		}
      		else if( next_type == operate ){
      			if( (next_char == '*' || next_char == '&' ) && now_mode == Token::define_mode){
      				lexeme = lexeme + next_char;
      				i++;
      				continue;
				}
				else if( next_char == '%' && now_mode == Token::string_mode && state.top() == '\"'){
					lexeme = lexeme + next_char;
					i++;
					if((input[i] == 'd' ||  input[i] == 'f' || input[i] == 'c')){
						next_char = input[i];
						lexeme = lexeme + next_char;
					}
					else
						continue;
				}
				else if( next_char == '/' && (input[i+1] =='*' || input[i+1] =='/')){
					now_mode = Token::command_mode;
					while( next_type != new_line){
						lexeme = lexeme + next_char;
						i++;
						next_char = input[i];
						next_type = get_type(next_char);
					}
					i--;
				}
				else
					lexeme = lexeme + next_char;
				next_token.set_lexeme(lexeme);
				next_token.assign_type(now_mode);
			 	token_list.push_back(next_token);
			 	next_token.reset();
			 	lexeme = "";
				i++;
			}
      		else if(next_type == left_brace){
      			state.push(next_char);
			   	lexeme = lexeme + next_char;
			   	next_token.set_lexeme(lexeme);
			   	next_token.assign_type(now_mode);
			  	token_list.push_back(next_token);
			  	next_token.reset();
			  	lexeme = "";
		   		i++;
			}
			else if(next_type == right_brace){
				/*if(next_char == '}'){
					if(state.top() == '{')
						state.pop();
					else
						state.push('}');
				}
				else if(next_char == ']'){
					if(state.top() == '[')
						state.pop();
					else
						state.push('[');
				}
				else if(next_char == ')'){
					if(state.top() == '(')
						state.pop();
					else
						state.push(')');
				}*/ 
			   	lexeme = lexeme + next_char;
			   	next_token.set_lexeme(lexeme);
			   	next_token.assign_type(now_mode);
			  	token_list.push_back(next_token);
			  	next_token.reset();
			  	lexeme = "";
		   		i++;
			}
			else
			{
				lexeme = lexeme + "newline";
				next_token.set_lexeme(lexeme);
			   	next_token.assign_type(Token::newline_token);
			  	token_list.push_back(next_token);
			  	next_token.reset();
			  	lexeme = "";
			  	state = stack<char>();
		   		i++;
		   		stall=0;
		   		now_mode = Token::defaul;
			}
			if(!token_list.empty() && token_list[token_list.size()-1].mode != Token::defaul){
				now_mode=token_list[token_list.size()-1].mode;
			}
			//token_list[token_list.size()-1].print_token();
		}
		
		CFG c = CFG( token_list , symbol_table);
		c.check_token();
		token_list = c.gettoken_list();
		int count =0,sum=0;
		for(int j = 0; j < token_list.size() ;j++)
		{
				if(token_list[j].category == Token::none){
					cout  << "Line " << setw(4) << right << count+1 << "Error->" ;
					token_list[j].print_token();
					count++;
				}
				else{
					cout << "Line " << setw(4) << right << count+1   << ":";
					token_list[j].print_token();
					count++;
					sum++;
				}
		}
		
		cout << "\n\nTotal: " << count << " tokens"<< endl;
		return;
	}
	
	Scanner::char_type Scanner::get_type(char x){
		if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x=='.' || x=='_')) return letter;
		else if(x >= '0' && x <= '9') return digit;
		else if(x == ' ') return white_space;
		else if(x == '\n') return new_line;
		else if(x == '\t') return tab;
		else if(x == '\''|| x == '\"'|| x == '\\'|| x == '#'|| x == '|' || x == ',' || x == ';' || x == ':') {return symbol;}
		else if(x == '/' || x == '=' || x == '^' || x == '+' || x == '-' ||x == '*' || x == '&' || x == '%' || x == '!' || x == '>' || x == '<'){return operate;}
		else if(x == '{' || x == '(' || x == '[') return left_brace;
		else if(x == '}' || x == ')' || x == ']') return right_brace;
		else return none;
	}
