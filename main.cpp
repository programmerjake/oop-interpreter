#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int main()
{
    Parser parser(make_shared<IStreamParserInput>(make_shared<wifstream>("test.txt")));
    parser.run();
    return 0;
}
