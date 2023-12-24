#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->rollButton, SIGNAL(clicked()), this, SLOT(Roll()));

    QRegularExpression reg(R"((([1-9]\d*d[1-9]\d*(\+[1-9]\d*)?) *,? *)*)");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(reg);
    ui->inputLine->setValidator(validator);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::Roll() {
    if (ui->inputLine->hasAcceptableInput()) {
        ui->chartView->load(ui->inputLine->text().toStdString(), ui->rollsCountBox->value());
    }
}

