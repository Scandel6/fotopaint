#include "bajorrelieve.h"
#include "ui_bajorrelieve.h"
#include "imagenes.h"

Bajorrelieve::Bajorrelieve(int numfoto, int numres, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Bajorrelieve)
    , nfoto(numfoto)
    , nres(numres)
    , ntextura(3)
{
    ui->setupUi(this);
    actualizar();
    activar_callback_fotos(false);
    if(parent)
        move(parent->x() + DESP_X_HIJO, parent->y() + DESP_Y_HIJO);
}

Bajorrelieve::~Bajorrelieve()
{
    delete ui;
}


void Bajorrelieve::actualizar(){
    ver_bajorrelieve(nfoto, nres, ui->horizontalSlider->value() / 100.0, ui->dial->value(), ui->spinBox_3->value(), ntextura);
}

void Bajorrelieve::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
    actualizar();
}


void Bajorrelieve::on_dial_valueChanged(int value)
{
    actualizar();
}


void Bajorrelieve::on_spinBox_valueChanged(int arg1)
{
    actualizar();
}


void Bajorrelieve::on_radioButton_clicked()
{
    ntextura = 0;
    actualizar();
}


void Bajorrelieve::on_radioButton_2_clicked()
{
    ntextura = 1;
    actualizar();
}


void Bajorrelieve::on_radioButton_3_clicked()
{
    ntextura = 2;
    actualizar();
}


void Bajorrelieve::on_radioButton_4_clicked()
{
    ntextura = 3;
    actualizar();
}


void Bajorrelieve::on_Bajorrelieve_accepted()
{
    ver_bajorrelieve(nfoto, nres, ui->horizontalSlider->value() / 100.0, ui->dial->value(), ui->spinBox_3->value(), ntextura, true);

}


void Bajorrelieve::on_Bajorrelieve_finished(int result)
{
    activar_callback_fotos(true);
    destroyWindow("Bajorrelieve");
}

