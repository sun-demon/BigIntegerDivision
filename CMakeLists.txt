cmake_minimum_required(VERSION 3.23)
project(BigIntegerDivision)

set(CMAKE_CXX_STANDARD 14)

add_executable(BigIntegerDivision main.cpp
        BigInteger/BigInteger.cpp
        BigInteger/BigInteger.h
)