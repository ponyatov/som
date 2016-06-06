#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

Sym::Sym(string V) { val=V; }
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::head() { return val; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+head();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Str::Str(string V):Sym(V) {}
string Str::head() { return "'"+val+"'"; }

Vector::Vector():Sym("[]") {}

Op::Op(string V):Sym(V) {}

map<string,Sym*> glob;
void glob_init() {
	glob["MODULE"] = new Str(MODULE);
	glob["bl"] = new Str(" ");
}
