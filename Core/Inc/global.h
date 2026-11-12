/*
 * global.h
 *
 *  Created on: Nov 13, 2026
 *      Author: ASUS
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

typedef enum {
    INIT = 0,

    /* Chế độ Tự động (Automatic) */
    AUTO_RED1_GREEN2,
    AUTO_RED1_AMBER2,
    AUTO_GREEN1_RED2,
    AUTO_AMBER1_RED2,

    /* Chế độ Bằng tay (Manual) */
    MAN_INIT,
    MAN_RED1_GREEN2,
    MAN_RED1_AMBER2,
    MAN_GREEN1_RED2,
    MAN_AMBER1_RED2,

    /* Chế độ Chỉnh thời gian (Tuning) */
    TUN_INIT,
    TUN_RED,
    TUN_AMBER,
    TUN_GREEN
} SystemState_t;

extern SystemState_t status;

extern int time_red;
extern int time_amber;
extern int time_green;

extern int countdown1;
extern int countdown2;

extern int timer_traffic;
extern int timer_blink;

extern float env_temp;
extern float env_humi;

void global_init(void);


#endif /* INC_GLOBAL_H_ */
