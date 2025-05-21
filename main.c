/*
 * ������� ������ ��������� - ���������������� ���������
 * �����: �������� �.�., ������ ����231
 * ������: 2.0
 */
#include "diagnostic_db.h"
#include <locale.h>
#include <stdlib.h>

#define MAX_FILENAME_LEN 256

 /* ������� ����������� �������� ���� */
void displayMainMenu() {
    printf("\n������� ����:\n");
    printf("1. ��������� ������ �� �����\n");
    printf("2. �������� ��� ���������\n");
    printf("3. ����� ���������\n");
    printf("4. ����������� ���������\n");
    printf("5. �������� ����� ���������\n");
    printf("6. �������� ������������ ������\n");
    printf("7. ������� ������\n");
    printf("8. ��������� ������ � ����\n");
    printf("0. �����\n");
    printf("�������� ��������: ");
}

/* �������� ������� */
int main() {
    /* ��������� ����������� */
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    /* ���������� ���������� */
    DiagnosticProgram* programs = NULL;
    DiagnosticProgram* foundPrograms = NULL;
    int programCount = 0;
    int foundCount = 0;
    char filename[MAX_FILENAME_LEN];
    int choice;
    int index;
    int year;
    char searchTerm[MAX_NAME_LEN];

    /* ����������� */
    printf("\n================================\n");
    printf(" ���� ������ ��������������� ��������\n");
    printf("================================\n\n");

    /* ���� ����� ����� */
    printf("������� ��� ����� ������: ");
    scanf("%255s", filename);

    /* �������� ���� ��������� */
    while (1) {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: /* �������� �� ����� */
            if (readDataFromFile(&programs, &programCount, filename) == 0) {
                printf("������ ������� ���������. �������: %d\n", programCount);
            }
            else {
                printf("������ �������� ������!\n");
            }
            break;

        case 2: /* �������� ��� */
            if (displayData(programs, programCount) != 0) {
                printf("��� ������ ��� �����������\n");
            }
            break;

        case 3: /* ����� */
            printf("����� ��:\n1. ��������\n2. ���� �������\n3. �������������\n��������: ");
            scanf("%d", &choice);

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                continue;
            }

            DiagnosticProgram searchKey = { 0 };

            if (choice == 1) {
                printf("������� ��������: ");
                scanf("%s", searchKey.name);
            }
            else if (choice == 2) {
                printf("������� ���: ");
                scanf("%d", &searchKey.releaseYear);
            }
            else if (choice == 3) {
                printf("������� �������������: ");
                fgets(searchKey.manufacturer, MAX_MANUFACTURER_LEN, stdin);
                if ((strlen(searchKey.manufacturer) > 0) && (searchKey.manufacturer[strlen(searchKey.manufacturer) - 1] == '\n'))
                    searchKey.manufacturer[strlen(searchKey.manufacturer) - 1] = '\0';

            }
            else {
                printf("�������� �����\n");
                break;
            }

            if (searchPrograms(programs, programCount, searchKey, &foundPrograms, &foundCount) == 0) {
                displayData(foundPrograms, foundCount);
                free(foundPrograms);
            }
            else {
                printf("��������� �� �������\n");
            }
            break;

        case 4: /* ���������� */
            printf("����������� ��:\n1. ��������\n2. ���� �������\n��������: ");
            scanf("%d", &choice);

            if (choice == 1) {
                if (sortProgramsByName(programs, programCount) == 0) {
                    printf("���������� �� �������� ���������\n");
                }
            }
            else if (choice == 2) {
                if (sortProgramsByYear(programs, programCount) == 0) {
                    printf("���������� �� ���� ���������\n");
                }
            }
            else {
                printf("�������� �����\n");
            }
            break;

        case 5: /* ���������� */
            if (addNewProgram(&programs, &programCount) == 0) {
                printf("��������� ������� ���������\n");
            }
            else {
                printf("������ ���������� ���������\n");
            }
            break;

        case 6: /* �������������� */
            printf("������� ����� ������ ��� �������������� (1-%d): ", programCount);
            scanf("%d", &index);
            if (editProgram(programs, programCount, index - 1) == 0) {
                printf("������ ������� ��������\n");
            }
            else {
                printf("������ ��������������\n");
            }
            break;

        case 7: /* �������� */
            printf("������� ����� ������ ��� �������� (1-%d): ", programCount);
            scanf("%d", &index);
            if (deleteProgram(programs, &programCount, index - 1) == 0) {
                printf("������ ������� �������\n");
            }
            else {
                printf("������ ��������\n");
            }
            break;

        case 8: /* ���������� */
            if (writeDataToFile(programs, programCount, filename) == 0) {
                printf("������ ������� ���������\n");
            }
            else {
                printf("������ ���������� ������\n");
            }
            break;

        case 0: /* ����� */
            free(programs);
            printf("���������� ���������\n");
            return 0;

        default:
            printf("�������� �����. ���������� �����.\n");
        }
    }
}