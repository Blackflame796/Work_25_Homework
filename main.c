#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define N 20
#define R 20
#define MAX_RANGE 100

// Функция для генерации случайного числа в диапазоне от min до max
int random_range(int min, int max)
{
    int result = rand() % (max - min + 1) + min;
    return result;
}

// Функция для генерации массива случайных чисел
int *generate_random_array(int *array_ptr, int min_val, int max_val)
{
    for (int i = 0; i < N; i++)
    {
        array_ptr[i] = random_range(min_val, max_val);
    }
    return array_ptr;
}

// Функция для нахождения максимального значения в массиве
int find_max(int *array_ptr)
{
    int max_val = array_ptr[0];
    for (int i = 1; i < N; i++)
    {
        if (array_ptr[i] > max_val)
        {
            max_val = array_ptr[i];
        }
    }
    return max_val;
}

// Функция для нахождения минимального значения в массиве
int find_min(int *array_ptr)
{
    int min_val = array_ptr[0];
    for (int i = 1; i < N; i++)
    {
        if (array_ptr[i] < min_val)
        {
            min_val = array_ptr[i];
        }
    }
    return min_val;
}

// Функция для нахождения индекса максимального значения
int find_max_index(int *array_ptr)
{
    int max_index = 0;
    int max_val = array_ptr[0];
    for (int i = 1; i < N; i++)
    {
        if (array_ptr[i] > max_val)
        {
            max_val = array_ptr[i];
            max_index = i;
        }
    }
    return max_index;
}

// Функция для нахождения индекса минимального значения
int find_min_index(int *array_ptr)
{
    int min_index = 0;
    int min_val = array_ptr[0];
    for (int i = 1; i < N; i++)
    {
        if (array_ptr[i] < min_val)
        {
            min_val = array_ptr[i];
            min_index = i;
        }
    }
    return min_index;
}

// Функция для подсчета частот
int count_frequencies(int *array_ptr, int min_val, int max_val, int *counter_ptr)
{
    int range_size = max_val - min_val + 1;

    // Обнуляем счетчики
    for (int i = 0; i < range_size; i++)
    {
        counter_ptr[i] = 0;
    }

    // Подсчитываем частоты
    for (int i = 0; i < N; i++)
    {
        if (array_ptr[i] >= min_val && array_ptr[i] <= max_val)
        {
            counter_ptr[array_ptr[i] - min_val]++;
        }
    }

    // Находим максимальную частоту
    int max_count = 0;
    for (int i = 0; i < range_size; i++)
    {
        if (counter_ptr[i] > max_count)
        {
            max_count = counter_ptr[i];
        }
    }
    return max_count; // максимальная частота
}

// Функция для вычисления высот столбцов гистограммы
int *calculate_heights(int *counter_ptr, int *heights_ptr, int range_size, int r, int max_count)
{
    for (int i = 0; i < range_size; i++)
    {
        if (max_count > 0)
        {
            heights_ptr[i] = (int)((float)counter_ptr[i] / max_count * r);
        }
        else
        {
            heights_ptr[i] = 0;
        }
        // Гарантируем хотя бы один символ для ненулевых значений
        if (counter_ptr[i] > 0 && heights_ptr[i] == 0)
        {
            heights_ptr[i] = 1;
        }
    }
    return heights_ptr;
}

int *shift_left_from_extreme(int *array_ptr, int type)
{
    if (N <= 1)
        return array_ptr;

    int start_index;

    // Определение стартового индекса в зависимости от типа
    if (type == 0)
    {
        start_index = find_min_index(array_ptr);
    }
    else
    {
        start_index = find_max_index(array_ptr);
    }

    // Если стартовый индекс последний, сдвиг не нужен
    if (start_index >= N - 1)
        return array_ptr;

    // Сдвиг элементов влево начиная со стартового индекса
    for (int i = start_index; i < N - 1; i++)
    {
        array_ptr[i] = array_ptr[i + 1];
    }

    // Последний элемент обнуляем
    array_ptr[N - 1] = 0;
    return array_ptr;
}

void print_array(int *array_ptr)
{
    printf("[");
    for (int i = 0; i < N; i++)
    {
        printf(" %d ", array_ptr[i]);
    }
    printf("]\n");
}

void print_vertical_histogram(int *counter_ptr, int *heights_ptr, int min_val, int range_size, int r)
{
    // Рисуем гистограмму сверху вниз
    for (int level = r; level > 0; level--)
    {
        printf("    ");
        for (int i = 0; i < range_size; i++)
        {
            if (heights_ptr[i] >= level)
            {
                printf(" *  ");
            }
            else
            {
                printf("    ");
            }
        }
        printf("\n");
    }

    // Рисуем ось X с значениями
    printf("    ");
    for (int i = 0; i < range_size; i++)
    {
        printf("%2d  ", i + min_val);
    }
    printf("\n");

    // Рисуем ось X с частотами
    printf("    ");
    for (int i = 0; i < range_size; i++)
    {
        printf("(%d) ", counter_ptr[i]);
    }
    printf("\n");
}

int main()
{
    setlocale(LC_ALL, "RUS");
    /* объявление необходимых переменных */
    int min_range, max_range;
    int array[N];
    int *array_ptr = &array;
    int counter_ptr[MAX_RANGE];
    int heights[MAX_RANGE];
    int *heights_ptr = &heights;
    int extreme_index, extreme_value;
    int type;
    int output_index;

    printf("Введите минимальное значение диапазона: ");
    scanf("%d", &min_range);
    printf("Введите максимальное значение диапазона: ");
    scanf("%d", &max_range);

    // Проверка корректности диапазона
    if (min_range >= max_range)
    {
        printf("Ошибка: минимальное значение должно быть меньше максимального!\n");
        return 1;
    }

    /* получает текущее календарное время */
    long ltime = time(NULL);
    /* преобразование его к типу int */
    int stime = (unsigned)ltime / 2;
    /* инициализация генератора */
    srand(stime);

    int range_size = max_range - min_range + 1;
    if (range_size > MAX_RANGE)
    {
        printf("Ошибка: диапазон слишком большой! Максимальный размер диапазона: %d\n", MAX_RANGE);
        return 1;
    }

    printf("\nПараметры: N=%d, Диапазон=[%d,%d], R=%d\n", N, min_range, max_range, R);

    array_ptr = generate_random_array(array, min_range, max_range);

    printf("\nИсходный массив: ");
    print_array(array);

    int max_count = count_frequencies(array, min_range, max_range, counter_ptr);
    // Вычисляем высоты всех столбцов
    heights_ptr = calculate_heights(counter_ptr, heights, range_size, R, max_count);
    printf("\n\n");
    print_vertical_histogram(counter_ptr, heights, min_range, range_size, R);
    printf("Максимальная частота: %d\n", max_count);
    printf("Максимальная высота столбца: %d символов\n", R);
    printf("Сдвиг влево элементов от минимального/максимального значения\n");

    printf("Выберите тип преобразования (0 - от минимального, 1 - от максимального): ");
    scanf("%d", &type);

    if (type != 0 && type != 1)
    {
        printf("Ошибка: тип должен быть 0 или 1!\n");
        return 1;
    }

    if (type == 0)
    {
        extreme_index = find_min_index(array);
        extreme_value = find_min(array);
        printf("Минимальное значение: %d (позиция %d)\n", extreme_value, extreme_index);
    }
    else
    {
        extreme_index = find_max_index(array);
        extreme_value = find_max(array);
        printf("Максимальное значение: %d (позиция %d)\n", extreme_value, extreme_index);
    }

    array_ptr = shift_left_from_extreme(array, type);

    if (type == 0)
    {
        printf("\nМассив после сдвига влево от минимального значения:\n");
    }
    else
    {
        printf("\nМассив после сдвига влево от максимального значения:\n");
    }
    print_array(array);
    if (type == 0)
    {
        output_index = find_min_index(array);
        printf("Сдвиг начался с позиции: %d\n", output_index);
    }
    else
    {
        output_index = find_max_index(array);
        printf("Сдвиг начался с позиции: %d\n", output_index);
    }

    printf("\nПосле преобразования:\n");

    max_count = count_frequencies(array, min_range, max_range, counter_ptr);
    heights_ptr = calculate_heights(counter_ptr, heights, range_size, R, max_count);
    print_vertical_histogram(counter_ptr, heights, min_range, range_size, R);
    printf("Максимальная частота: %d\n", max_count);
    printf("Максимальная высота столбца: %d символов\n", R);
    return 0;
}