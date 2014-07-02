#ifndef ASTIDENTIFIER_H_INCLUDED
#define ASTIDENTIFIER_H_INCLUDED

#include "astexpression.h"
#include "astperiod.h"
#include "tokentype.h"

class ASTIdentifier final : public ASTExpression
{
private:
    wstring value;
protected:
    virtual shared_ptr<ASTType> calcType() override
    {
        shared_ptr<ASTExpression> period = shared_ptr<ASTPeriod>(new ASTPeriod(location, false, {shared_from_this()}));
        return period->getType();
    }
public:
    ASTIdentifier(LocationRange location, wstring value)
        : ASTExpression(location), value(value)
    {
    }
    ASTIdentifier(Token t)
        : ASTExpression(t.location), value(t.value)
    {
    }
    wstring getValue() const
    {
        return value;
    }
    virtual void dump(wostream & os, size_t) const override
    {
        os << value;
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTIdentifier>(location, value);
    }
};

class ASTObjectIdentifier final : public ASTExpression
{
protected:
    virtual shared_ptr<ASTType> calcType() override
    {
        shared_ptr<ASTExpression> period = shared_ptr<ASTPeriod>(new ASTPeriod(location, false, {shared_from_this()}));
        return period->getType();
    }
public:
    ASTObjectIdentifier(LocationRange location)
        : ASTExpression(location)
    {
    }
    wstring getValue() const
    {
        return getTokenAsPrintableString(TokenType::Object);
    }
    virtual void dump(wostream & os, size_t) const override
    {
        os << getTokenAsPrintableString(TokenType::Object);
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTObjectIdentifier>(location);
    }
};

class ASTSpecialIdentifier final : public ASTExpression
{
    TokenType type;
protected:
    virtual shared_ptr<ASTType> calcType() override
    {
        shared_ptr<ASTExpression> period = shared_ptr<ASTPeriod>(new ASTPeriod(location, false, {shared_from_this()}));
        return period->getType();
    }
public:
    ASTSpecialIdentifier(LocationRange location, TokenType type)
        : ASTExpression(location), type(type)
    {
    }
    wstring getValue() const
    {
        return getTokenAsPrintableString(type);
    }
    TokenType getType() const
    {
        return type;
    }
    virtual void dump(wostream & os, size_t) const override
    {
        os << getTokenAsPrintableString(type);
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTSpecialIdentifier>(location, type);
    }
};

#endif // ASTIDENTIFIER_H_INCLUDED
