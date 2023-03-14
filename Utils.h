/**
  ******************************************************************************
  * @file    Utils.h
  * @author  Мокин Д. В. (dimamokin@yandex.ru)
  * @brief   Файл с макросами, используемыми в проекте
  *
  *   Макроопределения для удобного краткого написания некоторых определений и
  *   функций
  *
  ******************************************************************************
  */

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED


// Макросы для отладки (трассировки) через последовательный интерфейс (UART)
// Только для Arduino!!!
#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE

    #define debug(x) Serial.print(#x); Serial.print(" = "); Serial.println(x)
    #define debugs(x) Serial.println(#x)
    #define debugln(x) Serial.println(x)
#else
    #define debug(x)
    #define debugs(x)
    #define debugln(x)

#endif


// Макрос для проверки указателя на функцию перед вызовом и вызова если он не нулевой
// нужно для безоспасности кода с указателями
#define CALL_IF_NOT_NULL(func) if (func) func();
//#define CALL_BY_POINTER(func) if (func) func();
// Макрос для передачи в качестве параметров указателя на массив (таблицу) с его размером
// (в байтах, то есть внутри принимающей функции должен быть
// пересчёт в строки sizeof(table)/sizeof(tableType) )
#define TABLE_WITH_SIZE(table) &table[0], sizeof(table)
#define TABLE(table) &table[0], sizeof(table)/sizeof(table[0])
#define OBJECT(table) &table, sizeof(table)
#define OBJECT1(table) &table, sizeof(table)

// Макрос для вычисления количества строк в произвольной таблице (массиве)
// Нужен для прохождения по всем строкам таблицы в цикле
#define ITEMS_IN(table) sizeof(table)/sizeof(table[0])

#define FOR_EVERY(array, i) for (int i = 0; i < (sizeof(array)/sizeof(array[0])); i++)

// Макрос для вставки в структуру символьного имени и его текстового строки
#define CODANTEX(code) code, #code

/*
typedef volatile union {
    uint32_t DWord;// Доступ к 4-байтному слову
    float Float; // Доступ к вещественному значению (4 байта)
    uint8_t Byte[4]; // Доступ к отдельным байтам
    uint16_t Word[2]; // Доступ к отдельным словам
} TVariant32;
*/

//extern void blink(int pin);

#endif // UTILS_H_INCLUDED
