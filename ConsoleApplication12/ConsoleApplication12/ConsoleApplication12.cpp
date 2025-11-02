#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>
using namespace std;
class Player {
    string name;
public:
    void SetName() {
        cout << "What your name? ";
        cin >> name;
        system("cls");
        this->name = name;
    }
    string GetName() const {
        return name;
    }
    void Print() {
        cout << "Hello " << name << "\n";
        Sleep(3000);
        system("cls");
        cout << "This game about a labyrinth,where you need need collected coin,don't get caught on enemy get there to exit.";
        cout << "\n";
        cout << "\n";
        cout << "\n";
        cout << "                                                        Good luck";
    }
};
class Game {
public:
    //Штуки на карте
    enum GameObject : short {
        HALL,
        WALL,
        COIN,
        ENEMY,
        HEALTH,
        ENERGY,
        ENERGY_ENEMY,
        MONEY_ENEMY
    };

    // Цвета
    enum Color : short {
        BLACK, DARKBLUE, DARKGREEN, TURQUOISE, DARKRED,
        PURPLE, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN,
        CYAN, RED, PINK, YELLOW, WHITE, LIGHTRED
    };
    // Кнопки
    enum Key : short {
        LEFT = 'a',
        RIGHT = 'd',
        UP = 'w',
        DOWN = 's',
    };
    class Statistics {
    public:
        void statistics(int walking, int coins_collected) {

            cout << "Your walk-" << walking << "m" << "\n";// кол-во шагов
            cout << "Your money-" << coins_collected;// кол-во монет
            ofstream out("stats.txt", std::ios::app);/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            out << "walk=" << walking << ", coins=" << coins_collected << "\n";
        }
    };
    // Отрисовка всех объектов карты
    class DrawObject {
    public:
        void draw_object(int map[25][65], int HEIGHT, int WIDTH, HANDLE h) {
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    switch (map[y][x]) {
                    case GameObject::HALL:
                        SetConsoleTextAttribute(h, Color::BLACK);
                        cout << " ";
                        break;
                    case GameObject::WALL:
                        SetConsoleTextAttribute(h, Color::DARKGREEN);
                        cout << (char)219; // Символ стены
                        break;
                    case GameObject::COIN:
                        SetConsoleTextAttribute(h, Color::YELLOW);
                        cout << (char)248; // Символ монеты 
                        break;
                    case GameObject::ENEMY:
                        SetConsoleTextAttribute(h, Color::PURPLE);
                        cout << "O"; // Враг
                        break;
                    case GameObject::ENERGY:
                        SetConsoleTextAttribute(h, Color::BLUE);
                        cout << "A"; // Энергия
                        break;
                    case GameObject::HEALTH:
                        SetConsoleTextAttribute(h, Color::RED);
                        cout << "#"; // Жизнь
                        break;
                    case GameObject::ENERGY_ENEMY:
                        SetConsoleTextAttribute(h, Color::DARKBLUE);
                        cout << "O"; // Ворог який забирае єнергію
                        break;
                    case GameObject::MONEY_ENEMY:
                        SetConsoleTextAttribute(h, Color::DARKYELLOW);
                        cout << "O"; // Ворог який забирае коіни
                        break;
                    }
                }
                cout << "\n";
            }
        }
    };
    class Items {
    public:
        // Отображение количества собранных монет, жизней и энергии
        void items(int coins_collected, int heart_collected, int energy_collected, const int WIDTH, HANDLE h) {
            COORD coins_info = { WIDTH + 1, 0 }; // Задаём координаты для вывода информации о монетах 
            SetConsoleCursorPosition(h, coins_info); // Перемещаем курсор консоли в указанные координаты
            SetConsoleTextAttribute(h, Color::YELLOW); // Меняем цвет текста на ярко-жёлтый для значения количества монет
            cout << "COINS:" << coins_collected << "   "; // Выводим текстовую метку "COINS:"


            COORD heart_info = { WIDTH + 1, 1 };
            SetConsoleCursorPosition(h, heart_info);
            SetConsoleTextAttribute(h, Color::RED);
            cout << "HEART:" << heart_collected;

            COORD energy_info = { WIDTH + 1, 2 };
            SetConsoleCursorPosition(h, energy_info);
            SetConsoleTextAttribute(h, Color::BLUE);
            cout << "ENERGY:" << energy_collected << "   ";
        }
    };
    class Defeat :Player {
    public:
        // Сообщения о поражении игрока
        void defeat(int heart_collected, int energy_collected, int walking, int coins_collected) {
            if (heart_collected == 0) {
                system("cls");//эта строка полностью очищяет экран
                cout << "I am so sorry!" << " Your character has run out of lives.\n";
                Statistics stat;
                stat.statistics(walking, coins_collected);
            }
            if (energy_collected <= 0) {
                system("cls");//эта строка полностью очищяет экран
                cout << "I am so sorry!" << " Your character is tired and can no longer walk.\n";
                Statistics stat;
                stat.statistics(walking, coins_collected);
            }
        }
    };
    // Проверка условия победы — герой добрался до выхода
    class Win :Player {
    public:
        void win(const int HEIGHT, const int WIDTH, int walking, int coins_collected, COORD hero) {
            if ((hero.X == WIDTH - 1) && (hero.Y == HEIGHT - 3)) {
                system("cls");
                cout << "Congratulations!" << " Your win\n";
                Statistics stat;
                stat.statistics(walking, coins_collected);
            }
        }
    };

    void labirint() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


        srand(time(0));
        rand();

        const int HEIGHT = 25; // Высота карты
        const int WIDTH = 65;  // Ширина карты
        int map[HEIGHT][WIDTH] = {}; // Игровая карта

        int walking = 0;       // Кол-во шагов, сделанных игроком
        int coins_collected = 0; //  Кол-во собранных монет
        int heart_collected = 3; //  Кол-во жизней у игрока в начале
        int energy_collected = 50; //  Кол-во энергии у игрока в начале

        COORD hero;
        hero.X = 0;
        hero.Y = 2;
        // Генерация карты
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                map[y][x] = rand() % 8;




                // Установка границ карты
                if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
                    map[y][x] = GameObject::WALL;

                // Дырки для финиша
                if (x == WIDTH - 1 && y == HEIGHT - 3 ||
                    x == WIDTH - 2 && y == HEIGHT - 3 ||
                    x == WIDTH - 3 && y == HEIGHT - 3)
                    map[y][x] = GameObject::HALL;

                // Дырки для старта 
                if ((x <= 2 && y == 2) || (x >= WIDTH - 3 && y == HEIGHT - 3))
                    map[y][x] = GameObject::HALL;

                // Уменшение обьъектов
                if (map[y][x] == GameObject::ENEMY && rand() % 2 != 0)
                    map[y][x] = GameObject::HALL;

                if (map[y][x] == GameObject::ENERGY && rand() % 5 != 0)
                    map[y][x] = GameObject::HALL;


                if (map[y][x] == GameObject::HEALTH && rand() % 5 != 0)
                    map[y][x] = GameObject::HALL;

                if (map[y][x] == GameObject::ENERGY_ENEMY && rand() % 5 != 0)
                    map[y][x] = GameObject::HALL;

                if (map[y][x] == GameObject::MONEY_ENEMY && rand() % 7 != 0)
                    map[y][x] = GameObject::HALL;
            }
        }

        // Отрисовка карты
        DrawObject draw;
        draw.draw_object(map, HEIGHT, WIDTH, h);

        // Отображение героя
        SetConsoleCursorPosition(h, hero);
        SetConsoleTextAttribute(h, Color::BLUE);
        cout << "O";

        // Отображение текущего состояния игрока
        Items item;
        item.items(0, 3, 50, 65, h);

        // Главный игровой цикл
        while (true) {
            int code = _getch();         // Считывание клавиши


            COORD old_position = hero;
            bool has_been_moved = false;

            // Обработка движения
            switch (code) {
                //case Key::LEFT:
            case Key::LEFT:
                if (hero.X > 0 && map[hero.Y][hero.X - 1] != GameObject::WALL) {
                    hero.X--;
                    has_been_moved = true;
                    walking++;//за каждый шаг +1 к walking в конце это пишеться в статистики
                    energy_collected--;//за каждый шаг -1 энергия изначально энергии 50 если её будет 0 игра закончиться
                }
                break;
            case Key::RIGHT:
                if (map[hero.Y][hero.X + 1] != GameObject::WALL) {
                    hero.X++; has_been_moved = true;
                    walking++;//за каждый шаг +1 к walking в конце это пишеться в статистики
                    energy_collected--;//за каждый шаг -1 энергия изначально энергии 50 если её будет 0 игра закончиться
                }
                break;
            case Key::UP:
                if (map[hero.Y - 1][hero.X] != GameObject::WALL) {
                    hero.Y--; has_been_moved = true;
                    walking++;//за каждый шаг +1 к walking в конце это пишеться в статистики
                    energy_collected--;//за каждый шаг -1 энергия изначально энергии 50 если её будет 0 игра закончиться
                }
                break;
            case Key::DOWN:
                if (map[hero.Y + 1][hero.X] != GameObject::WALL) {
                    hero.Y++; has_been_moved = true;
                    walking++;//за каждый шаг +1 к walking в конце это пишеться в статистики
                    energy_collected--;//за каждый шаг -1 энергия изначально энергии 50 если её будет 0 игра закончиться
                }
                break;

            }


            if (has_been_moved) {
                // Подбор предметов
                if (map[hero.Y][hero.X] == COIN) {
                    coins_collected++;
                    map[hero.Y][hero.X] = HALL;
                }
                if (map[hero.Y][hero.X] == ENERGY) {
                    energy_collected += 51;
                    map[hero.Y][hero.X] = HALL;
                }
                if (map[hero.Y][hero.X] == HEALTH) {
                    heart_collected++;
                    map[hero.Y][hero.X] = HALL;
                }
                if (map[hero.Y][hero.X] == ENERGY_ENEMY) {
                    energy_collected -= 19;
                    map[hero.Y][hero.X] = HALL;
                }
                if (map[hero.Y][hero.X] == MONEY_ENEMY) {
                    coins_collected -= 10;
                    map[hero.Y][hero.X] = HALL;
                }
                if (map[hero.Y][hero.X] == ENEMY) {
                    heart_collected--;
                    map[hero.Y][hero.X] = HALL;
                }


                // стирання ГГ в "старих" координатах
                SetConsoleCursorPosition(h, old_position);
                SetConsoleTextAttribute(h, Color::BLACK);
                cout << " ";
                // перемальовка ГГ в нових координатах
                SetConsoleCursorPosition(h, hero);
                SetConsoleTextAttribute(h, Color::BLUE);
                cout << "O";

                item.items(coins_collected, heart_collected, energy_collected, WIDTH, h);
                Win win;
                win.win(HEIGHT, WIDTH, walking, coins_collected, hero);
                Defeat defeat;
                defeat.defeat(heart_collected, energy_collected, walking, coins_collected);
            }
        }
    }
};
int main() {
    Player player;
    player.SetName();
    player.Print();
    Sleep(5000);
    system("cls");
    Game start;
    start.labirint();
}