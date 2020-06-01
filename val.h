#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType	T;
	int		I;
    string	S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    
    
    int GetInt() const { if( IsInt() ) return I; throw string(": Value not an integer"); }
    string GetStr() const { if( IsStr() ) return S; throw string(": Value not a string"); }
        
    
    Value operator+(const Value& op) const {
        if( IsInt() && op.IsInt() )
            return GetInt() + op.GetInt();
        if( IsStr() && op.IsStr() )
            return GetStr() + op.GetStr();
        throw string(": Type mismatch during addition");
    }
    
   
    Value operator-(const Value& op) const {
        if( IsInt() && op.IsInt() )
            return GetInt() - op.GetInt();
        if( IsStr() && op.IsStr() ){
            
            string theString=GetStr();
            size_t pos = theString.find(op.GetStr());
            if(pos != std::string::npos){
                theString.erase(pos, op.GetStr().length());
            }
            return theString;
        }	
            
        throw string(": Type mismatch during subtraction");
    }
    
    
    Value operator*(const Value& op) const {
        if( IsInt() && op.IsInt() ){
            return GetInt() * op.GetInt();
        }
        
        string returnString;
        if( IsInt() && op.IsStr() ){
            if( GetInt() >= 0 ){
                for(int i = 0 ; i < GetInt() ; i++){
                    returnString = returnString + op.GetStr();
                }
            	return returnString;
            }
            else{
                throw string(": Cannot multiply string by negative number");
            }
        }
        
        if( IsStr() && op.IsInt() ){
            if( op.GetInt() >= 0 ){
            	for(int i = 0 ; i < op.GetInt() ; i++){
                    returnString = returnString + GetStr();
                }
            	return returnString;
            }
        	else{
                throw string(": Cannot multiply string by negative number");
            }
        }
        
        throw string(": Type error on multiplication");
    }
    
    
    Value operator/(const Value& op) const {
        if( IsInt() && op.IsInt() )
            return GetInt() - op.GetInt();
        throw string(": Type error on division");
    }
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endif