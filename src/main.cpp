#include "../headers/includes.h"
#include "../headers/JackTokenizer.h"

int main()
{
    string file = "test.jack";
    JackTokenizer tokens(file);

    auto tokensCollection = tokens.getTokens();
    for (auto token : tokensCollection) {
        cout << "Type: " << token.first << "\tToken: " << token.second << endl;
    }
}
