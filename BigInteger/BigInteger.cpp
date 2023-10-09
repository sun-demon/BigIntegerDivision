//
// Created by Daniil on 07.10.2023.
//

#include "BigInteger.h"
#include <ostream>
#include <iomanip>

namespace DDP {
    const char *BigInteger::DivideByZero::what() const noexcept {
        return "Attempt to divide by zero";
    }


    BigInteger::BigInteger(std::string str) {
        if (str.empty()) _isNegative = false;
        else {
            if (str[0] == '-') {
                _isNegative = true;
                str = str.substr(1);
            } else
                _isNegative = false;
            for (auto i = static_cast<long long>(str.length()); i > 0; i -= 9) {
                if (i < 9)
                    _digits.push_back(std::stoi(str.substr(0, i)));
                else
                    _digits.push_back(std::stoi(str.substr(i - 9, 9)));
            }
            _removeLeadingZeros();
        }
    }
    BigInteger::BigInteger(long long ll) {
        if (ll < 0) {
            _isNegative = true;
            ll = -ll;
        } else
            _isNegative = false;
        for (; ll > 0; ll /= BASE)
            _digits.push_back(static_cast<int>(ll % BASE));
    }
    BigInteger::BigInteger(unsigned long long ull) {
        _isNegative = false;
        for (; ull > 0; ull /= BASE)
            _digits.push_back(static_cast<int>(ull % BASE));
    }
    BigInteger::BigInteger(int i) {
        if (i < 0) {
            _isNegative = true;
            i = -i;
        } else
            _isNegative = false;
        if (i != 0)
            _digits.push_back(i);
    }
    BigInteger::BigInteger(unsigned int ui) {
        _isNegative = false;
        for (; ui > 0; ui /= BASE)
            _digits.push_back(static_cast<int>(ui % BASE));
    }

    std::ostream &operator<<(std::ostream &os, const BigInteger &bi) {
        if (bi._digits.empty()) os << 0;
        else {
            if (bi._isNegative)
                os << '-';
            os << bi._digits.back();
            char old_fill = os.fill('0');
            for (long long i = static_cast<long long>(bi._digits.size()) - 2; i >= 0; --i)
                os << std::setw(9) << bi._digits[i];
            os.fill(old_fill);
        }
        return os;
    }


    bool operator==(const BigInteger &left, const BigInteger &right) {
        if (left._isNegative != right._isNegative || left._digits.size() != right._digits.size())
            return false;
        for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i)
            if (left._digits[i] != right._digits[i])
                return false;
        return true;
    }
    bool operator!=(const BigInteger &left, const BigInteger &right) {
        return !(left == right);
    }
    bool operator<(const BigInteger &left, const BigInteger &right) {
        if (left._isNegative != right._isNegative)
            return left._isNegative;
        bool isNegative = left._isNegative;
        if (left._digits.size() != right._digits.size())
            return isNegative ^ (left._digits.size() < right._digits.size());
        for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i)
            if (left._digits[i] != right._digits[i])
                return isNegative ^ (left._digits[i] < right._digits[i]);
        return false;
    }
    bool operator>(const BigInteger &left, const BigInteger &right) {
        return (right < left);
    }
    bool operator<=(const BigInteger &left, const BigInteger &right) {
        return !(left > right);
    }
    bool operator>=(const BigInteger &left, const BigInteger &right) {
        return !(left < right);
    }


    BigInteger BigInteger::operator+() const {
        return *this;
    }
    BigInteger BigInteger::operator-() const {
        BigInteger other(*this);
        other._isNegative = !_isNegative;
        return other;
    }


    BigInteger operator+(const BigInteger &left, const BigInteger &right) {
        if (left._isNegative != right._isNegative)
            return left._isNegative ? right - (-left) : left - (-right);
        bool isLeftFirst = left._digits >= right._digits;
        const BigInteger *ptrSecond = isLeftFirst ? &right : &left;
        BigInteger result = isLeftFirst ? left : right;
        for (size_t i = 0; i < ptrSecond->_digits.size(); ++i)
            result._digits[i] += ptrSecond->_digits[i];
        result._digits.push_back(0);
        for (size_t i = 1; i < result._digits.size(); ++i) {
            if (result._digits[i - 1] >= BigInteger::BASE) {
                result._digits[i - 1] -= BigInteger::BASE;
                ++result._digits[i];
            }
        }
        result._removeLeadingZeros();
        return result;
    }
    BigInteger operator-(const BigInteger &left, const BigInteger &right) {
        if (left._isNegative != right._isNegative)
            return left + (-right);
        bool isLeftFirst = left._isNegative ^ (left > right);
        const BigInteger *ptrSecond = isLeftFirst ? &right : &left;
        BigInteger result = isLeftFirst ? left : right;
        if (!isLeftFirst)
            result._isNegative = !result._isNegative;
        for (size_t i = 0; i < ptrSecond->_digits.size(); ++i)
            result._digits[i] -= ptrSecond->_digits[i];
        for (size_t i = 1; i < result._digits.size(); ++i) {
            if (result._digits[i - 1] < 0) {
                result._digits[i - 1] += BigInteger::BASE;
                --result._digits[i];
            }
        }
        result._removeLeadingZeros();
        return result;
    }


    BigInteger &BigInteger::operator+=(const BigInteger &other) {
        if (_isNegative != other._isNegative)
            return operator-=(-other);
        for (size_t i = 0; i < _digits.size(); ++i)
            _digits[i] += other._digits[i];
        for (size_t i = _digits.size(); i < other._digits.size(); ++i)
            _digits.push_back(other._digits[i]);
        _digits.push_back(0);
        for (size_t i = 1; i < _digits.size(); ++i) {
            if (_digits[i - 1] >= BASE) {
                _digits[i - 1] -= BASE;
                ++_digits[i];
            }
        }
        _removeLeadingZeros();
        return *this;
    }
    BigInteger &BigInteger::operator-=(const BigInteger &other) {
        if (_isNegative != other._isNegative)
            return operator+=(-other);
        for (size_t i = 0; i < _digits.size(); ++i)
            _digits[i] -= other._digits[i];
        for (size_t i = _digits.size(); i < other._digits.size(); ++i)
            _digits.push_back(-other._digits[i]);
        _removeLeadingZeros();
        bool isSignReversed = !_digits.empty() && _digits.back() < 0;
        if (isSignReversed) {
            _isNegative = !_isNegative;
            for (size_t i = 0; i < _digits.size(); ++i) {
                if (_digits[i] > 0) {
                    _digits[i] = BASE - _digits[i];
                    ++_digits[i + 1];
                } else
                    _digits[i] = -_digits[i];
            }
        } else {
            for (size_t i = 0; i < _digits.size(); ++i) {
                if (_digits[i] < 0) {
                    _digits[i] += BASE;
                    --_digits[i + 1];
                }
            }
        }
        _removeLeadingZeros();
        return *this;
    }


    BigInteger &BigInteger::operator++() {
        return operator+=(BigInteger(1));
    }
    BigInteger &BigInteger::operator--() {
        return operator-=(BigInteger(1));
    }


    BigInteger BigInteger::operator++(int) {
        BigInteger copy = *this;
        ++*this;
        return copy;
    }
    BigInteger BigInteger::operator--(int) {
        BigInteger copy = *this;
        --*this;
        return copy;
    }


    BigInteger operator>>(const BigInteger &bi, size_t n) {
        if (bi._digits.empty())
            return 0;
        if (n == 0)
            return bi;
        BigInteger result = bi;
        result._digits.resize(bi._digits.size() + n);
        for (size_t i = result._digits.size() - 1; i >= n; --i)
            result._digits[i] = result._digits[i - n];
        for (size_t i = 0; i < n; ++i)
            result._digits[i] = 0;
        return result;
    }
    BigInteger operator<<(const BigInteger &bi, size_t n) {
        if (bi._digits.size() <= n)
            return 0;
        if (n == 0)
            return bi;
        BigInteger result;
        result._digits.resize(bi._digits.size() - n);
        for (size_t i = 0; i < result._digits.size(); ++i)
            result._digits[i] = bi._digits[i + n];
        return result;
    }


    BigInteger abs(const BigInteger &bi) {
        return bi._isNegative ? -bi : bi;
    }


    BigInteger operator*(const BigInteger &left, const BigInteger &right) {
        BigInteger result;
        result._digits.assign(left._digits.size() + right._digits.size(), 0);
        for (size_t i = 0; i < left._digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < right._digits.size() || carry != 0; ++j) {
                long long cur = result._digits[i + j] + left._digits[i] * 1ll * (j < right._digits.size() ? right._digits[j] : 0) + carry;
                result._digits[i + j] = static_cast<int>(cur % BigInteger::BASE);
                carry = static_cast<int>(cur / BigInteger::BASE);
            }
        }
        result._isNegative = left._isNegative != right._isNegative;
        result._removeLeadingZeros();
        return result;
    }
    BigInteger operator/(const BigInteger &left, int right) {
        if (right == 0)
            throw BigInteger::DivideByZero();
        BigInteger result;
        result._digits.assign(left._digits.size(), 0);
        int carry = 0;
        for (auto i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i) {
            long long cur = left._digits[i] + carry * 1ll * BigInteger::BASE;
            result._digits[i] = int(cur / right);
            carry = int(cur % right);
        }
        result._removeLeadingZeros();
        return result;
    }
    BigInteger operator/(const BigInteger &left, const BigInteger &right) {
        if (right._digits.empty())
            throw BigInteger::DivideByZero();
        if (left._digits.empty() ||  abs(left) < abs(right))
            return 0;
        BigInteger absLeft = abs(left),
                   absRight = abs(right);
        BigInteger l = BigInteger(left._digits.back() / (right._digits.back() + 1)) >> (left._digits.size() - right._digits.size()),
                   r = (BigInteger((left._digits.back() + 1) / right._digits.back()) >> (left._digits.size() - right._digits.size())) + 1;
        for (BigInteger m = (l + r) / 2; l + 1 < r; m = (l + r) / 2)
            m * absRight <= absLeft ? l = m : r = m;
        if (left._isNegative != right._isNegative)
            l._isNegative = true;
        return l;
    }


    void BigInteger::_removeLeadingZeros() {
        while (!_digits.empty() && _digits.back() == 0)
            _digits.pop_back();
        if (_digits.empty())
            _isNegative = false;
    }
}