#include <rtx51tny.h>
#include <reg52.h>
#include <string.h>
#include "Key.h"
//#include "calculate.h"
#define Independent_button_1 21 // ��������1   ģʽ�л�
#define Independent_button_2 22 // ��������2   λ���л�
#define Independent_button_3 23 // ��������3    ����
#define Independent_button_4 24 // ��������4    ����
// 0-9 ����ʾ 11��12������13��14��15��16��
code unsigned char NT[] = {0x08, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x3F, 0x39, 0x0f, 0x70, 0x40, 0x80, 0x46};
#define Length 24
unsigned char show_length       = 20;  // ��ʾ����
int show_mode                   = -1; // ��ʾģʽ��0��������ʾģʽ��1���޸�ģʽ  -1������ģʽ
unsigned char Scintillation_bit = 0;  // ��˸λ
bit Scintillation_flag          = 0;  // 0��ʾ��˸λ��1��ʾ��˸λ��
long int answer                      = 49154;
// unsigned char Number[Length] = {10, 11, 12, 13, 14, 15, 16, 7, 8, 9, 1, 1, 2, 3, 4, 5,0,0,0,0,0,0,0,0};
unsigned char Number[Length] = {1, 2, 3, 15,11, 4, 2, 5, 14, 6, 2, 8, 16, 3, 2, 12, 14, 7, 8, 4};
// 123*(425-628/32)-784
sbit P2_2             = P2 ^ 2;
sbit P2_3             = P2 ^ 3;
sbit P2_4             = P2 ^ 4;
unsigned char Location = 0; // ��ʾλ��
unsigned char Offset   = 0; // ƫ����
unsigned char KeyNum;      // ���µİ���

void display(void) _task_ 0
{
    os_create_task(1);
    os_create_task(2);
    os_create_task(3);
    while (1) {
        P0 = 0x00;
        switch(Location)
	{
		case 0:P2_4=1;P2_3=1;P2_2=1;break;
		case 1:P2_4=1;P2_3=1;P2_2=0;break;
		case 2:P2_4=1;P2_3=0;P2_2=1;break;
		case 3:P2_4=1;P2_3=0;P2_2=0;break;
		case 4:P2_4=0;P2_3=1;P2_2=1;break;
		case 5:P2_4=0;P2_3=1;P2_2=0;break;
		case 6:P2_4=0;P2_3=0;P2_2=1;break;
		case 7:P2_4=0;P2_3=0;P2_2=0;break;
	}
        if (!show_mode)
            P0 = NT[Number[Location + Offset]];
        else if (show_mode) {
            if (Scintillation_bit != Location)
                P0 = NT[Number[Location + Offset]];
            else
                P0 = Scintillation_flag ? 0x00 : NT[Number[Location + Offset]];
        } else if (show_mode == -1) {
            P0 = NT[Number[Location]];
        } else {
        }
        Location++;
        Location = Location % 8;
        os_wait2(K_TMO, 1);
    }
}
int i = 0;
void show_mode_change(void) _task_ 1
{
    while (1) {
        if (show_mode == 0) // ��ʾģʽ��ÿ��ˢ��8��λ��
        {
            if (show_length >= 8) {
                Offset += 1;
                Offset %= (show_length - 7);
            }
            for (i = 0; i <= 3; i++)
                os_wait2(K_TMO, 700);
        } else if (show_mode == 1 || show_mode == -1) // �޸�ģʽ��ÿ�η�ת��ǰλ�����
        {
            Scintillation_flag = !Scintillation_flag;
            for (i = 0; i <= 2; i++)
                os_wait2(K_TMO, 430);
        }
    }
}

void Task_3(void) _task_ 3
{

    while (1) {
        Key_Loop();
        os_wait2(K_TMO, 70);
    }
}
int count = 0;
void Task_2(void) _task_ 2
{
    while (1) {
        KeyNum = Key();
        if (KeyNum) {
            if (KeyNum == Independent_button_1) { // ��������1
                if (show_mode == -1) {
                    show_mode         = 0;
                    Scintillation_bit = 0;
                    Offset            = 0;
                } else if (show_mode == 0 || show_mode == 1) {
                    show_mode         = !show_mode;
                    Scintillation_bit = 0;
                    Offset            = 0;
                    show_length       = 1;
                }
            } else if (KeyNum == Independent_button_2) { // ��������2
                if (show_mode == 1) {
                    Scintillation_bit++;
                    if (Scintillation_bit > 8) {
                        Scintillation_bit = 8;
                        Offset++;
                    }
                }
            } else if (KeyNum == Independent_button_3) { // ��������3
                show_mode = -1;
                for (count = 0; count < Length; count++) {
                    Number[count] = 0;
                }
                Offset            = 0;
                Scintillation_bit = 0;
            } else if (KeyNum == Independent_button_4) { // ��������4
                                                         // ����
                   //answer=ans_calculate(Number);
										answer              = 49154;
                   for(count=0;count<Length;count++)
                   {
                       Number[count]=0;
                   }
                   count=4;
                   while (answer)
                   {
                       Number[count] = answer % 10;
                       answer /= 10;
                       count--;
                   }                   
            } else if (KeyNum <= 16) {
                if (show_mode == 1) {
                    Number[Scintillation_bit + Offset] = KeyNum;
                } else if (show_mode == -1) {
                    Number[Scintillation_bit + Offset] = KeyNum;
                    Scintillation_bit++;
                    show_length++;
                    if (Scintillation_bit > 8) {
                        Scintillation_bit = 8;
                        Offset++;
                    }
                }
            }
        }
        os_wait2(K_TMO, 10);
    }
}