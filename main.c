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
#define FRAC_MAX	5		
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
	int i1, j1, k1;
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
			memset(buf,0,MAX_LEN);
			cur_note = i;
			tact = 1.0/(1<<k);
			sprintf(buf,"%s%s[1/%d]", buf, note[i], 1<<k );
			// Add other notes if the duration is < 1
			for (i1 = NEG_INT; i1< POS_INT; i1++ ) {
				for (k1=0; k1<FRAC_MAX; k1++){
					//printf("i1 = %d, k1 = %d\n", i1, k1);
					// генерируем ноту исходя из текущего значения интервала для каждой ноты. Интервал для первой ноты уже задан
					// иначе добавляем ноту.
					test_note = cur_note + i1;
					//printf("test_note = %d, ", test_note);
					if (test_note>=0 && test_note<TOT_NOTES){
						// Попали в диапазон 88 нот. Теперь нужно посчитать длительность интервала, который остался 
						//до конца такта.
						test_dur_tact = tact + 1.0/(1<<k1);
						//printf("test_dur = %lf\n", test_dur_tact);
						if (test_dur_tact < 1.0001) {
							// попали в длительность такта.
							// можно добавлять ноту в буфер

							sprintf(buf, "%s,", buf); // разделитель

							cur_note = test_note;
							tact = test_dur_tact;
							sprintf(buf,"%s%s[1/%d]", buf, note[cur_note], 1<<k1 );
							//if (tact >= 1.0) { cnt ++;  goto tact_complete;}
						}
					}						
					if (tact > 0.999999 && tact < 1.000001) { 
						printf("%s\n", buf);
						cnt ++;  
						goto tact_complete;
					}		
					
				}
			}
tact_complete:
			;;
			
		}
	}
	printf("total = %ld\n", cnt );

	return 0;
}