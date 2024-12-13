//
// Created by Hseyn Dinc on 13.12.24..
//

#pragma once
#include <QMainWindow>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class CalculatorGUI;
}
QT_END_NAMESPACE

class CalculatorGUI : public QMainWindow
{
    Q_OBJECT

public:
    CalculatorGUI(QWidget *parent = nullptr);
    ~CalculatorGUI();

private slots:
    void numberPressed();
    void operatorPressed();
    void equalPressed();
    void clearPressed();
    void scientificPressed();
    void memoryPressed();
    void decimalPressed();
    void toggleTheme();

private:
    Ui::CalculatorGUI *ui;
    QString currentNumber;
    double storedNumber;
    QString pendingOperator;
    QVector<QString> history;
    double memory;
    bool isDarkMode;

    void updateDisplay();
    void addToHistory(const QString &operation);
    void applyTheme();
    void setupThemeButton();
};