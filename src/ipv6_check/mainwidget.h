#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ElaWidget.h"
#include <QStackedWidget>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
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
    // 主界面控件
    QWidget *m_leftMenuWidget{nullptr};
    QStackedWidget *m_stackedWidget{nullptr};
    QPushButton *m_btnNetwork{nullptr};
    QPushButton *m_btnSettings{nullptr};

    // 页面
    NetworkInterfacePage *m_networkPage{nullptr};
    QWidget *m_settingsPage{nullptr};
};

#endif // MAINWIDGET_H
