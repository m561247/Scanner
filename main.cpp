#include<iostream>
#include<vector>
#include<stack>
#include<fstream>
#include<string>
#include<stdexcept>
#include "Scanner.h" 
using namespace std;
int main(void){
	
	string filename;
	ifstream source_file;
	cin >> filename;
	try{
		source_file.exceptions ( ifstream::goodbit );
		source_file.open(filename.c_str());
	}catch(ifstream::failure& e){
		cerr << "Exception opening/reading file" << endl;
		exit(0);
	}
	
	string whole_file;
	string file_line;

	while(getline(source_file, file_line))
	{
	 	whole_file = whole_file + file_line;
	 	whole_file += "\n";	
	}
   
   Scanner s;
   cout << "pass1\n";
   s.setinput(whole_file);
   s.scan();
   s.print_result();
	
	return 0;
	
} 
