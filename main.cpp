#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int main()
{
    Parser parser(make_shared<IStreamParserInput>(make_shared<wifstream>("test.txt")));
    shared_ptr<ASTNode> ast = parser.run();
    if(ast)
        ast->dump(wcout);
    return 0;
}
