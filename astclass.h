#ifndef ASTCLASS_H_INCLUDED
#define ASTCLASS_H_INCLUDED

#include "astcodeblock.h"
#include "tokentype.h"

class ASTClass final : public ASTCodeBlock
{
protected:
    vector<Token> modifiers;
    wstring name;
    wstring inherits;
    vector<wstring> implements;
public:
    ASTClass(LocationRange location, wstring name, wstring inherits, vector<wstring> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, vector<shared_ptr<ASTNode>> nodes)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name), inherits(inherits), implements(implements)
    {
    }
    ASTClass(LocationRange location, wstring name, wstring inherits, vector<wstring> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, initializer_list<shared_ptr<ASTNode>> il)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name), inherits(inherits), implements(implements)
    {
    }
    ASTClass(LocationRange location, wstring name, wstring inherits, vector<wstring> implements, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers)
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
        os << getTokenAsPrintableString(TokenType::Class) << L" " << name;
        if(inherits != "")
            os << getTokenAsPrintableString(TokenType::Inherits) << L" " << inherits;
        if(implements.size() > 0)
        {
            os << getTokenAsPrintableString(TokenType::Implements);
            wstring seperator = L" ";
            for(wstring interface : implements)
            {
                os << seperator << interface;
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
