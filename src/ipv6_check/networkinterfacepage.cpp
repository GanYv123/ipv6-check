#include "networkInterfacePage.h"
#include "networkinterfacemanager.h"

#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include "ElaMenu.h"
#include "ElaText.h"

NetworkInterfacePage::NetworkInterfacePage(QWidget *parent)
    : ElaScrollPage(parent),
    m_netModel(new NetworkInterfaceModel(this)),
    m_treeView(new ElaTreeView(this)),
    m_initButton(new ElaPushButton("初始化", this))
{
    setupCustomWidget();
    setupCentralWidget();
    setupConnections();
}

void NetworkInterfacePage::setupCustomWidget()
{
    QWidget *topBar = new QWidget(this);
    auto *layout = new QVBoxLayout(topBar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    auto *subTitle = new ElaText(this);
    subTitle->setText("网卡信息展示模块");
    subTitle->setTextPixelSize(15);
    subTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addWidget(subTitle);
    setCustomWidget(topBar);
}


void NetworkInterfacePage::setupCentralWidget()
{
    QWidget *contentWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_treeView->setModel(m_netModel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeView->setIndentation(30);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    layout->addWidget(m_initButton);
    layout->addWidget(m_treeView);

    addCentralWidget(contentWidget, true, true);
}

void NetworkInterfacePage::setupConnections()
{
    connect(m_initButton, &ElaPushButton::clicked, this, &NetworkInterfacePage::onInitNetworkClicked);
    connect(m_treeView, &ElaTreeView::customContextMenuRequested,
            this, &NetworkInterfacePage::onTreeViewContextMenuRequested);
}

void NetworkInterfacePage::onInitNetworkClicked()
{
    NetworkInterfaceManager::instance().refreshInterfaces();
    m_netModel->loadInterfaces();
}

void NetworkInterfacePage::onTreeViewContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = m_treeView->indexAt(pos);
    if (!index.isValid() || m_treeView->model()->hasChildren(index))
        return;

    ElaMenu menu;
    QAction *copyAction = menu.addAction(tr("复制"));
    QAction *selectedAction = menu.exec(m_treeView->viewport()->mapToGlobal(pos));

    if (selectedAction == copyAction) {
        QString text = index.data().toString();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text);
    }
}
