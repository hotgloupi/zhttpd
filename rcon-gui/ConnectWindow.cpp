#include "ConnectWindow.hpp"
#include "Rcon.hpp"
#include <QMessageBox>

namespace ZHTTPD
{
    namespace RCON
    {

        ConnectWindow::ConnectWindow(Rcon& rcon) :
            QMainWindow(&rcon.getMainWindow(), Qt::Tool), _rcon(rcon)
        {
            this->setupUi(this);
            this->_rcon.getSettings().beginGroup("ConnectWindow");
            if (this->_rcon.getSettings().contains("size"))
                this->resize(this->_rcon.getSettings().value("size", QSize(260, 120)).toSize());
            if (this->_rcon.getSettings().contains("pos"))
                this->move(this->_rcon.getSettings().value("pos", QPoint(100, 100)).toPoint());
            this->_rcon.getSettings().endGroup();
        }

        ConnectWindow::~ConnectWindow()
        {
        }

        void ConnectWindow::closeEvent(QCloseEvent*)
        {
            this->_rcon.getSettings().beginGroup("ConnectWindow");
            this->_rcon.getSettings().setValue("size", this->size());
            this->_rcon.getSettings().setValue("pos", this->pos());
            this->_rcon.getSettings().endGroup();
        }

        void ConnectWindow::on_goPushButton_clicked()
        {
            this->_rcon.getNetwork().connectToHost(this->addressLineEdit->text(), this->portSpinBox->value());
        }

        void ConnectWindow::showError(QString const& error)
        {
            QMessageBox::warning(this, "Error", error);
        }

        void ConnectWindow::keyPressEvent(QKeyEvent* e)
        {
            if ((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter))
                this->on_goPushButton_clicked();
            else
                this->QMainWindow::keyPressEvent(e);
        }

    }
}

