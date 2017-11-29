/*
	This is a music generator.
	[29.11.2017]
	Nickolas Yakutovich aka robotustra
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include "common.h"

#define START_NOTE	39
#define END_NOTE	50
#define NEG_INT		-5
#define POS_INT		5
#define FRAC_MAX	4		
#define MAX_LEN		512
#define TOT_NOTES	88


char* note[TOT_NOTES] = {
	"Ad", "Ad#", "Bb",
	"Cc", "Cc#", "Dc", "Dc#", "Ec", "Fc", "Fc#", "Gc", "Gc#", "Ac", "Ac#", "Bc",
	"Cb", "Cb#", "Db", "Db#", "Eb", "Fb", "Fb#", "Gb", "Gb#", "Ab", "Ab#", "Bb",
	"Ca", "Ca#", "Da", "Da#", "Ea", "Fa", "Fa#", "Ga", "Ga#", "Aa", "Aa#", "Ba",
	"C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1",
	"C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2",
	"C3", "C3#", "D3", "D3#", "E3", "F3", "F3#", "G3", "G3#", "A3", "A3#", "B3",
	"C4", "C4#", "D4", "D4#", "E4", "F4", "F4#", "G4", "G4#", "A4", "A4#", "B4",
	"C5"
};

char* dur[6] = {
	"1", "1/2", "1/4", "1/8", "1/16", "1/32" 
};


int main(int argc, char *argv[]) {
	int i, j, k;
	long cnt = 0;
	int nc = 0;

	double tact = 0.0;
	double test_dur_tact = 0.0;
	
	char buf[MAX_LEN];

	int cur_int[32]; // текущие интервалы для последующих нот в такте.
	int cur_dur[32]; // текущий показатель делителя длительности ноты
	int cur_mute[32]; // замьючена ли текущай нота.
	int cur_note = 0;
	int test_note = 0;
	char int_flag = 0;
	char dur_flag = 0;

	for (nc = 0; nc<32; nc ++){
		cur_int [nc] = NEG_INT;
		cur_dur [nc] = 0;
		cur_mute[nc] = 0;
	}


	//# 1
	for (i=START_NOTE; i<=END_NOTE; i++){ 
		for (k=0; k<FRAC_MAX; k++) {
			for (j=0; j<2; j++ ){
				memset(buf,0,MAX_LEN);
				if (j==0) sprintf(buf, "*" ); // если пауза
				cur_note = i;
				tact = 1.0/(1<<k);
				sprintf(buf,"%s%s[1/%d]", buf, note[i], 1<<k );
				// Add other notes if the duration is < 1
				nc = 1;
				do {
					// генерируем ноту исходя из текущего значения интервала для каждой ноты. Интервал для первой ноты уже задан
					if (tact >= 1.0) { cnt ++;  goto tact_complete;}
					// иначе добавляем ноту.
					test_note = cur_note + cur_int[nc];
					//printf("test_note = %d, ", test_note);
					if (test_note>=0 && test_note<TOT_NOTES){
						// Попали в диапазон 88 нот. Теперь нужно посчитать длительность интервала, который остался 
						//до конца такта.
						test_dur_tact = tact + 1.0/(1<<cur_dur[nc]);
						//printf("test_dur = %lf\n", test_dur_tact);
						if (test_dur_tact < 1.0001) {
							// попали в длительность такта.
							// можно добавлять ноту в буфер

							sprintf(buf, "%s,", buf); // разделитель

							if (cur_mute[nc]=0) sprintf(buf, "%s*", buf ); // если пауза
							cur_note = test_note;
							tact = test_dur_tact;
							sprintf(buf,"%s%s[1/%d]", buf, note[cur_note], 1<<cur_dur[nc] );
							if (tact >= 1.0) { cnt ++;  goto tact_complete;}
						}
					}
					// итерируем все для ноты до следующего раза.
					
					if(cur_int[nc]<POS_INT) 
					{	// не дошли до конца интервалов
						cur_int[nc] ++;
					} else 
					{
						cur_int[nc] = NEG_INT; // сбрасываем счетчик
						if (cur_dur[nc] < FRAC_MAX) cur_dur[nc] ++;
						else 
						{
							cur_dur[nc] = 0;
							if(cur_mute[nc]<1) cur_mute[nc] ++;
							else { 
								cur_mute[nc] = 0;
								nc++;
							}
						}
					}
					


				} while (nc < 32);

tact_complete:
				printf("%s\n", buf);
			}
		}
	}
	printf("total = %ld\n", cnt );

	return 0;
}