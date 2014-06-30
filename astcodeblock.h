#ifndef ASTCODEBLOCK_H_INCLUDED
#define ASTCODEBLOCK_H_INCLUDED

#include "astnode.h"
#include <unordered_map>

using namespace std;

class ASTNamespace;

class ASTCodeBlock : public ASTNode
{
protected:
    unordered_map<wstring, shared_ptr<ASTNode>> variables;
    vector<shared_ptr<ASTNamespace>> imports;
public:
    ASTCodeBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<shared_ptr<ASTNode>> nodes)
        : ASTNode(location, nodes), variables(variables), imports(imports)
    {
    }
    ASTCodeBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, initializer_list<shared_ptr<ASTNode>> il)
        : ASTNode(location, nodes), variables(variables), imports(imports)
    {
    }
    ASTCodeBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables)
        : ASTNode(location, nodes), variables(variables), imports(imports)
    {
    }
};

class ASTBlock final : public ASTCodeBlock
{
public:
    ASTBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<shared_ptr<ASTNode>> nodes)
        : ASTCodeBlock(location, imports, variables, nodes)
    {
    }
    ASTBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, initializer_list<shared_ptr<ASTNode>> il)
        : ASTCodeBlock(location, imports, variables, nodes)
    {
    }
    ASTBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables)
        : ASTCodeBlock(location, imports, variables)
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTBlock>(location, imports, variables, nodes);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        ASTNode::indent(os, indentLevel);
        os << getTokenAsPrintableString(TokenType::Block) << endl;
        for(shared_ptr<ASTNode> node : nodes)
        {
            node->dump(os, indentLevel + 1);
            os << endl;
        }
        ASTNode::indent(os, indentLevel);
        os << getTokenAsPrintableString(TokenType::EndBlock) << endl;
    }
};

class ASTGlobalBlock final : public ASTCodeBlock
{
public:
    ASTGlobalBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, vector<shared_ptr<ASTNode>> nodes)
        : ASTCodeBlock(location, imports, variables, nodes)
    {
    }
    ASTGlobalBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables, initializer_list<shared_ptr<ASTNode>> il)
        : ASTCodeBlock(location, imports, variables, nodes)
    {
    }
    ASTGlobalBlock(LocationRange location, vector<shared_ptr<ASTNamespace>> imports, unordered_map<wstring, shared_ptr<ASTNode>> variables)
        : ASTCodeBlock(location, imports, variables)
    {
    }
    virtual shared_ptr<ASTNode> dup() const override
    {
        return make_shared<ASTGlobalBlock>(location, imports, variables, nodes);
    }
    virtual void dump(wostream & os, size_t indentLevel) const override
    {
        for(shared_ptr<ASTNode> node : nodes)
        {
            node->dump(os, indentLevel);
            os << endl;
        }
    }
};

#endif // ASTCODEBLOCK_H_INCLUDED
