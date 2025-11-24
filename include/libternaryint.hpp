#ifndef LIBTERNARY_HPP
#define LIBTERNARY_HPP

#include <cstdint>
#include <iostream>

#pragma once

namespace Ternary
{
constexpr int BITS = 64;

// Операции над троичными числами по 64 трита (каждый трит = 2 бита: minus, plus)
// Код трита: 0 = 00, +1 = 01, -1 = 10. Код 11 запрещён
class TernaryInt
{
  protected:
    uint64_t _minus{}, _plus{};

    // single_add: складывает два набора тритов a и b (каждый задан парами битов minus/plus)
    // Возвращает результат в res_minus/res_plus и переносы (не сдвинутые) в carry_minus/carry_plus
    // Все операции — побитовые, одновременно для 64 тритов
    // Входные пары не содержат код 11, который не должен появляться в корректных данных
    static void single_add(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                           uint64_t &res_plus, uint64_t &carry_minus, uint64_t &carry_plus);

    // add_trits: складывает a и b (любые 64-тритовые числа) и возвращает результат
    // Все аргументы — uint64_t, где i-бит соответствует i-триту
    static void add_trits(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                          uint64_t &res_plus);

    // mul_trits: умножает a и b (любые 64-тритовые числа) и возвращает результат
    // Все аргументы — uint64_t, где i-бит соответствует i-триту
    static void mul_trits(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                          uint64_t &res_plus);

  public:
    TernaryInt() {};                             // Пустой конструктор
    TernaryInt(int64_t decimal);                 // Конструктор из десятичного представления
    int64_t to_decimal() const;                  // Преобразование в десятичное число
    explicit operator int64_t() const;           // Преобразование в десятичное в виде оператора приведения к int64_t
    int compare(const TernaryInt &b) const;      // сравнение -1 0 +1
    TernaryInt abs() const;                      // абсолютное значение
    TernaryInt &operator+=(const TernaryInt &y); // this += y
    TernaryInt operator+(const TernaryInt &y) const; // res = this + y
    TernaryInt &operator-=(const TernaryInt &y);     // this -= y
    TernaryInt operator-(const TernaryInt &y) const; // res = this - y
    TernaryInt operator-() const;                    // Унарный минус res = -this
    TernaryInt &operator*=(const TernaryInt &y);     // this *= y
    TernaryInt operator*(const TernaryInt &y) const; // res = this * y
    static void divmod(const TernaryInt &dividend, const TernaryInt &divisor, TernaryInt &quotient,
                       TernaryInt &remainder);       // деление с остатком
    TernaryInt &operator/=(const TernaryInt &y);     // this /= y
    TernaryInt operator/(const TernaryInt &y) const; // res = this / y
    TernaryInt &operator%=(const TernaryInt &y);     // this %= y
    TernaryInt operator%(const TernaryInt &y) const; // res = this % y
    TernaryInt &operator<<=(const int shift);        // this <<= shift
    TernaryInt operator<<(const int shift) const;    // res = this << shift
    TernaryInt &operator>>=(const int shift);        // this >>= shift
    TernaryInt operator>>(const int shift) const;    // res = this >> shift
    bool operator==(const TernaryInt &y) const;      // this == y ? true : false
    bool operator!=(const TernaryInt &y) const;      // this != y ? true : false
    bool operator<(const TernaryInt &y) const;       // this < y ? true : false
    bool operator<=(const TernaryInt &y) const;      // this <= y ? true : false
    bool operator>(const TernaryInt &y) const;       // this > y ? true : false
    bool operator>=(const TernaryInt &y) const;      // this >= y ? true : false
    friend std::ostream &operator<<(std::ostream &os,
                                    const TernaryInt &t); // Печать 64-тритового числа в удобочитаемой форме
};

} // namespace Ternary

#endif // LIBTERNARY_HPP
