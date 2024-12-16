#include "pch.h"
#include "framework.h"
#include <iostream>

extern "C" {
    unsigned int __stdcall Sum(unsigned int firstOperand, unsigned int secondOperand) {
        return firstOperand + secondOperand;
    }
    unsigned int __stdcall Pow(unsigned int operand, unsigned int power) {
        if (power == 0) {
            return 1;
        }
        if (power < 0) {
            return 0;
        }
        int result = 1;
        for (int i = 0; i < power; i++) {
            result *= operand;
        }
        return result;
    }







    unsigned int __stdcall StrCmp(const char* s1, const char* s2) {
        return strcmp(s1, s2);
    }

    void __stdcall writestr(const char* str) {
        std::cout << str << std::endl;
    }

    void __stdcall writechar(char ch) {
        std::cout << ch << std::endl;
    }

    void __stdcall writeint(unsigned int n) {
        std::cout << n << std::endl;
    }

    void __stdcall writebool(bool b) {
        if (b) {
            std::cout << "true" << std::endl;
        }
        else {
            std::cout << "false" << std::endl;
        }
    }
}
