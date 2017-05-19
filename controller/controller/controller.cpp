#include "common_definitions.h"

#include "nokia_5110_lcd.h"

#include <avr/interrupt.h>

#include "string.h"

void uart_init_9600(){
	UBRRH = (F_CPU / BAUD_9600 / 16 - 1) / 256;
	UBRRL = (F_CPU / BAUD_9600 / 16 - 1) % 256;
	UCSRB |= (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);
}

#define MAIN_MEMU_ITEMS 4
#define MAX_SEPPD 10
#define MAX_STEP 50
#define ITEM_BUF 16
#define ARROW '*'

#define MAIN_MENU	0
#define MODE_SELECT	1
#define SET_SPEED	2
#define SET_STEP	3
#define MANIPULATION 4


#define KEY_INC 65
#define KEY_DEC 66
#define KEY_OK 67
#define KEY_BACK 68

#define STAT_ITEMS 4
#define STOP 0
#define AUTO 1
#define MANI 2
#define STEP 3

unsigned char which_menu = MAIN_MENU;
unsigned char status = 0;
unsigned char real_status = 0;

void main_menu_select();
void main_menu_back();

class mode_select
{
	public:
		void inc_status()
		{
			if (status < (STAT_ITEMS - 1))
			{
				status ++;
			}
			else status = 0;
		}
		void dec_status()
		{
			if (status > 0)
			{
				status --;
			}
			else status = STAT_ITEMS - 1;
		}
		void key(unsigned char ch)
		{
			switch(ch)
			{
				case KEY_INC :
					inc_status();
				break;
				case KEY_DEC :
					dec_status();
				break;
				case KEY_BACK :
					status = real_status;
					main_menu_back();
				break;
				case KEY_OK :
					real_status = status;
				break;
			}
		}
		void show()
		{
			Lcdclear();
			Writestring(0, 0, (char*)"Set Status");
			switch(status)
			{
				case STOP :
					Writestring(14, 2, (char*)"STOP");
				break;
				case MANI :
					Writestring(14, 2, (char*)"MANI");
				break;
				case AUTO :
					Writestring(14, 2, (char*)"AUTO");
				break;
				case STEP :
					Writestring(14, 2, (char*)"STEP");
				break;
				default:
				break;
			}
			Writestring(0, 4, (char*)"Current Status");
			switch(real_status)
			{
				case STOP :
					Writestring(24, 5, (char*)"STOP");
				break;
				case MANI :
					Writestring(24, 5, (char*)"MANI");
				break;
				case AUTO :
					Writestring(24, 5, (char*)"AUTO");
				break;
				case STEP :
					Writestring(24, 5, (char*)"STEP");
				break;
				default:
				break;
			}
		}
}mode_select;

class main_menu{
	private:
		unsigned char arrow_where;
		char arrow_array[MAIN_MEMU_ITEMS];
	public:
		main_menu()
		{
			arrow_where = 0;
		}
		void set_arrow(char i)
		{
			arrow_where = i;	
		}
		void refresh_arrow()
		{
			memset(arrow_array, 0, MAIN_MEMU_ITEMS);
			arrow_array[arrow_where] = ARROW;
		}
		void inc_select()
		{
			if (arrow_where < (MAIN_MEMU_ITEMS - 1))
			{
				arrow_where ++;
			}
			else arrow_where = 0;
		}
		void dec_select()
		{
			if (arrow_where > 0)
			{
				arrow_where --;
			}
			else arrow_where = MAIN_MEMU_ITEMS - 1;
		}
		unsigned char get_select()
		{
			return arrow_where;
		}
		void key(unsigned char ch)
		{
			switch(ch)
			{
				case KEY_OK :
					main_menu_select();
				break;
				case KEY_DEC :
					inc_select();
				break;
				case KEY_INC :
					dec_select();
				break;
				default:
				break;
			}
		}
		void show()
		{	
			char buf[ITEM_BUF];
			refresh_arrow();
			Lcdclear();
			buf[0] = arrow_array[0];
			SetX_Y(0, 0);
			strcpy((buf + 1), (char*)" MODE Select");
			Writestring(0, 0, buf);
			buf[0] = arrow_array[1];
			SetX_Y(0, 1);
			strcpy((buf + 1), (char*)" Set Speed");
			Writestring(0, 1, buf);
			SetX_Y(0, 2);
			buf[0] = arrow_array[2];
			strcpy((buf + 1), (char*)" Set Step");
			Writestring(0, 2, buf);
			SetX_Y(0,3);
			buf[0] = arrow_array[3];
			strcpy((buf + 1), (char*)" Manipulation");
			Writestring(0, 3, buf);
			SetX_Y(0,5);
			strcpy(buf, (char*)"Status :");
			switch(real_status)
			{
				case  STOP :
					strcpy(buf + 8, " STOP");
				break;
				case  AUTO :
					strcpy(buf + 8, " AUTO");
				break;
				case  STEP :
					strcpy(buf + 8, " STEP");
				break;
				case  MANI :
					strcpy(buf + 8, " MANI");
				break;
			}
			Writestring(0, 5, buf);
		}
	
}main_menu;

class menu_speed
{
	private:
		char cur_speed;
	public:
		menu_speed()
		{
			cur_speed = 0;
		}
		void inc_speed()
		{
			if (cur_speed < MAX_SEPPD)
			{
				cur_speed ++;
			}
		}
		void dec_speed()
		{
			if (cur_speed > 0)
			{
				cur_speed --;
			}
		}
		void key(unsigned char ch)
		{
			switch(ch)
			{
				case KEY_BACK :
					main_menu_back();
				break;
				case KEY_OK :
					main_menu_back();
				break;
				case KEY_INC :
					inc_speed();
				break;
				case KEY_DEC :
					dec_speed();
				break;
				default:
				break;
			}
		}
		void show()
		{
			Lcdclear();
			Writestring(0, 0, (char*)"Set Speed");
			SetX_Y(12, 3);
			Writedata_dec(cur_speed);
		}
		
}menu_speed;


class menu_step
{
	private:
	char cur_step;
	public:
	menu_step()
	{
		cur_step = 0;
	}
	void inc_step()
	{
		if (cur_step < MAX_STEP)
		{
			cur_step ++;
		}
	}
	void dec_step()
	{
		if (cur_step > 0)
		{
			cur_step --;
		}
	}
	void key(unsigned char ch)
	{
		switch(ch)
		{
			case KEY_BACK :
				main_menu_back();
			break;
			case KEY_OK :
				main_menu_back();
			break;
			case KEY_INC :
				inc_step();
			break;
			case KEY_DEC :
				dec_step();
			break;
			default:
			break;
		}
	}
	void show()
	{
		Lcdclear();
		Writestring(0, 0, (char*)"Set STEP");
		SetX_Y(12, 3);
		Writedata_dec(cur_step);
	}
	
}menu_step;

unsigned char press_key;
unsigned char logic_key;

void to_logic_key()
{
	switch(press_key)
	{
		case 0x01 :
			logic_key = KEY_INC;
		break;
		case 0x02 :
			logic_key = KEY_DEC;
		break;
		case 0x04 :
			logic_key = KEY_BACK;
		break;
		case 0x08 :
			logic_key = KEY_OK;
		break;
		default:
			logic_key = 0xff;
		break;
	}
}

int main(void)
{
	NOKIA_5110_LCD_INIT
	DDRB = 0xff;
	PORTB = 0x00;
	uart_init_9600();
	UDR = 65;
	sei();
	Writestring(0, 0, "hello");
	main_menu.show();
    while(1)
    {
        //TODO:: Please write your application code 
		//UDR = PINB;
		press_key = PINB;
		_delay_ms(100);
		if (press_key == PINB && PINB != 0)
		{
			while(PINB);
			to_logic_key();
			switch(which_menu)
			{
				case MAIN_MENU :
					main_menu.key(logic_key);
				break;
				case SET_SPEED :
					menu_speed.key(logic_key);
				break;
				case SET_STEP :
					menu_step.key(logic_key);
				break;
				case MODE_SELECT :
					mode_select.key(logic_key);
				break;
				default:
					main_menu_back();
				break;
			}
			switch(which_menu)
			{
				case MAIN_MENU :
					main_menu.show();
				break;
				case SET_SPEED :
					menu_speed.show();
				break;
				case SET_STEP :
					menu_step.show();
				break;
				case MODE_SELECT :
					mode_select.show();
				break;
				default:
					main_menu.show();
				break;
			}
		}
		
    }
}

char urc;


ISR(SIG_UART_RECV){
	urc = UDR;
	UDR = urc;
	switch(which_menu)
	{
		case MAIN_MENU :
			main_menu.key(urc);
		break;
		case SET_SPEED :
			menu_speed.key(urc);
		break;
		case SET_STEP :
			menu_step.key(urc);
		break;
		case MODE_SELECT :
			mode_select.key(urc);
		break;
		default:
			main_menu_back();
		break;
	}
	switch(which_menu)
	{
		case MAIN_MENU :
			main_menu.show();
		break;
		case SET_SPEED :
			menu_speed.show();
		break;
		case SET_STEP :
			menu_step.show();
		break;
		case MODE_SELECT :
			mode_select.show();
		break;
		default:
			main_menu.show();
		break;
	}
}
void main_menu_select()
{
	which_menu = main_menu.get_select() + 1;
}

void main_menu_back()
{
	which_menu = 0;
}