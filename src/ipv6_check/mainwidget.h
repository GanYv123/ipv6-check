#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "NetworkInfo.h"
#include "networkinterfacemodel.h"
#include "ElaWidget.h"
#include "ElaTreeView.h"
#include "QGridLayout.h"
#include "ElaPushButton.h"
#include "ModelView/T_TreeViewModel.h"

class MainWidget : public ElaWidget
{
    Q_OBJECT

public:
    MainWidget(ElaWidget *parent = nullptr);
    ~MainWidget();

public:
    void initConnect();
    void initUI();

private slots:
    void on_pb_initNetWorkeInterface_clicked();

private:
    // 网卡信息
    QHash<QString,NetworkInterface> m_map_networkInterfaceInfo;

    // viewtree 中的模型
    NetworkInterfaceModel *m_netModel{nullptr};

    ElaTreeView *m_treeview{nullptr};
    // 布局
    QGridLayout *m_mainGridLayout{nullptr};

    // 控制按钮
    ElaPushButton* m_pb_initNetWorkeInterface{nullptr};  // 初始化网卡信息

    void initMainWidgetResolution();
};
#endif // MAINWIDGET_H
