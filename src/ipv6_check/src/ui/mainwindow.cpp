#include "mainwindow.h"
#include "ElaContentDialog.h"
#include "ElaTheme.h"
#include <QTimer>
#include "ElaApplication.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{
    initWindow();

    //额外布局
    initEdgeLayout();

    //中心窗口
    initContent();

    // 拦截默认关闭事件
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //移动到中心
    moveToCenter();

    //  如果你的windows版本低于Win11 调用原生Mica、Mica-Alt、Acrylic 会导致窗口绘制失效  Dwm_Blur仍可使用
    //   eTheme->setThemeMode(ElaThemeType::Dark);
    //   QTimer::singleShot(1, this, [=]() {
    //       eApp->setWindowDisplayMode(ElaApplicationType::Mica);
    //   });
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWindow()
{
    // setIsEnableMica(true);
    // setIsCentralStackedWidgetTransparent(true);
    setWindowIcon(QIcon(":/include/Image/Cirno.jpg"));
    resize(1200, 740);
    // ElaLog::getInstance()->initMessageLog(true);
    // eTheme->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);

    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg")); // 头像
    setUserInfoCardTitle("用户名称");
    setUserInfoCardSubTitle("用户邮箱");

    setWindowTitle("Network Interface Tools");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
}

void MainWindow::initEdgeLayout()
{

}

void MainWindow::initContent()
{

}
