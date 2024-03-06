#include "ipv6qt.h"
#include <qdebug.h>
#include <QMessageBox>
#include <QClipboard>
#include <qregularexpression.h>

ipv6Qt::ipv6Qt(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("ipv6 �鿴�� v2.0"));
    ui.bt_init->setCheckable(true);
    ui.bt_chooseNet->setEnabled(false);
    ui.bt_getip->setEnabled(false);
    ui.lineEdit_port->setEnabled(false);
}

ipv6Qt::~ipv6Qt(){}


void ipv6Qt::on_bt_chooseNet_clicked() {
    if (ui.comboBox->currentText().isEmpty()) { ui.lineEdit->setText("Null Info"); return; }
    
    qDebug() << "on_bt_chooseNet_clicked";
    QString ipv6_info = ipv6_infos[ui.comboBox->currentText()];
    //��ȡ����Ӧ��ipv6��ַ���ַ�������
    ui.lineEdit->setText('[' + ipv6_info + "]:");
}

void ipv6Qt::on_bt_getip_clicked() {
    //��ip��Ϣ���Ƶ����а�
    if (ui.lineEdit_port->text().isEmpty()) { 
        QMessageBox::warning(this, "error", "input your port numbers\n" + QString::fromLocal8Bit("������˿ں�!"));
        return;
    }
    
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui.lineEdit->text()+ui.lineEdit_port->text());
    QMessageBox::information(this, "successful", "ok!");
    //[240e:435:c030:5c:3922:c1a1:d16b:7204]:your_port2243
    qDebug() << ui.lineEdit->text()+ui.lineEdit_port->text();
}

bool ipv6Qt::judgeIpv6(QList<QNetworkInterface>& interfaces) {
    bool is_ipv6 = false;
    foreach(const QNetworkInterface & interface, interfaces) {
        // �ж��������ͣ�ɸѡ����������
        if (interface.type() == QNetworkInterface::Wifi) {
            // �������ӿ�����
            qDebug() << "Interface name: " << interface.name();
            ui.comboBox->addItem(interface.name());
            // ���IPv6��ַ
            foreach(const QNetworkAddressEntry & entry, interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                    if(!is_ipv6) is_ipv6 = true;
                    qDebug() << "IPv6 address: " << entry.ip().toString();
                    ipv6_infos.insert({ interface.name(),entry.ip().toString() });
                }
            }
        }
    }
    return is_ipv6;
}
//��ʼ�� 
void ipv6Qt::on_bt_init_clicked() {

    qDebug() << "on_bt_init_clicked";
    // ��ȡ��������ӿ�
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    if (!judgeIpv6(interfaces)) return;
    //Ѱ���Ƿ���ipv6
    ui.bt_init->setEnabled(false);
    ui.bt_chooseNet->setEnabled(true);
    ui.bt_getip->setEnabled(true);
    ui.lineEdit_port->setEnabled(true);
    QRegularExpression regex("[0-9]*"); //����ƥ������
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
    ui.lineEdit_port->setValidator(validator);
}
