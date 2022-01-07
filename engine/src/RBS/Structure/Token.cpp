#include <motor/RBS/Structure/Token.h>

#include <motor/RBS/VectorPrinter.h>

void Token::print(int level) const {
    VectorPrinter::print(*this, level);
}
