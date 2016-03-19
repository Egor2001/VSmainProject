#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include "stdafx.h"

using std::vector;
using std::string;
using std::map;

enum tkType { INVAL, NUM, VAR, FNC, OPER };

union tkValue
    {
    double  numVal;
    char   operVal;
    size_t nameVal;
    };

class Token
    {
    public:

    tkType  tk_type;
    tkValue tk_value;

    public:

    tkType  GetType  () { return tk_type; }
    tkValue GetValue () { return tk_value; }

    Token ():
        tk_type(INVAL), tk_value() {}

    Token (tkType type_set, double value_set):
        tk_type(type_set), tk_value()
        {
        if (type_set != NUM) throw string("not valid token type");

        tk_value.numVal = value_set;
        }

    Token (tkType type_set, size_t value_set):
        tk_type(type_set), tk_value()
        {
        if (type_set != VAR && type_set != FNC) throw string("not valid token type");

        tk_value.nameVal = value_set;
        }

    Token (tkType type_set, char value_set):
        tk_type(type_set), tk_value()
        {
        if (type_set != OPER) throw string("not valid token type");

        tk_value.operVal = value_set;
        }

   ~Token () {}
    };

#endif
