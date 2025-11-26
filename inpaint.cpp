#include "inpaint.h"
#include "ui_inpaint.h"
#include "imagenes.h"

void callback (int event, int x, int y, int flags, void *_nfoto);
int in_nfoto;
int in_radio_pincel;
Mat in_mascara;
Mat in_imagen;
bool in_previs;

void in_callback(int event, int x, int y, int flags, void *pt);

Inpaint::Inpaint(int numFoto, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Inpaint)
{
    ui->setupUi(this);
    in_nfoto = numFoto;
    in_radio_pincel = 20;
    in_imagen = foto[in_nfoto].img.clone();
    in_mascara = Mat(in_imagen.rows, in_imagen.cols, CV_8UC1, Scalar(0));
    setMouseCallback(foto[in_nfoto].nombre, in_callback);
    in_previs = true;
}

Inpaint::~Inpaint()
{
    delete ui;
    setMouseCallback(foto[in_nfoto].nombre, callback, (void *) in_nfoto);
}

void in_callback(int event, int x, int y, int flags, void *pt){
    if (flags == EVENT_FLAG_LBUTTON){
        circle(in_imagen, Point(x,y), in_radio_pincel, CV_RGB(255, 255, 255), -1);
        circle(in_mascara, Point(x,y), in_radio_pincel, CV_RGB(255, 255, 255), -1);
        imshow(foto[in_nfoto].nombre, in_imagen);
        if(in_previs){
            Mat res;
            inpaint(in_imagen, in_mascara, res, 3, INPAINT_TELEA);
            imshow("Inpaint", res);
        }
    } else {
        Mat clone = in_imagen.clone();
        circle(clone, Point(x, y), in_radio_pincel, CV_RGB(255, 255, 255), 1);
        imshow(foto[in_nfoto].nombre, clone);
    }
}

void Inpaint::on_horizontalSlider_valueChanged(int value)
{
    in_radio_pincel = value;
}


void Inpaint::on_pushButton_clicked()
{
    in_imagen = foto[in_nfoto].img.clone();
    in_mascara = Mat(in_imagen.rows, in_imagen.cols, CV_8UC1, Scalar(0));
    mostrar(in_nfoto);
}


void Inpaint::on_pushButton_2_clicked()
{
    // TODO: Cambiar 3 por el radio
    // TODO: Desplegable para elegir INPAINT_TELEA, INPAINT_NS
    Mat res;
    inpaint(in_imagen, in_mascara, res, 3, INPAINT_TELEA);
    imshow("Inpaint", res);
}


void Inpaint::on_Inpaint_accepted()
{
    inpaint(in_imagen, in_mascara, foto[in_nfoto].img, 3, INPAINT_TELEA);
    mostrar(in_nfoto);
    destroyWindow("Inpaint");
}


void Inpaint::on_Inpaint_rejected()
{
    mostrar(in_nfoto);
    destroyWindow("Inpaint");
}


void Inpaint::on_checkBox_stateChanged(int arg1)
{
    in_previs = arg1;
}

