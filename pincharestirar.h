#ifndef PINCHARESTIRAR_H
#define PINCHARESTIRAR_H

#include <QDialog>

namespace Ui {
class PincharEstirar;
}

class PincharEstirar : public QDialog
{
    Q_OBJECT

public:
    explicit PincharEstirar(QWidget *parent = nullptr);
    ~PincharEstirar();

private:
    Ui::PincharEstirar *ui;
};

#endif // PINCHARESTIRAR_H
