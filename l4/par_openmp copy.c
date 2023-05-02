#include <stdio.h>
#include <math.h>
#include <omp.h>

long double f(long double x)
{
    if (x <= 1)
    {
        return cos(x + pow(x, 3));
    }
    return exp(-x * x) - x * x + 2 * x;
}

long double calculate_integral(long double a, long double b, long double eps)
{
    int n = 1;
    long double h = (b - a) / n;
    long double prev_sum = 0.0;
    long double sum = 0.0;

    do
    {
        prev_sum = sum;
        sum = 0.0;

#pragma omp parallel for reduction(+ \
                                   : sum)
        for (int i = 0; i < n; i++)
        {
            long double x = a + (i + 0.5) * h;
            long double partial_sum = f(x) * h;
            sum += partial_sum;
        }

        n *= 2;
        h /= 2;
    } while (fabs(sum - prev_sum) > eps);

    return sum;
}

int main()
{
    long double a = 0.0;
    long double b = 2.0;
    long double eps = 1e-15;

    long double result = calculate_integral(a, b, eps);

    printf("result: %.20Lf\n", result);
    return 0;
}