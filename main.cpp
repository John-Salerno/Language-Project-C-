#include <iostream>
#include <sstream>
#include <fstream>
#include "lex.h"
#include "parse.h"

using namespace std;


int main(int argc, char* argv[]) {

    istream* in = &cin;
    ifstream infile;
    string filename = "codefortesting.txt";

   infile.open(filename);
   if (infile.is_open()) {
        in = &infile;
    }

	int linenum=0;
    Pt* parseTree=Prog(*in,linenum);
    
 
    map<string, Value> symbols;
    if( parseTree != 0 ) {	
    	try{
        	parseTree->Eval(symbols);
    	}
    
    	catch(std::string& e ) {
        	cout <<"RUNTIME ERROR at "<< linenum << e << endl;
    	}
    }
    
    if (infile.is_open()){
        infile.close();
    }
    
 }
