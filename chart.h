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

#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <QLineSeries>
#include <QVector>
#include <QString>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//![1]
class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();

public slots:
    void handleTimeout();
    void boot(bool t);
    void stop();
    void start();
    void setNumber(int n);
    void set_Auto_flag(bool flag);
    void clear_graph();
    void set_clear_numbers();
    void set_range(bool flag, int i);

private:
    QTimer m_timer;
    QLineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axis;
    int Number_graph;
    int Number;
    bool auto_flag;
    bool s_flag;
    bool is_range;
    int range;
    qreal m_step;
    qreal m_x;
    qreal m_y;
    qreal m_x_min;
    qreal m_x_max;
    qreal m_y_min;
    qreal m_y_max;
    void change_range();

signals:
    qreal is_x();
    qreal is_y(int i);
    int is_time_interval();
    bool set_values();
    void clear_numbers();
};
//![1]

#endif /* CHART_H */
