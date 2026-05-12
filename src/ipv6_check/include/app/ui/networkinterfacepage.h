// NetworkInterfacePage.h
#ifndef NETWORKINTERFACEPAGE_H
#define NETWORKINTERFACEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include "ElaPushButton.h"
#include "ElaScrollPage.h"
#include "ElaTreeView.h"
#include "networkinterfacemodel.h"

class NetworkInterfacePage : public ElaScrollPage {
    Q_OBJECT

public:
    explicit NetworkInterfacePage(QWidget *parent = nullptr);

    void setupCustomWidget();
    void setupCentralWidget();
signals:
    void interfaceSelected(const QModelIndex &index);

private slots:
    void onInitNetworkClicked();
    void onTreeViewContextMenuRequested(const QPoint &pos);

private:
    ElaPushButton *m_initButton;
    ElaTreeView *m_treeView;
    NetworkInterfaceModel *m_netModel;

    void setupUI();
    void setupConnections();

};

#endif // NETWORKINTERFACEPAGE_H
