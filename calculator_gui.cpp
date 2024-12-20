//
// Created by Hseyn Dinc on 16.12.24..
//

#include "calculator_gui.h"
#include "./ui_calculator_gui.h"
#include <QStyle>
#include <QPushButton>
#include <cmath>
#include <QRegularExpression>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QBuffer>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>

void CalculatorGUI::toggleTheme()
{
    isDarkMode = !isDarkMode;
    applyTheme();
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
CalculatorGUI::CalculatorGUI(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CalculatorGUI), storedNumber(0), memory(0),
    isDarkMode(false), isRecording(false), tempAudioFile(nullptr), audioSource(nullptr)
{
    ui->setupUi(this);

    // Display-Einstellungen
    ui->expressionDisplay->setPlaceholderText("Berechnung");
    ui->display->setPlaceholderText("0");

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
    setupVoiceRecognition();
    applyTheme();
}

CalculatorGUI::~CalculatorGUI()
{
    delete ui;
    delete audioSource;
    delete tempAudioFile;
    delete whisperProcess;
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
        updateExpressionDisplay();
    }
}

void CalculatorGUI::operatorPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        storedNumber = currentNumber.toDouble();
        pendingOperator = button->text();
        currentNumber = "";
        updateExpressionDisplay();
    }
}

void CalculatorGUI::equalPressed()
{
    double result = 0;
    double currentNum = currentNumber.toDouble();
    QString expression = QString::number(storedNumber) + " " +
                        pendingOperator + " " +
                        QString::number(currentNum);

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
            ui->display->setText("Fehler: Division durch 0");
            return;
        }
    }

    currentNumber = QString::number(result);
    ui->expressionDisplay->setText(expression + " = " + currentNumber);
    updateDisplay();
    addToHistory(expression + " = " + currentNumber);
}

void CalculatorGUI::updateExpressionDisplay()
{
    if (pendingOperator.isEmpty()) {
        ui->expressionDisplay->setText(currentNumber);
    } else {
        ui->expressionDisplay->setText(QString::number(storedNumber) + " " +
                                     pendingOperator + " " +
                                     currentNumber);
    }
}

void CalculatorGUI::clearPressed()
{
    currentNumber = "0";
    storedNumber = 0;
    pendingOperator = "";
    ui->expressionDisplay->clear();
    updateDisplay();
}

void CalculatorGUI::scientificPressed()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    double num = currentNumber.toDouble();
    QString operation = button->text() + "(" + QString::number(num) + ")";

    if (button->text() == "√") {
        if (num >= 0) {
            currentNumber = QString::number(std::sqrt(num));
        } else {
            ui->display->setText("Fehler: Negative Wurzel");
            return;
        }
    } else if (button->text() == "sin") {
        currentNumber = QString::number(std::sin(num));
    } else if (button->text() == "^") {
        storedNumber = num;
        pendingOperator = "^";
        currentNumber = "";
        operation = QString::number(num) + "^";
    }

    ui->expressionDisplay->setText(operation);
    updateDisplay();
}

void CalculatorGUI::decimalPressed()
{
    if (!currentNumber.contains('.')) {
        currentNumber += '.';
        updateDisplay();
        updateExpressionDisplay();
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

void CalculatorGUI::setupThemeButton()
{
    themeSlider = new QSlider(Qt::Horizontal, this);
    themeSlider->setFixedWidth(50);
    themeSlider->setRange(0, 1);
    themeSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: 1px solid #999999;
            height: 20px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #f0f0f0, stop:1 #333333);
            border-radius: 10px;
        }
        QSlider::handle:horizontal {
            background: white;
            border: 1px solid #999999;
            width: 24px;
            height: 24px;
            border-radius: 12px;
            margin: -2px 0;
        }
    )");

    connect(themeSlider, &QSlider::valueChanged, [this](int value) {
        isDarkMode = (value == 1);
        applyTheme();
    });

    ui->buttonLayout->addWidget(themeSlider);
    ui->buttonLayout->setAlignment(themeSlider, Qt::AlignRight);
}

void CalculatorGUI::setupVoiceRecognition()
{
    voiceButton = new QPushButton("🎤", this);
    voiceButton->setFixedSize(50, 50);
    voiceButton->setStyleSheet("border-radius: 25px; background-color: #4CAF50; color: white;");
    ui->buttonLayout->addWidget(voiceButton);

    // Audio Setup
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device = QMediaDevices::defaultAudioInput();
    if (device.isNull()) {
        ui->display->setText("Kein Mikrofon gefunden!");
        return;
    }

    audioSource = new QAudioSource(device, format, this);
    whisperProcess = new QProcess(this);

    // Modell-Pfad setzen
    modelPath = QDir::homePath() + "/whisper-models/ggml-base.bin";

    // Temporäre Datei für Audio
    tempAudioFile = new QTemporaryFile(this);

    connect(voiceButton, &QPushButton::pressed, this, &CalculatorGUI::startVoiceRecognition);
    connect(voiceButton, &QPushButton::released, this, &CalculatorGUI::stopVoiceRecognition);

    // Whisper Prozess-Handler
    connect(whisperProcess, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitCode == 0) {
            QString result = QString::fromUtf8(whisperProcess->readAllStandardOutput()).trimmed();
            ui->display->setText("Erkannt: " + result);
            processCommand(result);
        } else {
            ui->display->setText("Fehler bei der Spracherkennung");
        }
    });
}

void CalculatorGUI::startVoiceRecognition()
{
    if (!isRecording) {
        if (!tempAudioFile->open()) {
            ui->display->setText("Fehler beim Erstellen der Audio-Datei");
            return;
        }
        audioFilePath = tempAudioFile->fileName();
        audioSource->start(tempAudioFile);
        isRecording = true;
        voiceButton->setStyleSheet("border-radius: 25px; background-color: #FF5252; color: white;");
        ui->display->setText("Aufnahme läuft...");
    }
}

void CalculatorGUI::stopVoiceRecognition()
{
    if (isRecording) {
        audioSource->stop();
        isRecording = false;
        voiceButton->setStyleSheet("border-radius: 25px; background-color: #4CAF50; color: white;");
        ui->display->setText("Verarbeite Audio...");

        // Debug: Zeige Dateigröße
        QFile file(audioFilePath);
        ui->expressionDisplay->setText("Audio Größe: " + QString::number(file.size()) + " bytes");

        // Starte Whisper-Verarbeitung
        QStringList arguments;
        arguments << "-m" << "/Users/hseyndinc/whisper-models/ggml-base.bin"
                 << "-f" << audioFilePath
                 << "-l" << "de";

        whisperProcess->setProgram("/usr/local/bin/whisper-cpp");
        whisperProcess->setArguments(arguments);

        // Debug-Ausgaben
        connect(whisperProcess, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
            QString errorStr;
            switch (error) {
                case QProcess::FailedToStart:
                    errorStr = "Failed to start"; break;
                case QProcess::Crashed:
                    errorStr = "Process crashed"; break;
                case QProcess::Timedout:
                    errorStr = "Timeout"; break;
                case QProcess::WriteError:
                    errorStr = "Write error"; break;
                case QProcess::ReadError:
                    errorStr = "Read error"; break;
                default:
                    errorStr = "Unknown error"; break;
            }
            ui->display->setText("Fehler: " + errorStr);
        });

        connect(whisperProcess, &QProcess::readyReadStandardError, [this]() {
            QString error = whisperProcess->readAllStandardError();
            ui->expressionDisplay->setText("Error: " + error);
        });

        whisperProcess->start();

        // Debug: Zeige Kommando
        ui->display->setText("Befehl: " + whisperProcess->program() + " " +
                           whisperProcess->arguments().join(" "));
    }
}

void CalculatorGUI::processCommand(const QString& command)
{
    QRegularExpression numberRegex("\\d+");
    QRegularExpressionMatchIterator i = numberRegex.globalMatch(command);
    QVector<double> numbers;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        numbers.append(match.captured().toDouble());
    }

    if (numbers.size() < 2) {
        ui->display->setText("Fehler: Zwei Zahlen erforderlich");
        return;
    }

    double result = 0;
    if (command.contains("plus") || command.contains("addiere")) {
        result = numbers[0] + numbers[1];
        currentNumber = QString::number(result);
    }
    else if (command.contains("minus") || command.contains("subtrahiere")) {
        result = numbers[0] - numbers[1];
        currentNumber = QString::number(result);
    }
    else if (command.contains("mal") || command.contains("multipliziere")) {
        result = numbers[0] * numbers[1];
        currentNumber = QString::number(result);
    }
    else if (command.contains("geteilt") || command.contains("dividiere")) {
        if (numbers[1] != 0) {
            result = numbers[0] / numbers[1];
            currentNumber = QString::number(result);
        } else {
            ui->display->setText("Fehler: Division durch Null");
            return;
        }
    }
    else if (command.contains("lösche") || command.contains("zurücksetzen")) {
        clearPressed();
        return;
    }

    updateDisplay();
    addToHistory(QString("%1 = %2").arg(command).arg(result));
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
            font-size: 18px;
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