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

#include "serialport.h"
#include <QtCore/QDebug>
#include <QString>


SerialPort::SerialPort(QObject *parent, QString port):
    QSerialPort (parent),
    PortName(port),
    Number_graph(1),
    values(false),
    str_values("")
{

}

SerialPort::~SerialPort()
{
    if (this->isOpen())
            this->close();
    m_timer.stop();
    stop_thread();
}

void SerialPort::setPort(QString port)
{
    PortName = port;
}

QString SerialPort::isTime()
{
    return QString::number(my_time.elapsed() / 1000);
}

bool SerialPort::is_values()
{
    return this->values;
}

void SerialPort::Change_in_time()
{
    QByteArray sig = this->readAll();
    this->str_values += sig;

//    this->str_values = "12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n";
    if(this->str_values.isEmpty())
    {
        this->values = false;
        return;
    }
    else
    {
        this->values = true;
    }

//    this->str_values = "12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n";
    QStringList list1 = this->str_values.split("\r\n\r\n");
    this->str_values = list1.back();

    m_x = ((qreal)my_time.elapsed()) / 1000;

    if(pause_bool)
    {
        x_end = ((qreal)my_time.elapsed()) / 1000;
        return;
    }
    switch (take_value) {
    case 1:
    {
        x_begin = 0;
        if(x.size() > 0)
            x_begin = x_end;
        qreal h = (m_x - x_begin) / (list1.size() + 1);
        for(int i = 0; i < list1.size()-1; i++)
        {
            QStringList list2 = list1.at(i).split("\r\n");

            if((Number_graph > list2.size()) && (m_x < is_time_interval() * 2))
                continue;
            if(list2.size() < Number_graph)
                Number_graph = list2.size();

            m_mutex.lock();
            x.push_back(x_begin + (i+1) * h);
            for(int j = 0; (j <  list2.size()) && (j <  Number_graph); j++)
            {
                Number[j] = list2.at(j);
                if(!Number.at(j).isEmpty())
                    m_y[j] = Number.at(j).toDouble();
                y.append(m_y.at(j));
            }
            m_mutex.unlock();
        }
        if(x.size() != 0)
        {
            x_end = x.last();
        }
    }
        break;
    case 2:
    {
        if(list1.size() > 2)
        {
            QStringList list2 = list1.at(list1.size() / 2).split("\r\n");

            if((Number_graph > list2.size()) && (m_x < is_time_interval() * 2))
                break;
            if(list2.size() < Number_graph)
                Number_graph = list2.size();

            for(int j = 0; (j <  list2.size()) && (j <  Number_graph); j++)
            {
                Number[j] = list2.at(j);
                if(!Number.at(j).isEmpty())
                    m_y[j] = Number.at(j).toDouble();
            }
        }
        m_mutex.lock();
        x.push_back(m_x);
        for(int i = 0; i < Number_graph; i++)
            y.append(m_y[i]);

        m_mutex.unlock();
    }
        break;
    case 3:
    {
        QVector<qreal> sum_y(Number_graph);
        for(int j = 0; j < Number_graph; j++)
            sum_y[j] = 0;
        int n = 0;

        for(int i = 0; i < list1.size()-1; i++)
        {

            QStringList list2 = list1.at(i).split("\r\n");

            if((Number_graph > list2.size()) && (m_x < is_time_interval() * 2))
                continue;
            if(list2.size() < Number_graph)
                Number_graph = list2.size();

            for(int j = 0; (j <  list2.size()) && (j <  Number_graph); j++)
            {
                Number[j] = list2.at(j);
                if(!Number[j].isEmpty())
                    sum_y[j] += Number.at(j).toDouble();
            }
            n++;
        }
        m_mutex.lock();
        x.push_back(m_x);
        for(int i = 0; i < Number_graph; i++)
        {
            m_y[i] = sum_y[i] / n;
            y.append(m_y[i]);
        }
        m_mutex.unlock();
    }
        break;
    }


}

void SerialPort::Change_in_values()
{
    QByteArray sig = this->readAll();
    this->str_values += sig;

//    this->str_values = "12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4";
    if(this->str_values.isEmpty())
    {
        this->values = false;
        return;
    }
    else
    {
        this->values = true;
    }

//    this->str_values = "12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4\r\n22.4\r\n\r\n12.0\r\n22.1\r\n33.2\r\n1.4";
    QStringList list1 = this->str_values.split("\r\n\r\n");
    this->str_values = list1.back();
    if(pause_bool)
    {
        return;
    }
    switch (take_value) {
    case 1:
    {
        for(int i = 0; i < list1.size() - 1; i++)
        {
            QStringList list2 = list1.at(i).split("\r\n");
            if(Number_graph  < list2.size())
            {
                if((!list2.at(0).isEmpty()) && (!list2.at(1).isEmpty()))
                {
                    m_x = list2.at(0).toDouble();
                    m_y[0] = list2.at(1).toDouble();
                }

                m_mutex.lock();
                if(x.size() == 0)
                {
                    x.push_back(m_x);
                    y.append(m_y[0]);
                }
                else
                    if((x.back() != m_x) || (y.back() != m_y[0]))
                    {
                        x.push_back(m_x);
                        y.append(m_y[0]);
                    }

                m_mutex.unlock();
            }
        }
    }
        break;
    case 2:
    {
        if(list1.size() > 2)
        {
            QStringList list2 = list1.at(list1.size() / 2).split("\r\n");

            if((Number_graph > list2.size()) && (m_x < is_time_interval() * 2))
                break;

            if(Number_graph  < list2.size())
            {
                if((!list2.at(0).isEmpty()) && (!list2.at(1).isEmpty()))
                {
                    m_x = list2.at(0).toDouble();
                    m_y[0] = list2.at(1).toDouble();
                }

            }
        }
        m_mutex.lock();
        if(x.size() == 0)
        {
            x.push_back(m_x);
            y.append(m_y[0]);
        }
        else
            if((x.back() != m_x) || (y.back() != m_y[0]))
            {
                x.push_back(m_x);
                y.append(m_y[0]);
            }
        m_mutex.unlock();


    }
        break;
    case 3:
    {
        qreal sum_x = 0;
        qreal sum_y = 0;
        int n = 0;
        for(int i = 0; i < list1.size() - 1; i++)
        {
            QStringList list2 = list1.at(i).split("\r\n");
            if(Number_graph  < list2.size())
            {
                if((!list2.at(0).isEmpty()) && (!list2.at(1).isEmpty()))
                {
                    sum_x += list2.at(0).toDouble();
                    sum_y += list2.at(1).toDouble();
                    n++;
                }
            }
        }

        m_mutex.lock();
        if(n > 0)
        {
            m_x = sum_x / n;
            m_y[0] = sum_y / n;
            if(x.size() == 0)
            {
                x.push_back(m_x);
                y.append(m_y[0]);
            }
            else
                if((x.back() != m_x) || (y.back() != m_y[0]))
                {
                    x.push_back(m_x);
                    y.append(m_y[0]);
                }
        }
        m_mutex.unlock();
        break;
    }
    }

}

void SerialPort::clear_numbers()
{
    m_mutex.lock();
    x.clear();
    y.clear();
    my_time.restart();
    m_mutex.unlock();
}

void SerialPort::pause_timer()
{
    if(pause_bool)
    {
        pause_bool = false;
    }
    else
    {
        pause_bool = true;
    }
}

void SerialPort::save(QString path)
{
    QFile *file = new QFile(path);
    QTextStream out(file);
    if (file->open(QIODevice::WriteOnly))
    {
        out << "t ";
        for(int i = 0; i < Number_graph; i++)
        {
            out << "f_" << i << "_(t) ";
        }
        out << endl;
        for(int i = 0; i < x.size(); i++)
        {
            out << x.at(i) << " ";
            for(int j = i * Number_graph; j <  (i + 1) * Number_graph; j++)
                out << y.at(j) << " ";
            out << endl;
        }
        info_connection(tr("Saved"));
    }
    else
        info_connection(tr("No saved"));

    file->close();
}


void SerialPort::start()
{
    this->setPortName(PortName);
    pause_bool = false;
//    qDebug() << PortName;
    if(!this->open(QIODevice::ReadOnly))
    {
//        qDebug() << this->errorString() << endl;
        info_connection(this->errorString());
        return;
    }
    info_connection(tr("Connection"));

    x.clear();
    y.clear();
    Number_graph = is_number_graph();
    take_value = is_take();
    Number.resize(Number_graph);
    m_y.resize(Number_graph);
    for(int i = 0; i < Number_graph; i++)
        n.append(0);

    m_timer.setInterval(is_time_interval() * 1000);

    boot(false);
    this->values = false;

    if(is_values_graph())
    {
        QObject::connect(&m_timer, &QTimer::timeout, this, &SerialPort::Change_in_values);
        this->Number_graph = 1;
    }
    else
        QObject::connect(&m_timer, &QTimer::timeout, this, &SerialPort::Change_in_time);

    m_timer.start();
    my_time.start();
}

void SerialPort::stop()
{
    if(this->isOpen())
    {
        this->close();
    }
    info_connection(tr("Disabled"));

    boot(true);
    m_timer.stop();
    if(is_values_graph())
    {
        QObject::disconnect(&m_timer, &QTimer::timeout, this, &SerialPort::Change_in_values);
    }
    else
        QObject::disconnect(&m_timer, &QTimer::timeout, this, &SerialPort::Change_in_time);
    m_y.clear();
    Number.clear();
}

qreal SerialPort::is_x()
{
    qreal t = 0;
    m_mutex.lock();
    if(x.size() > 0)
        t = x.at(x.size()-1);
    m_mutex.unlock();
    return t;
}

qreal SerialPort::is_y(int i)
{
    qreal t = 0;
    if(i >= Number_graph)
        return 0;
    m_mutex.lock();
    if(y.size() > 0)
        t = y.at(y.size()  - Number_graph + i);
    m_mutex.unlock();
    return t;
}

QVector<qreal> SerialPort::is_XY(int Number_element)
{
    QVector<qreal> XY;
    if(Number_element >= Number_graph)
        return XY;
    m_mutex.lock();
    if(x.size() > 0)
    {
        for(int i = n.at(Number_element); i < x.size(); i++)
        {
                XY.append(x.at(i));
        }
        for(int i = n.at(Number_element); i < x.size(); i++)
        {
                XY.append(y.at(i * Number_graph + Number_element));
        }
        n[Number_element] = x.size();
    }
    m_mutex.unlock();
    return XY;
}
