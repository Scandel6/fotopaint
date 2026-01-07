#include "rotaryreescalar.h"
#include "ui_rotaryreescalar.h"
#include "imagenes.h"

int rr_nfoto;
int rr_angulo;
double rr_tam;

RotarYReescalar::RotarYReescalar(int numfoto, int numres, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RotarYReescalar)
    , nres(numres)
{
    ui->setupUi(this);
    rr_nfoto = numfoto;
    rr_angulo = 90;
    rr_tam = 1.0;

}

RotarYReescalar::~RotarYReescalar()
{
    delete ui;
}

void RotarYReescalar::on_horizontalSlider_valueChanged(int value)
{
    rr_angulo = value;
    ver_rotar_y_reescalar(rr_nfoto, nres, rr_angulo, rr_tam);
}


void RotarYReescalar::on_horizontalSlider_2_valueChanged(int value)
{
    rr_tam = value / 25.0;
    ver_rotar_y_reescalar(rr_nfoto, nres, rr_angulo, rr_tam);
}


void RotarYReescalar::on_RotarYReescalar_accepted()
{
    ver_rotar_y_reescalar(rr_nfoto, nres, rr_angulo, rr_tam, true);
    destroyWindow("Rotar/Reescalar");
}


void RotarYReescalar::on_RotarYReescalar_rejected()
{
    destroyWindow("Rotar/Reescalar");
}

