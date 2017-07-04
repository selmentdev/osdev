#include "../crt.h"

int (abs)(int n) {
    return (n < 0) ? (-n) : (n);
}

long int (labs)(long int n) {
    return (n < 0) ? (-n) : (n);
}

long long int (llabs)(long long int n) {
    return (n < 0) ? (-n) : (n);
}
