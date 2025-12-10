#include "pincharestirar.h"
#include "ui_pincharestirar.h"

PincharEstirar::PincharEstirar(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PincharEstirar)
{
    ui->setupUi(this);
}

PincharEstirar::~PincharEstirar()
{
    delete ui;
}
