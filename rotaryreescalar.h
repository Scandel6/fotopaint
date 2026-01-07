#ifndef ROTARYREESCALAR_H
#define ROTARYREESCALAR_H

#include <QDialog>

namespace Ui {
class RotarYReescalar;
}

class RotarYReescalar : public QDialog
{
    Q_OBJECT

public:
    explicit RotarYReescalar(int numfoto, int numres, QWidget *parent = nullptr);
    ~RotarYReescalar();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_RotarYReescalar_accepted();

    void on_RotarYReescalar_rejected();

private:
    Ui::RotarYReescalar *ui;
    int nres;
};

#endif // ROTARYREESCALAR_H
