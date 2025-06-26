#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ElaWidget.h"
#include "SideMenuWidget.h"

#include <QStackedWidget>
#include <QGridLayout>
#include <QCloseEvent>

class NetworkInterfacePage;

class MainWidget : public ElaWidget
{
    Q_OBJECT

public:
    explicit MainWidget(ElaWidget *parent = nullptr);
    ~MainWidget();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initConnect();
    void initMainWidgetResolution();

private:
    // 左侧菜单栏
    SideMenuWidget *m_sideMenu{nullptr};

    // 页面容器
    QStackedWidget *m_stackedWidget{nullptr};

    // 页面
    NetworkInterfacePage *m_networkPage{nullptr};
    QWidget *m_settingsPage{nullptr};
};

#endif // MAINWIDGET_H
