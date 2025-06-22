#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "NetworkInfo.h"
#include "networkinterfacemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_pb_initNetworkInfo_clicked();

private:
    Ui::MainWidget *ui;

    // 网卡信息
    QHash<QString,NetworkInterface> m_map_networkInterfaceInfo;

    // viewtree 中的模型
    NetworkInterfaceModel *m_netModel;
};
#endif // MAINWIDGET_H
