#include "./application.h"
#include "4004_chip.h"

uint32_t cicles_limit;
uint32_t cicle;
int c_bank;
int c_chip;
bool shound_run;
bool animated_run;
bool run;

void application_init(){
	cicles_limit = 0;
	cicle = 0;
	c_bank = 0;
	c_chip = 0;
	shound_run = true;
	animated_run = false;
	run = false;

	cli_screen_startup();
}

void application_main_loop(chip_4004 *c){
	char opt;
	while(shound_run){
		cli_main_view(c, c_bank, c_chip, cicle);
		if(!run)
			opt = application_input();

		if(run){
			cicle += chip_cycle(c, 99999999);
		}
		switch(opt){
			case 'r':
				run = true;
				break;
			case 'n':
				cicles_limit = 1;
				cicle += chip_cycle(c, cicles_limit);
				break;
			case 'o':
				printf("(0) Set bank\n(1) Set chip\n");
				getchar();
				switch(getchar()){
					case '0':
						printf("Bank: (0-7)\n");
						scanf("%d", &c_bank);
						break;
					case '1':
						printf("Chip: (0-3)\n");
						scanf("%d", &c_chip);
						break;
				}
				break;
			case 'q':
				shound_run = false;
				break;

		}
	}

}

char application_input(){
	printf("\nACTIONS\n(n) next   (a) animated   (r) run   (o) operations   (q) quit\n");
	return getchar();
}
