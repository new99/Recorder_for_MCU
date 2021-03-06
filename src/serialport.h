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

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QIODevice>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QMutex>
#include <QVector>
#include <QTime>

/****************************************************************************
** class SerialPort
**
**
****************************************************************************/

class SerialPort : public QSerialPort
{
    Q_OBJECT
private:
    QString PortName;
    QVector<QString> Number;
    QTimer m_timer;
    QMutex	m_mutex;
    int Number_graph;
    QVector<qreal> x;
    QVector<qreal> y;
    qreal m_x;
    QVector<qreal> m_y;
    QTime my_time;
    bool values;
    QString str_values;
    QVector<int> n;
    int take_value;
    bool pause_bool;
    qreal x_begin;
    qreal x_end;
    bool autosave;
    QFile *file;
    void setting_autosave();

public slots:
    void start();
    void stop();
    qreal is_x();
    qreal is_y(int i);
    QVector<qreal> is_XY(int Number_element);
    void setPort(QString port);
    QString isTime();
    bool is_values();
    void Change_in_time();
    void Change_in_values();
    void clear_numbers();
    void pause_timer();
    void set_autosave(bool i);

public:
    SerialPort(QObject *parent = nullptr, QString port = "");
    ~SerialPort();
    void save();
    void work_autosave();

signals:
    void boot(bool t);
    int is_number_graph();
    bool is_values_graph();
    int is_time_interval();
    int is_take();
    void stop_thread();
    void info_connection(QString str);


};

#endif // SERIALPORT_H
