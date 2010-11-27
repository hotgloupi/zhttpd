#include "GraphWidget.hpp"
#include "Rcon.hpp"
#include <QColor>
#include <QPainter>

namespace zhttpd
{
    namespace RCON
    {

        GraphWidget::GraphWidget(Rcon& rcon, QWidget* parent) :
			QWidget(parent), _rcon(rcon), _highest_value(1)
        {
            this->setupUi(this);
			this->_image = QImage(this->widget->size(), QImage::Format_ARGB32_Premultiplied);
        }

        GraphWidget::~GraphWidget()
        {
        }

        void GraphWidget::paintEvent(QPaintEvent* ev)
        {
			QPainter painter(this);
			painter.drawImage(this->widget->pos(), this->_image);
			QWidget::paintEvent(ev);
		}

		void GraphWidget::resizeEvent(QResizeEvent* ev)
		{
			QWidget::resizeEvent(ev);
			this->_image = QImage(this->widget->size(), QImage::Format_ARGB32_Premultiplied);
		}

        void GraphWidget::setName(QString const& name)
        {
            this->nameLabel->setText(name);
        }

        void GraphWidget::_drawGraph()
        {
			this->_painter.begin(&this->_image);
			this->_drawLines();
			this->_painter.end();
        }

        void GraphWidget::_drawLines()
        {
			int width = this->widget->width();
			int height = this->widget->height();
			QList<float>::iterator it = this->_values.begin();
			QList<float>::iterator itEnd = --this->_values.end();
			this->_painter.fillRect(this->_image.rect(), QBrush(Qt::white));
			QPainterPath path;
			int pos = width - this->_values.size();
			while (pos < 0)
			{
				this->_values.removeLast();
				pos = width - this->_values.size();
			}
			path.moveTo(pos, height);
            for (; it != itEnd && width > pos; --itEnd, ++pos)
				path.lineTo(pos, height - static_cast<int>(*itEnd * height / this->_highest_value));
			path.lineTo(pos, height - static_cast<int>(*itEnd * height / this->_highest_value));
			path.lineTo(pos + height, height);
			path.closeSubpath();

			QLinearGradient grad;
			grad.setColorAt(0, QColor(0, 250, 0));
			grad.setColorAt(1, QColor(0, 75, 0));
			grad.setStart(0, 0);
			grad.setFinalStop(0, height);
			this->_painter.setBrush(QBrush(grad));
			this->_painter.drawPath(path);
        }

        void GraphWidget::addValue(float value)
        {
            if (value < 0)
                value = 0;
			this->_values.push_front(value);
			if (this->_highest_value < value)
                this->_highest_value = value;
			this->_drawGraph();
			this->valueLabel->setText(QString::number(value));
			this->update();
        }
    }
}

