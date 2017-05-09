#define FORWARD	0
#define BACK	1

#define PORTX PORTB //port for soft pwm motor

#ifndef THC
	#define THC 1000
#endif
#ifndef TLS
	#define TLS 10
#endif

#define DEFAULT_PWMS 1000

void motor_port_init();

int get_TS();
char get_DIR();
unsigned int get_PWMS();
void inc_TS();
void dec_TS();
void set_TS(unsigned int val);
void set_DIR(char dir);
void change_DIR();
void set_PWMS(unsigned int val);

void reset_PWMC();
void motor_step();
void motor_action();
void motor_forward();
void motor_back();
void motor_stop();