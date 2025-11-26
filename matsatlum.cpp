#include "matsatlum.h"
#include "ui_matsatlum.h"
#include "imagenes.h"

MatSatLum::MatSatLum(int numfoto, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MatSatLum)
    , nfoto(numfoto)
{
    ui->setupUi(this);
}

MatSatLum::~MatSatLum()
{
    delete ui;
}

void MatSatLum::on_horizontalSlider_valueChanged(int value)
{
    ver_matiz_sat_lum(nfoto, ui->dial->value(), value / 100.0, ui->horizontalSlider_2->value() / 100.0);
}


void MatSatLum::on_horizontalSlider_2_valueChanged(int value)
{
    ver_matiz_sat_lum(nfoto, ui->dial->value(), ui->horizontalSlider->value() / 100.0, value / 100.0);
}


void MatSatLum::on_dial_valueChanged(int value)
{
    ver_matiz_sat_lum(nfoto, value, ui->horizontalSlider->value() / 100.0, ui->horizontalSlider_2->value() / 100.0);
}


void MatSatLum::on_MatSatLum_accepted()
{
    ver_matiz_sat_lum(nfoto, ui->dial->value(), ui->horizontalSlider->value() / 100.0, ui->horizontalSlider_2->value() / 100.0, true);
}


void MatSatLum::on_MatSatLum_rejected()
{
    mostrar(nfoto);
}

