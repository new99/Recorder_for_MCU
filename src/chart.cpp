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
#include <QColor>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(nullptr),
    m_axis(new QValueAxis),
    m_mutex(QMutex::NonRecursive),
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
    m_y_max(0),
    pause_bool(true)
{
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    Str_Color_Names << "red" << "green" << "blue" << "black" << "magenta" << "gray" << "darkCyan" << "yellow" << "darkGreen" << "darkMagenta";
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
    QVector<qreal> XY = this->is_XY(Number);
//    qDebug() << XY.size();
    if((XY.size() < 2) && (XY.size() % 2 != 0))
        return;
    auto yAxis = this->axes(Qt::Vertical).back();
    m_mutex.lock();
    for(int i = 0; i < XY.size() / 2; i++)
    {
        m_x = XY.at(i);
        m_y = XY.at(XY.size() / 2 + i);

        if(m_series->count() == 0)
        {
            m_y_max = m_y * 1.01;
            yAxis->setMax(m_y_max);
            m_y_min = m_y * 0.99;
            yAxis->setMin(m_y_min);
            m_x_max = m_x * 1.01;
            m_x_min = m_x * 0.99;
        }
        if(m_y > 0)
        {
            if(m_y > m_y_max)
            {
                m_y_max = m_y * 1.01;
                yAxis->setMax(m_y_max);
            }
            if(m_y < m_y_min)
            {
                m_y_min = m_y * 0.99;
                yAxis->setMin(m_y_min);
            }
        }
        else
        {
            if(m_y > m_y_max)
            {
                m_y_max = m_y * 0.99;
                yAxis->setMax(m_y_max);
            }
            if(m_y < m_y_min)
            {
                m_y_min = m_y * 1.01;
                yAxis->setMin(m_y_min);
            }
        }

        m_series->append(m_x, m_y);

    }

    this->clear_graph();
    this->change_range();
    m_mutex.unlock();
}

void Chart::boot(bool t)
{
    if(t)
    {
        this->stop();
        m_axis->setVisible(true);
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
    pause_bool = false;
    m_series = new QLineSeries(this);
    QPen color_graph(QColor(Str_Color_Names.at(Number)));

    color_graph.setWidth(2);
    m_series->setPen(color_graph);
    this->addSeries(m_series);
    this->createDefaultAxes();

    auto xAxis = this->axes(Qt::Horizontal).back();
    auto yAxis = this->axes(Qt::Vertical).back();
    m_axis->setVisible(false);
    xAxis->setRange(m_x_min, m_x_max);
    yAxis->setRange(m_y_min, m_y_max);
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
    m_x = 0.0;
    m_y = 0.0;
    m_x_min = 0.0;
    m_x_max  = 1e-16;
    m_y_min = 0;
    m_y_max = 1e-16;
    auto xAxis = this->axes(Qt::Horizontal).back();
    auto yAxis = this->axes(Qt::Vertical).back();
    m_mutex.lock();
    xAxis->setRange(m_x_min, m_x_max);
    yAxis->setRange(m_y_min, m_y_max);
    m_series->clear();

    this->clear_numbers();
    m_mutex.unlock();
}

void Chart::set_range(bool flag, int i)
{
    is_range = flag;
    range = i;
}

void Chart::pause_timer()
{
    if(pause_bool)
    {
        m_timer.start();
        pause_bool = false;
    }
    else
    {
        m_timer.stop();
        pause_bool = true;
    }
}

void Chart::gragh_chart(QVector<qreal> x, QVector<qreal> y)
{
    m_series = new QLineSeries(this);
    QPen color_graph(QColor(Str_Color_Names.at(Number)));
    color_graph.setWidth(2);
    m_series->setPen(color_graph);
    this->addSeries(m_series);
    this->createDefaultAxes();


    m_axis->setVisible(false);

    for(int i = 0; i < x.size(); i++)
    {
        m_series->append(x.at(i), y.at(i));
    }
    this->auto_range(x, y);

}

void Chart::auto_range(QVector<qreal> x, QVector<qreal> y)
{
    qSort(x.begin(), x.end());
    qSort(y.begin(), y.end());
    auto xAxis = this->axes(Qt::Horizontal).back();
    auto yAxis = this->axes(Qt::Vertical).back();
    xAxis->setRange(x.first(), x.last());
    yAxis->setRange(y.first(), y.last());
}

void Chart::change_range()
{
    if(is_range)
    {
        m_x_max = m_x;
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

    auto xAxis = this->axes(Qt::Horizontal).back();
    xAxis->setRange(m_x_min, m_x_max);
}

void Chart::stop()
{
    m_timer.stop();
}




