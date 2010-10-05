#ifndef __GRAPHWIDGET_HPP__
#define __GRAPHWIDGET_HPP__

#include <QImage>
#include <QPainter>
#include <QList>

#include "ui_graph.h"

namespace ZHTTPD
{
    namespace RCON
    {

        class Rcon;

        class GraphWidget :
            public QWidget, private Ui::GraphWidget
        {
            Q_OBJECT

            public:
                explicit GraphWidget(Rcon& rcon, QWidget* parent);
                ~GraphWidget();
                void setName(QString const& name);
                void addValue(float value);

            private:
                virtual void paintEvent(QPaintEvent* ev);
				virtual void resizeEvent(QResizeEvent* ev);
                void _drawGraph();
                void _drawLines();
                Rcon& _rcon;
				QImage _image;
				QPainter _painter;
                QList<float> _values;
                float _highest_value;
				bool _need_paint;
        };

    }
}

#endif

