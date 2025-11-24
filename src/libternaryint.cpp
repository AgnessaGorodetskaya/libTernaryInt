#include "../include/libternaryint.hpp"

namespace Ternary
{

// single_add: складывает два набора тритов a и b (каждый задан парами битов minus/plus)
// Возвращает результат в res_minus/res_plus и переносы (не сдвинутые) в carry_minus/carry_plus
// Все операции — побитовые, одновременно для 64 тритов
// Входные пары не содержат код 11, который не должен появляться в корректных данных
void
TernaryInt::single_add(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                       uint64_t &res_plus, uint64_t &carry_minus, uint64_t &carry_plus)
{
    // нулевые маски
    uint64_t a0 = ~(a_minus | a_plus);
    uint64_t b0 = ~(b_minus | b_plus);

    // результирующие биты
    res_minus = (a_plus & b_plus) // +1 + +1 -> -1 (перенос carry+)
                | (a_minus & b0)  // -1 +  0 -> -1
                | (a0 & b_minus); //  0 + -1 -> -1

    res_plus = (a_minus & b_minus) // -1 + -1 -> +1 (перенос carry-)
               | (a0 & b_plus)     //  0 + +1 -> +1
               | (a_plus & b0);    // +1 +  0 -> +1

    // первичные переносы, не сдвинутые
    carry_minus = a_minus & b_minus; // -1 + -1 -> carry -1 в следующий трит
    carry_plus = a_plus & b_plus;    // +1 + +1 -> carry +1 в следующий трит
}

// add_trits: складывает a и b (любые 64-тритовые числа) и возвращает результат
// Все аргументы — uint64_t, где i-бит соответствует i-триту
void
TernaryInt::add_trits(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                      uint64_t &res_plus)
{
    // Первичная сумма и переносы
    uint64_t c_minus, c_plus;
    single_add(a_minus, a_plus, b_minus, b_plus, res_minus, res_plus, c_minus, c_plus);

    // Итеративно добавляем переносы, пока они не исчезнут
    while (c_minus | c_plus) {
        // На каждой итерации перенос сдвигается на 1 трит в старший разряд
        c_minus <<= 1;
        c_plus <<= 1;
        // Затем мы складываем его с текущим результатом (используем single_add)
        // получая новый результат и новые переносы
        single_add(res_minus, res_plus, c_minus, c_plus, res_minus, res_plus, c_minus, c_plus);
    }
}

// mul_trits: умножает a и b (любые 64-тритовые числа) и возвращает результат
// Все аргументы — uint64_t, где i-бит соответствует i-триту
void
TernaryInt::mul_trits(uint64_t a_minus, uint64_t a_plus, uint64_t b_minus, uint64_t b_plus, uint64_t &res_minus,
                      uint64_t &res_plus)
{
    res_minus = res_plus = 0;
    for (int i = 0; i < BITS; ++i) {
        bool bm = (b_minus >> i) & 1;
        bool bp = (b_plus >> i) & 1;

        if (bp) {
            // b_i = 1 -> прибавить a * 3^i
            add_trits(res_minus, res_plus, a_minus, a_plus, res_minus, res_plus);
        } else if (bm) {
            // b_i = -1 -> вычесть a * 3^i
            add_trits(res_minus, res_plus, a_plus, a_minus, res_minus, res_plus);
        }

        // Сдвигаем a на один трит
        a_minus <<= 1;
        a_plus <<= 1;
    }
}

TernaryInt::TernaryInt(int64_t decimal) // Конструктор из десятичного представления
{
    for (int i = 0; decimal != 0 && i < BITS; ++i) {
        int trit = static_cast<int>(decimal % 3);
        if (trit < 0) {
            trit += 3; // для отрицательных чисел корректируем остаток
        }
        switch (trit) {
        case 0: // трит = 0
            decimal /= 3;
            break;
        case 1: // трит = +1
            _plus |= (1ULL << i);
            decimal = (decimal - 1) / 3;
            break;
        case 2: // трит = -1
            _minus |= (1ULL << i);
            decimal = (decimal + 1) / 3;
            break;
        default:
            break;
        }
    }
}

int64_t
TernaryInt::to_decimal() const // Преобразование в десятичное число
{
    int64_t res = 0;
    int64_t power = 1; // 3^i = 3^0
    for (int i = 0; i < BITS && power > 0; ++i) {
        bool m = (_minus >> i) & 1;
        bool p = (_plus >> i) & 1;
        if (m) {
            res -= power;
        } else if (p) {
            res += power;
        }
        power *= 3;
    }
    return res;
}

TernaryInt::
operator int64_t() const
{
    return to_decimal();
}

int
TernaryInt::compare(const TernaryInt &b) const // сравнение
{
    for (int i = BITS - 1; i >= 0; --i) {
        int am = (_minus >> i) & 1;
        int ap = (_plus >> i) & 1;
        int bm = (b._minus >> i) & 1;
        int bp = (b._plus >> i) & 1;

        // код трита: -1, 0, +1
        int av = ap - am;
        int bv = bp - bm;

        if (av < bv) {
            return -1;
        }
        if (av > bv) {
            return +1;
        }
    }
    return 0;
}

TernaryInt
TernaryInt::abs() const // абсолютное значение
{
    return (compare(0) < 0 ? -(*this) : (*this));
}

TernaryInt &
TernaryInt::operator+=(const TernaryInt &y) // this += y
{
    add_trits(_minus, _plus, y._minus, y._plus, _minus, _plus);
    return *this;
}

TernaryInt
TernaryInt::operator+(const TernaryInt &y) const // res = this + y
{
    TernaryInt res{*this};
    res += y;
    return res;
}

TernaryInt &
TernaryInt::operator-=(const TernaryInt &y) // this -= y
{
    // сложение с -y (меняем местами y_plus и y_minus)
    add_trits(_minus, _plus, y._plus, y._minus, _minus, _plus);
    return *this;
}

TernaryInt
TernaryInt::operator-(const TernaryInt &y) const // res = this - y
{
    TernaryInt res{*this};
    res -= y;
    return res;
}

TernaryInt
TernaryInt::operator-() const // Унарный минус res = -this
{
    TernaryInt res{};
    res._minus = _plus; // меняем местами _plus и _minus
    res._plus = _minus;
    return res;
}

TernaryInt &
TernaryInt::operator*=(const TernaryInt &y) // this *= y
{
    mul_trits(_minus, _plus, y._minus, y._plus, _minus, _plus);
    return *this;
}

TernaryInt
TernaryInt::operator*(const TernaryInt &y) const // res = this * y
{
    TernaryInt res{*this};
    res *= y;
    return res;
}

void
TernaryInt::divmod(const TernaryInt &dividend, const TernaryInt &divisor, TernaryInt &quotient,
                   TernaryInt &remainder) // деление с остатком
{
    // заглушка с конвертацией в десятичную, так как код ниже пока не работает

    int64_t dividend_dec = dividend.to_decimal();
    int64_t divisor_dec = divisor.to_decimal();
    quotient = dividend_dec / divisor_dec;
    remainder = dividend_dec % divisor_dec;

    // if (divisor.compare(0) == 0) {
    //     std::fprintf(stderr, "Division by zero\n");
    //     std::abort();
    // }

    // // определяем знак результата
    // bool neg_result = (dividend.compare(0) < 0) ^ (divisor.compare(0) < 0);

    // TernaryInt a = dividend.abs();
    // TernaryInt d = divisor.abs();

    // quotient._minus = quotient._plus = 0;
    // remainder._minus = remainder._plus = 0;

    // // идём по тритам сверху вниз
    // for (int i = BITS - 1; i >= 0; --i) {

    //     // remainder *= 3 (сдвиг на один трит)
    //     remainder._minus <<= 1;
    //     remainder._plus <<= 1;

    //     // remainder += трит (digit) из числителя
    //     bool am = (a._minus >> i) & 1;
    //     bool ap = (a._plus >> i) & 1;
    //     if (ap) {
    //         remainder += 1;
    //     } else if (am) {
    //         remainder -= 1;
    //     }

    //     // теперь надо определить, какой трит поставить в частное
    //     // пробуем remainder >= divisor
    //     if (remainder.compare(d) >= 0) {
    //         remainder -= d;
    //         quotient._plus |= (1ULL << i); // q_i = +1
    //     }
    //     // пробуем remainder <= -divisor
    //     else if (remainder.compare(-d) <= 0) {
    //         remainder += d;
    //         quotient._minus |= (1ULL << i); // q_i = -1
    //     }
    //     // иначе q_i = 0
    // }

    // // установка знака результата
    // if (neg_result) {
    //     quotient = -quotient;
    // }
}

TernaryInt &
TernaryInt::operator/=(const TernaryInt &y) // this /= y
{
    TernaryInt remainder;
    divmod(*this, y, *this, remainder);
    return *this; // quotient
}

TernaryInt
TernaryInt::operator/(const TernaryInt &y) const // res = this / y
{
    TernaryInt res{*this};
    res /= y;
    return res;
}

TernaryInt &
TernaryInt::operator%=(const TernaryInt &y) // this %= y
{
    TernaryInt quotient;
    divmod(*this, y, quotient, *this);
    return *this; // reminder
}

TernaryInt
TernaryInt::operator%(const TernaryInt &y) const // res = this % y
{
    TernaryInt res{*this};
    res %= y;
    return res;
}

TernaryInt &
TernaryInt::operator<<=(const int shift) // this <<= y
{
    _minus <<= shift;
    _plus <<= shift;
    return *this;
}

TernaryInt
TernaryInt::operator<<(const int shift) const // res = this << y
{
    TernaryInt res{*this};
    res <<= shift;
    return res;
}

TernaryInt &
TernaryInt::operator>>=(const int shift) // this >>= y
{
    _minus >>= shift;
    _plus >>= shift;
    return *this;
}

TernaryInt
TernaryInt::operator>>(const int shift) const // res = this >> y
{
    TernaryInt res{*this};
    res >>= shift;
    return res;
}

bool
TernaryInt::operator==(const TernaryInt &y) const // this == y ? true : false
{
    return (compare(y) == 0) ? true : false;
}

bool
TernaryInt::operator!=(const TernaryInt &y) const // this != y ? true : false
{
    return (compare(y) != 0) ? true : false;
}

bool
TernaryInt::operator<(const TernaryInt &y) const // this < y ? true : false
{
    return (compare(y) < 0) ? true : false;
}

bool
TernaryInt::operator<=(const TernaryInt &y) const // this <= y ? true : false
{
    return (compare(y) <= 0) ? true : false;
}

bool
TernaryInt::operator>(const TernaryInt &y) const // this > y ? true : false
{
    return (compare(y) > 0) ? true : false;
}

bool
TernaryInt::operator>=(const TernaryInt &y) const // this >= y ? true : false
{
    return (compare(y) >= 0) ? true : false;
}

// Печать 64-тритового числа в удобочитаемой форме
std::ostream &
operator<<(std::ostream &os, const TernaryInt &t)
{
    for (int i = BITS - 1; i >= 0; --i) {
        uint64_t m = (t._minus >> i) & 1ULL;
        uint64_t p = (t._plus >> i) & 1ULL;
        if (m && !p) {
            os << "-";
        } else if (!m && !p) {
            os << "0";
        } else if (!m && p) {
            os << "+";
        } else {
            os << "?"; // код 11 запрещён
        }
    }
    return os;
}
}; // namespace Ternary
