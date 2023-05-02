#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NTHREADS 10       // Количество потоков
#define CIRCLE_RADIUS 10 // Радиус круга
#define SIDE 1000         // Сторона квадрата

// Структура для хранения координат центра очередного круга
struct circle
{
    int x;
    int y;
};

// Массив координат центров кругов
struct circle circles[SIDE * SIDE];
int current = 0;

// Функция для вычисления расстояния между двумя точками
double distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Функция для проверки, можно ли разместить круг в данной позиции
int canPlace(int x, int y)
{
    for (int i = 0; i < current; i++)
        if (distance(x, y, circles[i].x, circles[i].y) < 2 * CIRCLE_RADIUS)
            return 0;
    return 1;
}

// Функция для каждого потока
void *thread_func(void *arg)
{
    int tid = *((int *)arg); // ID потока

    for (int i = tid; i < SIDE * SIDE; i += NTHREADS)
    {
        int x = i % SIDE;
        int y = i / SIDE;

        if (canPlace(x, y))
        {
            circles[current].x = x;
            circles[current].y = y;
            current++;
        }
    }
}

int main()
{
    pthread_t threads[NTHREADS];
    int tids[NTHREADS];

    for (int i = 0; i < NTHREADS; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Количество размещенных кругов: %d\n", current);
}