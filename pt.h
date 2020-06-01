
#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"

class Pt {
protected:
	int	linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
    virtual Value Eval(map<string,Value>& symbols)=0;
    
    int plusCount() const {
        int totalPlus=0;
        if(left){
            totalPlus= totalPlus + left->plusCount();
        }
        if(right){
            totalPlus= totalPlus + right->plusCount();
        }
        
        if(isPlus()){
            totalPlus++;
        }
        return totalPlus;
    }
    
    int eqCount() const {
        int totalEQ=0;
        if(left){
            totalEQ= totalEQ + left->eqCount();
        }
        if(right){
            totalEQ= totalEQ + right->eqCount();
        }
        
        if(isEq()){
            totalEQ++;
        }
        return totalEQ;
    }
    
    int maxDepth() const {
        int ldepth=0;
        int rdepth=0;
        
        if(left){
            ldepth= ldepth + left->maxDepth();
        }
        if(right){
            rdepth= rdepth + right->maxDepth();
        }
        
        if(ldepth>rdepth){
            return ldepth+1;
        }
        else{
            return rdepth+1;
        }
    }

};

class StmtList : public Pt {
public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    
    Value Eval(map<string,Value>& symbols) {
        left->Eval(symbols);
        if( right )
            right->Eval(symbols);
        return Value();
    }
};


//printstmt
class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map<string,Value>& symbols) {
        Value val=left->Eval(symbols);
        
        cout << val;
        return Value();
        
    }
};

//println stmt
class Println : public Pt {
public:
    Println(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map<string,Value>& symbols) {
        Value val=left->Eval(symbols);
        
        cout << val << endl;
        return Value();
        
    }
};


//repeat
class Repeat : public Pt {
public:
    Repeat(int line, Pt *e, Pt *st) : Pt(line,e,st) {}
    
    Value Eval(map<string,Value>& symbols) {
        
        if(left->Eval(symbols).IsInt()){
            int repeatNum=left->Eval(symbols).GetInt();
            while(repeatNum>0){
                right->Eval(symbols);
                repeatNum--;
            }
        }
        else{
            throw string(": control expression not an integer");
        }    
        
      	return Value();
    }
};


//EQ
class Assign : public Pt {
public:
    Assign(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
	bool isEq() const { return true; }
    
    Value Eval(map<string,Value>& symbols) {
        Value val=right->Eval(symbols);
        
        if(left->isIdent()){
            string key=left->getId();   
        	symbols[key]=val;
        }
        else{
            throw string(": Left side of equals is not an IDENTIFIER");
        }
        
        return Value();
    }
};

//PLUS
class PlusExpr : public Pt {
public:
    PlusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    bool isPlus() const { return true; }
    
    Value Eval(map<string,Value>& symbols) {
      	Value val1 = left->Eval(symbols);
        Value val2 = right->Eval(symbols);
        
        return val1+val2;
    }
};

//MINUS
class MinusExpr : public Pt {
public:
    MinusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map<string,Value>& symbols) {
      	Value val1 = left->Eval(symbols);
        Value val2 = right->Eval(symbols);
        
        return val1-val2;
    }
};


//STAR
class TimesExpr : public Pt {
public:
    TimesExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map<string,Value>& symbols) {
      	Value val1 = left->Eval(symbols);
        Value val2 = right->Eval(symbols);
        
        return val1*val2;
    }
};


//SLASH
class DivideExpr : public Pt {
public:
    DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map<string,Value>& symbols) {
      	Value val1 = left->Eval(symbols);
        Value val2 = right->Eval(symbols);
        
        return val1/val2;
    }
};


//integers
class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    
    Value Eval(map<string,Value>& symbols) {
        return Value(val);
    }
};


//strings
class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    
    Value Eval(map<string,Value>& symbols) {
        return Value(val);
    }
};

//idents
class Ident : public Pt {
	string identifier;
    
public:
    Ident(Tok& t) : Pt(t.GetLinenum()){
    	identifier=t.GetLexeme();
    }
    bool isIdent() const { return true; }
    string getId() const { return identifier; }
    
    Value Eval(map<string,Value>& symbols) {
       	if ( symbols.find(identifier) == symbols.end() ){
            throw string(": Variable used but not assigned");
        }
        else{
            return symbols[identifier];
        }
    }

};

#endif /* PT_H_ */
