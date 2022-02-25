#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ERROR       -1
#define SUCCSESS    0

#define TRUE    1
#define FALSE   0

//Максимальное число узлов в графе. квадрат числа должен быть меньше 2^32
#define MAX_VALUE   (int)((1u << 15u) - 1u)

//Имя файла в который будет записана информация о графе на языке DOT.
#define GRAPH_FILE_MANE         "Graph.txt"

//Имя файла в который будет записанно изображение графа.
#define GRAPH_IMAGE_FILE_NAME   "Graph.png"

//Максимальная длинна системной команды.
#define COMMAND_MAX_LENGTH  1000

/*
Функция отправляет системе команду на создание файла с изображением графа.
graph_file_name - файл в котором граф записан на языке DOT
image_file_name - файл в который будет записанно изображение
*/
void ConvertGraphToPNG(const char* graph_file_name, const char* image_file_name);

/*
Функция генерирует текстовый файл в который на языке DOT будет записанна информация о графе.
file_mane - имя файла в который будет записан граф
adjacency_matrix - матрица смежности в виде которой хранится граф в нутри программы
matrix_size - размер матрицы смежности (количество столбцов равно количеству строк и равно размеру матрицы)
maximal_rows - массив хранящий номера строк матрицы, которым соответствуют узлы с максимальной степенью
rows_count - количество элементов в массиве maximal_rows
*/
void CreateGraphFile(const char* file_name, int* adjacency_matrix, int matrix_size, int* maximal_rows, int rows_count);

/*
Функция возвращяет номер элемента в массиве, в виде которого хранится квадратная матрица.
matrix_size - размер матрицы
row_mnumber - номер строки в которой хранится элемент
column_number - номер столбца в котором хранится элемент
*/
int GetId(int matrix_size, int row_number, int column_number);

/*
Функция записывает информацию считанную из консоли в матрицу смежности.
adjacency_matrix - массив в который будет записана информация
nodes_count - количество узлов во входящем графе
*/
void GetGraphFromConsole(int* adjacency_matrix, int nodes_count);

/*
Функция возвращяет степень указанной вершишы, которая находится в указанном графе.
adjacency_matrix - массив, хранящий информацию о графе в виде матрицы смежности
nodes_count - количество узлов в графе
node_number - номер узла для которого проверяется степень
*/
int GetNodeDegree(const int* adjacency_matrix, int nodes_count, int node_number);

/*
Если указанный узел имеет максимальную степень, функция возвращяет 1, иначе функция возвращяет 0.
max_rows - массив хранящий номера узлов с максимальной степенью
max_rows_count - длинна массива
node_number - номер узла для которого выполняется проверка
*/
int NodeIsMaximal(const int* max_rows, int max_rows_count, int node_number);

int main(void)
{
    int nodes_count;//количество узлов в графе

    printf("Set count of nodes.\n");

    scanf("%i", &nodes_count);//получаем количество узлов в новом графе

    if (nodes_count <= 0)                   //Если указанное количество узлов меньше 0, то
    {
        printf("Count of nodes is low.\n"); //выводим сообщение об ошибке и
        exit(ERROR);                        //завершаем работу программы.
    }

    if (nodes_count > MAX_VALUE)    //Если количество узлов в графе больше максимально допустимого, то
    {
        printf("Overflow.\n");      //выводим сообщение об ошибке и
        exit(ERROR);                //завершаем работу программы.
    }

    //Массив хранящий матрицу в виде которой хранится граф.
    int* adjacency_matrix;

    //Выделяем память под массив.
    adjacency_matrix = (int*)malloc(sizeof(int) * nodes_count * nodes_count);

    GetGraphFromConsole(adjacency_matrix, nodes_count);//Записываем граф в матрицу.

    //Максимальная из степеней узлов содержащихся в графе.
    int maximal_degree;
    maximal_degree = GetNodeDegree(adjacency_matrix, nodes_count, 0);//Записываем степень нулевого узла графа.

    for (int node = 1; node < nodes_count; node++)//Перебираем все узлы графа кроме первого. (он был взят на 2-е строки выше)
    {
        //Степень текущего узла.
        int degree = GetNodeDegree(adjacency_matrix, nodes_count, node);
        if (maximal_degree < degree)//Если максимальная степень меньше текущий степени.
        {
            maximal_degree = degree;//Записываем в значение максимальной степени значение степени текущего узла.
        }
    }

    //Массив хранящий список узлов с максимальной степенью.
    int* rows_list;

    //Выделяем память под массив.
    rows_list = (int*)malloc(sizeof(int));
    //Количество узлов с максимальной степенью.
    int rows_count = 0;

    for (int node = 0; node < nodes_count; node++)//Прербираем все узлы графа.
    {
        if (GetNodeDegree(adjacency_matrix, nodes_count, node) == maximal_degree)//Если степень текущего числа равна максимальной степени.
        {
            rows_count++;//Увеличиваем (инкременируем) количество элементов в массиве.
            rows_list = (int*)realloc(rows_list, sizeof(int) * rows_count);//Довыделяем память под массив, хранящий списоз узлов с максимальной степенью.
            rows_list[rows_count - 1] = node;//Записывае номер узла, имеющего максимальную степень.
        }
    }

    CreateGraphFile(GRAPH_FILE_MANE, adjacency_matrix, nodes_count, rows_list, rows_count);//Записываем граф в файл, помечая рёбра содержащие хотябы один узел с максимальной степенью.

    free(adjacency_matrix);//Удаляем граф из памяти (очищаем память).
    free(rows_list);//Удаляем массив содержащий список узлов с максимальной степенью.

    printf("The graph was wrote to %s\n", GRAPH_FILE_MANE);//Выводим сообщение об успешной записи графа в текстовый файл.

    ConvertGraphToPNG(GRAPH_FILE_MANE, GRAPH_IMAGE_FILE_NAME);//Создаём изображене графа.

    printf("Image was wrote to %s.\n", GRAPH_IMAGE_FILE_NAME);//Выводим сообщение об успешной записи графа.

    exit(SUCCSESS);//Завершаем работы программы.
}

void GetGraphFromConsole(int* adjacency_matrix, int nodes_count)
{
    for (int row = 0; row < nodes_count; row++)//Перебираем все строки в матрице.
    {
        printf("Row: %3i:  ", row + 1);//Выводим номер текущей строки матрицы.
        for (int column = 0; column < nodes_count; column++)//Перебираем все элементы матрицы на текущей строке.
        {
            //Значение элемента матрицы.
            int value;
            scanf("%i", &value);//Получаем значение элемента матрицы.
            if (value < 0)                                      //Если значени элемента меньше нуля, то
            {
                printf("Count of nodes is less then zero.\n");  //выводим сообщение об ошибке и
                exit(ERROR);                                    //завершаем работу программы.
            }
            adjacency_matrix[GetId(nodes_count, row, column)] = value;//Записываем считанное значение в матрицу, на позицию [row, colunm]
        }
    }
    return;//Завершаем работу функции.
}

int GetId(int matrix_size, int row_number, int column_number)
{
    /*
    Все элементы матрицы пронуерованны построчно.
    Пример:
             с т о л б ц ы
             0  1  2  3  4  5
     с  0    0  1  2  3  4  5
     т  1    6  7  8  9 10 11
     р  2   12 13 14 15 16 17
     о  3   18 19 20 21 22 23
     к  4   24 25 26 27 28 29
     и  5   30 31 32 33 34 35
    */
    return row_number * matrix_size + column_number;
}

int GetNodeDegree(const int* adjacency_matrix, int matrix_size, int node_number)
{
    //Степень вершины.
    int degree;
    degree = adjacency_matrix[GetId(matrix_size, node_number, node_number)];//Если узел имеет петлю, то он имеет два соединение одно из них учитывается сейчас, другое в цикле ниже..
    for (int column = 0; column < matrix_size; column++)//Перебираем все элементы в матрице смежности на указанной строке. (номер строки соответствует номеру узла)
    {
        degree += adjacency_matrix[GetId(matrix_size, node_number, column)];//Увеличиваем значение степени на кол-во рёбер, соединённых с текущим узлом. (кол-во рёбер может равняться нулю)
    }
    return degree;//Возвращяем значение степени.
}

void ConvertGraphToPNG(const char* graph_file_name, const char* image_file_name)
{
    char* command;//Указатель на троку с командой.
    command = (char*)malloc(sizeof(char) * COMMAND_MAX_LENGTH);//выделяем память для строки в которую будет записанна команда для graphviz

    /*
    dot - команда
    -Tpng - аргумент (в данном случае можно назвать ключём) для команды. Обозначает, что выходной файл должен быть в формате png.
    GRAPH_FILE_MANE - название файла в котором граф записан на языке DOT
    -o - аргумент (в данном случае можно назвать ключём) для команды. Обозначает, что за ним будет следовать арнумент в котором будет указанно название файла в который будет зписанно отображение графа.
    GRAPH_IMAGE_FILE_NAME - название файла в котором записанно отображение графа
    */
    sprintf(command, "dot -Tpng %s -o %s", graph_file_name, image_file_name);//печатаем команду (вывод происходит также, как в консоль, НО в строку)

    system(command);//Выполняем команду.

    free(command);//Очищаем память от строки.

    return;//Завершаем работу функции
}

void CreateGraphFile(const char* file_name, int* adjacency_matrix, int matrix_size, int* maximal_rows, int rows_count)
{
    //Указатель на файл, в который будет записываться граф. 
    FILE* f;
    f = fopen(file_name, "w+");//Открываем файл на запись, предварительно удалив его содержимое.

    fprintf(f, "graph G{\n");//Объявляем граф внутри файла.

    for (int node = 0; node < matrix_size; node++)//перебираем все узлы графа
    {
        if (NodeIsMaximal(maximal_rows, rows_count, node))//Если текущий узел требуется раскрасить.
        {
            fprintf(f, "%i [color=green];\n", node);//Записываем узел в файл как раскрашений.
        }
        else//иначе
        {
            fprintf(f, "%i;\n", node);//Записываем узел в файл как не раскрашений.
        }
    }

    //Номер первого найденного в матрице узла имеющего ребро.
    int first_node;
    //Флаг, показывающий, найден ли прошлый узел.
    int first_node_found;

    for (int row = 0; row < matrix_size; row++)//Перебираем все строки в матрице.
    {
        //Флаг хранящий инрмацию имеет ли узел номер row максимальную степень.
        int first_node_is_maximal = NodeIsMaximal(maximal_rows, rows_count, row);
        for (int column = row; column < matrix_size; column++)//Перебираем все элементы на данной строке находящиеся после главной диагонали включительно.
        {
            int edges = adjacency_matrix[GetId(matrix_size, row, column)];//количество рёбер (или петель) между двумя узлами
            if (first_node_is_maximal || NodeIsMaximal(maximal_rows, rows_count, column))//Если один из узлов имеет максимальную степень.
            {
                for (int edge = 0; edge < edges; edge++)//Перебираем все рёбра.
                {
                    fprintf(f, "%i -- %i [color=green];\n", row, column);//Записываем ребро в файл,как раскрашенное.
                }
            }
            else//иначе
            {
                for (int edge = 0; edge < edges; edge++)//Перебираем все рёбра.
                {
                    fprintf(f, "%i -- %i;\n", row, column);//Записываем ребро в файл, как не раскрашенное.
                }
            }
        }
    }

    fprintf(f, "}\n");//Завершаем запись графа.
    fclose(f);//Закрываем файл в который записывали граф.

    return;//Завершаем выполнение функции.
}

int NodeIsMaximal(const int* max_nodes, int max_rows_count, int node_number)
{
    for (int node = 0; node < max_rows_count; node++)//перебираем все элемента массива, хранящего список номеров узлов имеющих максимальную степень
    {
        if (max_nodes[node] == node_number)//Если номер узла в списке совпал с номером узля, для которого проводится проверка, то
        {
            return TRUE;//функция возвращяет 1 (истину).
        }
    }

    /*
    Если функция не нашла ни одно совпадения,
    значит проверяемый узел не имеет максимальную степень,
    значит функция возвращает 0 (ложь).
    */
    return FALSE;
}