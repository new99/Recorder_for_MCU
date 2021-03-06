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

#include "graphwindows.h"
#include "graphwindows.h"

#include "chart.h"
#include "chartview.h"
#include <QString>
#include <QtMath>
#include <QToolBar>
#include <QLCDNumber>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QDebug>


GraphWindows::GraphWindows(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle(tr("Graphs window"));
    window = new Info_Dialog(this);

    this->resize(window->size());
    QToolBar *Tb = new QToolBar(this);
    QPushButton *P_w = new QPushButton(this);
    P_w->setText(tr("Settings"));
    QObject::connect(P_w, &QPushButton::released, window, &Info_Dialog::boot);
    Tb->addWidget(P_w);
    this->addToolBar(Tb);

    m_timer = new QTimer(this);
    time = new QTime;
    QObject::connect(m_timer, &QTimer::timeout, this, &GraphWindows::lcd_time);
    QObject::connect(window->ui->stop_pushButton, &QPushButton::released, m_timer, &QTimer::stop);
    m_timer->setInterval(1000);

    port = new SerialPort(this);
    port->setPort(window->ui->comboBox->currentText());

    QObject::connect(window->ui->comboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), port, &SerialPort::setPort);
    QObject::connect(port, &SerialPort::boot, window, &Info_Dialog::button);
    QObject::connect(port, &SerialPort::boot, this, &GraphWindows::boot);
    QObject::connect(port, &SerialPort::is_time_interval, window, &Info_Dialog::is_time_interval);
    QObject::connect(port, &SerialPort::is_number_graph, window, &Info_Dialog::is_number_graph);
    QObject::connect(window->ui->start_pushButton, &QPushButton::released, port, &SerialPort::start);
    QObject::connect(window->ui->stop_pushButton, &QPushButton::released, port, &SerialPort::stop);
    QObject::connect(window->ui->save_pushButton, &QPushButton::released, port, &SerialPort::save);
    QObject::connect(port, &SerialPort::is_number_graph, window, &Info_Dialog::is_number_graph);
    QObject::connect(port, &SerialPort::is_values_graph, window->ui->to_values_radioButton, &QRadioButton::isChecked);
    QObject::connect(port, &SerialPort::info_connection, window->ui->info_label, &QLabel::setText);
    QObject::connect(port, &SerialPort::is_take, window, &Info_Dialog::is_take);
    QObject::connect(window->ui->pause_pushButton, &QPushButton::released, port, &SerialPort::pause_timer);
    QObject::connect(window->ui->Read_pushButton, &QPushButton::released, this, &GraphWindows::read_file);

    QObject::connect(window->ui->pushButton, &QPushButton::released, window, &Info_Dialog::reject);
    QObject::connect(window->ui->pushButton, &QPushButton::released, this, &GraphWindows::close);
    QObject::connect(window->ui->clear_pushButton, &QPushButton::released, this, &GraphWindows::restart);

    bool_Chart = false;

    if(QFile("settings.conf").exists())
    {
        QSettings settings("settings.conf", QSettings::IniFormat);
        settings.beginGroup("Info_Dialog");
        if(settings.value("Graph_to_time").toBool())
            window->ui->to_time_radioButton_2->setChecked(true);
        if(settings.value("Graph_to_values").toBool())
            window->ui->to_values_radioButton->setChecked(true);
        window->ui->range_checkBox->setChecked(settings.value("Range_bool").toBool());
        window->ui->range_spinBox->setValue(settings.value("Range").toInt());
        window->ui->auto_checkBox->setChecked(settings.value("auto_clear").toBool());
        window->ui->number_spinBox->setValue(settings.value("Number_graph").toInt());
        window->ui->time_interval_lineEdit->setText(settings.value("Time_interval").toString());
        window->ui->save_checkBox->setChecked(settings.value("Auto_save").toBool());
        switch (settings.value("Take").toInt())
        {
        case 1:
            window->ui->all_radioButton->setChecked(true);
            break;
        case 2:
            window->ui->last_radioButton->setChecked(true);
            break;
        case 3:
            window->ui->mean_radioButton->setChecked(true);
            break;
        }
        settings.endGroup();
    }



}

GraphWindows::~GraphWindows()
{
    delete lcd;
    delete time;
}

void GraphWindows::launch()
{
    if(bool_Chart)
    {
        delete parentChart;
    }
    parentChart = new QWidget(this);
    centralWidget = new QWidget(parentChart);
    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *HLayout1 = new QHBoxLayout(mainLayout->parentWidget());
    mainLayout->addLayout(HLayout1);

    lcd = new QLCDNumber(mainLayout->parentWidget());
    lcd->setSegmentStyle(QLCDNumber::Flat);
    HLayout1->addWidget(lcd);

    if(window->ui->to_values_radioButton->isChecked())
        window->ui->number_spinBox->setValue(1);
    if(window->is_number_graph() !=0)
        for(int i = 0; i < qreal(window->is_number_graph()) /int(qSqrt(window->is_number_graph())); i++)
        {
            QHBoxLayout *HLayout = new QHBoxLayout(mainLayout->parentWidget());
            for(int j = 0; j <  int(qSqrt(window->is_number_graph())); j++)
                if(i * int(qSqrt(window->is_number_graph())) + j < window->is_number_graph())
                {
                    this->new_Chart(HLayout, i * int(qSqrt(window->is_number_graph())) + j);
                }
                else
                {
                    break;
                }
            mainLayout->addLayout(HLayout);
        }

    port->set_autosave(window->ui->save_checkBox->isChecked());
    bool_Chart = true;
    this->resize(window->size().width() * 2, window->size().height() * 2);
    this->setCentralWidget(centralWidget);
    time->start();
    m_timer->start();

    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("Info_Dialog");
    settings.setValue("Graph_to_time", window->ui->to_time_radioButton_2->isChecked());
    settings.setValue("Graph_to_values", window->ui->to_values_radioButton->isChecked());
    settings.setValue("Range_bool", window->ui->range_checkBox->isChecked());
    settings.setValue("Range", window->ui->range_spinBox->value());
    settings.setValue("auto_clear", window->ui->auto_checkBox->isChecked());
    settings.setValue("Number_graph", window->is_number_graph());
    settings.setValue("Time_interval", window->is_time_interval());
    settings.setValue("Auto_save", window->ui->save_checkBox->isChecked());
    settings.setValue("Take", window->is_take());
    settings.endGroup();

}

void GraphWindows::new_Chart(QHBoxLayout *HLayout, int i)
{
    Chart *chart = new Chart();
    chart->setParent(HLayout->parent());
    QString str = "MCU f_" + QString::number(i) + "_(t)";
    chart->setTitle(str);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    ChartView *chartView = new ChartView(chart, HLayout->parentWidget());
    chartView->setRenderHint(QPainter::Antialiasing);
    HLayout->addWidget(chartView);

    chart->setNumber(i);
    chart->set_Auto_flag(window->ui->auto_checkBox->isChecked());
    chart->set_range(window->ui->range_checkBox->isChecked(), window->ui->range_spinBox->value());

    QObject::connect(chart, &Chart::is_x, port, &SerialPort::is_x);
    QObject::connect(chart, &Chart::is_y, port, &SerialPort::is_y);
    QObject::connect(chart, &Chart::is_XY, port, &SerialPort::is_XY);
    QObject::connect(chart, &Chart::is_time_interval, window, &Info_Dialog::is_time_interval);
    QObject::connect(port, &SerialPort::boot, chart, &Chart::boot);
    QObject::connect(chart, &Chart::set_values, port, &SerialPort::is_values);
    QObject::connect(chart, &Chart::clear_numbers, port, &SerialPort::clear_numbers);
    QObject::connect(window->ui->clear_pushButton, &QPushButton::released, chart, &Chart::set_clear_numbers);
    QObject::connect(window->ui->pause_pushButton, &QPushButton::released, chart, &Chart::pause_timer);

    chart->start();
}

void GraphWindows::new_Chart(QHBoxLayout *HLayout, int i, QVector<qreal> x, QVector<qreal> y)
{
    qDebug() << x.last() << y.last();
    Chart *chart = new Chart();
    chart->setParent(HLayout->parent());
    QString str = "MCU f_" + QString::number(i) + "_(t)";
    chart->setTitle(str);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    ChartView *chartView = new ChartView(chart, HLayout->parentWidget());
    chartView->setRenderHint(QPainter::Antialiasing);
    HLayout->addWidget(chartView);

    chart->setNumber(i);
    chart->gragh_chart(x, y);
    chart->show();

}

void GraphWindows::read_file()
{

    QString path = QFileDialog::getOpenFileName(nullptr, tr("Read Data MCU"), "",
                                                tr("Text (*.txt);; Text (*.out);; All Files (*)"));
    QFile *file = new QFile(path);
    QTextStream in(file);
    int num = 0;
    QVector<qreal> x;
    QVector<QVector<qreal>> y;
    if (file->open(QIODevice::ReadOnly))
    {
        if(!file->atEnd())
        {
            QString str = file->readLine();
            QStringList list1 = str.split(" ");
            num = list1.size()-2;
            y.resize(num);
            while(!file->atEnd())
            {
                QString str = file->readLine();
                QStringList list1 = str.split(" ");
                x.append(list1.at(0).toDouble());
                for(int i = 0; i < num; i++)
                {
                    y[i].append(list1.at(i+1).toDouble());
                }
            }
        }
        if(bool_Chart)
        {
            delete parentChart;
        }
        parentChart = new QWidget(this);
        centralWidget = new QWidget(parentChart);
        mainLayout = new QVBoxLayout(centralWidget);
        QHBoxLayout *HLayout1 = new QHBoxLayout(mainLayout->parentWidget());
        mainLayout->addLayout(HLayout1);
        if(num > 0)
            for(int i = 0; i < qreal(num) / int(qSqrt(num)); i++)
            {
                QHBoxLayout *HLayout = new QHBoxLayout(mainLayout->parentWidget());
                for(int j = 0; j <  int(qSqrt(num)); j++)
                    if(i * int(qSqrt(window->is_number_graph())) + j < num)
                    {
                        qDebug() << i * int(qSqrt(num)) + j;

                        this->new_Chart(HLayout, i * int(qSqrt(num)) + j, x, y.at(i * int(qSqrt(num)) + j));
                    }
                    else
                    {
                        break;
                    }
                mainLayout->addLayout(HLayout);
            }
        this->resize(window->size().width() * 2, window->size().height() * 2);
        this->setCentralWidget(centralWidget);

        window->ui->info_label->setText(tr("Read"));
    }
    else
        window->ui->info_label->setText(tr("No read"));
    file->close();
}



void GraphWindows::boot(bool t)
{

    if(!t)
        this->launch();
}

void GraphWindows::lcd_time()
{
    lcd->display(QString::number(int(time->elapsed() / 60 / 60 / 1000))
                 + ":" + QString::number(int(time->elapsed() /1000 / 60) % 60)
                        + ":" + QString::number(int(time->elapsed()/ 1000) % 60 ));
}

void GraphWindows::restart()
{
    time->restart();
}
