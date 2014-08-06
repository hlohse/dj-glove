#ifndef DJ_GLOVE_PC_DJ_GLOVE_H_
#define DJ_GLOVE_PC_DJ_GLOVE_H_

struct DjGlove {
    bool punched;
    bool button_push_0;
    bool button_push_1;
    bool button_push_2;
    bool button_push_3;
    bool button_push_4;
    bool button_touch_0;
    bool button_touch_1;
    bool button_touch_2;
    bool button_touch_3;
    bool button_flip;
    int  poti_0;
    int  poti_1;
    int  poti_2;
    int  distance;
    int  orientation_x;
    int  orientation_y;
    int  orientation_z;
    int  flex;
    int  channel;
    int  program;

    DjGlove();
    ~DjGlove();
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_H_

