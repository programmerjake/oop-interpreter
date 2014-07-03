#ifndef ASTTYPEPOINTER_H_INCLUDED
#define ASTTYPEPOINTER_H_INCLUDED

#include "asttype.h"
#include "tokentype.h"

class ASTTypePointer final : public ASTType
{
public:
    ASTTypePointer(LocationRange location, shared_ptr<ASTNode> node)
        : ASTType(location, {node})
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTTypePointer>(location, nodes[0]);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        os << getTokenAsPrintableString(TokenType::Pointer) << L" " << getTokenAsPrintableString(TokenType::To) << L" ";
        nodes[0]->dump(os, indentLevel);
    }
};

#endif // ASTTYPEPOINTER_H_INCLUDED
