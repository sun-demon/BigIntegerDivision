#ifndef BIGINTEGERDIVISION_BIGINTEGER_H
#define BIGINTEGERDIVISION_BIGINTEGER_H


#include <string>
#include <vector>

namespace DDP {
    class BigInteger {
    public:
        class DivideByZero : public std::exception {
        public:
            const char * what() const noexcept override;
        };

        explicit BigInteger(std::string str = "");
//        explicit BigInteger(signed long long ll);
//        explicit BigInteger(unsigned long long ull);
        explicit BigInteger(int i);
//        explicit BigInteger(unsigned int ui);

        friend std::ostream &operator<<(std::ostream &os, const BigInteger &bi);

        friend bool operator==(const BigInteger &left, const BigInteger &right);
        friend bool operator!=(const BigInteger &left, const BigInteger &right);
        friend bool operator<(const BigInteger &left, const BigInteger &right);
        friend bool operator>(const BigInteger &left, const BigInteger &right);
        friend bool operator<=(const BigInteger &left, const BigInteger &right);
        friend bool operator>=(const BigInteger &left, const BigInteger &right);

        BigInteger operator+() const;
        BigInteger operator-() const;

        friend BigInteger operator+(const BigInteger &left, const BigInteger &right);
        friend BigInteger operator-(const BigInteger &left, const BigInteger &right);

        BigInteger &operator+=(const BigInteger &other);
        BigInteger &operator-=(const BigInteger &other);

        BigInteger &operator++();
        BigInteger &operator--();

        BigInteger operator++(int);
        BigInteger operator--(int);

        friend BigInteger operator>>(const BigInteger &bi, size_t n);
        friend BigInteger operator<<(const BigInteger &bi, size_t n);

        friend BigInteger abs(const BigInteger &bi);

        friend BigInteger operator*(const BigInteger &left, const BigInteger &right);
        friend BigInteger operator/(const BigInteger &left, int right);
        friend BigInteger operator/(const BigInteger &left, const BigInteger &right);
        friend BigInteger operator%(const BigInteger &left, const BigInteger &right);

        std::string toString() const;
//
//        bool odd() const;
//
//        bool even() const;
//
//        BigInteger pow(const BigInteger &n) const;

        static const int BASE = 1000000000;

    private:
        void _removeLeadingZeros();

        std::vector<int> _digits;
        bool _isNegative;
    };
}

#endif //BIGINTEGERDIVISION_BIGINTEGER_H
