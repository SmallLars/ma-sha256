#include "general.h"

#include <cstdarg>
#include <iomanip>

using std::ostream;
using std::setfill;
using std::setw;

bool in_array(unsigned needle, unsigned n_args, ...) {
    va_list ap;
    va_start(ap, n_args);
    for (unsigned i = 0; i < n_args; i++) {
        if (needle == va_arg(ap, unsigned)) {
            va_end(ap);
            return true;
        }
    }
    va_end(ap);
    return false;
}

void printTime(ostream& out, unsigned time) {
    out << setfill('0');
    out << setw(2) << time / 3600 << ":";
    out << setw(2) << (time / 60) % 60 << ":";
    out << setw(2) << time % 60;
    out << setfill(' ');
}
