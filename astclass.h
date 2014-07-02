#ifndef ASTCLASS_H_INCLUDED
#define ASTCLASS_H_INCLUDED

#include "astcodeblock.h"
#include "tokentype.h"
#include "astperiod.h"

class ASTClass final : public ASTCodeBlock
{
protected:
    vector<Token> modifiers;
    shared_ptr<ASTPeriod> name;
    shared_ptr<ASTPeriod> inherits;
    vector<shared_ptr<ASTPeriod>> implements;
public:
    ASTClass(LocationRange location, shared_ptr<ASTPeriod> name, shared_ptr<ASTPeriod> inherits, vector<shared_ptr<ASTPeriod>> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, vector<shared_ptr<ASTNode>> nodes)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name), inherits(inherits), implements(implements)
    {
    }
    ASTClass(LocationRange location, shared_ptr<ASTPeriod> name, shared_ptr<ASTPeriod> inherits, vector<shared_ptr<ASTPeriod>> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, initializer_list<shared_ptr<ASTNode>> il)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name), inherits(inherits), implements(implements)
    {
    }
    ASTClass(LocationRange location, shared_ptr<ASTPeriod> name, shared_ptr<ASTPeriod> inherits, vector<shared_ptr<ASTPeriod>> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers)
        : ASTCodeBlock(location, imports, variables), modifiers(modifiers), name(name), inherits(inherits), implements(implements)
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTClass>(location, name, inherits, implements, imports, variables, modifiers, nodes);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        ASTNode::indent(os, indentLevel);
        for(Token t : modifiers)
        {
            os << t.toSourceString() << L" ";
        }
        os << getTokenAsPrintableString(TokenType::Class) << L" ";
        name->dump(os, indentLevel + 1);
        if(inherits != nullptr)
        {
            os << L" " << getTokenAsPrintableString(TokenType::Inherits) << L" ";
            inherits->dump(os, indentLevel + 1);
        }
        if(implements.size() > 0)
        {
            os << L" " << getTokenAsPrintableString(TokenType::Implements);
            wstring seperator = L" ";
            for(shared_ptr<ASTPeriod> interface : implements)
            {
                os << seperator;
                interface->dump(os, indentLevel + 1);
                seperator = L", ";
            }
        }
        os << endl;
        for(shared_ptr<ASTNode> node : nodes)
        {
            node->dump(os, indentLevel + 1);
            os << endl;
        }
        ASTNode::indent(os, indentLevel);
        os << getTokenAsPrintableString(TokenType::EndClass) << endl;
    }
};

#endif // ASTCLASS_H_INCLUDED
