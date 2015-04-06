#include "../headers/includes.h"
#include "../headers/JackTokenizer.h"

int main()
{
    fstream file("test.jack");
    JackTokenizer tokens(file);

    auto tokensCollection = tokens.getTokens();
    for (auto token : tokensCollection) {
        cout << token << endl;
    }
}
