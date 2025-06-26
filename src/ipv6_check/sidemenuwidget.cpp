#include "SideMenuWidget.h"
#include <QVBoxLayout>

#include "SideMenuWidget.h"
#include <QVBoxLayout>
#include <QWidget>

SideMenuWidget::SideMenuWidget(QWidget *parent)
    : ElaScrollPage(parent)
{
    setTitleVisible(false);
    QWidget *menuWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(menuWidget);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    m_btnNetwork = new ElaPushButton("网络信息", this);
    m_btnSettings = new ElaPushButton("设置页面", this);

    layout->addWidget(m_btnNetwork);
    layout->addWidget(m_btnSettings);
    layout->addStretch();

    addCentralWidget(menuWidget, true, true);
}

ElaPushButton *SideMenuWidget::networkButton() const
{
    return m_btnNetwork;
}

ElaPushButton *SideMenuWidget::settingsButton() const
{
    return m_btnSettings;
}
