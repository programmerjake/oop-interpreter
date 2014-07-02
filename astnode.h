#ifndef ASTNODE_H_INCLUDED
#define ASTNODE_H_INCLUDED

#include <memory>
#include <cwchar>
#include <string>
#include <vector>
#include <ostream>
#include <initializer_list>
#include "location.h"

using namespace std;

class ASTNode : public enable_shared_from_this<ASTNode>
{
    friend class Parser;
protected:
    LocationRange location;
    weak_ptr<ASTNode> lexicalParent;
    vector<shared_ptr<ASTNode>> nodes;
    shared_ptr<ASTNode> setLexicalParent(weak_ptr<ASTNode> lexicalParent)
    {
        this->lexicalParent = lexicalParent;
        return shared_from_this();
    }
public:
    ASTNode(LocationRange location, vector<shared_ptr<ASTNode>> nodes)
        : location(location), nodes(nodes)
    {
    }
    ASTNode(LocationRange location, initializer_list<shared_ptr<ASTNode>> il)
        : location(location), nodes(il)
    {
    }
    ASTNode(LocationRange location)
        : location(location)
    {
    }
    virtual ~ASTNode()
    {
    }
    LocationRange getLocation() const
    {
        return location;
    }
    virtual shared_ptr<ASTNode> dup() const = 0;
    virtual void dump(wostream & os, size_t indentLevel) const = 0;
    void dump(wostream & os) const
    {
        return dump(os, 0);
    }
    static void indent(wostream & os, size_t indentLevel)
    {
        for(size_t i = 0; i < indentLevel * TabWidth; i++)
        {
            os << L" ";
        }
    }
};

#endif // ASTNODE_H_INCLUDED
