#ifndef ASTEXPRESSION_H_INCLUDED
#define ASTEXPRESSION_H_INCLUDED

#include "asttype.h"

class ASTExpression : public ASTNode
{
private:
    shared_ptr<ASTType> type;
protected:
    virtual shared_ptr<ASTType> calcType() = 0;
public:
    ASTExpression(LocationRange location, vector<shared_ptr<ASTNode>> nodes)
        : ASTNode(location, nodes)
    {
    }
    ASTExpression(LocationRange location, initializer_list<shared_ptr<ASTNode>> il)
        : ASTNode(location, nodes)
    {
    }
    ASTExpression(LocationRange location)
        : ASTNode(location)
    {
    }
    shared_ptr<ASTType> getType()
    {
        if(type == nullptr)
            type = calcType();
        return type;
    }
};

#endif // ASTEXPRESSION_H_INCLUDED
