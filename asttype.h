#ifndef ASTTYPE_H_INCLUDED
#define ASTTYPE_H_INCLUDED

#include "astnode.h"

class ASTType : public ASTNode
{
public:
    ASTType(LocationRange location, vector<shared_ptr<ASTNode>> nodes)
        : ASTNode(location, nodes)
    {
    }
    ASTType(LocationRange location, initializer_list<shared_ptr<ASTNode>> il)
        : ASTNode(location, nodes)
    {
    }
    ASTType(LocationRange location)
        : ASTNode(location)
    {
    }
};

#endif // ASTTYPE_H_INCLUDED
