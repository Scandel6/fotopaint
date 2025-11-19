//---------------------------------------------------------------------------

#include "video.h"
#include <math.h>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES PRIVADAS               //////////////
///////////////////////////////////////////////////////////////////

static VideoCapture camara;
// Capturador de cámara usado actualmente

static Mat img_media;
// Imagen media acumulada de la cámara, con profundidad 32F

static int frames_img_media;
// Número de frames que se han acumulado en la media img_media

string FiltroVideo= "Todos los formatos (*.avi *.mpg *.wmv *.mov);;Archivos AVI (*.avi);;Archivos MPG (*.mpg *.mpeg);;Archivos WMV (*.wmv);;Archivos MOV (*.mov);;Otros (*.*)";

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE VIDEO       //////////////
///////////////////////////////////////////////////////////////////

void rotar_video (int nfoto, string nombre, int modo, int nframes, int codec, double fps)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada && !nombre.empty() && nframes>0);
    Mat rotada;
    rotar_angulo(foto[nfoto].img, rotada, 0, 1, modo);
    VideoWriter writer(nombre, codec, fps, rotada.size());
    if (writer.isOpened()) {
        for (int i= 0; i<=nframes; i++) {
            rotar_angulo(foto[nfoto].img, rotada, i*360.0/nframes, 1, modo);
            namedWindow("Imagen a vídeo - Rotación", WINDOW_NORMAL);
            imshow("Imagen a vídeo - Rotación", rotada);
            waitKey(1);
            writer << rotada;
        }
        writer.release();
        destroyWindow("Imagen a vídeo - Rotación");
    }
}

//---------------------------------------------------------------------------

int inic_camara (int numero)
{
    if (camara.isOpened())
        camara.release();
    camara.open(numero);
    if (camara.isOpened()) {
        Mat img;
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        camara >> img;
        resizeWindow("Imagen de cámara", img.size().width, img.size().height);
        imshow("Imagen de cámara", img);
        return int(1000.0/30);
    }
    else
        return 0;
}

//---------------------------------------------------------------------------

void fin_camara (void)
{
    if (camara.isOpened()) {
        camara.release();
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        destroyWindow("Imagen de cámara");
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    destroyWindow("Imagen media");
}

//---------------------------------------------------------------------------

void acumular_media (bool primera)
{
    assert(camara.isOpened());
    Mat frame;
    camara >> frame;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", frame);
    if (primera) {
        frames_img_media= 1;
        frame.convertTo(img_media, CV_32FC3);
    }
    else {
        frames_img_media++;
        Mat frame32F;
        frame.convertTo(frame32F, CV_32FC3);
        img_media= frame32F + img_media;
        img_media.convertTo(frame, CV_8UC3, 1.0/frames_img_media);
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    imshow("Imagen media", frame);
}

//---------------------------------------------------------------------------

void media_a_nueva (int nfoto)
{
    Mat res(img_media.size(), CV_8UC3);
    img_media.convertTo(res, CV_8UC3, 1.0/frames_img_media);
    crear_nueva (nfoto, res);
}

//---------------------------------------------------------------------------

void mostrar_camara (void)
{
    Mat img;
    camara >> img;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", img);
}

//---------------------------------------------------------------------------

void capturar_de_camara(int nres){
    // VideoCapture cap("C:\OpenCV\OpenCV4.10.0G\samples\bin\vtest.avi");
    VideoCapture cap(0);
    if(cap.isOpened()) {
        Mat frame;
        int tecla = -1;

        while(cap.read(frame) && (tecla = waitKey(1)) == -1){
            imshow("Pulse una tecla para capturar (Esc salir)", frame);
        }
        if (!frame.empty() && tecla != 27){
            crear_nueva(nres, frame);
        }
        destroyWindow("Pulse una tecla para capturar (Esc salir)");
    } else {
        cerr << "No se ha podido abrir la cámara";
    }
}

//---------------------------------------------------------------------------

void star_wars(int nfoto, string nombre, int nframes, double fps, int codec, vector<string> texto,
               Scalar color, double escala, bool guardar){
    VideoWriter wri;
    if (guardar) {
        wri.open(nombre, codec, fps, foto[nfoto].img.size());
        if (!wri.isOpened()){
            cerr << "No se ha podido abrir el vídeo Star Wars." << endl;
            return;
        }
    }

    int w_vent = 1000, h_vent = 1000;
    int w_img = foto[nfoto].img.cols, h_img = foto[nfoto].img.rows;

    Size s = getTextSize("Prueba", FONT_HERSHEY_DUPLEX, escala, escala * 1.5, NULL);

    int h_line = s.height;
    Size tamTexto(w_vent, 2 * h_vent + h_line * 1.5 * texto.size());

    Mat imTexto(tamTexto, CV_8UC3, CV_RGB(0,0,0));
    for(int i = 0; i < texto.size(); i++) {
        putText(imTexto, texto[i], Point(0, h_vent + 1.5 * h_line * (i + 1)), FONT_HERSHEY_DUPLEX, escala, color, escala * 1.5);
    }


    Point2f ptorig[4] = {
        Point2f(0, h_vent / 2),
        Point2f(w_vent, h_vent/2),
        Point2f(w_vent, h_vent /2 + h_vent),
        Point2f(0, h_vent / 2 + h_vent)
    };

    Point2f ptdest[4] = {
        Point2f(w_img * 0.45, 0),
        Point2f(w_img * 0.55, 0),
        Point2f(w_img * 1.1, h_img),
        Point2f(-0.1 * w_img, h_img)
    };

    for (int i = 0; i < nframes; i++){
        ptorig[0].y = double(imTexto.rows - h_vent) * i / nframes;
        ptorig[1].y = ptorig[0].y;
        ptorig[2].y = ptorig[0].y + h_vent;
        ptorig[3].y = ptorig[2].y;

        Mat M = getPerspectiveTransform(ptorig, ptdest);
        Mat imTextPers;
        warpPerspective(imTexto, imTextPers, M, foto[nfoto].img.size(), INTER_CUBIC);

        Mat frame = foto[nfoto].img.clone();
        imTextPers.copyTo(frame, imTextPers);

        if (guardar)
            wri.write(frame);

        imshow("Star Wars", frame);

        if (waitKey(1) != -1) return;
    }
}

//---------------------------------------------------------------------------
