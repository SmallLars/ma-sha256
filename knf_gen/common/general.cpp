#include "general.h"

#include <cstdarg>

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
