// Komm_dinamic.cpp: определяет точку входа для консольного приложения.
//
/*
ПРОЧТИ МЕНЯ!
!!!при внесения каких-либо изменений в программу , просьба оставлять комментарии своих действий и дату здесь,
а так же в самом коде, как это сделано мною сейчас. спасибо.

В данный момент в программе реализован поиск пути при постоянном хождении только по левой ветке,
переименование, правильное построение гамильтонового цикла.

ВАЖНЫЕ ОБЪЕКТЫ:
stepCounter-счётчик шагов, к нему в данный момент подвязано все, что касается переименования и изменений
array_copy[шаг][строка][столбец]-вектор, хранящий матрицы на каждом шаге(для дальейшей возможности возврата на любой шаг)

В разработке:
массив для хранения нижних оценок(подвязанный к счётчику шагов).
Интерфейс?
шаг по правой ветке(пока не работает по неизвестным причинам).

Олег. 16.03.18
*/

#include "stdafx.h"
#include <iostream>
#include <conio.h>

using namespace std;

struct dot
{
	int index_i;
	int index_j;
};
#pragma region Глобальные переменные

int sizeArray;//размер матрицы
int assessment = 0;//Нижняя граница(меняющаяся в хождении по дереву)
int initialAssessment = 0;//нижняя граница(изначальная)(не меняется в процессе хождения по дереву)
int zeroCounter = 0;//счётчик для количества нулей
bool admin = false;//отвечает за отображение деталей
dot *way = new dot[sizeArray];//структура , хранящая путь


const int infinity = 999;

#pragma endregion


struct zero {
	int zero_i;
	int zero_j;
	int zero_assessment;

};

#pragma region Прототипы функций(не лезь,убьёт)
void arrayExample(int **array);
void random(int **array);
void manually(int **array);
void symmetry(int **array);
void InputConst(int **array);
void Editing(int **array);
void printArray(int **array);
void ForLine(int **array);
void ForColumn(int **array);
void onTheLeft(int ***array_copy, zero *null, int stepCounter, int WaypointСounter, int **LineName);
void branchSelection(zero *null, int **array, int **LineName);
void RenameLine(int maxZeroScore_i, int maxZeroScore_j, int **LineName, int stepCounter);
void bildDotWay(int maxZeroScore_i, int maxZeroScore_j, int stepCounter, int WaypointСounter, int** LineName);
int ZeroEstimation(int **array, int razmer, int zeros_i, int zeros_j);
void printStruct(zero *null, int zeroCounter);
void ZerosAssessment(int **array, zero *null);
void printWay();
void bildTrueWay();
void printAssessment();

#pragma endregion
//Функции иниацилизации матрицы.
#pragma region Функция решения конкретного примера
void arrayExample(int **array) {
	sizeArray = 5;

	int example[5][5] = { { infinity, 54,27,31,48 } ,
	{ 76,infinity,1,18,41 },
	{ 36,23,infinity, 33,51 },
	{ 87,32,72,infinity, 62 },
	{ 94,24,6,77,infinity } };
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{
			array[i][j] = example[i][j];
		}

	}
	cout << "Ожидаемое решение(нумерация с 1): \n(3,1), (1,4), (4,2), (2,5), (5,3), \nДлина маршрута равна F(Mk) = 146\n";
}


#pragma endregion
#pragma region  функция заполнение массива случайными значениями

void random(int **array)//заполнение массива случайными значениями
{
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)//заполняем матрицу рандомом
		{
			array[i][j] = 9 + rand() % 100;

		}
		array[i][i] = infinity;
	}

}
#pragma endregion
#pragma region Функция ручного заполнения матрицы
void manually(int **array)
{
	cout << "Задайте матрицу вручную:" << endl;

	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{

			cout << "Введите элемент [" << i + 1 << "][" << j + 1 << "]/n";
			cin >> array[i][j];

		}
		array[i][i] = infinity;
	}
}
#pragma endregion
#pragma region Функция заполнения симетричной матрицы
void symmetry(int **array) {
	cout << "Заполнение симметричной матрицы.\n";
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)//заполняем матрицу рандомом
		{
			if (i < j)
			{
				array[i][j] = 9 + rand() % 100;
			}

		}
		array[i][i] = infinity;//костыль
	}

	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)//заполняем матрицу рандомом
		{
			if (i > j)
			{
				array[i][j] = array[j][i];
			}

		}

	}
}
#pragma endregion
#pragma region Функция заполнения матрицы константой
void InputConst(int **array) {
	cout << "Задайте консанту:" << endl;
	int constanta = 0;
	cin >> constanta;

	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)//заполняем матрицу рандомом
		{
			array[i][j] = constanta;

		}
		array[i][i] = infinity;
	}
}
#pragma endregion
//Функции для работы с матрицей
#pragma region Функция редактирования матрицы
void Editing(int **array) {
	while (_getch() != 27)
	{

		cout << "Введите индексы элемента, который хотите отредактировать(нумерация с 1)\nСтрока:";
		int i = 0, j = 0;
		cin >> i;
		cout << "Столбец:";
		cin >> j;
		--i;
		--j;

		if (i == j)
		{
			cout << "Ошибка. Вы не можете редактировать элементы на главной диагонали.\n";
			break;
		}
		else if (i<0 || i>sizeArray || j<0 || j>sizeArray) {
			cout << "Ошибка. Указанного элемента (" << i + 1 << ";" << j + 1 << " не существует." << endl;
			break;
		}
		cout << "Введите значение :";
		int element = 0;
		cin >> element;
		array[i][j] = element;
		printArray(array);
		cout << "Нажмите ESC для выхода,другую клавишу-продолжить" << endl;
	}

}
#pragma endregion
#pragma region функция для печати матрицы
void printArray(int **array)
{
	cout << "--------------------------------------------------------\n";
	for (size_t i = 0; i < sizeArray; i++) {
		for (size_t j = 0; j < sizeArray; j++)//вывод на экран мссива
		{
			cout << array[i][j] << '\t';

		}
		cout << endl << endl;
		//cout << '\t';

	}
	cout << "--------------------------------------------------------\n";
}
#pragma endregion
#pragma region Функция поиска минимального элемента в строке и его вычитания 
void ForLine(int **array)
{
	int min_i, min_j = 0, min = array[0][0];// индексы минимального и его значение
											//идем по строкам, делаем 0

	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{
			if (array[i][j] < min)
			{
				min = array[i][j];
				min_i = i;
				min_j = j;
			}
		}

		if (min < infinity)
		{
			assessment += min;
			//__________________________________________Admin________________________________________________
			if (admin == true) {
				cout << "______________________________________________________________" << endl;
				cout << "Минимальный элемент в строке " << i + 1 << " = " << min << ". Его индекс  = [" << min_i + 1 << "][" << min_j + 1 << "]" << endl;
				cout << "______________________________________________________________" << endl;
			}
			//________________________________________________________________________________________________
			if (min != 0)
			{
				for (size_t j = 0; j < sizeArray; j++)
				{
					if (min_i != j && array[i][j] != infinity)
					{
						array[i][j] -= min;//отнимаем минимальный от всех элементов строки
					}
				}
			}
		}
		else
		{
			//_________________________________________Admin______________________________
			if (admin == true)
				cout << "Строка " << i + 1 << " вычеркнута. \n";
			//____________________________________________________________________________
		}
		min = array[0][0]; min_i = 0; min_j = 0;
	}
	//_________________________________________Admin______________________________
	if (admin == true)
		printArray(array);//выводим на экран матрицу
						  //___________________________________________________________________________
	printAssessment();// выводим оценку
}
#pragma endregion
#pragma region Функция поиска минимального элемента в столбце и его вычитания
void ForColumn(int **array)
{
	int min_i, min_j = 0, min = array[0][0];// индексы минимального и его значение
											//идем по столбцам
	for (size_t j = 0; j < sizeArray; j++)
	{
		for (size_t i = 0; i < sizeArray; i++)
		{
			if (array[i][j] < min)
			{
				min = array[i][j];
				min_i = i;
				min_j = j;
			}
		}

		if (min < infinity)
		{
			assessment += min;
			//_________________________________________Admin______________________________
			if (admin == true) {
				cout << "______________________________________________________________" << endl;
				cout << "Минимальный элемент в столбце " << j + 1 << " = " << min << ". Его индекс  = [" << min_i + 1 << "][" << min_j + 1 << "]" << endl;
				cout << "______________________________________________________________" << endl;
			}
			//_________________________________________Admin______________________________
			if (min != 0)
			{
				for (size_t i = 0; i < sizeArray; i++)
				{
					if (min_j != i && array[i][j] != infinity) {
						array[i][j] -= min;
					}
				}
			}
		}
		else
		{
			//_________________________________________Admin______________________________
			if (admin == true)
				cout << "Столбец " << j + 1 << " вычеркнут. \n";
			//_________________________________________Admin______________________________
		}
		min = array[0][0]; min_i = 0; min_j = 0;
	}
	//_________________________________________Admin______________________________
	if (admin == true)
		printArray(array);//выводим на экран матрицу
						  //_________________________________________Admin______________________________
	printAssessment();//выводим на экран оценку
}
#pragma endregion
#pragma region Функция шага по левой ветви дерева
void onTheLeft(int ***array_copy, zero *null, int stepCounter, int WaypointСounter, int **LineName) {


	int maxZeroScore_i = 0;//строка с 0, у которого максимальная оценка
	int maxZeroScore_j = 0;//столбец с 0, у которого максимальная оценка

	int maxScore = -1;


	for (size_t i = 0; i < zeroCounter; i++)//ищем 0 с наибольшей оценкой
	{
		if (null[i].zero_assessment > maxScore)
		{
			maxScore = null[i].zero_assessment;
			maxZeroScore_i = null[i].zero_i;
			maxZeroScore_j = null[i].zero_j;
		}
	}


	bildDotWay(maxZeroScore_i, maxZeroScore_j, stepCounter, WaypointСounter, LineName);
	//вычеркиваем строку, столбец, диагональный элемент
	//строку где был диагональеый, ставим на место вычеркнутой(начинаем с этого)

	//________________________________________________________Admin______________________
	if (admin == true) {
		cout << "Массив, с которым работаем.\n";
		printArray(array_copy[stepCounter]);
	}
	//___________________________________________________________________________________
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{
			array_copy[stepCounter + 1][i][j] = array_copy[stepCounter][i][j];//переписали матрицу в следующий шаг
		}
	}
	//________________________________________________________Admin______________________
	if (admin == true)
		cout << "maxZeroScore_i= " << maxZeroScore_i << " \nmaxZeroScore_j= " << maxZeroScore_j << endl;
	//___________________________________________________________________________________
	array_copy[stepCounter + 1][maxZeroScore_j][maxZeroScore_i] = infinity;
	//заменили диагональный бесконечностью(вычеркнули)
	for (size_t j = 0; j < sizeArray; j++)
	{
		array_copy[stepCounter + 1][maxZeroScore_i][j] = array_copy[stepCounter + 1][maxZeroScore_j][j];
		//поставили строку с диагональным на место вычеркнутой

	}
	RenameLine(maxZeroScore_i, maxZeroScore_j, LineName, stepCounter);//вызываем функцию переименования(памяти)


	for (size_t j = 0; j < sizeArray; j++)
	{
		array_copy[stepCounter + 1][maxZeroScore_j][j] = infinity;//вычеркнули строку(заменили бесконечностью)
	}

	for (size_t i = 0; i < sizeArray; i++)
	{
		array_copy[stepCounter + 1][i][maxZeroScore_j] = infinity;//вычеркнули столбец(заменили бесконечностью)
	}
	cout << "Поставили строку с диагональным на место вычеркнутой.\n";
	printArray(array_copy[stepCounter + 1]);

	ForLine(array_copy[stepCounter + 1]);
	ForColumn(array_copy[stepCounter + 1]);



}
#pragma endregion
#pragma region Функция выбора ветви
void branchSelection(zero *null, int **array, int **LineName) {
	int maxZeroScore_i = 0;//строка с 0, у которого максимальная оценка
	int maxZeroScore_j = 0;//столбец с 0, у которого максимальная оценка
	int scoreByLeftBranch = 0;//оценка по левой ветви
	int scoreByRightBranch = 0;//оценка по правой ветви
	int stepCounter = 0;//счетчик шагов
	int WaypointСounter = 0;//счетчик для точек пути( ХЗ ЗАЧЕМ ОН НУЖЕН?????)
	int ***array_copy = new int**[sizeArray/*количество возможных шагов*/];//массив матриц для хранения изменений исходной  матрицы
	for (size_t i = 0; i<sizeArray; i++)
	{
		array_copy[i] = new int*[sizeArray];
		for (size_t j = 0; j < sizeArray; j++)
		{
			array_copy[i][j] = new int[sizeArray];
			//выделили память под трёхмерный массив

		}
	}
	int maxScore = 0;//максимальная оценка
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{
			array_copy[stepCounter][i][j] = array[i][j];
			//скопировали матрицу в массив изменений на шаге 0(изначальный массив)

		}
	}

	//__________заполняем первый столбец в массиве переименования строк
	for (size_t i = 0; i < sizeArray; i++)
	{
		LineName[i][0] = i;
	}
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 1; j < sizeArray; j++)
		{
			LineName[i][j] = -1;
		}
	}


shiftRight:
#pragma region Оценка шага по правой ветви


	for (size_t i = 0; i < zeroCounter; i++)//ищем 0 с наибольшей оценкой
	{
		if (null[i].zero_assessment > maxScore)
		{
			maxScore = null[i].zero_assessment;
			maxZeroScore_i = null[i].zero_i;
			maxZeroScore_j = null[i].zero_j;


		}
	}
	scoreByRightBranch = assessment + maxScore;

#pragma endregion

	WaypointСounter = 0;

	while (stepCounter != (sizeArray - 1))
		//выполняем пока количество проложенных ветвей не равно количеству городов(последняя ветвь -путь в изначальный город)
	{

		scoreByLeftBranch = assessment;
		//WaypointСounter++;
		onTheLeft(array_copy, null, stepCounter, WaypointСounter, LineName);

		if (scoreByRightBranch <= scoreByLeftBranch)
		{
			cout << " Оценка по левой ветке дерева на шаге " << stepCounter << " стала больше оценки по шагу вправо.\n";
			scoreByLeftBranch = 0;
			assessment = initialAssessment;//сюда потом прикрутить значение из будущего массива оценок
			stepCounter = 0;
			ZerosAssessment(array_copy[stepCounter], null);
			cout << "Ноль[" << maxZeroScore_i + 1 << "][" << maxZeroScore_j + 1 << "] замёнен на бесконечность.(нумерация с 1)\n";
			array_copy[stepCounter][maxZeroScore_i][maxZeroScore_j] = infinity;
			cout << "Начинаем новый обход...\n";
			goto shiftRight;
		}
		else
		{
			stepCounter++;

			zeroCounter = 0;
			for (size_t i = 0; i < sizeArray; i++)
				for (size_t j = 0; j < sizeArray; j++)
				{
					if (array_copy[stepCounter][i][j] == 0)
						zeroCounter++;//посчитали 0
				}
			null = new zero[zeroCounter];//выделили память


			ZerosAssessment(array_copy[stepCounter], null);
			printStruct(null, zeroCounter);
			WaypointСounter++;

		}
	}

	cout << "Работа программы завершена. проложено ветвей:  " << stepCounter + 1 << endl;
	printWay();
	//exit(0);


}
#pragma endregion
#pragma region функция памяти переименований

void RenameLine(int maxZeroScore_i, int maxZeroScore_j, int **LineName, int stepCounter) {
	if (admin == true)
	{
		cout << "Массив изменений на текущий момент(шаг " << stepCounter << ")\n";
		for (size_t i = 0; i < sizeArray; i++)
		{
			for (size_t j = 0; j < sizeArray; j++)
			{
				cout << LineName[i][j] << " \t";
			}
			cout << "\n";
			//cout << "\t";
		}
	}


	for (size_t i = 0; i < sizeArray; i++)
	{
		if (LineName[i][stepCounter] == maxZeroScore_i)
		{
			LineName[i][stepCounter + 1] = maxZeroScore_j;
		}
		else if (LineName[i][stepCounter] == maxZeroScore_j)
		{
			LineName[i][stepCounter + 1] = maxZeroScore_i;
		}
		else
			LineName[i][stepCounter + 1] = LineName[i][stepCounter];
	}

}
#pragma endregion
#pragma region Функция постороение ветви с истинным названием точки

void bildDotWay(int maxZeroScore_i, int maxZeroScore_j, int stepCounter, int WaypointСounter, int** LineName) {

	int true_i = -1;
	for (size_t i = 0; i < sizeArray; i++)
	{
		if (LineName[i][stepCounter] == maxZeroScore_i)
		{
			true_i = LineName[i][0];
			break;
		}
	}



	way[WaypointСounter].index_i = true_i;
	way[WaypointСounter].index_j = maxZeroScore_j;
}
#pragma endregion
//Функции для работы с нулями
#pragma region Оценка нуля
int ZeroEstimation(int **array, int razmer, int zeros_i, int zeros_j)//функция принимает матрицу, ее размер, индекс нуля по строке, индекс нуля по столбцу
{
	int rating = 0;//оценка
	int i = 0, j = 0;
	int min = array[0][0];
	j = zeros_j;
	for (i = 0; i < razmer; i++) {//ищем минимальный в столбце 0
		if (array[i][j] < min&&i != zeros_i)//сам 0 мы не смотрим
		{
			min = array[i][j];
		}
	}
	rating += min;

	min = array[0][0];
	i = zeros_i;
	for (j = 0; j < razmer; j++) {//ищем минимальный в строке 0
		if (array[i][j] < min&&j != zeros_j) {
			min = array[i][j];
		}
	}
	rating += min;

	return rating;
}
#pragma endregion
#pragma region Печать массива структур, содержащего информацию о 0
void printStruct(zero *null, int zeroCounter)
{
	for (size_t i = 0; i < zeroCounter; i++)
	{
		cout << i + 1 << " ноль: координаты [" << null[i].zero_i << "][" << null[i].zero_j << "]\nОценка:" << null[i].zero_assessment << endl;
	}
}
#pragma endregion
#pragma region Функция оценки нулей и записи ее в структуру
void ZerosAssessment(int **array, zero *null) {
	cout << "Оцениваем нули..." << endl;
	int t = 0;//счетчик
	for (size_t i = 0; i < sizeArray; i++)
	{
		for (size_t j = 0; j < sizeArray; j++)
		{
			if (array[i][j] == 0)
			{
				null[t].zero_i = i;
				null[t].zero_j = j;
				null[t].zero_assessment = ZeroEstimation(array, sizeArray, i, j);
				t++;

			}
		}
	}
}
#pragma endregion
//Функции пути
#pragma region печать пути
void printWay() {
	cout << "--------------------------------------------------\n";
	cout << " Ветви:\n";
	for (size_t i = 0; i < sizeArray - 1; i++)
	{
		cout << "(" << way[i].index_i << ")(" << way[i].index_j << ") ";
	}

	cout << "\n--------------------------------------------------\n";
}
#pragma endregion
#pragma region Функция построения гамильтонового цикла
void bildTrueWay() {
	dot* trueWay = new dot[sizeArray];
	for (size_t i = 0; i < sizeArray; i++)
	{
		trueWay[i].index_i = -1;
		trueWay[i].index_j = -1;
	}
	trueWay[0].index_i = way[0].index_i;
	trueWay[0].index_j = way[0].index_j;
	int t = 0;
	bool flag = false;//найдена ли следующая ветвь для выстроения в Гамильтонов цикл
	while (t < sizeArray - 2)
	{
		//cout << "while_" << t << endl;//отладка
		for (size_t i = 0; i < sizeArray; i++)
		{
			flag = false;//следующая ветвь не найдена пока не попадем в if
			if (trueWay[t].index_j == way[i].index_i)
			{
				trueWay[t + 1].index_i = way[i].index_i;
				trueWay[t + 1].index_j = way[i].index_j;
				t++;
				flag = true;
				break;
			}

		}
	}
	if (flag == false)
	{
		cout << "Не удалось построить гамильтонов цикл. \nНе найдена точка " << trueWay[t].index_j << endl;
		exit(2);
	}
	trueWay[sizeArray - 1].index_i = trueWay[sizeArray - 2].index_j;
	trueWay[sizeArray - 1].index_j = trueWay[0].index_i;

	cout << "--------------------------------------------------\n";
	cout << "Гамильтонов цикл:\n";
	for (size_t i = 0; i < sizeArray; i++)
	{
		cout << "(" << trueWay[i].index_i << ";" << trueWay[i].index_j << ") ";
	}

	cout << "\n--------------------------------------------------\n";
}
#pragma endregion
//Функции для работы с нижней границей
#pragma region Функция вывода нижней оценки на экран
void printAssessment()
{
	cout << "______________________________________________________________" << endl;
	cout << "Нижняя оценка: " << assessment << endl;
	cout << "______________________________________________________________" << endl;
}
#pragma endregion

void main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Введите размер матрицы: \n";
	cin >> sizeArray;
	cout << "Отображать детали?(1/0)" << endl;
	cin >> admin;



#pragma region создание динамического массива
	int **array = new int*[sizeArray];//исходная матрица
	for (size_t i = 0; i < sizeArray; i++)
	{
		array[i] = new int[sizeArray];
	}
#pragma endregion

#pragma region Menu
	cout << "Как заполнить матрицу?" << endl;
	cout << "1.Случайными числами\n2.Случайными симметричными числами\n3.Константой\n4.Вручную\n5.Значениями из примера[5][5]\n";

	switch (_getch())
	{
	case 49://cod '1'
		random(array); //заполнение массива случайными значениями
		break;

	case 50:
		symmetry(array);
		break;
	case 51:
		InputConst(array);
		break;
	case 52:
		manually(array);
		break;
	case 53:
		arrayExample(array);
		break;

	default:
		cout << "Неверное действие.\n";
		exit(1);
		break;
	}


	cout << "Исходная матрица:" << endl;
	printArray(array);
l1:	cout << "Продолжить /редактировать? (y/r)" << endl;

	switch (_getch())
	{
	case 121:
		break;
	case 114:
		//редактирование
		cout << "Редактирование.\n";
		Editing(array);
		goto l1;

	}
#pragma endregion


	ForLine(array);
	ForColumn(array);
	initialAssessment = assessment;
#pragma region Создаем массив структур, в котором будет храниться информация о каждом нуле.

	for (size_t i = 0; i < sizeArray; i++)
		for (size_t j = 0; j < sizeArray; j++)
		{
			if (array[i][j] == 0)
				zeroCounter++;//посчитали 0
		}

	zero *null = new zero[zeroCounter];//выделили память

	for (size_t i = 0; i < zeroCounter; i++) {
		null[i].zero_assessment = 0;
		null[i].zero_i = 0;
		null[i].zero_j = 0;
	}//проинициализировали структуры 0.
#pragma endregion
#pragma region создаем массив с информацией о переименованиии строк
	int**LineName = new int*[sizeArray];
	for (size_t i = 0; i < sizeArray; i++)
	{
		LineName[i] = new int[sizeArray];
	}

#pragma endregion

	ZerosAssessment(array, null);
	printStruct(null, zeroCounter);

	branchSelection(null, array, LineName);

	cout << "Построение пути... " << endl;
	bildTrueWay();
	cout << "Завершено.\n";
	//exit(0);
	system("pause");

}


