#include "MainWidget.h"
#include "networkInterfacePage.h"
#include "ElaContentDialog.h"
#include "ElaText.h"

#include <QApplication>
#include <QScreen>
#include <QSplitter>
#include <QVBoxLayout>

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

    // 左侧菜单栏（封装类）
    m_sideMenu = new SideMenuWidget(this);

    // 右侧页面区
    m_stackedWidget = new QStackedWidget(this);
    m_networkPage = new NetworkInterfacePage(this);

    m_settingsPage = new QWidget(this);
    auto *text = new ElaText("设置界面", this);
    auto *layout = new QVBoxLayout(m_settingsPage);
    layout->addWidget(text);
    m_settingsPage->setLayout(layout);

    m_stackedWidget->addWidget(m_networkPage);
    m_stackedWidget->addWidget(m_settingsPage);

    // 分割器
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_sideMenu);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 4);

    splitter->setStyleSheet(R"(
    QSplitter::handle {
        background-color: #c0c0c0;
    }
    QSplitter::handle:hover {
        background-color: #a0a0a0;
    }
)");
    splitter->setHandleWidth(4);

    mainLayout->addWidget(splitter);
}

void MainWidget::initConnect()
{
    connect(m_sideMenu->networkButton(), &QPushButton::clicked, this, [=]() {
        m_stackedWidget->setCurrentWidget(m_networkPage);
    });

    connect(m_sideMenu->settingsButton(), &QPushButton::clicked, this, [=]() {
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
