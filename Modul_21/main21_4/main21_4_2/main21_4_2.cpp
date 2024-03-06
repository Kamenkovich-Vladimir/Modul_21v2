#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

struct coordinates {
    int x = 0;
    int y = 0;
};

struct characters {
    bool side = false; //Флаг принадлежности к команде, по умолчанию - Е (ложь - тёмная сторона силы)
    std::string name = "Unknown";
    int health = 0;
    int armor = 0;
    int damage = 0;
    coordinates location;
};

void take_damage(const characters& attacker, characters& defending) {
    std::cout << defending.name << " took damage - " << attacker.damage << " from " << attacker.name << std::endl;
    defending.armor -= attacker.damage;

    if (defending.armor < 0) {
        defending.health += defending.armor;
        defending.armor = 0;
    }
    std::cout << defending.name << "'s health: " << defending.health << std::endl;
}

void show_battlefield(const std::string field[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void initializing_opponent(std::vector<characters>& character) {
    for (int i = 0; i < 5; i++) {
        character[i].name = "Enemy#" + std::to_string(i);
        character[i].health = (std::rand() % 101) + 50;
        character[i].armor = std::rand() % 51;
        character[i].damage = (std::rand() % 16) + 15;
    }
}

void initializing_battlefield(std::string field[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            field[i][j] = ".";
        }
    }
}

void first_initialization_character_location(std::string field[20][20], std::vector<characters>& character) {
    int x, y;

    for (int i = 0; i < 6; i++) {
        do {
            x = std::rand() % 20;
            y = std::rand() % 20;
        } while (field[x][y] != ".");

        field[x][y] = (i < 5) ? "E" : "P";

        character[i].location.x = x;
        character[i].location.y = y;
    }
}
void initialization_character_location(std::string field[20][20], std::vector<characters>& character) {
    for (int i = 0; i < 6; i++) {
        if (character[i].location.x > 19)
            continue;
        else if (i < 5)
            field[character[i].location.x][character[i].location.y] = "E";
        else if (i == 5)
            field[character[i].location.x][character[i].location.y] = "P";
        else
        {
            std::cerr << "Error in characters location\n\n";
            break;
        }

    }

}

void outfield(characters& character, int i, std::string field[20][20]) {
    field[character.location.x][character.location.y] = ".";
    character.location.x = 21;
    character.location.y = 21;
}

void enemy_move(int dir, characters& enemy, characters& player, int i, std::string field[20][20]) {

    switch (dir) {
    case 0:     //Left
        if (enemy.location.y > 0 && field[enemy.location.x][enemy.location.y - 1] == ".") {
            field[enemy.location.x][enemy.location.y - 1] = "E";

            field[enemy.location.x][enemy.location.y] = ".";
            enemy.location.y -= 1;
            std::cout << enemy.name << " moved left\n\n";
        }
        else if (field[enemy.location.x][enemy.location.y - 1] == "P")
            take_damage(enemy, player);
        break;
    case 1:     //Right
        if (enemy.location.y < 19 && field[enemy.location.x][enemy.location.y + 1] == ".") {
            field[enemy.location.x][enemy.location.y + 1] = "E";

            field[enemy.location.x][enemy.location.y] = ".";
            enemy.location.y += 1;
            std::cout << enemy.name << " moved right\n\n";
        }
        else if (field[enemy.location.x][enemy.location.y + 1] == "P")
            take_damage(enemy, player);
        break;
    case 2:     //Up
        if (enemy.location.x > 0 && field[enemy.location.x - 1][enemy.location.y] == ".") {
            field[enemy.location.x - 1][enemy.location.y] = "E";

            field[enemy.location.x][enemy.location.y] = ".";
            enemy.location.x -= 1;
            std::cout << enemy.name << " moved up\n\n";
        }
        else if (field[enemy.location.x - 1][enemy.location.y] == "P")
            take_damage(enemy, player);
        break;
    case 3:     //Down
        if (enemy.location.x < 19 && field[enemy.location.x + 1][enemy.location.y] == ".") {
            field[enemy.location.x + 1][enemy.location.y] = "E";

            field[enemy.location.x][enemy.location.y] = ".";
            enemy.location.x += 1;
            std::cout << enemy.name << " moved down\n\n";
        }
        else if (field[enemy.location.x + 1][enemy.location.y] == "P")
            take_damage(enemy, player);
        break;
    }
}

void enemy_direction_movement(std::vector<characters>& character, std::string field[20][20]) {
    int dir, delta_x, delta_y;            //Определим упрощённо направление движения противника в сторону персонажа Игрока

    for (int i = 0; i < 5; i++) {

        delta_x = character[i].location.x - character[5].location.x;
        if (delta_x > 0)
            dir = 2;
        else if (delta_x < 0)
            dir = 3;
        else
        {
            delta_y = character[i].location.y - character[5].location.y;
            if (delta_y > 0)
                dir = 0;
            else
                dir = 1;
        }
        if (character[i].health < 0) {
            continue;           //Предотвращаем действия выведенного из игры персонажа соперника
        }
        enemy_move(dir, character[i], character[5], i, field);    //Ход врага

        //Выводим персонаж Игрока за поле, если погиб в неравной схватке         
        if (character[5].health < 0) {
            outfield(character[5], 5, field);
            break;
        }
    }
}
void save_game(std::vector<characters>& character) {
    std::ofstream file_strim;
    file_strim.open("game.bin", std::ios::binary);
    if (!file_strim.is_open()) {
        std::cerr << "The file 'in the specified path'game.bin cannot be opened\n";
    }
    else {
        for (int i = 0; i < 6; i++) {
            file_strim.write((char*)&character[i].side, sizeof(bool));
            file_strim.write((char*)&character[i].health, sizeof(int));
            file_strim.write((char*)&character[i].armor, sizeof(int));
            file_strim.write((char*)&character[i].damage, sizeof(int));
            file_strim.write((char*)&character[i].location.x, sizeof(int));
            file_strim.write((char*)&character[i].location.y, sizeof(int));
            size_t len = character[i].name.size();
            file_strim.write((char*)&len, sizeof(size_t));
            file_strim.write(character[i].name.data(), len);
        }
        std::cout << "The game data has been successfully written to the 'game.bin' file \n";

        file_strim.close();
    }
}

void load_game(std::vector<characters>& character) {
    //Проверка наличия файла game.bin в папке проекта
    if (std::ifstream("game.bin")) {
        std::ifstream file_strim;
        file_strim.open("game.bin", std::ios::binary);
        if (!file_strim.is_open()) {
            std::cerr << "The file 'game.bin' cannot be opened\n";
            
        }
        else {
            for (int i = 0; i < 6; i++) {
                file_strim.read((char*)&character[i].side, sizeof(character[i].side));

                file_strim.read((char*)&character[i].health, sizeof(character[i].health));
                file_strim.read((char*)&character[i].armor, sizeof(character[i].armor));
                file_strim.read((char*)&character[i].damage, sizeof(character[i].damage));
                file_strim.read((char*)&character[i].location.x, sizeof(character[i].location.x));
                file_strim.read((char*)&character[i].location.y, sizeof(character[i].location.y));
                size_t len;
                file_strim.read((char*)&len, sizeof(size_t));
                character[i].name.resize(len);
                file_strim.read((char*)character[i].name.data(), len);
            }
            std::cout << "The game data has been successfully readen from the 'game.bin' file \n";
            file_strim.close();
        }
        
    }
    else
        std::cerr << "The file 'game.bin' is missed\n";
}

//Компилятор предложил часть данных перенести в кучу
std::string field[20][20];  //Поле боя
std::vector<characters> character(6);

int main()
{
    std::cout << "************** Turn-based role-playing game *************\n\n";

    srand(time(0));
    
    char contin; //Вспомогательная переменная для промежуточного просмотра игрового поля и продолжения игры

    //Инициализация персонажей соперника  
    initializing_opponent(character);

    //Инициализация персонажа Игрока
    character[5].side = true;
    std::cout << "Enter name of your character: ";
    std::cin >> character[5].name;

    std::cout << "Enter the value of your character's health: ";
    std::cin >> character[5].health;

    std::cout << "Enter the value of your character's armor: ";
    std::cin >> character[5].armor;

    std::cout << "Enter the value of your character's damage: ";
    std::cin >> character[5].damage;

    //Инициализация игрового поля
    initializing_battlefield(field);

    //Инициализация локации персонажей
    first_initialization_character_location(field, character);

    //Выводим игровое поле на экран
    show_battlefield(field);
    std::cout << "To continue the game, enter any simbol:  ";
    std::cin >> contin;

    //Игровой цикл пока не сдохнут все противники.Проверка здоровья персонажа Игрока и выход из игры при летальном исходе - внутри цикла
    while (true) {

        //Последовательно каждый персонаж, если живой, делает ход. Последним делает ход персонаж Игрока
        enemy_direction_movement(character, field); //Выбор направления, а затем ход врага

        //Выводим игровое поле на экран
        show_battlefield(field);
        if (character[5].health < 0) { //Если персонаж Игрока испустил дух, то заканчиваем битву
            break;
        }
        //Ход делает Игрок
        std::cout << "Specify the command to select the direction of movement (impact)\n";
        std::cout << "('l' - left, 'r' - right, 'u' - up, 'd' - down,)\n";
        std::cout << "or 's' - to save Game, or 'z' to load saved Game, another simbol - skips a turn\n";

        char ch_dir;
        std::cin >> ch_dir;

        if (ch_dir == 'l') {
            if (character[5].location.y > 0 && field[character[5].location.x][character[5].location.y - 1] == ".") {
                field[character[5].location.x][character[5].location.y - 1] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.y -= 1;
                std::cout << character[5].name << " moved left\n\n";
            }
            else if (field[character[5].location.x][character[5].location.y - 1] == "E") {
                //Идентификация ближайшего врага и атака на него
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x) &&
                        (character[i].location.y == character[5].location.y - 1)) {
                            take_damage(character[5], character[i]);
                            if (character[i].health < 0) {
                                outfield(character[i], i, field);
                            }
                    break;
                    }
                }    
            }
        }
        else if (ch_dir == 'r') {
            if (character[5].location.y < 19 && field[character[5].location.x][character[5].location.y + 1] == ".") {
                field[character[5].location.x][character[5].location.y + 1] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.y += 1;
                std::cout << character[5].name << " moved right\n\n";
            }
            else if (field[character[5].location.x][character[5].location.y + 1] == "E") {
                //Идентификация ближайшего врага и атака на него
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x) &&
                        (character[i].location.y == character[5].location.y + 1)) {
                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            outfield(character[i], i, field);
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 'u') {
            if (character[5].location.x > 0 && field[character[5].location.x - 1][character[5].location.y] == ".") {
                field[character[5].location.x - 1][character[5].location.y] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.x -= 1;
                std::cout << character[5].name << " moved up\n\n";
            }
            else if (field[character[5].location.x - 1][character[5].location.y] == "E") {
                //Идентификация ближайшего врага и атака на него
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x - 1) &&
                        (character[i].location.y == character[5].location.y)) {
                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            outfield(character[i], i, field);
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 'd') {
            if (character[5].location.x < 19 && field[character[5].location.x + 1][character[5].location.y] == ".") {
                field[character[5].location.x + 1][character[5].location.y] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.x += 1;
                std::cout << character[5].name << " moved down\n\n";
            }
            else if (field[character[5].location.x + 1][character[5].location.y] == "E") {
                //Идентификация ближайшего врага и атака на него
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x + 1) &&
                        (character[i].location.y == character[5].location.y)) {
                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            outfield(character[i], i, field);
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 's') {   //Сохранение игры в файл
            save_game(character);
            return 0;
        }
        else if (ch_dir == 'z') {   //Загрузка игры из файла
            load_game(character);
            
            //Инициализация игрового поля
            initializing_battlefield(field);

            //Инициализация локации персонажей    
            initialization_character_location(field, character); 
        }
        else
            std::cout << character[5].name << " skips a turn\n\n";      //Пропуск хода Игроком

        //Вывод игрового поля на экран
        show_battlefield(field);

        if (character[0].health >= 0 || character[1].health >= 0 || character[2].health >= 0 || character[3].health >= 0 ||
            character[4].health >= 0) {
            std::cout << "To continue the game, enter any simbol:  ";
            std::cin >> contin;
        }
        else
            break;
    }

    //Вывод результата
    if (character[5].health < 0)
        std::cout << "Enemy win!!!\n\n";
    else
        std::cout << "Player win!!!\n\n";
 }