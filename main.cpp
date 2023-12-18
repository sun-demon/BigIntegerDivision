#include "BigInteger/BigInteger.h"
#include <conio.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace DDP;


size_t absSize(const BigInteger &bi);
string toAbsString(const BigInteger &bi);

void pressAnyKeyForContinue() {
    cout << "PressAnyKeyForContinue" << flush;
    getch();
}

int main() {
    cout << "Big integer division!"
         << "Enter two integers (dividend and divider):" << endl;
    string strDivision, strDivider;
    std::cin >> strDivision >> strDivider;
    BigInteger division(strDivision),
               divider(strDivider);
    cout << "Result:\r\n";

    if (division == BigInteger(0)) {
        cout << "0" << endl;
        return 0;
    }

    BigInteger mantissa = division / divider;
    BigInteger exponent = BigInteger(30);
    size_t nMantissaDigits = absSize(mantissa);
    for (; nMantissaDigits < 30; nMantissaDigits = absSize(mantissa)) {
        size_t nBigShift = (30 - nMantissaDigits) / 9,
               nSmallShift = (30 - nMantissaDigits) % 9;
        division = division >> nBigShift;
        int secondMultiplier = 1;
        for (size_t i = 0; i < nSmallShift; ++i)
            secondMultiplier *= 10;
        division = division * BigInteger(secondMultiplier);
        mantissa = division / divider;
        exponent -= BigInteger(nBigShift * 9 + nSmallShift);
    }
    if (nMantissaDigits > 30) {
        size_t nBigShift = (nMantissaDigits - 30) / 9,
               nSmallShift = (nMantissaDigits - 30) % 9;
        division = division << nBigShift;
        int secondDivider = 1;
        for (size_t i = 0; i < nSmallShift; ++i)
            secondDivider *= 10;
        division = division / secondDivider;
        mantissa = division / divider;
        exponent += BigInteger(nBigShift * 9 + nSmallShift);
    }

    auto oldFill = cout.fill('0');
    cout << ((mantissa < BigInteger(0)) ? "-" : "+") << "0." << toAbsString(mantissa)
         << "E" << ((exponent < BigInteger(0)) ? "-" : "+") << setw(5) << toAbsString(exponent) << endl;
    pressAnyKeyForContinue();
    return 0;
}


size_t absSize(const BigInteger &bi) {
    if (bi == BigInteger(0))
        return 0;
    return bi.toString().size() - ((bi < BigInteger(0)) ? 1 : 0);
}
string toAbsString(const BigInteger &bi) {
    if (bi < BigInteger(0))
        return bi.toString().substr(1);
    return bi.toString();
}