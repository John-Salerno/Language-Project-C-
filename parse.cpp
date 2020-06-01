
#include "parse.h"

namespace Parser {
bool pushed_back = false;
Tok	pushed_token;

static Tok GetNextToken(istream& in, int& line) {
	if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line)
{
	Pt *sl = Sl(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;

	return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line) {
	Pt *s = Stmt(in, line);
	if( s == 0 ){
		return 0;
    }

	if( Parser::GetNextToken(in, line) != SC ) {
		ParseError(line-1, "Missing semicolon");
		delete s;
		return 0;
	}

	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {
	Pt *st=0;
    
    Tok t = Parser::GetNextToken(in, line);
    
    switch(t.GetToken()){
        case PRINT:
            st=PrintStmt(in,line);
            break;
        
        case PRINTLN:
            st=PrintlnStmt(in,line);
            break;
        
        case REPEAT:
            st=RepeatStmt(in,line);
            break;
        
        case DONE:
            break;
            
        case ERR:
            ParseError(line,"Bad Token");
            break;
     
        case IDENT:
            Parser::PushBackToken(t);
            st=Expr(in,line);
            break;
        
        default:
            ParseError(line,"Invalid statement");
            break;
            
    }
    return st;
    

}

Pt *PrintStmt(istream& in, int& line) {
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Print(l, ex);
}

Pt *PrintlnStmt(istream& in, int& line) {
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after println");
		return 0;
	}

	return new Println(l, ex);

}

Pt *RepeatStmt(istream& in, int& line) {
    
    Pt *ex = Expr(in, line);
    if(ex==0){
        ParseError(line, "Missing expression after repeat");
		return 0;
    }
    
    Tok t = Parser::GetNextToken(in, line);
    if(t!=BEGIN){
        ParseError(line, "Missing BEGIN after Expr");
		return 0;
    }
    
    Pt *st= Stmt(in, line);
    if(st==0){
        ParseError(line, "Missing stmt after BEGIN");
		return 0;
    }
    
    Tok t2 = Parser::GetNextToken(in, line);
    if(t2!=END){
        ParseError(line, "Missing END after stmt");
		return 0;
    }
    
    return new Repeat(line, ex, st);
    
    

}

Pt *Expr(istream& in, int& line) {
	Pt *su = Sum(in,line);
    if(su==0){
        return 0;
    }
    while(true){
        Tok t=Parser::GetNextToken(in, line);
        
        if(t!=EQ){
            Parser::PushBackToken(t);
            return su;
        }
        
        Pt *su2= Sum(in,line);
        if(su2==0){
            ParseError(line, "Missing assignment after EQ sign");
            return 0;
        }
        
        su= new Assign(t.GetLinenum(), su, su2);
    }
    
    

}

Pt *Sum(istream& in, int& line) {
	Pt *t1 = Prod(in, line);
	if( t1 == 0 ) {
        cout<<"null in sum"<<endl;
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

Pt *Prod(istream& in, int& line) {
	Pt *pr1 = Primary(in, line);
	if( pr1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return pr1;
		}

		Pt *pr2 = Prod(in, line);
		if( pr2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == STAR )
			pr1 = new TimesExpr(t.GetLinenum(), pr1, pr2);
		else
			pr1 = new DivideExpr(t.GetLinenum(), pr1, pr2);
	}
}

Pt *Primary(istream& in, int& line) {
	Tok t = Parser::GetNextToken(in, line);

	if( t == IDENT ) {
		return new Ident(t);
	}
	else if( t == SCONST){
        return new SConst(t);
    }
    else if( t == ICONST){
        return new IConst(t);
    }
    else if( t == LPAREN){
        Pt *ex=Expr(in,line);
        if(ex==0){
            cout<<"null in prim"<<endl;
            return 0;
        }
        
        if(Parser::GetNextToken(in, line) != RPAREN){
        	ParseError(line, "Expression is not closed with RPAREN");
            return 0;
    	}
        
        return ex;
    }
    else{
        
        ParseError(line, "No primary");
        return 0;
    }

}

