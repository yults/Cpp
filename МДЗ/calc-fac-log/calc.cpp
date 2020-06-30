#include "calc.h"

#include <cctype> // for std::isspace
#include <cmath> // various math functions
#include <iostream> // for error reporting via std::cerr

namespace {

const double maxRelDiff = 2.20e-16;
	
bool is_equal(double A, double B)
{
    double diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    double largest = (B > A) ? B : A;
    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}
	
const std::size_t max_decimal_digits = 10;

enum class Op 
{
      ERR
    , SET
    , ADD
    , SUB
    , MUL
    , DIV
    , REM
    , NEG
    , POW
    , SQRT
    , LOG
    , FAC
};

std::size_t arity(const Op op)
{
    switch (op) {
        // error
        case Op::ERR: return 0;
        // unary
        case Op::NEG: return 1;
        case Op::SQRT: return 1;
        case Op::FAC: return 1;
        // binary
        case Op::SET: return 2;
        case Op::ADD: return 2;
        case Op::SUB: return 2;
        case Op::MUL: return 2;
        case Op::DIV: return 2;
        case Op::REM: return 2;
        case Op::POW: return 2;
        case Op::LOG: return 2;
    }
    return 0;
}

Op parse_op(const std::string & line, std::size_t & i)
{
    const auto rollback = [&i, &line] (const std::size_t n) {
        i -= n;
        std::cerr << "Unknown operation " << line << std::endl;
        return Op::ERR;
    };
    switch (line[i++]) {
        case '0': [[fallthrough]];
        case '1': [[fallthrough]];
        case '2': [[fallthrough]];
        case '3': [[fallthrough]];
        case '4': [[fallthrough]];
        case '5': [[fallthrough]];
        case '6': [[fallthrough]];
        case '7': [[fallthrough]];
        case '8': [[fallthrough]];
        case '9':
            --i; // a first digit is a part of op's argument
            return Op::SET;
        case '+':
            return Op::ADD;
        case '-':
            return Op::SUB;
        case '*':
            return Op::MUL;
        case '/':
            return Op::DIV;
        case '%':
            return Op::REM;
        case '_':
            return Op::NEG;
        case '^':
            return Op::POW;
        case '!':
        	return Op::FAC;
        case 'L':
                switch (line[i++]) {
                    case 'O':
                        switch (line[i++]) {
                            case 'G':
                                return Op::LOG;
                            default:
                                return rollback(3);
                        }
                    default:
                        return rollback(2);
                }
        case 'S':
                switch (line[i++]) {
                    case 'Q':
                        switch (line[i++]) {
                            case 'R':
                                switch (line[i++]) {
                                    case 'T':
                                        return Op::SQRT;
                                    default:
                                        return rollback(4);
                                }
                            default:
                                return rollback(3);
                        }
                    default:
                        return rollback(2);
                }
        default:
                return rollback(1);
    }
}

std::size_t skip_ws(const std::string & line, std::size_t i)
{
    while (i < line.size() && std::isspace(line[i])) {
        ++i;
    }
    return i;
}

double parse_arg(const std::string & line, std::size_t & i)
{
    double res = 0;
    std::size_t count = 0;
    bool good = true;
    bool integer = true;
    double fraction = 1;
    bool is_negative = false;
    while (good && i < line.size() && count < max_decimal_digits) {
        switch (line[i]) {
            case '-':
                is_negative = true;
                i++;
                break;
        }
        switch (line[i]) {
            case '0': [[fallthrough]];
            case '1': [[fallthrough]];
            case '2': [[fallthrough]];
            case '3': [[fallthrough]];
            case '4': [[fallthrough]];
            case '5': [[fallthrough]];
            case '6': [[fallthrough]];
            case '7': [[fallthrough]];
            case '8': [[fallthrough]];
            case '9':
                if (integer) {
                    res *= 10;
                    res += line[i] - '0';
                }
                else {
                    fraction /= 10;
                    res += (line[i] - '0') * fraction;
                }
                ++i;
                ++count;
                break;
            case '.':
                integer = false;
                ++i;
                break;
            default:
                good = false;
                break;
        }
    }
    if (i < line.size()) {
        std::cerr << "Argument isn't fully parsed, suffix left: '" << line.substr(i) << "'" << std::endl;
    }
    if (is_negative) res = -res;
    is_negative = false;
    return res;
}

double unary(const double current, const Op op)
{
    switch (op) {
        case Op::NEG:
            return -current;
        case Op::SQRT:
            if (current > 0) {
                return std::sqrt(current);
            }
            else {
                std::cerr << "Bad argument for SQRT: " << current << std::endl;
                return current;
            }
         case Op::FAC:
        	if (current > 0 && is_equal(current, round(current))) {
                double fac = 1;
                for (int i = 1; i <= current; i++)
                    fac *= i;
                return fac;
            }
            else {
                std::cerr << "Bad argument for factorial: " << current << std::endl;
                return current;
            }
        default:
            return current;
    }
}

double binary(const Op op, const double left, const double right)
{
    switch (op) {
        case Op::SET:
            return right;
        case Op::ADD:
            return left + right;
        case Op::SUB:
            return left - right;
        case Op::MUL:
            return left * right;
        case Op::DIV:
            if (right != 0) {
                return left / right;
            }
            else {
                std::cerr << "Bad right argument for division: " << right << std::endl;
                return left;
            }
        case Op::REM:
            if (right != 0) {
                return std::remainder(left, right);
            }
            else {
                std::cerr << "Bad right argument for remainder: " << right << std::endl;
                return left;
            }
        case Op::POW:
            return std::pow(left, right);
        case Op::LOG:
        	if (left <= 0) {
                std::cerr << "Bad argument for logarithm: " << left << std::endl;
                return left;
            }
            else {
                if (right <= 0 || is_equal(right, 1)) {
                    std::cerr << "Bad base for logarithm: " << right << std::endl;
                    return left;
                }
                else
                {
                    return log(left)/log(right);
                }
            }
        default:
            return left;
    }
}

} // anonymous namespace

double process_line(const double current, const std::string & line)
{
    std::size_t i = 0;
    const auto op = parse_op(line, i);
    switch (arity(op)) {
        case 2: {
                    i = skip_ws(line, i);
                    const auto arg = parse_arg(line, i);
                    return binary(op, current, arg);
                }
        case 1: return unary(current, op);
        default: break;
    }
    return current;
}
