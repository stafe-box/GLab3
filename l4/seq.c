#include <stdio.h>
#include <math.h>

int main()
{
    long double n;
    long double sum = 0;
    long double h = 0.02;
    long double old_I = 0;
    long double x = 0;

    n=1e-15;
    do
    {
        old_I = sum;
        sum = 0;
        h /= 2.0;
        for (int i = 0; i < 2 / h; i++)
        {
            x = i * h + h / 2.0;
            if (i * h <= 1)
                sum += cos(x + pow(x, 3));
            else
                sum += exp(-x * x) - x * x + 2 * x;
        }
        sum *= h;
    } while (fabs(sum - old_I) / 3.0 >= n);
    printf("%.20Lf\n", sum);
    return 0;
}