#ifndef ASTNAMESPACE_H_INCLUDED
#define ASTNAMESPACE_H_INCLUDED

#include "astcodeblock.h"
#include "tokentype.h"

class ASTNamespace final : public ASTCodeBlock
{
protected:
    vector<Token> modifiers;
    wstring name;
public:
    ASTNamespace(LocationRange location, wstring name, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, vector<shared_ptr<ASTNode>> nodes)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name)
    {
    }
    ASTNamespace(LocationRange location, wstring name, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers, initializer_list<shared_ptr<ASTNode>> il)
        : ASTCodeBlock(location, imports, variables, nodes), modifiers(modifiers), name(name)
    {
    }
    ASTNamespace(LocationRange location, wstring name, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<Token> modifiers)
        : ASTCodeBlock(location, imports, variables), modifiers(modifiers), name(name)
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTNamespace>(location, name, imports, variables, modifiers, nodes);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        ASTNode::indent(os, indentLevel);
        for(Token t : modifiers)
        {
            os << t.toSourceString() << L" ";
        }
        os << getTokenAsPrintableString(TokenType::Namespace) << L" " << name << endl;
        for(shared_ptr<ASTNode> node : nodes)
        {
            node->dump(os, indentLevel + 1);
            os << endl;
        }
        ASTNode::indent(os, indentLevel);
        os << getTokenAsPrintableString(TokenType::EndNamespace) << endl;
    }
};

#endif // ASTNAMESPACE_H_INCLUDED
