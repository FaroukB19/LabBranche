//
// Created by farouk on 19-03-14.
//
//SAlut farouk c pool :ppppp

#ifndef TP2_TYPE_H
#define TP2_TYPE_H

#include <iostream>
#include <vector>

using namespace std;

class Type {
public:
    Type();

    Type( string nom, vector<string> seqID ) :
            nom( nom ),
            seqID( seqID ) {
    };

    virtual ~Type();

    string getnom() { return nom; }

    vector<string> getseqID() { return seqID; }

protected:

private:
    string nom;
    vector<string> seqID;
};

Type::Type()
{

}


Type::~Type()
{

}

#endif //TP2_TYPE_H
