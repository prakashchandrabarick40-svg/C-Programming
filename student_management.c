
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

typedef struct {
    int roll;
    char name[50];
    float marks;
} Student;

void addStudent(void) {
    FILE *fp = fopen(FILE_NAME, "ab");
    Student s;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);

    printf("Enter Student Name: ");
    scanf(" %49[^\n]", s.name);

    printf("Enter Marks (0-100): ");
    scanf("%f", &s.marks);

    if (s.marks < 0 || s.marks > 100) {
        printf("Invalid marks! Please enter 0-100.\n");
        fclose(fp);
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\nStudent added successfully!\n");
}

void displayStudents(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    int count = 0;

    if (fp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("\n%-10s %-25s %-10s\n",
           "Roll No.", "Name", "Marks");
    printf("-----------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-10d %-25s %-10.2f\n",
               s.roll, s.name, s.marks);
        count++;
    }

    if (count == 0)
        printf("No student records found.\n");

    fclose(fp);
}

void searchStudent(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    int roll, found = 0;

    if (fp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("Enter roll number to search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\nStudent Found!\n");
            printf("Roll Number : %d\n", s.roll);
            printf("Name        : %s\n", s.name);
            printf("Marks       : %.2f\n", s.marks);
            printf("Result      : %s\n",
                   s.marks >= 40 ? "PASS" : "FAIL");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found.\n");

    fclose(fp);
}

void deleteStudent(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    Student s;
    int roll, found = 0;

    if (fp == NULL || temp == NULL) {
        printf("Unable to open file.\n");
        if (fp != NULL) fclose(fp);
        if (temp != NULL) fclose(temp);
        return;
    }

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        if (remove(FILE_NAME) == 0 &&
            rename("temp.dat", FILE_NAME) == 0) {
            printf("Student deleted successfully!\n");
        } else {
            printf("Could not replace the records file.\n");
        }
    } else {
        remove("temp.dat");
        printf("Student not found.\n");
    }
}

int main(void) {
    int choice;

    do {
        printf("\n=================================\n");
        printf("    STUDENT MANAGEMENT SYSTEM\n");
        printf("=================================\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("---------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                printf("Thank you for using the system!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 5);

    return 0;
}
