/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QString>
#include <QStringList>
#include <QList>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axis(new QValueAxis),
    Number_graph(1),
    Number(0),
    auto_flag(false),
    s_flag(false),
    is_range(false),
    range(20),
    m_step(0),
    m_x(0),
    m_y(0),
    m_x_min(0),
    m_x_max(1e-30),
    m_y_min(0),
    m_y_max(0)
{
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}

Chart::~Chart()
{
    delete m_series;
    delete m_axis;
}

void Chart::handleTimeout()
{
    if(!set_values())
        return;

    m_x = this->is_x();
    m_y = this->is_y(Number);


    if(m_series->count() == 0)
    {
        m_y_max = m_y * 1.01;
        this->axisY()->setMax(m_y_max);
        m_y_min = m_y * 0.99;
        this->axisY()->setMin(m_y_min);
    }
    if(m_y > m_y_max)
    {
        m_y_max = m_y * 1.01;
        this->axisY()->setMax(m_y_max);
    }
    if(m_y < m_y_min)
    {
        m_y_min = m_y * 0.99;
        this->axisY()->setMin(m_y_min);
    }
    m_series->append(m_x, m_y);

    this->clear_graph();
    this->change_range();
}

void Chart::boot(bool t)
{
    if(t)
    {
        this->stop();
    }
    else
    {
        if(s_flag)
        {
            delete this;
        }
        else
        {
            s_flag = true;
        }
    }
}



void Chart::start()
{
    m_timer.setInterval(is_time_interval() * 1000);
    m_series = new QLineSeries(this);
    QPen green(Qt::red);
    green.setWidth(2);
    m_series->setPen(green);
    this->addSeries(m_series);
    this->createDefaultAxes();
    this->setAxisX(m_axis, m_series);
    m_axis->setVisible(false);
    this->axisX()->setRange(m_x_min, m_x_max);
    this->axisY()->setRange(m_y_min, m_y_max);
    m_timer.start();
}

void Chart::setNumber(int n)
{
    Number = n;
}

void Chart::set_Auto_flag(bool flag)
{
    auto_flag = flag;
}

void Chart::clear_graph()
{
    if(!auto_flag)
        return;
    if(m_series->count() < 3)
        return;

    bool t1 = ((m_series->at(m_series->count() - 3).x()) - (m_series->at(m_series->count() - 2).x())) > 0;
    bool t2 = ((m_series->at(m_series->count() - 2).x()) - (m_series->at(m_series->count() - 1).x())) > 0;

    if(t1 != t2)
    {
        qreal x1 = m_series->at(m_series->count() - 2).x();
        qreal x2 = m_series->at(m_series->count() - 1).x();
        qreal y1 = m_series->at(m_series->count() - 2).y();
        qreal y2 = m_series->at(m_series->count() - 1).y();

        m_series->clear();

        m_series->append(x1, y1);
        m_series->append(x2, y2);
        this->clear_numbers();
    }

}

void Chart::set_clear_numbers()
{
    m_x = 0;
    m_y = 0;
    this->axisX()->setRange(m_x_min, m_x_max);
    this->axisY()->setRange(m_y_min, m_y_max);
    m_series->clear();
    this->clear_numbers();
}

void Chart::set_range(bool flag, int i)
{
    is_range = flag;
    range = i;
}

void Chart::change_range()
{
    if(is_range)
    {
        qreal eps = m_x_max - m_x_min;
        if(eps > range)
            m_x_min += eps - range;

    }
    else
    {
        if(m_x > m_x_max)
            m_x_max = m_x;

        if(m_x < m_x_min)
            m_x_min = m_x;
    }

    this->axisX()->setRange(m_x_min, m_x_max);

}

void Chart::stop()
{
    m_timer.stop();
}




