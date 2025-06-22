#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include "networkinterfacemanager.h"
#include "QStandardItemModel"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_netModel(new NetworkInterfaceModel(this))
{
    ui->setupUi(this);
    ui->treeView->setModel(m_netModel);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pb_initNetworkInfo_clicked()
{
    NetworkInterfaceManager::instance().refreshInterfaces();
    m_netModel->loadInterfaces();
    ui->treeView->expandAll();
}

