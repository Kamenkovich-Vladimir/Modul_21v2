#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct entry {
    std::string name = "Unknown";
    std::string surname = "Unknown";
    std::string date = "Unknown";
    double payment = 0.00;
};

int main()
{
    std::cout << "************** Filling out the statement *************\n\n";

    std::string command, date; 

    while (true)
    {
        std::cout << "Enter the command ('list' - read the file, 'add' - add an entry, another command - exiting the program)\n";
        std::cin >> command;
        if (command == "list") {
            std::vector<entry> vec_entries;

            //Проверка наличия файла statement.txt в папке проекта
            if (std::ifstream("statement.txt")) {
                
                std::ifstream statement_strim;
                statement_strim.open("statement.txt");
                if (!statement_strim.is_open()) {
                    std::cerr << "The file in the specified path is missing or cannot be opened\n";
                    return 0;
                }
                std::string curr_line;
                //Читаем файл
                while (true) {
                    entry curr_entry;
                    
                    statement_strim >> curr_entry.name >> curr_entry.surname >> curr_entry.date >> curr_entry.payment;
                    if (!statement_strim.eof())
                        vec_entries.push_back(curr_entry);
                    else
                        break;
                }
                //Выводим на экран
                for (int i = 0; i < vec_entries.size(); i++) {
                    std::cout.setf(std::ios::fixed);
                    std::cout.precision(2);
                    std::cout << vec_entries[i].name << "\t\t" << vec_entries[i].surname << "\t\t" << vec_entries[i].date
                        << "\t" << vec_entries[i].payment << std::endl;
                }
                std::cout << std::endl;
                statement_strim.close();
            }
            else {
                //Файл отсутствует
                std::cout << "The file with the statement is missing.\n";
                std::cout << "To create a file and add entries to the statement, use the command 'add'\n" << std::endl;
            }
        }
        else if (command == "add") {
            std::ofstream statement_strim;
            statement_strim.open("statement.txt", std::ios::app);

            if (!statement_strim.is_open()) {
                std::cerr << "The file in the specified path is missing or cannot be opened\n";
                return 0;
            }
            entry curr_entry;

            std::cout << "Input name: ";
            std::cin >> curr_entry.name;

            std::cout << "Input surname: ";
            std::cin >> curr_entry.surname;

            //Ввод и валидация формата даты 'dd.mm.yyyy'
            bool valid_date = false;
            while (!valid_date) {

                std::cout << "Input date of payment: ";
                std::cin >> date;

                if (date.length() != 10)
                    std::cout << "Invalid format of date 'dd.mm.yyyy'\n";
                else if (date[2] != '.' || date[5] != '.')
                    std::cout << "The day, month and year should be separated by dots\n";
                else if (std::stoi(date.substr(0, 2)) <= 0 || std::stoi(date.substr(0, 2)) > 31)
                    std::cout << "The wrong value of the day\n";
                //Проверку количества дней в месяце делать поленился, но это механика
                else if (std::stoi(date.substr(3, 2)) <= 0 || std::stoi(date.substr(3, 2)) > 12)
                    std::cout << "The wrong value of the month\n";
                //Должна быть, конечно, проверка на превышение текущей даты
                else if ((std::stoi(date.substr(6)) <= 2000) || (std::stoi(date.substr(6)) > 2024))
                    std::cout << "The wrong value of the year\n";
                else {
                    valid_date = true;
                    curr_entry.date = date;
                }
            }
            double payment = 0.00;
            while (payment <= 0) {
                std::cout << "Input payment: ";
                std::cin >> payment;

                if (payment <= 0)
                    std::cout << "The payout should be higher\n";
                else
                    curr_entry.payment = payment;

            }
            //Записываем в файл "валидированные" данные
            statement_strim.setf(std::ios::fixed);
            statement_strim.precision(2);
            statement_strim << curr_entry.name << " " << curr_entry.surname << " " << curr_entry.date << " " << curr_entry.payment << std::endl;

            std::cout << std::endl;
            statement_strim.close();
        }
        else
            return 0;
    }
}