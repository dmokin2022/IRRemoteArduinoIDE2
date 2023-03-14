#include "PultBeelineRASSE001new.h"

ON_PRESSED(KEY_DIGIT_1) {
//void onKeyPressedKEY_DIGIT_1() {
    Serial.println("1 pressed");
}

ON_AUTOREPEAT(KEY_DIGIT_2) {
//void onKEY_DIGIT_1Pressed() {
    Serial.println("2 autorepeat");
}

ON_RELEASED(KEY_DIGIT_3) {
//void onKEY_DIGIT_2Pressed() {
    Serial.println("3 released");
}


PultButtonItem keyTable[] = {

    //REACTION_ON_PRESSED(KEY_DIGIT_1),
    //REACTION_ON_RELEASED(KEY_DIGIT_2),
    //REACTION_ON_AUTOREPEAT(KEY_DIGIT_3)
    //{KEY_DIGIT_1, &onKEY_DIGIT_1Pressed, 0, 0},
    //{KEY_DIGIT_1, &onKEY_DIGIT_1Pressed, &onKEY_DIGIT_2Autorepeat, &onKEY_DIGIT_3Released},
    {KEY_DIGIT_1, PTR_ON_PRESSED(KEY_DIGIT_1), PTR_ON_AUTOREPEAT(KEY_DIGIT_2), PTR_ON_RELEASED(KEY_DIGIT_3)},
    {KEY_DIGIT_2, 0, &onKEY_DIGIT_2Autorepeat, 0},
    {KEY_DIGIT_3, 0, 0, &onKEY_DIGIT_3Released}
};
