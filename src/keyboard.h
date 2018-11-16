#ifndef KEYBOARD_H
#define KEYBOARD_H



struct KBEvent
{
    int countK_a = 0;
    int countK_w = 0;
    int countK_s = 0;
    int countK_d = 0;
};
KBEvent getKBchange() noexcept;

#endif // KEYBOARD_H
