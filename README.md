![GitHub license](https://img.shields.io/github/license/new99/Recorder_for_MCU.svg?style=flat-square)](https://github.com/new99/Recorder_for_MCU/blob/master/LICENSE)

# Recorder_for_MCU
Recorder for MCU – программа для мониторинга данных с последовательного порта в реальном времени. Программа написана на C++ с использованием библиотеки Qt.


## Зависимость
![Qt >= 5.7](https://img.shields.io/badge/QT-%3E%3D5.7-green.svg)](https://www.qt.io/)

## Как использовать
Программа с последовательного порта считывает структурированную строку в виде:
`"f1/r/nf2/r/n … fn/r/n/r/n"`,где `f1`, `f2` и `fn` — данные передаваемые с микроконтроллера, `/r/n`  - разделение данных, а в конце `/r/n/r/n` — окончание строки.

![Settings](./Settings.png "Settings")


* `Port` отвечает за подключение последовательного порта;
* `Graph` для мониторинга данных имеет два параметра `to_time` - мониториг по времени и `to_values` - мониторинг зависимости одного значения от другого;
* `Range` если галочка установлена, то оси абцисс задается фиксированый диапозон;
* `Update` если установлена галочка то происходит автоматическое обновление графика, когда значение по оси абцисс, кнопка `Up` обновляет график немедлено;
* `Number of charts` количество графиков;
* `time interval` интервал обновления графиков;
* `Take` отвечает каким образом регистрировать значения в `time interval`;
* Кнопка `Save` сохраняет результат в файле;

## Пример
В качестве примера представлена работа Recorder_for_MCU с Arduino.
`Serial.println(f1)` - стандартная функция для вывода в Arduino впредставляться будет в виде `"f1/r/n"`
`Serial.println()` - пустая строка сигнализирует об оканчании считывания данных

### Пример работы с использование параметра to_time для Graph
Исходный код для Arduino
```c
double x = 0;
void setup()
{
  Serial.begin(9600);
}

void loop() {
  double f1 = sin(x);
  double f2 = sin(2 * x);
  double f3 = 2 * f1 + f2;
  x += 0.003;
  Serial.println(f1, 5);
  Serial.println(f2, 5);
  Serial.println(f3, 5);
  Serial.println();
}
```

![to_time](./Examples/Example_1/to_time.png "to_time")

### Пример работы с использование to_values для параметра Graph
Исходный код для Arduino
```c
double x = 0;
int a = 1;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  double f1 = exp(x)-1;
  if(x < -10)
    a = 1;
  if(x > 10)
    a = -1;

  x += a * 0.001;
  Serial.println(x, 5);
  Serial.println(f1, 5);
  Serial.println();
}
```

![to_values](./Examples/Example_2/to_values.png "to_values")
