//
// Created by Hseyn Dinc on 13.12.24.
//


#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>

class FuturisticCalculator
{
private:
    std::vector<std::string> history;
    double memory;

public:
    FuturisticCalculator() : memory(0)
    {
        displayWelcome();
    }

    void displayWelcome()
    {
        std::cout << "\n╔════════════════════════════════════╗\n";
        std::cout << "║     FUTURISTIC CALCULATOR v1.0     ║\n";
        std::cout << "╚════════════════════════════════════╝\n";
    }

    void displayMenu()
    {
        std::cout << "\n┌─────────── OPERATIONS ───────────┐\n";
        std::cout << "│ 1. Basic Operations (+-*/)\t│\n";
        std::cout << "│ 2. Scientific Functions\t\t│\n";
        std::cout << "│ 3. Memory Operations\t\t│\n";
        std::cout << "│ 4. View History\t\t│\n";
        std::cout << "│ 5. Exit\t\t\t│\n";
        std::cout << "└──────────────────────────────────┘\n";
    }

    void basicOperations()
    {
        double a, b;
        char op;

        std::cout << "Enter first number: ";
        std::cin >> a;
        std::cout << "Enter operator (+,-,*,/): ";
        std::cin >> op;
        std::cout << "Enter second number: ";
        std::cin >> b;

        double result;
        switch (op)
        {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b != 0)
                result = a / b;
            else
            {
                std::cout << "Error: Division by zero!\n";
                return;
            }
            break;
        default:
            std::cout << "Invalid operator!\n";
            return;
        }

        std::string operation = std::to_string(a) + " " + op + " " +
                                std::to_string(b) + " = " + std::to_string(result);
        history.push_back(operation);

        std::cout << "\n═══════ Result ═══════\n";
        std::cout << result << std::endl;
    }

    void scientificOperations()
    {
        std::cout << "\n┌────── Scientific Menu ──────┐\n";
        std::cout << "│ 1. Square Root\t\t   │\n";
        std::cout << "│ 2. Power\t\t   │\n";
        std::cout << "│ 3. Sine\t\t   │\n";
        std::cout << "│ 4. Cosine\t\t   │\n";
        std::cout << "│ 5. Logarithm\t\t   │\n";
        std::cout << "└──────────────────────────┘\n";

        int choice;
        double num, result;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Enter number: ";
            std::cin >> num;
            result = sqrt(num);
            history.push_back("sqrt(" + std::to_string(num) + ") = " + std::to_string(result));
            break;
        case 2:
            double power;
            std::cout << "Enter base: ";
            std::cin >> num;
            std::cout << "Enter power: ";
            std::cin >> power;
            result = pow(num, power);
            history.push_back(std::to_string(num) + "^" + std::to_string(power) + " = " + std::to_string(result));
            break;
            // Add more scientific operations as needed
        }

        std::cout << "\n═══════ Result ═══════\n";
        std::cout << result << std::endl;
    }

    void memoryOperations()
    {
        std::cout << "\n┌────── Memory Operations ──────┐\n";
        std::cout << "│ 1. Store to Memory\t     │\n";
        std::cout << "│ 2. Recall Memory\t     │\n";
        std::cout << "│ 3. Clear Memory\t     │\n";
        std::cout << "└────────────────────────────┘\n";

        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Enter number to store: ";
            std::cin >> memory;
            break;
        case 2:
            std::cout << "Memory value: " << memory << std::endl;
            break;
        case 3:
            memory = 0;
            std::cout << "Memory cleared!" << std::endl;
            break;
        }
    }

    void viewHistory()
    {
        std::cout << "\n═══════ Calculation History ═══════\n";
        for (const auto &calc : history)
        {
            std::cout << calc << std::endl;
        }
    }

    void run()
    {
        while (true)
        {
            displayMenu();
            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                basicOperations();
                break;
            case 2:
                scientificOperations();
                break;
            case 3:
                memoryOperations();
                break;
            case 4:
                viewHistory();
                break;
            case 5:
                std::cout << "Thank you for using Futuristic Calculator!\n";
                return;
            default:
                std::cout << "Invalid choice!\n";
            }
        }
    }
};

int main()
{
    FuturisticCalculator calc;
    calc.run();
    return 0;
}