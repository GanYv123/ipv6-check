#include "mainwidget.h"
#include "networkinterfacemanager.h"
#include "QStandardItemModel"
#include "QHeaderView"
#include "ElaMessageButton.h"
#include <QGuiApplication>
#include <QScreen>

MainWidget::MainWidget(ElaWidget *parent)
    : ElaWidget(parent)
    , m_netModel(new NetworkInterfaceModel(this))
    , m_treeview(new ElaTreeView(this))
{
    initMainWidgetResolution();
    initUI();
    initConnect();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initConnect()
{
    connect(m_pb_initNetWorkeInterface,&ElaPushButton::clicked
            ,this,&MainWidget::on_pb_initNetWorkeInterface_clicked);
}

void MainWidget::initUI()
{
    m_pb_initNetWorkeInterface = new ElaPushButton("初始化");

    m_treeview->setModel(m_netModel);
    m_mainGridLayout = new QGridLayout(this);
    m_mainGridLayout->addWidget(m_treeview,0,0);

    m_mainGridLayout->setContentsMargins(0,0,0,0);
    this->setLayout(m_mainGridLayout);
    m_mainGridLayout->addWidget(m_pb_initNetWorkeInterface,1,0);
    // 调整treeview自适应列宽
    m_treeview->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeview->setIndentation(30);

}

void MainWidget::on_pb_initNetWorkeInterface_clicked()
{
    NetworkInterfaceManager::instance().refreshInterfaces();
    m_netModel->loadInterfaces();
}

void MainWidget::initMainWidgetResolution()
{
    // 获取主屏幕分辨率
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 计算窗口尺寸（比如设置为屏幕的 70% 大小）
    int windowWidth = static_cast<int>(screenWidth * 0.7);
    int windowHeight = static_cast<int>(screenHeight * 0.7);

    // 设置窗口大小
    resize(windowWidth, windowHeight);

    // 可选：居中显示
    move((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
}

