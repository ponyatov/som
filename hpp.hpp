#ifndef _H_SOM
#define _H_SOM

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string val;
	Sym(string);
	vector<Sym*> nest; void push(Sym*);		// nested
	virtual string dump(int=0); 			// \ dumping
	virtual string head();
	string pad(int);						// /
	virtual string str();					// str(object)
	virtual Sym* eval();
	virtual Sym* add(Sym*);
};

struct Error: Sym { Error(string); };

extern map<string,Sym*> glob;
extern void glob_init();

struct Str: Sym { Str(string); string head(); Sym*add(Sym*); };

struct Vector: Sym { Vector(); };

struct Op: Sym { Op(string); Sym*eval(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_SOM
