#ifndef INPAINT_H
#define INPAINT_H

#include <QDialog>

namespace Ui {
class Inpaint;
}

class Inpaint : public QDialog
{
    Q_OBJECT

public:
    explicit Inpaint(int numFoto, QWidget *parent = nullptr);
    ~Inpaint();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Inpaint_accepted();

    void on_Inpaint_rejected();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::Inpaint *ui;
};

#endif // INPAINT_H
