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

#ifndef GRAPHWINDOWs_H
#define GRAPHWINDOWs_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QThread>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "serialport.h"
#include "info_dialog.h"
#include "chart.h"
#include <QLCDNumber>
#include <QTime>
#include <QTimer>

class GraphWindows : public QMainWindow
{
    Q_OBJECT
private:
    void new_Chart(QHBoxLayout *mainLayout, int i);
    Info_Dialog *window = nullptr;
    SerialPort *port = nullptr;
    QLCDNumber *lcd = nullptr;
    QTime *time = nullptr;
    QTimer *m_timer = nullptr;
    qreal begin_x;
    qreal end_x;
    qreal auto_flag;
    QWidget *centralWidget = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    bool bool_Chart;

public:
    explicit GraphWindows(QWidget *parent = nullptr);
    ~GraphWindows();


signals:
    int is_number_graph();

public slots:
    void launch();
    void boot(bool t);
    void lcd_time();
    void restart();
};

#endif // GRAPHWINDOWs_H
