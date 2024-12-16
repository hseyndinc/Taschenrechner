//
// Created by Hseyn Dinc on 16.12.24..
//

#pragma once
#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QSpeechRecognizer>
#include <QVoice>

QT_BEGIN_NAMESPACE
namespace Ui { class CalculatorGUI; }
QT_END_NAMESPACE

class CalculatorGUI : public QMainWindow {
    Q_OBJECT

public:
    CalculatorGUI(QWidget *parent = nullptr);
    ~CalculatorGUI();

    private slots:
        // Bestehende Slots...
        void startVoiceRecognition();
    void handleSpeechResult(const QString& text);

private:
    // Bestehende Member...
    QSpeechRecognizer* speechRecognizer;
    QPushButton* voiceButton;
    void setupVoiceRecognition();
    void processVoiceCommand(const QString& command);
};