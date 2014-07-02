#ifndef ASTPERIOD_H_INCLUDED
#define ASTPERIOD_H_INCLUDED

#include "astexpression.h"
#include "tokentype.h"

class ASTPeriod final : public ASTExpression
{
private:
    bool startsWithPeriod;
protected:
    virtual shared_ptr<ASTType> calcType() override;
public:
    ASTPeriod(LocationRange location, bool startsWithPeriod, vector<shared_ptr<ASTNode>> nodes)
        : ASTExpression(location, nodes), startsWithPeriod(startsWithPeriod)
    {
    }
    ASTPeriod(LocationRange location, bool startsWithPeriod, initializer_list<shared_ptr<ASTNode>> il)
        : ASTExpression(location, nodes), startsWithPeriod(startsWithPeriod)
    {
    }
    ASTPeriod(LocationRange location, bool startsWithPeriod = false)
        : ASTExpression(location), startsWithPeriod(startsWithPeriod)
    {
    }
    bool doesStartWithPeriod() const
    {
        return startsWithPeriod;
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTPeriod>(location, startsWithPeriod, nodes);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        wstring seperator = L"";
        if(startsWithPeriod)
            os << getTokenAsPrintableString(TokenType::Period);
        for(shared_ptr<ASTNode> node : nodes)
        {
            os << seperator;
            node->dump(os, indentLevel);
            seperator = getTokenAsPrintableString(TokenType::Period);
        }
    }
};

#endif // ASTPERIOD_H_INCLUDED
