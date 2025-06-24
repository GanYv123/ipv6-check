#include "mainWidget.h"
#include "networkInterfacePage.h"
#include "ElaContentDialog.h"

#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

MainWidget::MainWidget(ElaWidget *parent)
    : ElaWidget(parent)
{
    setWindowTitle("网卡监视工具");
    initMainWidgetResolution();
    initUI();
    initConnect();
}

MainWidget::~MainWidget() = default;

void MainWidget::initUI()
{
    auto *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 左侧菜单栏
    m_leftMenuWidget = new QWidget(this);
    auto *menuLayout = new QVBoxLayout(m_leftMenuWidget);
    menuLayout->setContentsMargins(10, 10, 10, 10);
    menuLayout->setSpacing(10);

    m_btnNetwork = new QPushButton("网络信息", this);
    m_btnSettings = new QPushButton("设置页面", this);

    menuLayout->addWidget(m_btnNetwork);
    menuLayout->addWidget(m_btnSettings);
    menuLayout->addStretch();

    // 右侧页面区（堆叠）
    m_stackedWidget = new QStackedWidget(this);
    m_networkPage = new NetworkInterfacePage(this);
    m_settingsPage = new QWidget(this);
    m_settingsPage->setLayout(new QVBoxLayout); // 占位

    m_stackedWidget->addWidget(m_networkPage);
    m_stackedWidget->addWidget(m_settingsPage);

    // 使用分割器分隔左侧和右侧
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_leftMenuWidget);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 4);

    mainLayout->addWidget(splitter);
}

void MainWidget::initConnect()
{
    connect(m_btnNetwork, &QPushButton::clicked, this, [=]() {
        m_stackedWidget->setCurrentWidget(m_networkPage);
    });

    connect(m_btnSettings, &QPushButton::clicked, this, [=]() {
        m_stackedWidget->setCurrentWidget(m_settingsPage);
    });
}

void MainWidget::initMainWidgetResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    constexpr float resolution = 0.6;
    int windowWidth = static_cast<int>(screenWidth * resolution);
    int windowHeight = static_cast<int>(screenHeight * resolution);

    resize(windowWidth, windowHeight);
    move((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    event->ignore(); // 默认不关闭

    auto *dialog = new ElaContentDialog(this);
    dialog->setLeftButtonText("取消");
    dialog->setMiddleButtonText("最小化");
    dialog->setRightButtonText("退出");

    connect(dialog, &ElaContentDialog::leftButtonClicked, this, [=]() {
        dialog->close();
    });

    connect(dialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        dialog->close();
        this->showMinimized();
    });

    connect(dialog, &ElaContentDialog::rightButtonClicked, this, [=]() {
        dialog->close();
        this->deleteLater(); // 或 qApp->quit();
    });

    dialog->show(); // 非模态对话框
}
