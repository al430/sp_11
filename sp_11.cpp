#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <Windows.h>
#include "Settings.h"
#include <conio.h>
using namespace std;

int count_robots = 0;

class Robot
{
    int energy = 60;
    int points = 0;
    int status = 1;
    int status_arena = 0;
    int iD = 0;

public:
    string random_number = "";
    HANDLE hThread;
    DWORD idThread;
    void SetEnergy(int e)
    {
        if (e < 0)
        {
            std::cout << "Энергия не может быть отрицательной или делать энергию отрицательной";
            return;
        }
        energy = e;
    }
    int GetEnergy()
    {
        return energy;
    }
    void SetPoints(int e)
    {
        if (e < 0)
        {
            std::cout << "Очки не может быть отрицательной";
            return;
        }
        points = e;
    }
    int GetPoints()
    {
        return points;
    }
    void SetStatus(int e)
    {
        if (e < 0 || e > 2)
        {
            std::cout << "Статусы: 0 = работает. 1 = приостановлен. 2 = завершил испытания";
            return;
        }
        status = e;
    }
    string GetStatusString()
    {
        if (status == 0)
            return "Работает";
        else if (status == 1)
            return "Приостановлен";
        else if (status == 2)
            return "Завершил испытание";
    }
    int GetStatus()
    {
        return status;
    }

    string GetStatusArenaString()
    {
        if (status_arena == 1)
            return "Препятствие";
        else if (status_arena == 2)
            return "Спринт";
        else if (status_arena == 3)
            return "Подзарядка";
        else if (status_arena == 4)
            return "Марафон";
        else if (status_arena == 5)
            return "Отдых между испытаниями";
        else if (status_arena == 6)
            return "Пропускает испытание";
        else if (status_arena == 0)
            return "Приостановлен";
    }
    void SetStatusArena(int status_)
    {
        status_arena = status_;
    }
    int GetId()
    {
        return iD;
    }
    void SetId(int j)
    {
        iD = j;
    }
    void STOP()
    {
        SetStatus(1);
        SetStatusArena(0);
    }
    void START()
    {
        SetStatus(0);
        SetStatusArena(5);
    }
    void ARENA_DOWN(bool g)
    {
        if (!g)
        {
            SetStatusArena(5);
            int count = rand() % 3 + 1;
            random_number = " | R: " + to_string(count);
            cout << "Робот " << iD << " отдых между испытаниями " << count << " с " << endl;
            Sleep(count * 1000);
        }
        else
        {
            int count = rand() % 3 + 1;
            random_number = " | R: " + to_string(count);
            cout << " " << iD << " отдых " << count << " с" << endl;
            Sleep(count * 1000);
        }
    }
    void ARENA_END()
    {
        SetStatus(2);
        SetStatusArena(0);
        cout << " " << iD << " завершил  итог " << points << " очки" << endl;
    }
    void ARENA(int number_round)
    {
        try
        {
            bool is_no = false;
            while (status == 1)
            {
                Sleep(1000);
            }

            SetStatus(0);
            if (number_round == 1)
            {
                if (energy >= 10)
                {
                    SetStatusArena(1);
                    cout << "Робот " << iD << " начал препятствие 3 сек" << endl;
                    Sleep(TIME_SLEEP_1_ROUND);
                    energy -= 10;
                    points += 15;
                    cout << " " << iD << " завершил Препятствие энергия: " << energy << " очки " << points << endl;
                }
                else
                {
                    SetStatusArena(6);
                    is_no = true;
                    cout << "Робот " << iD << " пропустил Препятствие - недостаточно энергии" << endl;
                }
            }
            else if (number_round == 2)
            {
                if (energy >= 20)
                {
                    SetStatusArena(2);
                    cout << "Робот " << iD << " начал Спринт 4 сек" << endl;
                    Sleep(TIME_SLEEP_2_ROUND);
                    energy -= 20;
                    points += 30;
                    cout << "Робот " << iD << " завершил Спринт  энергия " << energy << " очки " << points << endl;
                }
                else
                {
                    SetStatusArena(6);
                    is_no = true;
                    cout << "Робот " << iD << " пропустил Спринт недостаточно энергии" << endl;
                }
            }
            else if (number_round == 3)
            {
                SetStatusArena(3);
                int size_time = rand() % 5 + 1;
                random_number = " | R: " + to_string(size_time);
                cout << "Робот " << iD << " начал Подзарядка " << size_time << " сек" << endl;
                Sleep(size_time * 1000);
                int size_energy_plus = size_time * 10;

                if (energy + size_energy_plus > MAX_HEALTH_ROBOT)
                    energy = MAX_HEALTH_ROBOT;
                else
                    energy += size_energy_plus;
                cout << " " << iD << " зарядился энергия " << energy << endl;
            }
            else if (number_round == 4)
            {
                if (energy >= 60)
                {
                    SetStatusArena(4);
                    cout << " " << iD << " нач марафон " << endl;
                    Sleep(TIME_SLEEP_4_ROUND);
                    energy -= 60;
                    points += 55;
                    cout << " " << iD << " зав марафон - энергия: " << energy << " очки " << points << endl;
                }
                else
                {
                    SetStatusArena(6);
                    is_no = true;
                    cout << " " << iD << " пропустил марафон " << endl;
                }
            }

            ARENA_DOWN(is_no);
        }
        catch (const std::exception(e))
        {
            cout << e.what() << "\n";
        }
    }

};
Robot* robots = new Robot[0];
int* arena = new int[0];
int* arena_random_numbers = new int[0];
int arena_rounds = 0;
int GetMeBind(int code)
{
    if (code == 49)
        return 1;
    else if (code == 50)
        return 2;
    else if (code == 51)
        return 3;
    return code;
}

bool CheckAllRobots()
{
    for (int i = 0; i < count_robots; i++)
    {
        if (robots[i].GetStatus() != 2)
            return false;
    }
    return true;
}
void CloseAllHandle()
{
    for (int i = 0; i < count_robots; i++)
    {
        if (robots[i].hThread != NULL && robots[i].GetStatus() == 2)
        {
            WaitForSingleObject(robots[i].hThread, INFINITE);
            CloseHandle(robots[i].hThread);
        }
    }
}
void PrintWinnerList()
{
    for (int i = 0; i < count_robots; i++)
    {
        Robot max = robots[i];
        int index = i;
        for (int z = 0 + i; z < count_robots; z++)
        {
            if (max.GetPoints() < robots[z].GetPoints())
            {
                max = robots[z];
                index = z;
            }
        }
        robots[index] = robots[i];
        robots[i] = max;
    }
    for (int a = 0; a < count_robots; a++)
    {
        cout << a + 1 << ". ID: " << robots[a].GetId() << "| Очки: " << robots[a].GetPoints() << robots[a].random_number << "\n";
    }
    string result = "Победил ID: " + to_string(robots[0].GetId()) + " | C счетом: " + to_string(robots[0].GetPoints());
    int max_points = robots[0].GetPoints();
    int winners_count = 1;
    for (int a = 1; a < count_robots; a++)
    {
        if (robots[a].GetPoints() == max_points)
        {
            winners_count++;
        }
    }
    if (winners_count > 1)
    {
        result = "Ничья";
    }

    cout << "\nРезультаты испытаний: " << result;

    cout << "\nНажмите любую клавишу для завершения";
    if (_getch())
        return;

    Sleep(5000);
}
DWORD WINAPI Arena(LPVOID s)
{
    int id = (int)s;
    srand(arena_random_numbers[id]);
    for (int i = 0; i < arena_rounds; i++)
        robots[id].ARENA(arena[i]);
    robots[id].ARENA_END();
    return 0;
};
int CreateThreadx(int id)
{
    robots[id].hThread = CreateThread(NULL, 0, Arena, (void*)id, 0, &robots[id].idThread);

    if (robots[id].hThread == NULL)
        return GetLastError();

    return 0;
}
void GetArena()
{
    try
    {
        ifstream file(FILE_NAME, ios::binary);
        if (!file.is_open())
            throw new invalid_argument("Файл не открылся");
        file.seekg(0, ios::end);
        int size = file.tellg() / sizeof(int);
        arena = new int[size];
        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(arena), size * sizeof(int));
        file.close();
        arena_rounds = size;
    }
    catch (const std::exception(e))
    {
        cout << e.what() << "\n";
    }
}
bool CreateRobots(int count)
{
    bool active = true;
    srand(time(NULL));
    try
    {
        if (count < 3 || count > 10)
        {
            throw new invalid_argument("Количество роботов должно быть от 3 до 10");
        }
        count_robots = count;
        robots = new Robot[count];
        arena_random_numbers = new int[count];

        for (int i = 0; i < count; i++)
        {
            Robot g = Robot();
            g.SetId(i);
            arena_random_numbers[i] = rand();
            robots[i] = g;
        }
    }
    catch (const std::invalid_argument(e))
    {
        cout << e.what() << "\n";
        active = false;
    }
    return active;
}
void GoArena()
{
    try
    {
        if (count_robots <= 2)
        {
            throw new invalid_argument("Количество роботов меньше допустимого или они пока не созданы");
        }
        for (int i = 0; i < count_robots; i++)
        {
            CreateThreadx(i);
        }
    }
    catch (const std::exception(e))
    {
        cout << e.what() << "\n";
    }
}

void PrintAllRobots()
{
    if (count_robots <= 0)
    {
        cout << "\nРоботов пока нет";
        if (_getch())
            return;
    }
    cout << "Список роботов:\n";
    for (int i = 0; i < count_robots; i++)
    {
        cout << "ID: " << robots[i].GetId() << " | Энергия: " << robots[i].GetEnergy() << " | Очки: " << robots[i].GetPoints() << " | Статус: " << robots[i].GetStatusString() << " | Действие: " << robots[i].GetStatusArenaString() << "\n";
    }
    cout << "\nУправление:\n";
    cout << "1 - Приостановить робота\n";
    cout << "2 - Возобновить робота\n";
    cout << "3 - Выход\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    GetArena();
    int count = 0;
    cout << "Введите количество роботов (3-10): ";
    cin >> count;
    if (!CreateRobots(count))
    {
        return 1;
    }
    GoArena();
    Sleep(1000);

   
    for (int i = 0; i < count_robots; i++)
    {
        robots[i].START();
    }

    
    PrintAllRobots();

    while (!CheckAllRobots())
    {
        PrintAllRobots();
        int key = _getch();
        int code = GetMeBind(key);
        if (code == 1)
        {
            int id;
            cout << "\nВведите ID робота для приостановки: ";
            cin >> id;
            if (id >= 0 && id < count_robots)
            {
                robots[id].STOP();
            }
        }
        else if (code == 2)
        {
            int id;
            cout << "\nВведите ID робота для возобновления: ";
            cin >> id;
            if (id >= 0 && id < count_robots)
            {
                robots[id].START();
            }
        }
        else if (code == 3)
        {
            break;
        }
    }

    CloseAllHandle();
    PrintWinnerList();

    return 0;
}
