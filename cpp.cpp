#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { glob_init(); return yyparse(); }

Sym::Sym(string V) { val=V; }
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::str() { return val; }
string Sym::head() { return str(); }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+head();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym*G = glob[val]; if (G) return G;					// global lookup
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)	// nested eval()
		(*it) = (*it)->eval();
	return this; }

Sym* Sym::eq(Sym*o) { glob[val]=o; return o; }
Sym* Sym::add(Sym*o) { return new Error("undef "+head()+"+"+o->head()); }

Error::Error(string V):Sym(V) { yyerror(val); }

Str::Str(string V):Sym(V) {}
string Str::head() { string S="'";
	for (int i=0;i<val.length();i++)
		switch (val[i]) {
			case '\n': S+="\\n"; break;
			case '\t': S+="\\t"; break;
			default: S+=val[i];
		}
	return S+"'";}
Sym* Str::add(Sym*o) { return new Str(val+o->str()); }

Vector::Vector():Sym("[]") {}

Op::Op(string V):Sym(V) {}
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();		// quote or nested eval
	if (val=="=") return nest[0]->eq(nest[1]);			// =
	if (val=="+") return nest[0]->add(nest[1]);			// +
	return this;
}

map<string,Sym*> glob;
void glob_init() {
	glob["MODULE"] = new Str(MODULE);
	glob["ABOUT"] = new Str(ABOUT);
	glob["AUTHOR"] = new Str(AUTHOR);
	glob["LICENSE"] = new Str(LICENSE);
	glob["GITHUB"] = new Str(GITHUB);
	glob["bl"] = new Str(" ");
}
