#ifndef ASTTYPE_H_INCLUDED
#define ASTTYPE_H_INCLUDED

#include "astnode.h"

class ASTType : public ASTNode
{
public:
    ASTType(LocationRange location, weak_ptr<ASTNode> lexicalParent, vector<shared_ptr<ASTNode>> nodes)
        : ASTNode(location, lexicalParent, nodes)
    {
    }
    ASTType(LocationRange location, weak_ptr<ASTNode> lexicalParent, initializer_list<shared_ptr<ASTNode>> il)
        : ASTNode(location, lexicalParent, nodes)
    {
    }
    ASTType(LocationRange location, weak_ptr<ASTNode> lexicalParent)
        : ASTNode(location, lexicalParent)
    {
    }
};

#endif // ASTTYPE_H_INCLUDED
