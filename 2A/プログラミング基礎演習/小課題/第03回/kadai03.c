#include <stdio.h>
#include <math.h>

double f(double);
double dfdx(double);
double newton(double);

int main() {
    double x0 = 1.0;
    double solution = newton(x0);
    printf("solution is %f\n", solution);
    return 0;
}

double f(double x) {
    return pow(exp(1.0), x) + x;
}

double dfdx(double x) {
    return pow(exp(1.0), x) + 1;
}

double newton(double xn) {
    double xn1 = xn - f(xn) / dfdx(xn);
    if (fabs(xn1 - xn) > 0.0001) {
        return newton(xn1);
    } else {
        return xn1;
    }
}
