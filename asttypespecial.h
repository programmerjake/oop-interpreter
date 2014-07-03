#ifndef ASTTYPESPECIAL_H_INCLUDED
#define ASTTYPESPECIAL_H_INCLUDED

#include "asttype.h"
#include "tokentype.h"

class ASTTypeSpecial final : public ASTType
{
private:
    TokenType type;
public:
    ASTTypeSpecial(LocationRange location, TokenType type)
        : ASTType(location), type(type)
    {
    }
    TokenType getType() const
    {
        return type;
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTTypeSpecial>(location, type);
    }
    virtual void dump(wostream & os, size_t) const override
    {
        os << getTokenAsPrintableString(type);
    }
};

#endif // ASTTYPESPECIAL_H_INCLUDED
