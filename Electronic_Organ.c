/*
 * Electronic_Organ.c
 *
 *  Created on: 2021��8��27��
 *      Author: ZEL
 */
#include "msp430.h"
#include "Tone.h"
#include "Music.h"
#define MOD_1 1 //�Ÿ�ģʽ
#define MOD_2 2 //����ģʽ
unsigned int mode;

//����
#define LOW_SPEED 0x2000
#define SPEED 0x1000
#define HIGH_SPEED 0x800
unsigned int play_speed = SPEED;

//����
#define VOLUME 10
#define HIGH_VOLUME 50
unsigned int volume = VOLUME;

/*
 * Key_Law
 * 1.0 k1 mi
 * 1.1 k2 si
 * 1.2--key_blue --up
 * 1.3 k4 la
 * 1.4 k5 re
 * 1.5 k6 sol
 * 1.6 k7 do
 * 1.7 k8 fa
 * 2.0 key_red --low
 * 2.1 buzz
 */
//���������ƺ���������ָ����CCR0�������ض�������
void delay();

void buzz(unsigned int TA1CCR0_set, unsigned int beat)
{
    if(TA1CCR0_set != jmp)
    {
        TA1CCR0=TA1CCR0_set;
        TA1CCR1=TA1CCR0_set / volume;
        delay(beat);
        TA1CCR0 = 0;
    }
}

//��ʱ����
void delay(unsigned int beat)
{
    unsigned int i;
    for(i = 0;i < beat; i++)
    {
        unsigned int j;
        for(j = 0; j < play_speed; j++);
    }
}

//�������ļ����ͣ
void pause()
{
    unsigned int j;
    for(j = 0; j < play_speed;j++);
}

//�������ɺ���
void Song_generate(const unsigned int *Song, const unsigned int *Beats, const unsigned int note_num, unsigned int Led_pin)
{
    unsigned int i;
    unsigned int beats_played = 0;
    //ԭ�ٲ���
    play_speed = SPEED;
    volume = VOLUME;
    for(i = 0;i < note_num; i++)
    {
        P2OUT ^= Led_pin; //�ƹ�ȡ��
        buzz(Song[i], Beats[i]);
        beats_played += Beats[i];
        if(beats_played == b4)
        {
            beats_played = 0;
            pause();
        }
        if((P1IN & BIT7) == 0) //����K8��ֹͣ����
        {
            P2OUT |= Led_pin;
            return;
        }
    }
    P2OUT |= Led_pin;
}

//��������ģʽ
void Songs_singing_MODE()
{
    P2OUT &= ~BIT3;
    if((P1IN & BIT2) != 0 && (P2IN & BIT0) != 0) //�����������л�������ģʽ
    {
        mode = MOD_2; //ģʽ�л�
    }
    else if((P1IN & BIT4) == 0) //K5���£������໨�ɣ�L3��
    {
        Song_generate(QHC, Beats_QHC, note_num_QHC, L3_pin);
    }
    else if((P1IN & BIT5) == 0) //K6���£����žջ�̨�� L5��
    {
        Song_generate(JHT, Beats_JHT, note_num_JHT, L5_pin);
    }
    else if((P1IN & BIT6) == 0) //K7���£� �������������ӣ� L6��
    {
        Song_generate(SLDRZ, Beats_SLDRZ, note_num_SLDRZ, L6_pin);
    }
}

//����ģʽ
void Play_song_MODE( )
{
    if((P1IN & BIT2) != 0 && (P2IN & BIT0) != 0) //���º�����л�������ģʽ
    {
        mode = MOD_1; //ģʽ�л�
    }
    if((P1IN & BIT0) == 0) //1.0 --mi
    {
        while((P1IN & BIT0) == 0)
        {
            P2OUT ^= BIT5;
            if((P1IN & BIT2) != 0) buzz(mi_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(mi_l, b1_2);
            else buzz(mi_m, b1_2);
            P2OUT ^= BIT5;
        }
    }
    else if((P1IN & BIT1) == 0)
    {
        while((P1IN & BIT1) == 0)
        {
            P2OUT ^= BIT2;
            P2OUT ^= BIT4;
            P2OUT ^= BIT5;
            if((P1IN & BIT2) != 0) buzz(si_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(si_l, b1_2);
            else buzz(si_m, b1_2);
            P2OUT ^= BIT2;
            P2OUT ^= BIT4;
            P2OUT ^= BIT5;
        }
    }
    else if((P1IN & BIT3) == 0)
    {
        while((P1IN & BIT3) == 0)
        {
            P2OUT ^= BIT4;
            P2OUT ^= BIT5;
            if((P1IN & BIT2) != 0) buzz(la_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(la_l, b1_2);
            else buzz(la_m, b1_2);
            P2OUT ^= BIT4;
            P2OUT ^= BIT5;
        }
    }
    else if((P1IN & BIT4) == 0)
    {
        while((P1IN & BIT4) == 0)
        {
            P2OUT ^= BIT4;
            if((P1IN & BIT2) != 0) buzz(re_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(re_l, b1_2);
            else buzz(re_m, b1_2);
            P2OUT ^= BIT4;
        }
    }
    else if((P1IN & BIT5) == 0)
    {
        while((P1IN & BIT5) == 0)
        {
            P2OUT ^= BIT2;
            P2OUT ^= BIT5;
            if((P1IN & BIT2) != 0) buzz(sol_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(sol_l, b1_2);
            else buzz(sol_m, b1_2);
            P2OUT ^= BIT2;
            P2OUT ^= BIT5;
        }
    }
    else if((P1IN & BIT6) == 0)
    {
        while((P1IN & BIT6) == 0)
        {
            P2OUT ^= BIT2;
            if((P1IN & BIT2) != 0) buzz(do_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(do_l, b1_2);
            else buzz(do_m, b1_2);
            P2OUT ^= BIT2;
        }
    }
    else if((P1IN & BIT7) == 0)
    {
        while((P1IN & BIT7) == 0)
        {
            P2OUT ^= BIT2;
            P2OUT ^= BIT4;
            if((P1IN & BIT2) != 0) buzz(fa_h, b1_2);
            else if((P2IN & BIT0) != 0) buzz(fa_l, b1_2);
            else buzz(fa_m, b1_2);
            P2OUT ^= BIT2;
            P2OUT ^= BIT4;
        }
    }
}

int main()
{
    WDTCTL = WDTPW + WDTHOLD; //�رտ��Ź�
    P2SEL |=BIT1; //�� P2.1Ϊ��ʱ�� TA1 �� PWM �������
    P2SEL2 &=~(BIT1); //P2.1 Ϊ�Ƚ��� 1 �� PWM �������
    P2DIR |= BIT1;
    TA1CTL |=TASSEL0; //ѡ�� TA1 ����ʱ��Ϊ ACLK��ʹ���ϵ縴λ���ã��� 32768Hz
    TA1CCTL1|=OUTMOD1;
    TA1CTL |=TACLR+MC0; //������������ʽ��ʹ�������� 0 ��ʼ������������ TA1CCR0 ���ִ� 0 ������

    //LED�������ã� p2.2,  2.4, 2.5��� //
    P2SEL &= ~(BIT2 + BIT4 + BIT5);
    P2SEL2 &= ~(BIT2 +BIT4 + BIT5);
    P2DIR |= (BIT2 + BIT4 + BIT5);
    P2OUT |= (BIT2 + BIT4 + BIT5);

    //�����������ã�p1ȫΪ���� p2.0����
    P1SEL = 0x00;
    P1SEL2 = 0x00;
    P1DIR = 0x00;
    P1REN = 0xff;
    P1OUT = 0xfb;

    P2SEL &= ~BIT0;
    P2SEL2 &= ~BIT0;
    P2DIR &= ~BIT0;
    P2REN |= BIT0;
    P2OUT &= ~BIT0;

    //�ж���������1.0, 1.1, 1.3,
    P1IES |= (BIT0 + BIT1 + BIT3);
    P1IFG &=~(BIT0 + BIT1 + BIT3);
    P1IE |= (BIT0 + BIT1 + BIT3);

    TA1CCR0 = 0;

    mode = MOD_1;
    _EINT();
    while(1){
        if(mode == MOD_1)
        {
            Songs_singing_MODE();
        }
        else if(mode == MOD_2)
        {
            Play_song_MODE();
        }
    };
}


//�ж�
#pragma vector=PORT1_VECTOR
__interrupt void port_ISR( )
{
    if(mode == MOD_1)//ģʽ1
    {
        if((P1IN & BIT0) == 0) //k1 --��ͣ
        {
            unsigned int temp = TA1CCR0;
            TA1CCR0 = 0;
            while((P1IN & BIT0) != 0);
            TA1CCR0 = temp;
        }
        else if((P1IN & BIT1) == 0) //k2 ����ģʽ
        {
            play_speed = HIGH_SPEED;
        }
        else if((P1IN & BIT3) == 0) //k4 ����ģʽ
        {
            play_speed = LOW_SPEED;

        }
    }
    P1IFG &=~(BIT0 + BIT1 + BIT3);
}

