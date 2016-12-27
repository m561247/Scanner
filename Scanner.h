#include<string>
#include<vector>
#include<stack>
#include<iostream>
#include<map>
#include <fstream>
#include "Token.h"
#include "CFG.h"
using namespace std;
class Scanner{
	public:
		Scanner(string filename);
		void setinput(string in);
		void scan();
		enum char_type {none,white_space, letter,slash, digit,symbol, left_brace, right_brace, colon, new_line, tab,operate};
		void print_result();
		ofstream outfile;
	private:
		
		int stall;
		Token::Mode now_mode;
		vector<Token> token_list;
		stack<char> state;
		map<string,int> symbol_table;
		string input;
		char_type get_type(char x);		
};
