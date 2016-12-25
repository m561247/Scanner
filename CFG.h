#ifndef CFG_H
#define CFG_H
#include<stack>
#include "Token.h"
#include<vector>
#include<map>
#include <algorithm>
using namespace std;
class CFG
{
	public:
		CFG(vector<Token>,map<string,int>);
		void check_token();
		
		vector<Token> gettoken_list();
	protected:
		vector<Token> token_list;
		stack<char>  state;
		map<string,int> symbol_table;
	private:
		void eraceSpace();
		void connectPointer();
		void check_library();
		void connectOperater();
		string Lower(string);
};

#endif
