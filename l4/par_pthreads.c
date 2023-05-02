#include <stdio.h>
#include <math.h>
#include <pthread.h>

long double f(long double x)
{
    if (x <= 1)
    {
        return cos(x + pow(x, 3));
    }
    return exp(-x * x) - x * x + 2 * x;
}

typedef struct
{
    long double a;
    long double b;
    long double eps;
    long double sum;
    int thread_id;
} IntegralParams;

pthread_mutex_t mutex;

void *calculate_integral(void *arg)
{
    IntegralParams *params = (IntegralParams *)arg;
    long double a = params->a;
    long double b = params->b;
    long double eps = params->eps;
    int thread_id = params->thread_id;

    int n = 1;
    long double h = (b - a) / n;
    long double prev_sum = 0.0;
    params->sum = 0.0;

    do
    {
        prev_sum = params->sum;
        params->sum = 0.0;

        for (int i = 0; i < n; i++)
        {
            long double x = a + (i + 0.5) * h;
            long double partial_sum = f(x) * h;
            params->sum += partial_sum;
        }

        n *= 2;
        h /= 2;
    } while (fabs(params->sum - prev_sum) > eps);

    pthread_mutex_lock(&mutex);
    IntegralParams *global_params = (IntegralParams *)arg;
    global_params->sum += params->sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    long double a = 0.0;
    long double b = 2.0;
    long double eps = 1e-15;
    int num_threads = 10;

    pthread_mutex_init(&mutex, NULL);

    IntegralParams params[num_threads];
    pthread_t threads[num_threads];

    long double subinterval = (b - a) / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        params[i].a = a + i * subinterval;
        params[i].b = a + (i + 1) * subinterval;
        params[i].eps = eps;
        params[i].sum = 0.0;
        params[i].thread_id = i;
        pthread_create(&threads[i], NULL, calculate_integral, (void *)&params[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    long double result = 0.0;
    for (int i = 0; i < num_threads; i++)
    {
        result += params[i].sum;
    }
    result /= 2;
    printf("result: %.20Lf\n", result);

    return 0;
}
