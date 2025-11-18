#include "capturardevideo.h"
#include "ui_capturardevideo.h"

CapturarDeVideo::CapturarDeVideo(string nombre, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CapturarDeVideo)
{
    ui->setupUi(this);
    if (cap.open(nombre)){
        int nframes = cap.get(CAP_PROP_FRAME_COUNT);
        ui->horizontalSlider->setMaximum(nframes - 1);
        ui->spinBox->setMaximum(nframes - 1);
        Mat frame;
        if (cap.read(frame))
            imshow("Frame del vídeo", frame);
    }
}

CapturarDeVideo::~CapturarDeVideo()
{
    delete ui;
}

bool CapturarDeVideo::isOpened(){
    return cap.isOpened();
}

void CapturarDeVideo::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
    cap.set(CAP_PROP_POS_FRAMES, value);
    Mat frame;
    if(cap.read(frame))
        imshow("Frame del vídeo", frame);
}


void CapturarDeVideo::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}


void CapturarDeVideo::on_CapturarDeVideo_accepted()
{
    cap.set(CAP_PROP_POS_FRAMES, ui->spinBox->value());
    Mat frame;
    if(cap.read(frame))
        crear_nueva(primera_libre(), frame);
    destroyWindow("Frame del vídeo");
}


void CapturarDeVideo::on_CapturarDeVideo_rejected()
{
    destroyWindow("Frame del vídeo");
}

