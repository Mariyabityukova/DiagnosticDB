/*
 * Главный модуль программы - пользовательский интерфейс
 * Автор: Битюкова М.С., группа бИСТ231
 * Версия: 2.0
 */
#include "diagnostic_db.h"
#include <locale.h>
#include <stdlib.h>

#define MAX_FILENAME_LEN 256

 /* Функция отображения главного меню */
void displayMainMenu() {
    printf("\nГлавное меню:\n");
    printf("1. Загрузить данные из файла\n");
    printf("2. Показать все программы\n");
    printf("3. Поиск программы\n");
    printf("4. Сортировать программы\n");
    printf("5. Добавить новую программу\n");
    printf("6. Изменить существующую запись\n");
    printf("7. Удалить запись\n");
    printf("8. Сохранить данные в файл\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

/* Основная функция */
int main() {
    /* Настройка локализации */
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    /* Объявление переменных */
    DiagnosticProgram* programs = NULL;
    DiagnosticProgram* foundPrograms = NULL;
    int programCount = 0;
    int foundCount = 0;
    char filename[MAX_FILENAME_LEN];
    int choice;
    int index;
    int year;
    char searchTerm[MAX_NAME_LEN];

    /* Приветствие */
    printf("\n======================================\n");
    printf(" БАЗА ДАННЫХ ДИАГНОСТИЧЕСКИХ ПРОГРАММ\n");
    printf("======================================\n\n");

    /* Ввод имени файла */
    printf("Введите имя файла данных: ");
    scanf("%255s", filename);

    /* Основной цикл программы */
    while (1) {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: /* Загрузка из файла */
            if (readDataFromFile(&programs, &programCount, filename) == 0) {
                printf("Данные успешно загружены. Записей: %d\n", programCount);
            }
            else {
                printf("Ошибка загрузки данных!\n");
            }
            break;

        case 2: /* Показать все */
            if (displayData(programs, programCount) != 0) {
                printf("Нет данных для отображения\n");
            }
            break;

        case 3: /* Поиск */
            printf("Поиск по:\n1. Названию\n2. Году выпуска\n3. Производителю\nВыберите: ");
            scanf("%d", &choice);

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                continue;
            }

            DiagnosticProgram searchKey = { 0 };

            if (choice == 1) {
                printf("Введите название: ");
                scanf("%s", searchKey.name);
            }
            else if (choice == 2) {
                printf("Введите год: ");
                scanf("%d", &searchKey.releaseYear);
            }
            else if (choice == 3) {
                printf("Введите производителя: ");
                fgets(searchKey.manufacturer, MAX_MANUFACTURER_LEN, stdin);
                if ((strlen(searchKey.manufacturer) > 0) && (searchKey.manufacturer[strlen(searchKey.manufacturer) - 1] == '\n'))
                    searchKey.manufacturer[strlen(searchKey.manufacturer) - 1] = '\0';

            }
            else {
                printf("Неверный выбор\n");
                break;
            }

            if (searchPrograms(programs, programCount, searchKey, &foundPrograms, &foundCount) == 0) {
                displayData(foundPrograms, foundCount);
                free(foundPrograms);
            }
            else {
                printf("Программы не найдены\n");
            }
            break;

        case 4: /* Сортировка */
            printf("Сортировать по:\n1. Названию\n2. Году выпуска\nВыберите: ");
            scanf("%d", &choice);

            if (choice == 1) {
                if (sortProgramsByName(programs, programCount) == 0) {
                    printf("Сортировка по названию выполнена\n");
                }
            }
            else if (choice == 2) {
                if (sortProgramsByYear(programs, programCount) == 0) {
                    printf("Сортировка по году выполнена\n");
                }
            }
            else {
                printf("Неверный выбор\n");
            }
            break;

        case 5: /* Добавление */
            if (addNewProgram(&programs, &programCount) == 0) {
                printf("Программа успешно добавлена\n");
            }
            else {
                printf("Ошибка добавления программы\n");
            }
            break;

        case 6: /* Редактирование */
            printf("Введите номер записи для редактирования (1-%d): ", programCount);
            scanf("%d", &index);
            if (editProgram(programs, programCount, index - 1) == 0) {
                printf("Запись успешно изменена\n");
            }
            else {
                printf("Ошибка редактирования\n");
            }
            break;

        case 7: /* Удаление */
            printf("Введите номер записи для удаления (1-%d): ", programCount);
            scanf("%d", &index);
            if (deleteProgram(programs, &programCount, index - 1) == 0) {
                printf("Запись успешно удалена\n");
            }
            else {
                printf("Ошибка удаления\n");
            }
            break;

        case 8: /* Сохранение */
            if (writeDataToFile(programs, programCount, filename) == 0) {
                printf("Данные успешно сохранены\n");
            }
            else {
                printf("Ошибка сохранения данных\n");
            }
            break;

        case 0: /* Выход */
            free(programs);
            printf("Завершение программы\n");
            return 0;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}
