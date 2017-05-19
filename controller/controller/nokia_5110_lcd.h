#define DDRX DDRD
#define RST PD3
#define SCE PD4
#define D_C PD5
#define SDIN PD6
#define SCK PD7

#define COMAND 0
#define DATAIN 1


#define NOKIA_5110_LCD_INIT	\
IOinit();\
Lcdinit();\
Lcdclear();\
SetX_Y(0,0);
void refresh_screen();
void refresh();
void IOinit(void);
void Writebyte(unsigned char d_cflag, unsigned char din);
void Lcdinit(void);
void Lcdclear(void);
void SetX_Y(unsigned char x, unsigned char y);
void Writeletter(unsigned char letter);
void Writedata_hex(unsigned char outdata);
void Writedata_dec(unsigned char outdata);
void Writestring(unsigned char x_add, unsigned char y_add, char *str);