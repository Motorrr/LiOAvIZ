#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

typedef struct {
	int size;
	int** original;
	int** result;
	int isCalculated;
	int isDirected;
} Graph;

Graph* createGraph(int size, int directed) {
	Graph* g = (Graph*)malloc(sizeof(Graph));
	if (!g) {
		printf("Ошибка выделения памяти для структуры графа\n");
		return NULL;
	}

	g->size = size;
	g->isCalculated = 0;
	g->isDirected = directed;

	g->original = (int**)malloc(size * sizeof(int*));
	g->result = (int**)malloc(size * sizeof(int*));

	if (!g->original || !g->result) {
		printf("Ошибка выделения памяти для матриц\n");
		free(g);
		return NULL;
	}

	for (int i = 0; i < size; i++) {
		g->original[i] = (int*)malloc(size * sizeof(int));
		g->result[i] = (int*)malloc(size * sizeof(int));

		if (!g->original[i] || !g->result[i]) {
			printf("Ошибка выделения памяти для строк матрицы\n");
			for (int j = 0; j < i; j++) {
				free(g->original[j]);
				free(g->result[j]);
			}
			free(g->original);
			free(g->result);
			free(g);
			return NULL;
		}

		for (int j = 0; j < size; j++) {
			if (i == j) {
				g->original[i][j] = 0;
				g->result[i][j] = 0;
			}
			else {
				g->original[i][j] = 0;
				g->result[i][j] = 0;
			}
		}
	}
	return g;
}

void freeGraph(Graph* g) {
	if (!g) return;

	for (int i = 0; i < g->size; i++) {
		free(g->original[i]);
		free(g->result[i]);
	}
	free(g->original);
	free(g->result);
	free(g);
}

void copyMatrix(int** source, int** dest, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			dest[i][j] = source[i][j];
		}
	}
}

int isNoPath(int value) {
	return value == 0;
}

void floydWarshall(Graph* g) {
	if (g->size == 0) {
		printf("Граф пустой!\n");
		return;
	}

	printf("Выполнение алгоритма Флойда-Уоршелла...\n");
	printf("Количество вершин: %d\n", g->size);
	printf("Это может занять некоторое время для больших графов...\n");

	copyMatrix(g->original, g->result, g->size);

	for (int k = 0; k < g->size; k++) {
		for (int i = 0; i < g->size; i++) {
			for (int j = 0; j < g->size; j++) {
				if (i == j || i == k || j == k) continue;

				if (!isNoPath(g->result[i][k]) && !isNoPath(g->result[k][j])) {
					int newDist = g->result[i][k] + g->result[k][j];

					if (isNoPath(g->result[i][j]) || newDist < g->result[i][j]) {
						g->result[i][j] = newDist;
					}
				}
			}
		}

		if (g->size > 50 && (k + 1) % (g->size / 10) == 0) {
			printf("Прогресс: %d%%\n", (k + 1) * 100 / g->size);
		}
	}

	g->isCalculated = 1;
	printf("Алгоритм выполнен успешно!\n");
}

void printOriginalMatrix(Graph* g) {
	if (g->size == 0) {
		printf("Граф пустой!\n");
		return;
	}

	printf("\nИСХОДНАЯ матрица смежности:\n");
	printf("Размер: %d вершин\n", g->size);
	printf("Тип графа: %s\n", g->isDirected ? "Ориентированный" : "Неориентированный");
	printf("0 означает отсутствие пути между вершинами\n\n");

	printf(" ");
	for (int j = 0; j < g->size; j++) {
		printf("%4d ", j);
	}
	printf("\n");

	for (int i = 0; i < g->size; i++) {
		printf("%4d:", i);
		for (int j = 0; j < g->size; j++) {
			printf("%5d", g->original[i][j]);
		}
		printf("\n");
	}

	printf("\nВсего элементов: %d x %d = %d\n", g->size, g->size, g->size * g->size);
}

void printResultMatrix(Graph* g) {
	if (!g->isCalculated) {
		printf("Алгоритм еще не выполнен! Сначала выполните пункт 4.\n");
		return;
	}

	if (g->size == 0) {
		printf("Граф пустой!\n");
		return;
	}

	printf("\nМАТРИЦА КРАТЧАЙШИХ ПУТЕЙ (результат):\n");
	printf("Размер: %d вершин\n", g->size);
	printf("Тип графа: %s\n", g->isDirected ? "Ориентированный" : "Неориентированный");
	printf("0 означает отсутствие пути между вершинами\n\n");

	printf(" ");
	for (int j = 0; j < g->size; j++) {
		printf("%4d ", j);
	}
	printf("\n");

	for (int i = 0; i < g->size; i++) {
		printf("%4d:", i);
		for (int j = 0; j < g->size; j++) {
			printf("%5d", g->result[i][j]);
		}
		printf("\n");
	}

	printf("\nВсего элементов: %d x %d = %d\n", g->size, g->size, g->size * g->size);
}

void printAbout() {
	printf("\n=========================================\n");
	printf(" ИНФОРМАЦИЯ О ПРОГРАММЕ\n");
	printf("=========================================\n");
	printf("Курсовая работа: Алгоритм Флойда-Уоршелла\n");
	printf("Язык программирования: C\n");
	printf("Автор: Мелюшев Матвей Максимович\n");
	printf("Группа: 24ВВВ3\n");
	printf("\nОсобенности программы:\n");
	printf("- Поддержка графов любого размера\n");
	printf("- Ориентированные и неориентированные графы\n");
	printf("- 0 означает отсутствие пути\n");
	printf("- Сохранение и загрузка из файлов\n");
	printf("- Полный вывод матриц любого размера\n");
	printf("- Прогресс-бар для больших графов\n");
	printf("=========================================\n");
}

void printMenu() {
	printf("\n=== АЛГОРИТМ ФЛОЙДА-УОРШЕЛЛА ===\n");
	printf("1. Создать новый граф\n");
	printf("2. Заполнить случайными значениями\n");
	printf("3. Ввести матрицу вручную\n");
	printf("4. Выполнить алгоритм Флойда\n");
	printf("5. Показать исходную матрицу\n");
	printf("6. Показать матрицу результатов\n");
	printf("7. Сохранить в файл\n");
	printf("8. Загрузить из файла\n");
	printf("9. О программе\n");
	printf("0. Выход\n");
	printf("Выбор: ");
}

int main() {
	setlocale(LC_ALL, "Russian");
	Graph* graph = NULL;
	int choice, size, directed;

	printf("\n");

	do {
		printMenu();
		if (scanf("%d", &choice) != 1) {
			printf("Ошибка ввода! Введите число.\n");
			while (getchar() != '\n');
			continue;
		}

		switch (choice) {
		case 1:
			printf("Введите размер графа (количество вершин): ");
			if (scanf("%d", &size) != 1) {
				printf("Ошибка ввода! Введите число.\n");
				break;
			}

			if (size <= 0) {
				printf("Размер графа должен быть положительным числом!\n");
				break;
			}

			if (size > 50) {
				printf("\n⚠️ВНИМАНИЕ: Граф из %d вершин создаст матрицу %dx%d\n", size, size, size);
				printf(" Вывод матрицы займет много строк на экране.\n");
				printf(" Вы уверены, что хотите продолжить? (1 - да, 0 - нет): ");
				int confirm;
				scanf("%d", &confirm);
				if (!confirm) {
					printf("Создание графа отменено.\n");
					break;
				}
			}

			printf("Выберите тип графа:\n");
			printf("1. Ориентированный (ребра имеют направление)\n");
			printf("0. Неориентированный (ребра без направления)\n");
			printf("Ваш выбор: ");
			if (scanf("%d", &directed) != 1) {
				printf("Неверный ввод! Используется ориентированный граф по умолчанию.\n");
				directed = 1;
			}

			if (graph) freeGraph(graph);
			graph = createGraph(size, directed);
			if (!graph) {
				printf("Не удалось создать граф. Возможно, недостаточно памяти.\n");
			}
			else {
				printf("Граф успешно создан (%d вершин, тип: %s)\n",
					size, directed ? "ориентированный" : "неориентированный");
			}
			break;

		case 2:
			if (!graph) {
				printf("Сначала создайте граф (пункт 1)!\n");
				break;
			}

			if (graph->size == 0) {
				printf("Граф пустой!\n");
				break;
			}

			srand(time(NULL));
			printf("Заполнение графа случайными значениями...\n");

			for (int i = 0; i < graph->size; i++) {
				for (int j = 0; j < graph->size; j++) {
					if (i != j) {
						int value = (rand() % 100 < 30) ? 0 : (rand() % 10 + 1);
						graph->original[i][j] = value;

						if (!graph->isDirected) {
							graph->original[j][i] = value;
						}
					}
				}

				if (graph->size > 100 && (i + 1) % (graph->size / 10) == 0) {
					printf("Заполнено: %d%%\n", (i + 1) * 100 / graph->size);
				}
			}
			graph->isCalculated = 0;
			printf("Граф заполнен случайными значениями\n");
			break;

		case 3:
			if (!graph) {
				printf("Сначала создайте граф (пункт 1)!\n");
				break;
			}

			if (graph->size == 0) {
				printf("Граф пустой!\n");
				break;
			}

			printf("Введите матрицу %dx%d:\n", graph->size, graph->size);
			printf("Формат ввода:\n");
			printf("- Для существующего пути: положительное число (1-999)\n");
			printf("- Для отсутствия пути: 0\n");
			printf("- Расстояние до самой себя всегда 0\n");
			printf("Тип графа: %s\n\n", graph->isDirected ? "Ориентированный" : "Неориентированный");

			for (int i = 0; i < graph->size; i++) {
				for (int j = 0; j < graph->size; j++) {
					if (i == j) {
						graph->original[i][j] = 0;
						if (graph->isDirected || i == 0) {
							printf("dist[%d][%d] = 0 (расстояние до себя)\n", i, j);
						}
					}
					else {
						if (!graph->isDirected && j < i) {
							graph->original[i][j] = graph->original[j][i];
							continue;
						}

						printf("dist[%d][%d]: ", i, j);
						int val;
						scanf("%d", &val);

						if (val < 0) {
							printf("Ошибка! Вес не может быть отрицательным. Используется 0.\n");
							val = 0;
						}

						graph->original[i][j] = val;

						if (!graph->isDirected) {
							graph->original[j][i] = graph->original[i][j];
						}
					}
				}
			}
			graph->isCalculated = 0;
			printf("Матрица успешно введена\n");
			break;

		case 4:
			if (!graph) {
				printf("Сначала создайте граф (пункт 1)!\n");
				break;
			}

			if (graph->size == 0) {
				printf("Граф пустой!\n");
				break;
			}

			floydWarshall(graph);
			break;

		case 5:
			if (!graph) {
				printf("Граф не создан!\n");
				break;
			}
			printOriginalMatrix(graph);
			break;

		case 6:
			if (!graph) {
				printf("Граф не создан!\n");
				break;
			}
			printResultMatrix(graph);
			break;

		case 7:
			if (!graph) {
				printf("Граф не создан!\n");
				break;
			}
			{
				char filename[100];
				printf("Что сохранить?\n");
				printf("1. Исходную матрицу\n");
				printf("2. Матрицу результатов\n");
				printf("Выбор: ");
				int saveChoice;
				scanf("%d", &saveChoice);

				printf("Введите имя файла: ");
				scanf("%s", filename);
				FILE* file = fopen(filename, "w");
				if (file) {
					fprintf(file, "%d %d\n", graph->size, graph->isDirected);

					for (int i = 0; i < graph->size; i++) {
						for (int j = 0; j < graph->size; j++) {
							int value;
							if (saveChoice == 1) {
								value = graph->original[i][j];
							}
							else {
								if (!graph->isCalculated) {
									printf("Алгоритм еще не выполнен! Сохраняется исходная матрица.\n");
									value = graph->original[i][j];
								}
								else {
									value = graph->result[i][j];
								}
							}
							fprintf(file, "%d ", value);
						}
						fprintf(file, "\n");
					}
					fclose(file);
					printf("Матрица %dx%d сохранена в файл: %s\n", graph->size, graph->size, filename);
				}
				else {
					printf("Ошибка открытия файла!\n");
				}
			}
			break;

		case 8:
		{
			char filename[100];
			printf("Введите имя файла: ");
			scanf("%s", filename);
			FILE* file = fopen(filename, "r");
			if (file) {
				int size, directed;
				if (fscanf(file, "%d %d", &size, &directed) != 2) {
					printf("Ошибка чтения файла! Неверный формат.\n");
					fclose(file);
					break;
				}

				if (size <= 0) {
					printf("Неверный размер графа в файле: %d\n", size);
					fclose(file);
					break;
				}

				if (size > 50) {
					printf("\n⚠️ВНИМАНИЕ: Загружается граф из %d вершин\n", size);
					printf(" Вывод матрицы займет много строк на экране.\n");
					printf(" Вы уверены, что хотите продолжить? (1 - да, 0 - нет): ");
					int confirm;
					scanf("%d", &confirm);
					if (!confirm) {
						printf("Загрузка графа отменена.\n");
						fclose(file);
						break;
					}
				}

				if (graph) freeGraph(graph);
				graph = createGraph(size, directed);
				if (!graph) {
					printf("Не удалось создать граф. Недостаточно памяти.\n");
					fclose(file);
					break;
				}

				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						int val;
						if (fscanf(file, "%d", &val) != 1) {
							printf("Ошибка чтения данных из файла.\n");
							fclose(file);
							freeGraph(graph);
							graph = NULL;
							return 1;
						}
						graph->original[i][j] = val;
					}
				}
				fclose(file);
				graph->isCalculated = 0;
				printf("Граф загружен из файла: %s\n", filename);
				printf("Размер: %d вершин, тип: %s\n",
					graph->size, graph->isDirected ? "ориентированный" : "неориентированный");
			}
			else {
				printf("Ошибка открытия файла!\n");
			}
		}
		break;

		case 9:
			printAbout();
			break;

		case 0:
			printf("Выход из программы...\n");
			break;

		default:
			printf("Неверный выбор! Введите число от 0 до 9\n");
		}
	} while (choice != 0);

	if (graph) freeGraph(graph);
	printf("Программа завершена.\n");
	return 0;
}
