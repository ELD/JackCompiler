#include "../headers/includes.h"
#include "../headers/JackTokenizer.h"

int main()
{
    string file = "test.jack";
    JackTokenizer tokens(file);

    while (tokens.hasMoreTokens()) {
        auto tokenType = tokens.tokenType();
        auto token = tokens.getNextToken();
        cout << "Type: " << tokenType << "\tToken: " << token << endl;
    }
}
