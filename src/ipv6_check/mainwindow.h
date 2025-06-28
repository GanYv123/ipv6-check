#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include "ElaWindow.h"

class ElaContentDialog;

class MainWindow : public ElaWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    ElaContentDialog* _closeDialog{nullptr};

};

#endif // MAINWINDOW_H
