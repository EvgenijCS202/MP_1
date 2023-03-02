#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

string inputFile = "info.txt";
string outputFile = "output.txt";
string timeStampsFile = "timestamps.txt";

int dimsNum = 7;

int dims[7] = { 100, 1000, 2000, 5000, 10000, 25000, 100000 };

struct Child {
    int year;
    int squad;
    string name;
    string date;
    int shift;

    Child(
    int year,
    int squad,
    string name,
    string date,
    int shift
    )
    {
        this->year = year;
        this->squad = squad;
        this->name = name;
        this->date = date;
        this->shift = shift;
    }

    friend bool operator== (const Child& a, const Child& b) {
        return (a.year == b.year && a.squad == b.squad && a.name == b.name && a.shift == b.shift)?true:false;
    }

    friend bool operator< (const Child& a, const Child& b) {
        return (a.year < b.year || ((a.year == b.year) && (a.squad < b.squad)) ||
                ((a.year == b.year) && (a.squad == b.squad) && (a.shift < b.shift))||
                ((a.year == b.year) && (a.squad == b.squad) && (a.shift == b.shift) && (a.name < b.name)))?true:false;
    }

    friend bool operator<= (const Child& a, const Child& b) {
        return (a < b || a == b)?true:false;
    }

    friend bool operator> (const Child& a, const Child& b) {
        return (!(a < b) && !(a == b))?true:false;
    }

    friend bool operator>= (const Child& a, const Child& b) {
        return (!(a < b))?true:false;
    }


    friend ostream& operator<<(ostream& os, const Child& a) {
        os << a.year << ' ' << a.squad << ' ' << a.name << ' ' << a.date << ' ' << a.shift << '\n';
        return os;
    }
};

string randomString(const int len) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

string randomDate(int minY, int maxY)
{
    return to_string(rand() % 28 + 1)+'.'+to_string(rand() % 12+1)+'.'+to_string(minY + rand() % (maxY - minY));
}

void createInfo() {
    srand((unsigned)time(NULL));
    ofstream fout(inputFile);

    for (int i = 0; i < dimsNum; ++i) {
        //Вывод числа записей
        fout << dims[i] << "\n";
        for (int j = 0; j < dims[i]; ++j) {
            //Вывод полей по порядку
            fout << 2016 - rand() % 40 << "\n";   // Год
            fout << rand() % 13 + 1 << "\n";   // Отряд
            fout << randomString(10)  << "\n";   // ФИО
            fout << randomDate(1990, 2016) << "\n"; // Дата рождения
            fout << rand() % 5 + 1 << "\n"; // Смена
        }
    }
}

vector<vector<Child>> readInfo() {
    ifstream fin(inputFile);

    vector<vector<Child>> result;

    int dim;
    int year;
    int squad;
    string name;
    string date;
    int shift;

    for (int i = 0; i < dimsNum; ++i) {
        //Ввод числа записей
        fin >> dim;
        vector<Child> v;
        for (int j = 0; j < dim; ++j) {
            //Ввод полей по порядку
            fin >> year >> squad >> name >> date >> shift;
            Child temp(year, squad, name, date, shift);
            v.push_back(temp);
        }
        result.push_back(v);
    }

    return result;
}

vector<Child> selectSort(vector<Child> vec) {
    vector<Child> a = vec;
    int min = 0; // индекс минимального значения
    for (int i = 0; i < a.size(); i++)
    {
        min = i; // изначально min - номер текущей ячейки
        for (int j = i + 1; j < a.size(); j++)
            min = (a[j] < a[min]) ? j : min;
        // после нахождения минимального элемента, поменяем его местами с текущим
        std::swap(a[i], a[min]);
    }

    return a;
}

vector<Child> insertSort(vector<Child> vec) {
    vector<Child> a = vec;
    int i, j;
    for (i = 0; i < a.size(); i++)
    {
        Child x = a[i];
        // Поиск места для вставки
        for (j=i-1; j>=0 && a[j] > x; j--)
            a[j+1] = a[j]; // двигаем неподходящие элементы
        a[j+1] = x; // Помещаем на нужное место
    }
    return a;
}

vector<Child> shakerSort(vector<Child> vec) {
    vector<Child> arr = vec;

    int control = arr.size() - 1;
    int left = 0;
    int right = arr.size() - 1;
    do {
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                control = i;
            }
        }
        right = control;
        for (int i = right; i > left; i--) {
            if (arr[i] < arr[i - 1]) {
                swap(arr[i], arr[i - 1]);
                control = i;
            }
        }
        left = control;
    } while (left < right);

    return arr;
}

void writeOutput(string title, vector<vector<Child>>& res) {
    ofstream fout(outputFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < dimsNum; ++i) {
        //Вывод числа записей
        fout << "Размер: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            //Вывод объекта
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(string title, vector<chrono::steady_clock::time_point> time) {
    ofstream fout(timeStampsFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < time.size() - 1; ++i)
        fout << "Время для сортировки " << dims[i] << " значений: " << chrono::duration_cast<chrono::milliseconds>(time[i+1] - time[i]).count() << " [ms]\n";

    fout << "\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    createInfo();

    vector<vector<Child>> initialArray = readInfo();

    vector<vector<Child>> selectSortRes, insertSortRes, shakerSortRes;

    vector<chrono::steady_clock::time_point> time;

    time.push_back(chrono::steady_clock::now());

    for (int i = 0; i < dimsNum; ++i) {
        selectSortRes.push_back(selectSort(initialArray[i]));
        time.push_back(chrono::steady_clock::now());
    }

    writeOutput("Сортировка выбором:\n", selectSortRes);
    writeTime("Сортировка выбором:\n", time);
    time.clear();

    time.push_back(chrono::steady_clock::now());

    for (int i = 0; i < dimsNum; ++i) {
        insertSortRes.push_back(insertSort(initialArray[i]));
        time.push_back(chrono::steady_clock::now());
    }

    writeOutput("Сортировка вставкой:\n", insertSortRes);
    writeTime("Сортировка вставкой:\n", time);
    time.clear();

    time.push_back(chrono::steady_clock::now());

    for (int i = 0; i < dimsNum; ++i) {
        shakerSortRes.push_back(shakerSort(initialArray[i]));
        time.push_back(chrono::steady_clock::now());
    }

    writeOutput("Сортировка шейкером:\n", shakerSortRes);
    writeTime("Сортировка шейкером:\n", time);
    time.clear();


    return 0;
}
