#ifndef SIDEMENUWIDGET_H
#define SIDEMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "ElaScrollPage.h"
#include "ElaPushButton.h"

class SideMenuWidget : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit SideMenuWidget(QWidget *parent = nullptr);
    ElaPushButton *networkButton() const;
    ElaPushButton *settingsButton() const;

private:
    ElaPushButton *m_btnNetwork{nullptr};
    ElaPushButton *m_btnSettings{nullptr};
};

#endif // SIDEMENUWIDGET_H
