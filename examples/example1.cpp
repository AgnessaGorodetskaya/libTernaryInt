#include <iostream>
#include "../include/libternaryint.hpp"

// Пример использования
int main(void)
{
    Ternary::TernaryInt a = 57934588798797;
    Ternary::TernaryInt b = -32432408797923;
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;  // преобразование в десятичное через приведение типа
    std::cout << "  B = " << b << ' ' << b.to_decimal() << std::endl;  // преобразование в десятичное через метод to_decimal()
    Ternary::TernaryInt c = a + b;
    std::cout << "A+B = " << c << ' ' << int64_t(c) << std::endl;
    c = a - b;
    std::cout << "A-B = " << c << ' ' << int64_t(c) << std::endl;
    c = -b;
    std::cout << " -B = " << c << ' ' << int64_t(c) << std::endl << std::endl;

    a = 12345679;
    b = -8;
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    c = a * b;
    std::cout << "A*B = " << c << ' ' << int64_t(c) << std::endl << std::endl;

    a = 1003;
    b = 5; // 201 -2
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    Ternary::TernaryInt q, r;
    Ternary::TernaryInt::divmod(a, b, q, r); // получение частного и остатка одной операцией
    std::cout << "A/B = " << q << ' ' << int64_t(q) << ' ' << int64_t(r) << std::endl;

    a = -1003;
    b = 5; // -201 +2
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    Ternary::TernaryInt::divmod(a, b, q, r); // получение частного и остатка одной операцией
    std::cout << "A/B = " << q << ' ' << int64_t(q) << ' ' << int64_t(r) << std::endl;

    a = -1003;
    b = -5; // 201 +2
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    Ternary::TernaryInt::divmod(a, b, q, r); // получение частного и остатка одной операцией
    std::cout << "A/B = " << q << ' ' << int64_t(q) << ' ' << int64_t(r) << std::endl;

    a = 1003;
    b = -5; // -201 -2
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    Ternary::TernaryInt::divmod(a, b, q, r); // получение частного и остатка одной операцией
    std::cout << "A/B = " << q << ' ' << int64_t(q) << ' ' << int64_t(r) << std::endl;
    q = a / b;  // когда остаток не нужен
    r = a % b;  // отдельно остаток
    std::cout << "A/B = " << q << ' ' << int64_t(q) << ' ' << int64_t(r) << std::endl << std::endl;

    a = -11;
    b = -12;
    std::cout << "  A = " << a << ' ' << int64_t(a) << std::endl;
    std::cout << "  B = " << b << ' ' << int64_t(b) << std::endl;
    if (a >= b) {
        std::cout << "Compare result: A >= B" << std::endl;
    } else {
        std::cout << "Compare result: A < B" << std::endl;
    }
    a <<= 3;
    std::cout << "A<<3= " << a << ' ' << int64_t(a) << std::endl;
    b >>= 2;
    std::cout << "B>>2= " << b << ' ' << int64_t(b) << std::endl;

    return 0;
}
