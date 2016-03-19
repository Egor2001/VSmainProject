#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "stdafx.h"
#include "Token.h"

using std::invalid_argument;

class Parser
{
private:

	typedef double(*ps_fnc)(double);

	bool                    ps_parsed;

	vector<Token>           ps_tokens;
	vector<Token>::iterator ps_tokpos;

	string                  ps_strexp;
	string::iterator        ps_exppos;

	map<string, size_t>     ps_varmap;
	map<string, size_t>     ps_fncmap;

	vector<double>          ps_varvec;
	vector<ps_fnc>          ps_fncvec;

	void tokParse();
	void tokNumber();
	void tokName();
	void tokOper();
	void tokSpaces();

	inline double psNumber();// N ::= [0-9]+ {'.' [0-9]+}*
	inline double psVarFnc();// V ::= [A-z] {[A-z]|[0-9]}* B?
	inline double psBracket();// B ::= '(' S ')'
	inline double psFactor();// F ::= '-'? N | V | B
	inline double psMulDiv();// M ::= B {('*'|'/') B}*
	inline double psSumSub();// S ::= M {('+'|'-') M}*

public:

	Parser();
	Parser(const string& strexp_set);
	~Parser();

	double& operator[] (const string& var_name)
	{
		return ps_varvec[ps_varmap[var_name]];
	}

	void SetExp(const string& strexp_set)
	{
		ps_tokens = vector<Token>();
		ps_tokpos = ps_tokens.begin();

		ps_strexp = strexp_set;
		ps_strexp.push_back('\0');
		ps_exppos = ps_strexp.begin();

		ps_parsed = false;
	}

	void SetVar(const string& var_name, double var_set)
	{
		if (ps_varmap.count(var_name))
			ps_varvec[ps_varmap[var_name]] = var_set;
		else
		{
			ps_varvec.push_back(var_set);

			ps_varmap[var_name] = ps_varvec.size() - 1;
		}
	}

	void SetFnc(const string& fnc_name, ps_fnc fnc_set)
	{
		if (ps_fncmap.count(fnc_name))
			ps_fncvec[ps_fncmap[fnc_name]] = fnc_set;
		else
		{
			ps_fncvec.push_back(fnc_set);

			ps_fncmap[fnc_name] = ps_fncvec.size() - 1;
		}
	}

	void PrintTokens(FILE* file);
	double GetResult();
};

//=============================================================================

Parser::Parser() :
	ps_parsed(false),
	ps_tokens(),
	ps_tokpos(ps_tokens.begin()),
	ps_strexp(""),
	ps_exppos(ps_strexp.begin()),
	ps_varmap(),
	ps_fncmap(),
	ps_varvec(),
	ps_fncvec()
{
	ps_strexp.push_back('\0');
	
	SetVar("PI", M_PI);
	SetFnc( "sin",  sin); SetFnc( "cos",  cos); SetFnc( "tan",  tan);
	SetFnc("asin", asin); SetFnc("acos", acos); SetFnc("atan", atan);
	SetFnc("sqrt", sqrt); SetFnc("sqr", [](double num) { return num*num; });
}

//-----------------------------------------------------------------------------

Parser::Parser(const string& strexp_set) :
	ps_parsed(false),
	ps_tokens(),
	ps_tokpos(ps_tokens.begin()),
	ps_strexp(strexp_set),
	ps_exppos(ps_strexp.begin()),
	ps_varmap(),
	ps_fncmap(),
	ps_varvec(),
	ps_fncvec()
{
	ps_strexp.push_back('\0');
	
	SetVar("PI", M_PI);
	SetFnc( "sin",  sin); SetFnc( "cos",  cos); SetFnc( "tan",  tan);
	SetFnc("asin", asin); SetFnc("acos", acos); SetFnc("atan", atan);
	SetFnc("sqrt", sqrt); SetFnc("sqr", [](double num) { return num*num; });
}

//-----------------------------------------------------------------------------

Parser::~Parser()
{}

//=============================================================================

void Parser::tokParse()
{
	while (ps_exppos != ps_strexp.end())
	{
		tokSpaces();
			 if (isdigit(*ps_exppos)) tokNumber();
		else if (isalpha(*ps_exppos)) tokName();
		else                          tokOper();
	}

	ps_tokens.push_back(Token());

	ps_tokpos = ps_tokens.begin();
	ps_parsed = true;
}

//-----------------------------------------------------------------------------

void Parser::tokNumber()
{
	size_t beforeDot = 0, afterDot = 0;
	int fracDigNum = 0;

	while (isdigit(*ps_exppos) &&
					ps_exppos < ps_strexp.end())
	{
		beforeDot = beforeDot * 10 + (*ps_exppos - '0');
		ps_exppos++;
	}

	if ((*ps_exppos) != '.')
	{
		ps_tokens.push_back(Token(NUM, (double)beforeDot));

		return;
	}
	else ps_exppos++;

	while (isdigit(*ps_exppos) &&
					ps_exppos < ps_strexp.end())
	{
		afterDot = afterDot * 10 + (*ps_exppos - '0');
		fracDigNum++;
		ps_exppos++;
	}

	ps_tokens.push_back(Token(NUM, beforeDot + (double)afterDot / pow(10.0, fracDigNum)));
}

//-----------------------------------------------------------------------------

void Parser::tokName()
{
	string name;

	while (isalnum(*ps_exppos) &&
					ps_exppos < ps_strexp.end())
	{
		name.push_back(*ps_exppos);
		ps_exppos++;
	}
	tokSpaces();

	if ((*ps_exppos) == '(') ps_tokens.push_back(Token(FNC, ps_fncmap.find(name)->second));
	else                     ps_tokens.push_back(Token(VAR, ps_varmap.find(name)->second));
}

//-----------------------------------------------------------------------------

void Parser::tokOper()
{
	ps_tokens.push_back(Token(OPER, (*ps_exppos)));

	if (ps_exppos < ps_strexp.end()) ps_exppos++;
}

//-----------------------------------------------------------------------------

void Parser::tokSpaces()
{
	while (isspace(*ps_exppos) &&
				    ps_exppos < ps_strexp.end()) ps_exppos++;
}

//=============================================================================

double Parser::psNumber()
{
	if (ps_tokpos->GetType() != NUM)
		throw invalid_argument("psNumber number expected");

	double res = ps_tokpos->GetValue().numVal;

	ps_tokpos++;

	return res;
}

//-----------------------------------------------------------------------------

double Parser::psVarFnc()
{
	int pos = ps_tokpos->GetValue().nameVal;

	if (ps_tokpos->GetType() == VAR)
	{
		ps_tokpos++;

		return ps_varvec[pos];
	}
	else if (ps_tokpos->GetType() == FNC)
	{
		ps_tokpos++;

		return ps_fncvec[pos](psBracket());
	}
	else throw invalid_argument("psVarFnc name expected");
}

//-----------------------------------------------------------------------------

double Parser::psBracket()
{
	if (ps_tokpos->GetValue().operVal != '(')
		throw invalid_argument("psBracket '(' expected");

	ps_tokpos++;

	double res = psSumSub();

	if (!(ps_tokpos->GetType()          == OPER &&
		  ps_tokpos->GetValue().operVal == ')'))
		throw invalid_argument("psBracket ')' expected");

	ps_tokpos++;

	return res;
}

//-----------------------------------------------------------------------------

double Parser::psFactor()
{
	bool neg = ps_tokpos->GetType() == OPER &&
			   ps_tokpos->GetValue().operVal == '-';
	
	if (neg) ps_tokpos++;

	double res = 0.0;

		 if (ps_tokpos->GetType() == OPER) res = psBracket();
	else if (ps_tokpos->GetType() ==  NUM) res = psNumber();
	else                                   res = psVarFnc();

	return neg ? -res : res;
}

//-----------------------------------------------------------------------------

double Parser::psMulDiv()
{
	double res = psFactor();

	while (ps_tokpos->GetType() == OPER)
	{
			 if (ps_tokpos->GetValue().operVal == '*') { ps_tokpos++; res *= psFactor(); }
		else if (ps_tokpos->GetValue().operVal == '/') { ps_tokpos++; res /= psFactor(); }
		else break;
	}

	return res;
}

//-----------------------------------------------------------------------------

double Parser::psSumSub()
{
	double res = psMulDiv();

	while (ps_tokpos->GetType() == OPER)
	{
			 if (ps_tokpos->GetValue().operVal == '+') { ps_tokpos++; res += psMulDiv(); }
		else if (ps_tokpos->GetValue().operVal == '-') { ps_tokpos++; res -= psMulDiv(); }
		else break;
	}

	return res;
}

//-----------------------------------------------------------------------------

void Parser::PrintTokens(FILE* file)
{
	if (!ps_parsed)
	{
		fprintf(file, "Expression was not parsed \n");

		return;
	}

	for (vector<Token>::iterator it = ps_tokens.begin(); it < ps_tokens.end(); it++)
	{
		switch (it->tk_type)
		{
		case   NUM: fprintf(file, "{%lg} "	  , it->GetValue(). numVal); break;
		case   VAR: fprintf(file, "{var[%d]} ", it->GetValue().nameVal); break;
		case   FNC: fprintf(file, "{fnc[%d]} ", it->GetValue().nameVal); break;
		case  OPER: fprintf(file, "{%c} "	  ,	it->GetValue().operVal); break;
		case INVAL: fprintf(file, "{INVAL}");                            break;
		default: break;
		}
	}
}

//-----------------------------------------------------------------------------

double Parser::GetResult()
{
	if (!ps_parsed) tokParse();
	else ps_tokpos = ps_tokens.begin();

	return psSumSub();
}

//-----------------------------------------------------------------------------

#endif
