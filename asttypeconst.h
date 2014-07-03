#ifndef ASTTYPECONST_H_INCLUDED
#define ASTTYPECONST_H_INCLUDED

#include "asttype.h"
#include "tokentype.h"

class ASTTypeConst final : public ASTType
{
public:
    ASTTypeConst(LocationRange location, shared_ptr<ASTNode> node)
        : ASTType(location, {node})
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTTypeConst>(location, nodes[0]);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        os << getTokenAsPrintableString(TokenType::Const) << L" ";
        nodes[0]->dump(os, indentLevel);
    }
};

#endif // ASTTYPECONST_H_INCLUDED
