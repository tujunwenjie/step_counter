/*
 * pedometer_ui.c
 * �Ʋ�������ʵ�֣�����ע�ͣ�
 * ˵�������ļ����ƽ���ʱ��ʹ�õ� lcd.h �е���������ȫ��ɫֵ��
 *       extern u32 g_point_color; // ������ɫ����ɫ��ͨ���������֣�
 *       extern u32 g_back_color;  // ������ɫ
 * ��ʹ�ñ�����ǰ�����ڳ����ʼ���׶����������������Ե�������ɫ��
 */

#include "ui.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

// ��Ļ�ߴ�
#define SCREEN_W 480
#define SCREEN_H 800

// �ֺź꣺���û�Ҫ��ȫ��ʹ�� 24���ɸ���ʵ�������΢����
#define FONT_TITLE        24
#define FONT_STEPS        24
#define FONT_STEPS_SMALL  24
#define FONT_LABEL        24
#define FONT_INFO_VAL     24

// �������꣨����ֺ�24�Ż���
#define TITLE_H           64
#define STEPS_LABEL_Y     (TITLE_H + 8)
#define STEPS_TOP         (TITLE_H + 12)
#define STEPS_CENTER_X    (SCREEN_W / 2)
#define STEPS_BIG_Y       (STEPS_TOP + 28)
#define STEPS_SMALL_Y     (STEPS_BIG_Y + 50)
#define PROG_LABEL_Y      (STEPS_SMALL_Y + 32)
#define PROG_Y            (PROG_LABEL_Y + 26)
#define PROG_H            28
#define PROG_X            40
#define PROG_W            (SCREEN_W - 2*PROG_X)
// ��Ϣ����Ϊ�������У���ǩ����ֵ���Ҳ�
#define INFO_LABEL_Y      (PROG_Y + PROG_H + 18)
#define INFO_LABEL_X      10
#define INFO_VALUE_X      (SCREEN_W - 140)
#define INFO_LINE_H       36
#define INFO_LINE0_Y      (INFO_LABEL_Y)
#define INFO_LINE1_Y      (INFO_LABEL_Y + INFO_LINE_H)
#define INFO_LINE2_Y      (INFO_LABEL_Y + INFO_LINE_H*2)
#define INFO_LINE3_Y      (INFO_LABEL_Y + INFO_LINE_H*3)
#define INFO_VAL_W        120

// ��������ߴ磨���������Ӱ��
#define ERASE_STEPS_LEFT   35
#define ERASE_STEPS_RIGHT  (SCREEN_W - 35)
#define ERASE_STEPS_TOP    (STEPS_BIG_Y - 4)
#define ERASE_STEPS_BOTTOM (STEPS_BIG_Y + 50)

#define ERASE_INFO_COL_W   (INFO_COL_W - 20)
#define ERASE_INFO_VAL_H   32

// ������һ�λ��Ƶ�ֵ�������ظ�����
static pedometer_params_t last = {0xFF, 0xFFFF, 0xFFFF, 0xFFFFFFFF, -1.0f, -1.0f, 0xFF};

// ����������ʱ���ַ��� hh:mm:ss �� mm:ss�������� g_back_color ������Ӧ���Σ�
static void draw_time_str(uint32_t time_s, uint16_t x, uint16_t y, uint8_t font) {
    char buf[16];
    uint32_t h = time_s / 3600;
    uint32_t m = (time_s % 3600) / 60;
    uint32_t s = time_s % 60;
    if (h > 0) {
        snprintf(buf, sizeof(buf), "%02u:%02u:%02u", (unsigned)h, (unsigned)m, (unsigned)s);
    } else {
        snprintf(buf, sizeof(buf), "%02u:%02u", (unsigned)m, (unsigned)s);
    }
    // ������������ʹ�� g_back_color��
    lcd_fill(x, y, x + 130, y + 32, g_back_color);
    lcd_show_string(x, y, 130, 32, FONT_INFO_VAL, (uint8_t*)buf, g_point_color);
}

// ���ƺ����������progress ȡֵ 0.0 - 1.0
static void draw_progress(float progress, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    // ��������ʹ�� g_g_back_color �Ľ���ǳ�ң�ֱ���� g_g_back_color �ĶԱ�ɫ��
    // Ϊ�˱�֤�ɼ��ԣ������� LCD �ⳣ�� LGRAY ��Ϊ���ȱ���
    lcd_fill(x, y, x + w, y + h, LGRAY);
    uint16_t inner_w = (w > 4) ? (w - 4) : 0;
    uint16_t fill_w = (uint16_t)(inner_w * progress + 0.5f);
    if (fill_w > 0) {
        // ʹ�� g_point_color ��Ϊ�������ɫ���Ա�������һ����
        lcd_fill(x + 2, y + 2, x + 2 + fill_w - 1, y + h - 2, g_point_color);
    }
    lcd_draw_rectangle(x, y, x + w, y + h, g_point_color);
}

// ������ʾ���������ƾ��У����������ȹ��㣩
static void show_centered_int(uint16_t center_x, uint16_t y, uint32_t value, uint8_t digits, uint8_t font) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", (unsigned)value);
    // �����ı���ȣ��ַ��� * font * 0.6 (����ֵ)
    uint16_t est_w = (uint16_t)(strlen(buf) * (font * 0.6f));
    uint16_t x = (center_x > est_w/2) ? (center_x - est_w/2) : 0;
    // ʹ�� g_point_color ����������
    g_point_color = g_point_color; // ���ֱ���ʱ���ã�ʵ����ɫ���ⲿ���ã�
    lcd_show_num(x, y, value, digits, font, g_point_color);
}

// ��ʼ�����棨����ע�ͣ�
void PedometerUI_Init(void) {
    // ʹ�� g_g_back_color ��Ϊ����ɫ
    lcd_clear(g_back_color);

    // ����������ɫ������ͻ�Ա��⣩
    lcd_fill(0, 0, SCREEN_W, TITLE_H, DARKBLUE);
    // ����������ɫѡ���ɫ���� DARKBLUE �Աȣ�
        lcd_show_string((SCREEN_W/2) - 70, 16, 140, 32, FONT_TITLE, (uint8_t*)"Pedometer", g_point_color);

    // �����Ҳ�С״̬ռλ����ʼ��ʾ STOPPED��
    lcd_fill(SCREEN_W - 120, 12, SCREEN_W - 8, 56, DARKBLUE);
        lcd_show_string(SCREEN_W - 115, 18, 105, 30, FONT_LABEL, (uint8_t*)"Stopped", g_point_color);

    // ������ǩ
        lcd_show_string(35, STEPS_LABEL_Y, 90, 28, FONT_LABEL, (uint8_t*)"Steps", g_point_color);

    // ���������ʹ�� g_point_color ���������֣�
    // ע��: lcd_show_num �ڲ�ʹ�õ��� g_point_color��ȷ�����ⲿ���� g_point_color
    show_centered_int(STEPS_CENTER_X, STEPS_BIG_Y, 0, 6, FONT_STEPS);

    // С����ʾ current / goal
        lcd_show_string(35, STEPS_SMALL_Y, 410, 28, FONT_STEPS_SMALL, (uint8_t*)"0 / 0", g_point_color);

    // ��������ǩ
        lcd_show_string(PROG_X, PROG_LABEL_Y, 90, 28, FONT_LABEL, (uint8_t*)"Progress", g_point_color);
    
    // ������
    draw_progress(0.0f, PROG_X, PROG_Y, PROG_W, PROG_H);

    // ��Ϣ�����������У���ǩ����ֵ����
    lcd_show_string(INFO_LABEL_X, INFO_LINE0_Y, 120, 28, FONT_LABEL, (uint8_t*)"Duration", g_point_color);
    lcd_show_string(INFO_LABEL_X, INFO_LINE1_Y, 120, 28, FONT_LABEL, (uint8_t*)"Calories", g_point_color);
    lcd_show_string(INFO_LABEL_X, INFO_LINE2_Y, 120, 28, FONT_LABEL, (uint8_t*)"Speed", g_point_color);
    lcd_show_string(INFO_LABEL_X, INFO_LINE3_Y, 120, 28, FONT_LABEL, (uint8_t*)"Status", g_point_color);

    // Ĭ��ֵ��ֵ���Ҳ࣬��� INFO_VAL_W��
    draw_time_str(0, INFO_VALUE_X, INFO_LINE0_Y, FONT_INFO_VAL);
    lcd_show_float(INFO_VALUE_X, INFO_LINE1_Y, 0.0f, INFO_VAL_W, FONT_INFO_VAL, 24);
    lcd_show_float(INFO_VALUE_X, INFO_LINE2_Y, 0.0f, INFO_VAL_W, FONT_INFO_VAL, 24);
    lcd_show_string(INFO_VALUE_X, INFO_LINE3_Y, INFO_VAL_W, 28, FONT_INFO_VAL, (uint8_t*)"Static", g_point_color);

    // ǿ���״θ��£�������Ϊ������ֵ��
    last.start_flag = 0xFF;
    last.step_count = 0xFFFF;
    last.step_max = 0xFFFF;
    last.time_s = 0xFFFFFFFF;
    last.calories = -1.0f;
        last.speed = -1.0f;
    last.static_state = 0xFF;
}

// ���¶�̬�ֶΣ��������仯ʱ�Ż��ƣ�
void PedometerUI_Update(const pedometer_params_t *p) {
    if (!p) return;

    // ����С״̬��START/STOP��
    if (p->start_flag != last.start_flag) {
        lcd_fill(SCREEN_W - 120, 12, SCREEN_W - 8, 56, DARKBLUE);
        if (p->start_flag==1) 
				{
					 g_point_color=GREEN;
					 lcd_show_string(SCREEN_W - 115, 18, 105, 30, FONT_LABEL, (uint8_t*)"Running", g_point_color);
					 g_point_color=BLACK;
        } 
				else if (p->start_flag==0)
				{
           lcd_show_string(SCREEN_W - 115, 18, 105, 30, FONT_LABEL, (uint8_t*)"Stopped", g_point_color);
        }
				else 
				{
					g_point_color=RED;
					lcd_show_string(SCREEN_W - 115, 18, 105, 30, FONT_LABEL, (uint8_t*)"Finish!", g_point_color);
					g_point_color=BLACK;
				}
        last.start_flag = p->start_flag;
    }

    // ������Ŀ��ı� -> ���´����������
    if (p->step_count != last.step_count || p->step_max != last.step_max) {
        // ��������������
        lcd_fill(ERASE_STEPS_LEFT, ERASE_STEPS_TOP, ERASE_STEPS_RIGHT, ERASE_STEPS_BOTTOM, g_back_color);
        // ���ƴ����֣�ʹ�� g_point_color��
        show_centered_int(STEPS_CENTER_X, STEPS_BIG_Y, p->step_count, 6, FONT_STEPS);

        // ����С�� current / goal
        char smallbuf[32];
        snprintf(smallbuf, sizeof(smallbuf), "%u / %u", (unsigned)p->step_count, (unsigned)p->step_max);
        lcd_fill(35, STEPS_SMALL_Y - 2, SCREEN_W - 35, STEPS_SMALL_Y + 30, g_back_color);
    lcd_show_string(35, STEPS_SMALL_Y, 410, 28, FONT_STEPS_SMALL, (uint8_t**)smallbuf, g_point_color);

        // ���½��������������ʹ�� g_point_color��
        float prog = 0.0f;
        if (p->step_max > 0) prog = (float)p->step_count / (float)p->step_max;
        draw_progress(prog, PROG_X, PROG_Y, PROG_W, PROG_H);

        last.step_count = p->step_count;
        last.step_max = p->step_max;
    }

    // ʱ��ÿ����£�ֵ���Ҳࣩ
    if (p->time_s != last.time_s) {
        draw_time_str(p->time_s, INFO_VALUE_X, INFO_LINE0_Y, FONT_INFO_VAL);
        last.time_s = p->time_s;
    }

    // ��·��
    if (p->calories != last.calories) {
        uint16_t colx = INFO_VALUE_X;
        lcd_fill(colx, INFO_LINE1_Y, colx + INFO_VAL_W, INFO_LINE1_Y + ERASE_INFO_VAL_H, g_back_color);
        lcd_show_float(colx, INFO_LINE1_Y, p->calories, INFO_VAL_W, FONT_INFO_VAL, 24);
        last.calories = p->calories;
    }

    // �ٶȣ�m/s��
    if (p->speed != last.speed) {
        uint16_t colx = INFO_VALUE_X;
        lcd_fill(colx, INFO_LINE2_Y, colx + INFO_VAL_W, INFO_LINE2_Y + ERASE_INFO_VAL_H, g_back_color);
        lcd_show_float(colx, INFO_LINE2_Y, p->speed, INFO_VAL_W, FONT_INFO_VAL, 24);
        last.speed = p->speed;
    }

    // ��ֹ / �ƶ� ״̬
    if (p->static_state != last.static_state) {
        uint16_t colx = INFO_VALUE_X;
        lcd_fill(colx, INFO_LINE3_Y, colx + INFO_VAL_W, INFO_LINE3_Y + ERASE_INFO_VAL_H, g_back_color);
        if (p->static_state==0) 
				{
            lcd_show_string(colx, INFO_LINE3_Y, INFO_VAL_W, ERASE_INFO_VAL_H, FONT_INFO_VAL, (uint8_t*)"Static", g_point_color);
        } 
				else if(p->static_state==1)
				{
            lcd_show_string(colx, INFO_LINE3_Y, INFO_VAL_W, ERASE_INFO_VAL_H, FONT_INFO_VAL, (uint8_t*)"Moving", g_point_color);
        }
        last.static_state = p->static_state;
    }
}
