#include "pincharestirar.h"
#include "ui_pincharestirar.h"
#include "imagenes.h"

int pe_nfoto;
double pe_radio;
double pe_grado;
int pe_cx, pe_cy;

void pe_callback(int event, int x, int y, int flags, void *){
    if (flags == EVENT_FLAG_LBUTTON){
        pe_cx = x;
        pe_cy = y;
        ver_pinchar_estirar(pe_nfoto, 0, pe_cx, pe_cy, pe_grado, pe_radio);
    }
}

PincharEstirar::PincharEstirar(int numfoto, int numres, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PincharEstirar)
    , nres(numres)
{
    ui->setupUi(this);
    pe_nfoto = numfoto;
    pe_radio = 200;
    pe_grado = 100 * pe_radio * pe_radio / 2000.0;

    // En el medio
    pe_cx = foto[pe_nfoto].img.cols / 2;
    pe_cy = foto[pe_nfoto].img.rows / 2;
    ver_pinchar_estirar(pe_nfoto, nres, pe_cx, pe_cy, pe_grado, pe_radio);
    setMouseCallback("Pinchar/estirar", pe_callback);
}

PincharEstirar::~PincharEstirar()
{
    delete ui;
}

void PincharEstirar::on_horizontalSlider_valueChanged(int value)
{
    pe_radio = value;
    pe_grado = ui->horizontalSlider_2->value() * pe_radio * pe_radio / 2000.0;
    ver_pinchar_estirar(pe_nfoto, nres, pe_cx, pe_cy, pe_grado, pe_radio);
}


void PincharEstirar::on_horizontalSlider_2_valueChanged(int value)
{
    pe_grado = value * pe_radio * pe_radio / 2000.0;
    ver_pinchar_estirar(pe_nfoto, nres, pe_cx, pe_cy, pe_grado, pe_radio);
}


void PincharEstirar::on_PincharEstirar_accepted()
{
    ver_pinchar_estirar(pe_nfoto, nres, pe_cx, pe_cy, pe_grado, pe_radio, true);
    destroyWindow("Pinchar/estirar");
}


void PincharEstirar::on_PincharEstirar_rejected()
{
    destroyWindow("Pinchar/estirar");
}

