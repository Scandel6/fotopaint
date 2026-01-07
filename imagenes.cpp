//---------------------------------------------------------------------------

#include "imagenes.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <assert.h>
#include <vector>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES                        //////////////
///////////////////////////////////////////////////////////////////

ventana foto[MAX_VENTANAS];

tipo_herramienta herr_actual= HER_PUNTO;

int radio_pincel= 10;

Scalar color_pincel= CV_RGB(255, 255, 255);

int difum_pincel= 10;

bool preguntar_guardar= false;

bool callback_inactivo= false;

static int numpos= 0; // Número actual en el orden de posición de las ventanas

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void callback (int event, int x, int y, int flags, void *_nfoto);
Scalar color_arcoiris();

//---------------------------------------------------------------------------

void inic_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        foto[i].usada= false;
}

//---------------------------------------------------------------------------

void fin_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            destroyWindow(foto[i].nombre);
            foto[i].usada= false;
        }
    }
}

//---------------------------------------------------------------------------

int primera_libre (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (!foto[i].usada)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

void escribir_barra_estado (void)
{
    int usadas, modificadas;
    num_fotos(usadas, modificadas);
    w->setStatusBarText(QString::number(usadas)+" fotos abiertas, "+
                        QString::number(modificadas)+" modificadas.");
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, int ancho, int alto, Scalar color)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img.create(alto, ancho, CV_8UC3);
    foto[nfoto].img=  color;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, ancho, alto);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, Mat img)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada && !img.empty());
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img= img;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, img.cols, img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= nombre;
    foto[nfoto].nombref= Lt1(nombre);
    foto[nfoto].img= imread(foto[nfoto].nombref);
    if (foto[nfoto].img.empty())
        return;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= false;
    foto[nfoto].roi= Rect(0, 0, foto[nfoto].img.cols, foto[nfoto].img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void guardar_foto (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    foto[nfoto].modificada= false;
    // 1. Guardar la imagen con el mismo nombre que tiene
    if (nombre=="")
        imwrite(foto[nfoto].nombref, foto[nfoto].img);
    // 2. Guardar la imagen con un nombre distinto al actual
    else {
        imwrite(Lt1(nombre), foto[nfoto].img);
        setWindowTitle(foto[nfoto].nombre, nombre);
    }
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void cerrar_foto (int nfoto, bool destruir_ventana)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (destruir_ventana)
        destroyWindow(foto[nfoto].nombre);
    foto[nfoto].usada= false;
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void mostrar (int nfoto)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

int nombre_a_numero (string nombre)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (foto[i].usada && foto[i].nombre==nombre)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

int foto_activa (void)
{
    int maxorden= -1, maxpos= -1;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada && foto[i].orden>maxorden) {
            maxorden= foto[i].orden;
            maxpos= i;
        }
    }
    return maxpos;
}

//---------------------------------------------------------------------------

int num_fotos (int &usadas, int &modificadas)
{
    usadas= 0;
    modificadas= 0;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            usadas++;
            if (foto[i].modificada)
                modificadas++;
        }
    }
    return usadas;
}

//---------------------------------------------------------------------------

void activar_callback_fotos (bool activo)
{
    callback_inactivo= !activo;
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES AUXILIARES                      //////////////
///////////////////////////////////////////////////////////////////

void aux_pintar_segmento(Mat& im, Point p1, Point p2, Scalar color)
{
    if (difum_pincel == 0) {
        line(im, p1, p2, color, radio_pincel * 2 + 1, LINE_AA);
        return;
    }

    // Dar un poco de margen
    int margen = radio_pincel + difum_pincel + 2;

    int x_min = min(p1.x, p2.x) - margen;
    int y_min = min(p1.y, p2.y) - margen;
    int x_max = max(p1.x, p2.x) + margen;
    int y_max = max(p1.y, p2.y) + margen;
    Rect roi(x_min, y_min, x_max - x_min, y_max - y_min);

    roi = roi & Rect(0, 0, im.cols, im.rows); // https://docs.opencv.org/4.10.0/d2/d44/classcv_1_1Rect__.html intersección

    if (roi.empty()) return;

    Mat trozo = im(roi);

    Point p1_local = p1 - roi.tl();
    Point p2_local = p2 - roi.tl();

    // Lógica original
    Mat res(trozo.size(), im.type(), color);
    Mat cop(trozo.size(), im.type(), CV_RGB(0,0,0)); // Fondo negro

    line(cop, p1_local, p2_local, color, radio_pincel * 2 + 1, LINE_AA);

    blur(cop, cop, Size(difum_pincel * 2 + 1, difum_pincel * 2 + 1));

    multiply(res, cop, res, 1.0/255.0);
    bitwise_not(cop, cop);
    multiply(trozo, cop, trozo, 1.0/255.0);

    trozo = res + trozo;
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DEL CALLBACK PRINCIPAL          //////////////
///////////////////////////////////////////////////////////////////

static int downx, downy;
// Posición inicial del ratón al pinchar sobre la imagen actual

//---------------------------------------------------------------------------

void ninguna_accion (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    circle(res, Point(x, y), radio_pincel, CV_RGB(255,255,255), 2, LINE_AA);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_close (int factual)
{
    if (foto[factual].usada && !foto[factual].img.empty()) {
        if (foto[factual].modificada && preguntar_guardar) {
            QString cadena= "El archivo " + QString::fromStdString(foto[factual].nombre) +
                    " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(w, "Archivo modificado", cadena,
                                            QMessageBox::Yes | QMessageBox::No );
            if (resp==QMessageBox::Yes)
                guardar_foto(factual);
        }
        cerrar_foto(factual, false);
    }
}

//---------------------------------------------------------------------------

void cb_punto (int factual, int x, int y, Scalar color)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color, -1, LINE_AA);
    else {
        int tam = radio_pincel + difum_pincel;
        Rect roi(x-tam, y-tam, 2 * tam + 1, 2 * tam + 1);

        roi = roi & Rect(0, 0, im.cols, im.rows);

        if (roi.empty()) return;

        int posx = x - roi.x, posy = y - roi.y;

        Mat trozo = im(roi);

        Mat res(trozo.size(), im.type(), color);
        Mat cop(roi.size(), im.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, color, -1,  radio_pincel*2+1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(trozo, cop, trozo, 1.0/255.0);
        trozo= res + trozo;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_linea (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen

    aux_pintar_segmento(im, Point(downx, downy), Point(x, y), color_pincel);

    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_rectangulo (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        rectangle(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        rectangle(cop, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_elipse (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        ellipse(im, Point(downx, downy), Size(abs(x-downx), abs(y-downy)),0 , 0, 360, color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        ellipse(cop, Point(downx, downy), Size(abs(x-downx), abs(y-downy)), 0, 0, 360, color_pincel, radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_suavizado(int factual, int x, int y)
{
    Mat im = foto[factual].img;

    int tam_matriz = difum_pincel + 1; // Evitar 0
    if (tam_matriz % 2 == 0)
        tam_matriz += 1; // Siempre tiene que ser impar

    Rect roi(x - radio_pincel, y - radio_pincel, 2 * radio_pincel, 2 * radio_pincel);

    roi = roi & Rect(0, 0, im.cols, im.rows);

    if(roi.empty())
        return;

    Mat trozo = im(roi);
    Mat borrosa;

    GaussianBlur(trozo, borrosa, Size(tam_matriz, tam_matriz), 0);

    Mat cir(roi.size(), CV_8UC1, Scalar(0));
    circle(cir, Point(x - roi.x, y - roi.y), radio_pincel, Scalar(255), -1, LINE_AA); // -1 rellenar

    // Devolver cambios a original
    borrosa.copyTo(trozo, cir);

    imshow(foto[factual].nombre, im);
    foto[factual].modificada = true;
}

//---------------------------------------------------------------------------

void cb_ver_linea (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    line(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_rectangulo (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    rectangle(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_elipse (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    ellipse(res, Point(downx, downy), Size(abs(x-downx), abs(y-downy)), 0, 0, 360, color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}


//---------------------------------------------------------------------------

void cb_seleccionar (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    Rect nuevo= Rect(min(downx, x), min(downy, y),
                     max(downx, x)-min(downx, x)+1, max(downy, y)-min(downy, y)+1);
    if (nuevo.x<0)
        nuevo.x= 0;
    if (nuevo.y<0)
        nuevo.y= 0;
    if (nuevo.x+nuevo.width>im.size().width)
        nuevo.width= im.size().width-nuevo.x;
    if (nuevo.y+nuevo.height>im.size().height)
        nuevo.height= im.size().height-nuevo.y;
    foto[factual].roi= nuevo;
}

//---------------------------------------------------------------------------

void cb_ver_seleccion (int factual, int x, int y, bool foto_roi)
{
    Mat res= foto[factual].img.clone();
    Point p1, p2;
    if (foto_roi) {
        p1.x= foto[factual].roi.x;
        p1.y= foto[factual].roi.y;
        p2.x= foto[factual].roi.x+foto[factual].roi.width-1;
        p2.y= foto[factual].roi.y+foto[factual].roi.height-1;
    }
    else {
        p1= Point(downx, downy);
        p2= Point(x, y);
    }
    rectangle(res, p1, p2, CV_RGB(255,foto_roi?0:255,0),2);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_trazo(int factual, int x, int y, int event, int flags)
{
    static int x_anterior = -1;
    static int y_anterior = -1;

    if (event == EVENT_LBUTTONDOWN){
        x_anterior = x;
        y_anterior = y;
        cb_punto(factual, x, y, color_pincel);
    } else if(event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)){
        Mat im = foto[factual].img;

        if (x_anterior == -1){
            x_anterior = x;
            y_anterior = y;
        }

        aux_pintar_segmento(im, Point(x_anterior, y_anterior), Point(x, y), color_pincel);

        imshow(foto[factual].nombre, im);
        foto[factual].modificada = true;

        x_anterior = x;
        y_anterior = y;
    } else if(event == EVENT_LBUTTONUP){
        x_anterior = -1;
        y_anterior = -1;
    }
}

//---------------------------------------------------------------------------

void callback (int event, int x, int y, int flags, void *_nfoto)
{
    int factual= (long long) _nfoto;

    // 1. Eventos y casos especiales
    // 1.1. Evento cerrar ventana
    if (event==EVENT_CLOSE) {
        cb_close(factual);
        return;
    }
    // 1.2. Desactivado el callback al estar ejecutando una herramienta
    if (callback_inactivo) {
        return;
    }
    // 1.3. Evento obtiene el foco
    if (event==EVENT_FOCUS) {
        foto[factual].orden= numpos++;
        return;
    }
    // 1.4. El ratón se sale de la ventana
    if (x<0 || x>=foto[factual].img.size().width || y<0 || y>=foto[factual].img.size().height)
        return;
    // 1.5. Se inicia la pulsación del ratón
    if (event==EVENT_LBUTTONDOWN) {
        downx= x;
        downy= y;
    }

    // 2. Según la herramienta actual
    switch (herr_actual) {

    // 2.1. Herramienta PUNTO
    case HER_PUNTO:
        if (flags==EVENT_FLAG_LBUTTON && event!=EVENT_LBUTTONUP)
            cb_punto(factual, x, y, color_pincel);
        else
            ninguna_accion(factual, x, y);
        break;

    // 2.2. Herramienta ARCOIRIS
    case HER_ARCOIRIS:
        if (flags==EVENT_FLAG_LBUTTON && event!=EVENT_LBUTTONUP)
            cb_punto(factual, x, y, color_arcoiris());
        else
            ninguna_accion(factual, x, y);
        break;

    // 2.3. Herramienta LINEA
    case HER_LINEA:
        if (event==EVENT_LBUTTONUP)
            cb_linea(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_linea(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

    // 2.4. Herramienta RECTÁNGULO
    case HER_RECTANGULO:
        if (event==EVENT_LBUTTONUP)
            cb_rectangulo(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_rectangulo(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

    // 2.5. Herramienta ELIPSE
    case HER_ELIPSE:
        if (event==EVENT_LBUTTONUP)
            cb_elipse(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_elipse(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

    // 2.6. Herramienta SELECCION
    case HER_SELECCION:
        if (event==EVENT_LBUTTONUP)
            cb_seleccionar(factual, x, y);
        else if (event==EVENT_MOUSEMOVE)
            cb_ver_seleccion(factual, x, y, flags!=EVENT_FLAG_LBUTTON);
        break;
    // 2.7. Herramienta SUAVIZADO
    case HER_SUAVIZADO:
        if (flags==EVENT_FLAG_LBUTTON && event!=EVENT_LBUTTONUP)
            cb_suavizado(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;
    // 2.8. Herramienta TRAZO
    case HER_TRAZO:
        cb_trazo(factual, x, y, event, flags);
        break;
    }
    escribir_barra_estado();
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres)
{
    Mat img(foto[nfoto].img.size(), foto[nfoto].img.type());
    bitwise_not(foto[nfoto].img, img);
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void rotar_angulo (Mat imagen, Mat &imgRes, double angulo, double escala, int modo)
{
    double w= imagen.size().width, h= imagen.size().height;
    Size sres;
    if (modo==0) {     // Reescalar con proporción al original
        sres.width= int(w*escala);
        sres.height= int(h*escala);
    }
    else if (modo==1)  // Reescalar y ampliar para caber entera
        sres.width= sres.height= int(sqrt(w*w + h*h)*escala);
    else               // Reescalar y recortar para no mostrar borde
        sres.width= sres.height= int(min(w, h)*escala/sqrt(2.0));
    imgRes.create(sres, imagen.type());
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= sres.width/2+cx*escala;
    M.at<double>(1,2)= sres.height/2+cy*escala;
    imgRes= color_pincel;
    warpAffine(imagen, imgRes, M, sres, INTER_CUBIC);
}

//---------------------------------------------------------------------------

void rotar_exacto (int nfoto, int nres, int grado)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(nres>=0 && nres<MAX_VENTANAS && !foto[nres].usada);
    Mat entrada= foto[nfoto].img;
    Mat salida;
    if (grado == 0)
        salida= entrada.clone();
    else if (grado == 1) {
        transpose(entrada, salida);
        flip(salida, salida, 1);
    }
    else if (grado == 2)
        flip(entrada, salida, -1);
    else if (grado == 3) {
        transpose(entrada, salida);
        flip(salida, salida, 0);
    }
    crear_nueva(nres, salida);
}

//---------------------------------------------------------------------------

void ver_brillo_contraste_gamma (int nfoto, double suma, double prod, double gamma, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img;
    foto[nfoto].img.convertTo(img, CV_8UC3, prod, suma);
    Mat img32f;
    img.convertTo(img32f, CV_32FC3, 1.0/255.0);
    pow(img32f, 1.0/gamma, img32f); // como la formula
    img32f.convertTo(img, CV_8UC3, 255);
    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(tamx>0 && tamx&1 && tamy>0 && tamy&1);
    Mat img= foto[nfoto].img.clone();

    Mat trozo = img(foto[nfoto].roi);

    if (ntipo == 1)
        GaussianBlur(trozo, trozo, Size(tamx, tamy), 0);
    else if (ntipo == 2)
        blur(trozo, trozo, Size(tamx, tamy));
    else if (ntipo == 3)
        medianBlur(trozo, trozo, tamx);
    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void media_ponderada (int nf1, int nf2, int nueva, double peso)
{
    assert(nf1>=0 && nf1<MAX_VENTANAS && foto[nf1].usada);
    assert(nf2>=0 && nf2<MAX_VENTANAS && foto[nf2].usada);
    assert(nueva>=0 && nueva<MAX_VENTANAS && !foto[nueva].usada);
    Mat img= foto[nf1].img.clone();
    Mat cop;
    resize(foto[nf2].img, cop, img.size());
    addWeighted(img, peso, cop, 1.0-peso, 0, img);
    crear_nueva(nueva, img);
}

//---------------------------------------------------------------------------

void ver_histograma(int nfoto, int canal, int nres){

    QImage imq= QImage(":/imagenes/histbase.png");
    Mat img(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);
    // namedWindow("Imagen", WINDOW_NORMAL);
    // imshow("Imagen", img);
    Mat gris;
    Mat hist;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY); // Conversión a gris
    int canales[1]= {0};
    int bins[1]= {256};
    float rango[2]= {0, 256};
    const float *rangos[]= {rango};

    if (canal == 3)
        calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    else
        calcHist(&foto[nfoto].img, 1, &canal, noArray(), hist, 1, bins, rangos);

    double vmin, vmax;
    minMaxLoc(hist, &vmin, &vmax);

    for (int i= 0; i<256; i++){
        qDebug("Celda %d: %g", i, hist.at<float>(i));
        rectangle(img, Point(3 + i * 391 / 256, 185), Point(3+ (i+1) * 391 / 256, 185 - hist.at<float>(i) * 182 / vmax),
                  CV_RGB(canal == 2 ? 255: 0, canal == 1 ? 255 : 0, canal == 0 ? 255 : 0), -1);
    }
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void copiar_a_nueva(int nfoto, int nres){
    Mat nueva = foto[nfoto].img(foto[nfoto].roi).clone();
    crear_nueva(nres, nueva);
}

//---------------------------------------------------------------------------

void ver_ajuste_lineal(int nfoto, double pmin, double pmax, bool guardar){
    // Convertir a grises
    Mat gris, hist;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);

    // Cálculo de histograma
    int canales[1]= {0};
    int bins[1]= {256};
    float rango[2]= {0, 256};
    const float *rangos[]= {rango};
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);

    // Buscar percentiles minimo y máximo
    normalize(hist, hist, 100, 0, NORM_L1);
    int m, M;
    double acum;
    for(m = 0, acum = 0; m < 256 && acum < pmin; m++)
        acum += hist.at<float>(m);
    for(M = 255, acum = 0; M >= 0 && acum < pmax; M--)
        acum += hist.at<float>(M);
    if (M <= m)
        M = m + 1;

    // Aplicar operación de estiramiento
    double alfa = 255.0 / (M - m);
    double beta = -m * alfa;

    Mat res;
    foto[nfoto].img.convertTo(res, CV_8UC3, alfa, beta);
    imshow(foto[nfoto].nombre, res);
    if (guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void ver_bajorrelieve(int nfoto, int nres, double grado, double angulo, int tam, int textura, bool guardar){

    QString nombres[4] = {":/imagenes/arena.jpg", ":/imagenes/gris.jpg", ":/imagenes/cielo.jpg", ":/imagenes/madera.jpg"};

    // Generación de fondo
    QImage imq= QImage(nombres[textura]);
    Mat img(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);

    double factor = grado/pow(4, tam - 3); // normalizado por el tamaño de Sobel

    // Generación del bajorrelieve
    Mat gris, sobel, rotada;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    rotar_angulo(gris, rotada, angulo, 1.0, 1);
    Sobel(rotada, sobel, -1, 1, 0, tam, factor, 128, BORDER_REFLECT);
    rotar_angulo(sobel, rotada, -angulo, 1.0, 0);
    Rect roi((rotada.cols - gris.cols) / 2, (rotada.rows - gris.rows) / 2, gris.cols, gris.cols);
    gris = rotada(roi);

    // Mezclar fondo y bajorrelieve
    cvtColor(gris, gris, COLOR_GRAY2BGR); // Hacer que tenga los mismos canales que img
    resize(img, img, gris.size(), 0, 0, INTER_CUBIC); // Hacer que tengan el mismo tamaño
    addWeighted(img, 1.0, gris, 1.0, -128, img);

    imshow("Bajorrelieve", img);

    if (guardar){
        crear_nueva(nres, img);
    }
}

//---------------------------------------------------------------------------

Scalar color_arcoiris() {
    static int estado = 0;
    static Scalar colorActual = CV_RGB(255, 0, 0);

    int inc = 8;

    switch (estado) {
    case 0:
        colorActual.val[1] += inc;
        if(colorActual.val[1] >= 255) estado++;
        break;
    case 1:
        colorActual.val[2] -= inc;
        if(colorActual.val[2] <= 0) estado++;
        break;
    case 2:
        colorActual.val[0] += inc;
        if(colorActual.val[0] >= 255) estado++;
        break;
    case 3:
        colorActual.val[1] -= inc;
        if(colorActual.val[1] <= 0) estado++;
        break;
    case 4:
        colorActual.val[2] += inc;
        if(colorActual.val[2] >= 255) estado++;
        break;
    case 5:
        colorActual.val[0] -= inc;
        if(colorActual.val[0] <= 0) estado = 0;
        break;
    }

    return colorActual;
}

//---------------------------------------------------------------------------

void ver_matiz_sat_lum(int nfoto, int matiz, double sat, double lum, bool guardar){
    Mat hls;
    cvtColor(foto[nfoto].img, hls, COLOR_RGB2HLS_FULL);

    Mat canales[3];
    split(hls, canales);

    // Evitar saturación y que todo se vuelva rojo
    Mat im16;
    canales[0].convertTo(im16, CV_16S, 1, matiz);
    bitwise_and(im16, Scalar(255), im16);
    im16.convertTo(canales[0], CV_8U);

    canales[1] *= lum;
    canales[2] *=sat;

    merge(canales, 3, hls);
    Mat res;
    cvtColor(hls, res, COLOR_HLS2BGR_FULL);
    imshow(foto[nfoto].nombre, res);
    if (guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void ver_perspectiva(int norig, int ndest, Point2f ptorig[4], Point2f ptdest[4], bool guardar){
    Mat M = getPerspectiveTransform(ptorig, ptdest);
    Mat res = foto[ndest].img.clone(); // Abrir imagen en otra venatana
    warpPerspective(foto[norig].img, res, M, res.size(), INTER_CUBIC, BORDER_TRANSPARENT);

    if (guardar) {
        res.copyTo(foto[ndest].img);
        foto[ndest].modificada = true;
    }

    for(int i = 0; i < 4; i++){
        line(res, ptdest[i], ptdest[(i + 1) % 4], CV_RGB(255 ,255, 255), 2);
    }
    for(int i = 0; i < 4; i++){
        circle(res, ptdest[i], 8, CV_RGB(255, 255, 255), -1);
        circle(res, ptdest[i], 6, CV_RGB(0, 255, 0), -1);
    }
    imshow("Perspectiva", res);
}

//---------------------------------------------------------------------------

void escala_color(int nfoto, int nres){
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    cvtColor(gris, gris, COLOR_GRAY2BGR);
    Mat lut(256, 1, CV_8UC3);
    int vb = color_pincel.val[0];
    int vg = color_pincel.val[1];
    int vr = color_pincel.val[2];

    int vgris = (vb + vg + vr) / 3;

    for (int A = 0; A < 256; A++){
        if (A < vgris){
            lut.at<Vec3b>(A) = Vec3b(vb * A /vgris,
                                     vg * A /vgris,
                                     vr * A /vgris);
        } else {
            lut.at<Vec3b>(A) = Vec3b(255 + (A-255)*(vb-255)/(vgris-256),
                                     255 + (A-255)*(vg-255)/(vgris-256),
                                     255 + (A-255)*(vr-255)/(vgris-256));
        }
    }

    Mat res;
    LUT(gris, lut, res);
    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void ver_pinchar_estirar(int nfoto, int nres, int cx, int cy, double grado, double radio, bool guardar){
    Mat S(foto[nfoto].img.rows, foto[nfoto].img.cols, CV_32FC1);
    double radio2 = radio * radio;
    for (int y = 0; y < S.rows; y++){
        for (int x = 0; x < S.cols; x++){
            S.at<float>(y, x) = exp(-((x-cx)*(x-cx) + (y-cy)*(y-cy))/(radio2));
        }
    }

    // imshow("S", S);

    Mat Gx, Gy;
    Sobel(S, Gx, CV_32F, 1, 0);
    Sobel(S, Gy, CV_32F, 0, 1);

    Mat MapaX(S.rows, S.cols, S.type());
    Mat MapaY(S.rows, S.cols, S.type());

    multiply(S, Gx, Gx, grado);
    multiply(S, Gy, Gy, grado);

    for (int y = 0; y < S.rows; y++){
        for (int x = 0; x < S.cols; x++){
            MapaX.at<float>(y,x) = x + Gx.at<float>(y,x);
            MapaY.at<float>(y,x) = y + Gy.at<float>(y,x);
        }
    }

    Mat res;
    remap(foto[nfoto].img, res, MapaX, MapaY, INTER_CUBIC, BORDER_REFLECT);
    imshow("Pinchar/estirar", res);
    if(guardar)
        crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void ver_balance_blancos(int nfoto, int nres){
    Mat yuv;

    cvtColor(foto[nfoto].img, yuv, COLOR_BGR2YUV);

    vector<Mat> canales;
    split(yuv, canales);

    double media_u = mean(canales[1])[0];
    double media_v = mean(canales[2])[0];

    double dif_u = 128.0 - media_u;
    double dif_v = 128.0 - media_v;

    add(canales[1], Scalar(dif_u), canales[1]);
    add(canales[2], Scalar(dif_v), canales[2]);

    merge(canales, yuv);

    Mat res;
    cvtColor(yuv, res, COLOR_YUV2BGR);

    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void ver_rotar_y_reescalar(int nfoto, int nres, double angulo, double escala, bool guardar)
{
    Mat res;

    rotar_angulo(foto[nfoto].img, res, angulo, escala, 1);
    imshow("Rotar/Reescalar", res);
    if(guardar)
        crear_nueva(nres, res);
}

//---------------------------------------------------------------------------


string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------
;
