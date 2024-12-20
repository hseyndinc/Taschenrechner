//
// Created by Hseyn Dinc on 16.12.24..
//

#pragma once

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioSource>
#include <QRegularExpression>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QMetaObject>
#include <QObject>
#include <QWidget>
#include <QBuffer>
#include <QtMultimedia>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>

QT_BEGIN_NAMESPACE
namespace Ui {
    class CalculatorGUI;
}
QT_END_NAMESPACE

class CalculatorGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalculatorGUI(QWidget *parent = nullptr);
    ~CalculatorGUI();

    private slots:
        // Grundlegende Operationen
        void numberPressed();
    void operatorPressed();
    void equalPressed();
    void clearPressed();
    void decimalPressed();

    // Wissenschaftliche Funktionen
    void scientificPressed();

    // Memory Funktionen
    void memoryPressed();

    // Theme
    void toggleTheme();

    // Sprachsteuerung
    void startVoiceRecognition();
    void stopVoiceRecognition();
    void processCommand(const QString& command);

private:
    // UI Komponenten
    Ui::CalculatorGUI *ui;
    QPushButton* voiceButton;
    QSlider* themeSlider;

    // Rechner Status
    QString currentNumber;
    double storedNumber;
    QString pendingOperator;
    QVector<QString> history;
    double memory;
    bool isDarkMode;

    // Audio Components
    QAudioSource* audioSource;
    QByteArray audioData;
    bool isRecording;

    // Hilfsfunktionen
    void updateDisplay();
    void addToHistory(const QString &operation);
    void applyTheme();
    void setupThemeButton();
    void setupVoiceRecognition();
    void updateExpressionDisplay();
    QProcess* whisperProcess;
    QString modelPath;
    QString audioFilePath;
    QTemporaryFile* tempAudioFile;
};