//
// Created by Hseyn Dinc on 16.12.24..
//
#include "calculator_gui.h"
#include "./ui_calculator_gui.h"
#include <QStyle>
#include <QPushButton>
#include <cmath>

CalculatorGUI::CalculatorGUI(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CalculatorGUI), storedNumber(0), memory(0), isDarkMode(false)
{
    ui->setupUi(this);

    // Button-Typen setzen
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "button" + QString::number(i);
        if (QPushButton* button = findChild<QPushButton*>(buttonName)) {
            button->setProperty("type", "number");
        }
    }

    QVector<QString> operators = {"Add", "Subtract", "Multiply", "Divide"};
    for (const QString& op : operators) {
        QString buttonName = "button" + op;
        if (QPushButton* button = findChild<QPushButton*>(buttonName)) {
            button->setProperty("type", "operator");
        }
    }

    QVector<QString> functions = {"Sin", "Sqrt", "Power"};
    for (const QString& func : functions) {
        QString buttonName = "button" + func;
        if (QPushButton* button = findChild<QPushButton*>(buttonName)) {
            button->setProperty("type", "function");
        }
    }

    // Verbinde numerische Buttons
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "button" + QString::number(i);
        QPushButton *button = findChild<QPushButton *>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &CalculatorGUI::numberPressed);
        }
    }

    // Verbinde Operatorbuttons
    QVector<QString> operatorButtons = {"Add", "Subtract", "Multiply", "Divide"};
    for (const QString &op : operatorButtons) {
        QString buttonName = "button" + op;
        QPushButton *button = findChild<QPushButton *>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &CalculatorGUI::operatorPressed);
        }
    }

    // Weitere Verbindungen
    connect(ui->buttonEqual, &QPushButton::clicked, this, &CalculatorGUI::equalPressed);
    connect(ui->buttonClear, &QPushButton::clicked, this, &CalculatorGUI::clearPressed);
    connect(ui->buttonDecimal, &QPushButton::clicked, this, &CalculatorGUI::decimalPressed);

    // Wissenschaftliche Funktionen
    connect(ui->buttonSqrt, &QPushButton::clicked, this, &CalculatorGUI::scientificPressed);
    connect(ui->buttonPower, &QPushButton::clicked, this, &CalculatorGUI::scientificPressed);
    connect(ui->buttonSin, &QPushButton::clicked, this, &CalculatorGUI::scientificPressed);

    setupThemeButton();
    applyTheme();
}

CalculatorGUI::~CalculatorGUI()
{
    delete ui;
}

void CalculatorGUI::numberPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        if (currentNumber == "0") {
            currentNumber = button->text();
        } else {
            currentNumber += button->text();
        }
        updateDisplay();
    }
}

void CalculatorGUI::operatorPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        storedNumber = currentNumber.toDouble();
        pendingOperator = button->text();
        currentNumber = "";
        addToHistory(QString::number(storedNumber) + " " + pendingOperator);
    }
}

void CalculatorGUI::equalPressed()
{
    double result = 0;
    double currentNum = currentNumber.toDouble();

    if (pendingOperator == "+") {
        result = storedNumber + currentNum;
    } else if (pendingOperator == "-") {
        result = storedNumber - currentNum;
    } else if (pendingOperator == "*") {
        result = storedNumber * currentNum;
    } else if (pendingOperator == "/") {
        if (currentNum != 0) {
            result = storedNumber / currentNum;
        } else {
            ui->display->setText("Error");
            return;
        }
    }

    currentNumber = QString::number(result);
    updateDisplay();
    addToHistory(QString::number(currentNum) + " = " + currentNumber);
}

void CalculatorGUI::clearPressed()
{
    currentNumber = "0";
    storedNumber = 0;
    pendingOperator = "";
    updateDisplay();
}

void CalculatorGUI::scientificPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    double num = currentNumber.toDouble();
    if (button->text() == "√") {
        if (num >= 0) {
            currentNumber = QString::number(std::sqrt(num));
        } else {
            ui->display->setText("Error");
            return;
        }
    } else if (button->text() == "sin") {
        currentNumber = QString::number(std::sin(num));
    } else if (button->text() == "^") {
        storedNumber = num;
        pendingOperator = "^";
        currentNumber = "";
        return;
    }

    updateDisplay();
}

void CalculatorGUI::decimalPressed()
{
    if (!currentNumber.contains('.')) {
        currentNumber += '.';
        updateDisplay();
    }
}

void CalculatorGUI::memoryPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    if (button->text() == "M+") {
        memory += currentNumber.toDouble();
    } else if (button->text() == "M-") {
        memory -= currentNumber.toDouble();
    } else if (button->text() == "MR") {
        currentNumber = QString::number(memory);
        updateDisplay();
    } else if (button->text() == "MC") {
        memory = 0;
    }
}

void CalculatorGUI::updateDisplay()
{
    ui->display->setText(currentNumber);
}

void CalculatorGUI::addToHistory(const QString &operation)
{
    history.append(operation);
}

void CalculatorGUI::toggleTheme()
{
    isDarkMode = !isDarkMode;
    applyTheme();
}

void CalculatorGUI::setupThemeButton()
{
    QPushButton *themeButton = new QPushButton("Toggle Theme", this);
    connect(themeButton, &QPushButton::clicked, this, &CalculatorGUI::toggleTheme);
    ui->buttonLayout->addWidget(themeButton);
}

void CalculatorGUI::applyTheme()
{
    QString baseStyle = R"(
        QPushButton {
            border-radius: 30px;
            font-size: 18px;
            font-weight: bold;
            min-width: 80px;
            min-height: 60px;
        }
        QLineEdit {
            border-radius: 10px;
            padding: 10px;
            margin: 5px;
        }
    )";

    QString darkStyle = R"(
        QMainWindow {
            background-color: #1a1a1a;
        }
        QLineEdit {
            background-color: #333333;
            color: #ffffff;
            border: 2px solid #444444;
        }
        QPushButton {
            background-color: #333333;
            color: #ffffff;
            border: 2px solid #444444;
        }
        QPushButton:hover {
            background-color: #444444;
        }
        QPushButton[type="operator"] {
            background-color: #FF9800;
            color: white;
        }
        QPushButton[type="number"] {
            background-color: #424242;
        }
        QPushButton[type="function"] {
            background-color: #2196F3;
            color: white;
        }
    )";

    QString lightStyle = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QLineEdit {
            background-color: white;
            color: #333333;
            border: 2px solid #e0e0e0;
        }
        QPushButton {
            background-color: white;
            color: #333333;
            border: 2px solid #e0e0e0;
        }
        QPushButton:hover {
            background-color: #f0f0f0;
        }
        QPushButton[type="operator"] {
            background-color: #2196F3;
            color: white;
        }
        QPushButton[type="number"] {
            background-color: white;
        }
        QPushButton[type="function"] {
            background-color: #4CAF50;
            color: white;
        }
    )";

    setStyleSheet(baseStyle + (isDarkMode ? darkStyle : lightStyle));
}