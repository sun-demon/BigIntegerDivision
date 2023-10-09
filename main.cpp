#include <iostream>
#include "BigInteger/BigInteger.h"

using namespace std;
using namespace DDP;

int main() {
    cout << "Big integer division!"
         << "Enter two integers (dividend and divider):" << endl;
    string strDivision, strDivider;
    std::cin >> strDivision >> strDivider;
    BigInteger division(strDivision),
               divider(strDivider);
    cout << "Result:\r\n"
         << division / divider << endl;
    return 0;
}
