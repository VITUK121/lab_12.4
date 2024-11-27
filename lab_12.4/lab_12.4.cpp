#include <iostream>  
#include <Windows.h>  
#include <string>  
#include <fstream>  
#include <iomanip>  

using namespace std;

struct Student {
    char prizv[100];
    int mark_1;
    int mark_2;
    int mark_3;
};

int counter(string filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл!" << endl;
        return 0;
    }

    Student student;
    int count = 0;

    while (file.read((char*)(&student), sizeof(Student))) {
        count++;
    }

    file.close();
    return count;
}

void try_input(const char message[], unsigned short& var) {
    bool fail;
    do {
        fail = false;
        cout << message; cin >> var;
        if (cin.fail()) {
            fail = true;
            cout << "Неправильний ввід!\n";
            cin.clear();
            cin.ignore(65535, '\n');
        }
    } while (fail);

}

void del(string filename, string prizv, int& n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл!" << endl;
        return;
    }

    ofstream tempFile("temp.dat", ios::binary);
    if (!tempFile) {
        cout << "Не вдалося створити тимчасовий файл!" << endl;
        return;
    }

    Student student;
    bool found = false;

    while (file.read((char*)(&student), sizeof(Student))) {
        if (strcmp(student.prizv, prizv.c_str()) != 0) {
            tempFile.write((char*)(&student), sizeof(Student));
        }
        else {
            found = true;
        }
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.dat", filename.c_str());  // Correct extension here

    if (found) {
        cout << "Студента з прізвищем " << prizv << " видалено.\n";
    }
    else {
        cout << "Студента з прізвищем " << prizv << " не знайдено.\n";
    }
    n = counter(filename);
}


void writeFile(string filename, int& n) {
    ofstream file(filename, ios::binary | ios::app);

    if (!file) {
        cout << "Не вдалося відкрити файл!" << endl;
        return;
    }

    Student student;
    string continueInput;

    do {
        cout << "Введіть прізвище студента: ";
        cin >> student.prizv;

        cout << "Введіть оцінку 1: ";
        cin >> student.mark_1;

        cout << "Введіть оцінку 2: ";
        cin >> student.mark_2;

        cout << "Введіть оцінку 3: ";
        cin >> student.mark_3;

        file.write((char*)(&student), sizeof(Student));

        cout << "Бажаєте додати ще одного студента? (так/ні): ";
        cin >> continueInput;

    } while (continueInput == "так" || continueInput == "Так");

    file.close();
    cout << "Дані успішно записані у файл." << endl;
    n = counter(filename);
}

void correct(string filename, string prizv) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл!" << endl;
        return;
    }

    ofstream tempFile("temp.dat", ios::binary);
    if (!tempFile) {
        cout << "Не вдалося створити тимчасовий файл!" << endl;
        return;
    }

    Student student;
    bool found = false;

    while (file.read((char*)(&student), sizeof(Student))) {
        if (strcmp(student.prizv, prizv.c_str()) == 0) {
            found = true;
            cout << "Редагування інформації про студента " << prizv << endl;
            cout << "Нова оцінка 1: "; cin >> student.mark_1;
            cout << "Нова оцінка 2: "; cin >> student.mark_2;
            cout << "Нова оцінка 3: "; cin >> student.mark_3;
        }

        tempFile.write((char*)(&student), sizeof(Student));
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.dat", filename.c_str());

    if (found) {
        cout << "Інформацію про студента " << prizv << " оновлено.\n";
    }
    else {
        cout << "Студента з прізвищем " << prizv << " не знайдено.\n";
    }
}


int lenOfLongestPrizv(string filename, int n) {
    int res = 0;

    ifstream file(filename, ios::binary);
    Student student;

    for (int i = 0; i < n; i++) {
        file.read((char*)(&student), sizeof(Student));
        if (strlen(student.prizv) > res) {
            res = strlen(student.prizv);
        }
    }

    file.close();
    return res;
}

int calc_n(int n) {
    int res = 0;
    while (n != 0) {
        n /= 10;
        res++;
    }
    return res;
}

void printStudents(string filename, int n) {
    int width = lenOfLongestPrizv(filename, n);
    (width <= 8) ? (width = 8) : (NULL);
    int maxDigits = calc_n(n);

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 45; i++) {
        cout << "=";
    }
    cout << endl;

    cout << "|" << setw(maxDigits) << right << "№";
    cout << "|" << setw(width) << left << "Прізвище" << "| Оцінка 1 | Оцінка 2 | Оцінка 3 |" << endl;

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 45; i++) {
        cout << "-";
    }
    cout << endl;

    ifstream file(filename, ios::binary);
    Student student;

    for (int i = 0; i < n; i++) {
        file.read((char*)(&student), sizeof(Student));

        cout << "|" << setw(maxDigits) << right << i + 1;
        cout << "|" << setw(width) << left << student.prizv;
        cout << "|" << setw(10) << left << student.mark_1;
        cout << "|" << setw(10) << left << student.mark_2;
        cout << "|" << setw(10) << left << student.mark_3;
        cout << "|" << endl;
    }

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 45; i++) {
        cout << "-";
    }
    cout << endl;
    file.close();
}

bool compareByPrizv(Student a, Student b) {
    return strcmp(a.prizv, b.prizv) > 0;
}

bool compareByMark1(Student a, Student b) {
    return a.mark_1 > b.mark_1;
}

bool compareByMark2(Student a, Student b) {
    return a.mark_2 > b.mark_2;
}

bool compareByMark3(Student a, Student b) {
    return a.mark_3 > b.mark_3;
}

void sortStudents(string filename, int criteria) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Не вдалося відкрити файл!" << endl;
        return;
    }

    file.seekg(0, ios::end);
    int n = file.tellg() / sizeof(Student);
    file.seekg(0, ios::beg);

    if (n <= 1) {
        cout << "Файл містить менше двох студентів. Сортування неможливе." << endl;
        return;
    }

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Student student1, student2;
            file.seekg(i * sizeof(Student), ios::beg);
            file.read(reinterpret_cast<char*>(&student1), sizeof(Student));
            file.seekg(j * sizeof(Student), ios::beg);
            file.read(reinterpret_cast<char*>(&student2), sizeof(Student));

            bool needSwap = false;

            switch (criteria) {
            case 1:
                needSwap = compareByPrizv(student1, student2);
                break;
            case 2:
                needSwap = compareByMark1(student1, student2);
                break;
            case 3:
                needSwap = compareByMark2(student1, student2);
                break;
            case 4:
                needSwap = compareByMark3(student1, student2);
                break;
            default:
                cout << "Невірний критерій сортування!" << endl;
                return;
            }

            if (needSwap) {
                file.seekp(i * sizeof(Student), ios::beg);
                file.write(reinterpret_cast<char*>(&student2), sizeof(Student));
                file.seekp(j * sizeof(Student), ios::beg);
                file.write(reinterpret_cast<char*>(&student1), sizeof(Student));
            }
        }
    }

    file.close();
    cout << "Сортування завершено.\n";
}

void menu(string filename, int& n) {
    int option_1, course, it, res;
    string prizv;

    while (true) {
        cout << "Меню:\n[1] Додати студента\n[2] Видалити студента\n[3] Редагувати інформацію про студента\n[4] Вивід списку студентів\n[5] Сортування списку студентів за прізвищами\n[6] Сортування списку студентів за оцінкою 1\n[7] Сортування списку студентів за оцінкою 2\n[8] Сортування списку студентів за оцінкою 3\n[9] Вихід з програми\n : ";
        cin >> option_1;

        switch (option_1) {
        case 1:
            writeFile(filename, n);
            break;

        case 2:
            cout << "Введіть прізвище студента: "; cin >> prizv;
            del(filename, prizv, n);
            break;

        case 3:
            cout << "Введіть прізвище студента: "; cin >> prizv;
            correct(filename, prizv);
            break;

        case 4:
            printStudents(filename, n);
            break;

        case 5:
            sortStudents(filename, 1);
            break;

        case 6:
            sortStudents(filename, 2);
            break;

        case 7:
            sortStudents(filename, 3);
            break;

        case 8:
            sortStudents(filename, 4);
            break;

        case 9:
            return;

        default:
            cout << "Неправильний ввід!\n";
            break;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string filename;

    cout << "Ім'я файлу: ";
    cin >> filename;

    int n = counter(filename);

    menu(filename, n);
}