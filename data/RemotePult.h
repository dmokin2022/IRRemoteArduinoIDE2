#ifndef REMOTEPULT_H
#define REMOTEPULT_H

#include "IRremote.h"
#include "Utils.h"

// Макросы для упрощённого составления таблицы функций нажатия кнопок
#define REACTION_ON_PRESSED(key)    {key, &on##key##Pressed, 0, 0}
#define REACTION_ON_RELEASED(key)   {key, 0, &on##key##Released, 0}
#define REACTION_ON_AUTOREPEAT(key) {key, 0, 0, &on##key##Autorepeat}

#define ON_PRESSED(key) void on##key##Pressed()
#define ON_RELEASED(key) void on##key##Released()
#define ON_AUTOREPEAT(key) void on##key##Autorepeat()



enum keyStates {
    KEY_NO,
    KEY_PRESSED,
    KEY_RELEASED,
    KEY_AUTOREPEAT
};

#define NOT_FOUND -1


typedef struct _PultButtonItem {
    uint32_t buttonCode;    // код нажатой кнопки
    //keyStates keyState;
    void (*onButtonPressed)();  // указатель на функцию обработчик нажатой кнопки
    void (*onButtonAutorepeat)();
    void (*onButtonReleased)();
} PultButtonItem;


class RemotePult: public IRrecv  {
    public:
        PultButtonItem* pultButtonTable;    // указетель на таблицу кодов кнопок и обработчиков
        uint8_t pultButtonTableSize;    // размер таблицы в строках
        keyStates state;  // состояние нажатия кнопки. Определяетя keyStates

        decode_results results; // структура для хранения результатов приёма с пульта
        uint32_t newButtonCode;
        uint32_t lastButtonCode;  // код последней нажаток кнопки
        uint32_t lastButtonTime;  // время нажатия последней кнопки
        uint32_t timeEllapsed;      // время прошедшее с последней нажатой кнопки

        //uint32_t lastButtonCode;
        uint8_t buttonPositionInTable;

        RemotePult(int _recvpin, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize):IRrecv(_recvpin) {
            //IRrecv(_recvpin);   // вызов конструктора для родительского класса
            irparams.recvpin = _recvpin;
            irparams.blinkflag = 0;
            state = KEY_NO;
            lastButtonCode = -1;

            pultButtonTable = _pultButtonTable;
            pultButtonTableSize = _pultButtonTableSize;
        }


        RemotePult (int recvpin);
		RemotePult (int recvpin, int blinkpin);

        //bool codeIsFindedInTable(uint32_t _buttonCode);
        uint8_t findPositionInTable(uint32_t _buttonCode);

        void operate();
        void operate2();
        //virtual ~RemotePult();
    protected:
    private:
};


/*
RemotePult::RemotePult(int _recvpin, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize) {

    //IRrecv(_recvpin);   // вызов конструктора для родительского класса
    irparams.recvpin = _recvpin;
	irparams.blinkflag = 0;


    pultButtonTable = _pultButtonTable;
    pultButtonTableSize = _pultButtonTableSize/sizeof(PultButtonItem);

    //irrecv.enableIRIn(); // запускаем прием
    //this->
}
*/
void RemotePult::operate() {

    uint8_t i;

    Serial.println("+");

    if (pultButtonTable != 0) {

        if (decode( &results )) { // если данные пришли
            Serial.println( results.value, HEX ); // печатаем данные

            // Поиск совпадения кода нажатой кнопки со значениями в таблице
            for (i = 0; i < pultButtonTableSize; i++) {
                if (results.value == (pultButtonTable + i)->buttonCode) {
                    break;
                }
            }

            // Если код обнаружен в таблице
            if (i < pultButtonTableSize) {
                //lastButtonCode = results->value;    // сохраняем код последней нажатой кнопки
                buttonPositionInTable = i;

                // Вызов функции обработчика нажатия на кнопку если она задана в таблице
                if ((pultButtonTable + i)->onButtonPressed) (pultButtonTable + i)->onButtonPressed();

            // Если код не обнаружен в таблице
            } else {
                // Если полученный код соответствует повтору нажатия
                if (results.value == 0xFFFFFFFF) {
                    // Если предыдущий код был распознан
                    if (buttonPositionInTable < pultButtonTableSize) {
                        // Вызов функции обработчика нажатия на кнопку если она задана в таблице
                        if ((pultButtonTable + buttonPositionInTable)->onButtonPressed) {
                            (pultButtonTable + buttonPositionInTable)->onButtonPressed();
                        }
                    }
                // Если полученный код не соответствует повтору нажания
                // (любой не распознанный код)
                } else {
                    // Установка признакак нераспознанного кода
                    buttonPositionInTable = pultButtonTableSize;
                }
            }

            resume(); // принимаем следующую команду
        }
    }
}




// Поиск совпадения кода нажатой кнопки со значениями в таблице
uint8_t RemotePult::findPositionInTable(uint32_t _buttonCode) {
    uint8_t i;

    // Проход по всей таблице в поиске совпадающего кода
    for (i = 0; i < pultButtonTableSize; i++) {
        if (_buttonCode == (pultButtonTable + i)->buttonCode) {
            break;
        }
    }

    // Если код обнаружен в таблице
    if (i < pultButtonTableSize) return i;
    else return NOT_FOUND;

}

void RemotePult::operate2() {
    uint8_t i;
    // Если таблица соответствия кодов определена
    if (pultButtonTable != 0) {

        // Если данные пришли
        if (decode( &results )) {

            newButtonCode = results.value;  // код новой нажатой кнопки

            // Сохранение времени последнего прихода данных
            lastButtonTime = millis();

            Serial.println( newButtonCode, HEX ); // печатаем данные

            //buttonPositionInTable = findPositionInTable(newButtonCode);

            if (state == KEY_NO) {
                buttonPositionInTable = findPositionInTable(newButtonCode);
                if (buttonPositionInTable == NOT_FOUND) {

                } else {
                    state = KEY_PRESSED;

                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonPressed);

                    lastButtonCode = newButtonCode;
                }
            } else if (state == KEY_PRESSED) {
                if ((newButtonCode == lastButtonCode)||(newButtonCode == 0xFFFFFFFF)) {
                    state = KEY_AUTOREPEAT;

                    //lastButtonPositionInTable =
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);
                }
            } else if (state == KEY_AUTOREPEAT) {
                CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);

            }

        // Если данные не приходили
        } else {
            if (state == KEY_AUTOREPEAT) {
                timeEllapsed = millis() - lastButtonTime;
                if (timeEllapsed > 200) {
                    state = KEY_RELEASED;
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonReleased);
                }
            }
        }
    }
}

        /*
        switch (state) {
            case KEY_PRESSED: {

            } break;
            case KEY_AUTOREPEAT: {

            }

        }
            //buttonPositionInTable = codeIsFindedInTable(results.value);
            buttonPositionInTable = findPositionInTable(newButtonCode);

            if (buttonPositionInTable != 0) {
                // Перебор состояний и условий для кнопок
                if ((state == KEY_NO) &&
                    (newButtonCode != lastButtonCode) &&
                    (newButtonCode != 0xFFFFFFFF)) {

                    //state = KEY_PRESSED;
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonPressed);
                    state = KEY_AUTOREPEAT;
                }
                //else {
                //    state = KEY_AUTOREPEAT;
                //}

                if ((state == KEY_AUTOREPEAT)) {

                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);

                }

            }

            resume(); // принимаем следующую команду

        // Если данные не пришли
        } else {
            // Замер времени с последнего прихода данных
            timeEllapsed = millis() - lastButtonTime;
            // Если был автоповтор, а потом кнопка была отпущена и не нажималась больше
            // определённого времени, то генерируется событие отжатия кнопки
            if ((state == KEY_AUTOREPEAT) && (timeEllapsed > 500)) {
                CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonReleased);
                state = KEY_NO;
            }
        }


    }

}
*/


#endif // REMOTEPULT_H
