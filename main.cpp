//
// Created by Hseyn Dinc on 13.12.24.
//
#include "calculator_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalculatorGUI w;
    w.show();
    return a.exec();
}