//-----------------------------------------------------------------------------| Ip_GEN von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include "Ip_CALC.h"

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

int seed; 
float fn_[20];//--------------------------------------------------------------> f(x) beta gewichte  
double  n_i = 0;//------------------------------------------------------------> zufallsvektorelement ni                      

int iLauf, jLauf, logcode, ordnung;//-----------------------------------------> laufindex i
char dbuffer [9],tbuffer [9], null[50]; //------------------------------------> buffer fuer time und date

main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv     
{   
	FILE *fnStream, *outStream, *logStream, *protStream, *batStream; //-------> streams                                        
	
	if (argc == 1 ) //--------------------------------------------------------> Ip_GEN profil
	{
   		 _profil();
	}

	if (argc != 6) //---------------------------------------------------------> argumente 
	{
   		printf("\n[ ] Argumente.0\n"); _profil();
	}
	
	if ( atoi(argv[1]) == 0 ) //----------------------------------------------> format
	{
   			printf("\n[1] Format.0\n");_profil();
	}
	
	if ( atoi(argv[2]) == 0 && stricmp(argv[2], "0") != 0) 
	{
   			printf("\n[2] Format.0\n");_profil();
	}
	
	if ( atoi(argv[3]) >> 1) 
	{
   			printf("\n[3] Format.0\n");_profil();
	}

	if ( atoi(argv[3]) == 0 && stricmp(argv[3], "0") != 0) 
	{
   			printf("\n[3] Format.0\n");_profil();
	}
	
	if ( atoi(argv[4]) == 0 && stricmp(argv[4], "0") != 0)
	{
   			printf("\n[4] Format.0\n");_profil();
	}

	if ( atoi(argv[5]) == 0 && stricmp(argv[5], "0") != 0) 
	{
   			printf("\n[5] Format.0\n");_profil();
	}
	
	if ( atoi(argv[4]) == atoi(argv[5])) 
	{
   			printf("\n[4][5] min = max .0\n");_profil();
	}

	if ( atoi(argv[4]) > atoi(argv[5])) 
	{
   			printf("\n[4][5] min > max .0\n");_profil();
	}
   
	//-------------------------------------------------------------------------| definition von outputdateien                 
	outStream = fopen( "Ip_GEN_dat.asc", "w" );
	logStream = fopen( "Ip_GEN_log.txt", "a" );
	protStream = fopen( "Ip_GEN_prot.rtf", "a" );


	//------------------------------------------------------------------------> f(x) parameter einlesen
	if(atoi(argv[3]) == 0)
	{
		if (fopen ("Ip_GEN_fn.asc", "r") == NULL) //--------------------------> fehlt Ip_GEN_fn.asc wird eine Vorlage erstellt.
		{
			fnStream = fopen ("Ip_GEN_fn.asc", "w");
		
			fprintf( fnStream,"0\t\t 0_ordnung+1\n"); 
			fprintf( fnStream,"0\t\t 1_b0\n"); 
			fprintf( fnStream,"0\t\t 2_b1\n"); 
			fprintf( fnStream,"0\t\t 3_b2\n"); 
			fprintf( fnStream,"0\t\t 4_b3\n"); 
			fprintf( fnStream,"0\t\t 5_b4\n"); 
			fprintf( fnStream,"0\t\t 6_b5\n"); 
			fprintf( fnStream,"0\t\t 7_b6\n"); 
			fprintf( fnStream,"0\t\t 8_b7\n"); 
			fprintf( fnStream,"0\t\t 9_b8\n"); 
			fprintf( fnStream,"0\t\t10_b9\n"); 
			fprintf( fnStream,"0\t\t11_b10\n");
			fprintf( fnStream,"0\t\t12_b11\n");
			fprintf( fnStream,"0\t\t13_b12\n");
				
			fclose( fnStream );
			system("Ip_GEN_fn.asc");
		}
		fnStream = fopen( "Ip_GEN_fn.asc", "r" );
		
		fscanf(fnStream,"%i\t\t%s\n", &ordnung, null);
		
		for (iLauf = 1; iLauf <= ordnung; iLauf++)
		{
			fscanf(fnStream,"%f\t\t%s\n", &fn_[iLauf], null ); 

			//printf("%f\n", fn_[iLauf]);
		}
		fclose( fnStream  );
	}
   
	_kopfzeile();
   
	//-------------------------------------------------------------------------| Prozedur                                       
	
	if(atoi(argv[2]) != 0)	{ seed=atoi(argv[2]); }//-------------------------> anfangswert = argument2
	if(atoi(argv[2]) == 0)	{ seed=(time(0)-1234567890); } //-----------------> anfangswert = sec seit 13.02.2009 (= 1234567890sec seit time(0), 01.01.1970)

	qzufall(seed);

	for (iLauf = 0; iLauf < atoi(argv[1]); iLauf++)
	{
		if(atoi(argv[3]) == 1) //---------------------------------------------> Gvt
		{
			n_i = atoi(argv[4]) + (atoi(argv[5])-atoi(argv[4]))*quasizufallszahl;
		}

		if(atoi(argv[3]) == 0) //---------------------------------------------> f(x=ni)
		{
			for (jLauf = 1; jLauf <= ordnung; jLauf++)
			{
				n_i += fn_[jLauf]*pow(atoi(argv[4]) + (atoi(argv[5])-atoi(argv[4]))*quasizufallszahl,jLauf-1);// funktionsberechnung
			}
		}

		fprintf (outStream,"%.3f\n", n_i); //-----------------------------------> schreibt zufallszahl auf 3 kommastellen gerundet in outputdatei

		printf("%i\r", iLauf+1  );//------------------------------------------> bildschirmausgabe
	
		qzufall(quasizufallszahl*10);//---------------------------------------> resultierende Zufallszahl * 10 (nachkommaverschiebung)
		
		n_i = 0;
	}

	//-------------------------------------------------------------------------| Log          
	_strdate( dbuffer );    
	_strtime( tbuffer );

	
	fprintf (logStream,"Ip_PSG_ %s %s: \nZufallsvektor n(n)\n-------------------------------\n", dbuffer, tbuffer);

	fprintf (logStream,"Gvt[%s,%s], n=%s, seed=%i\n", argv[4], argv[5], argv[1], seed);

	if(atoi(argv[3]) == 0)//--------------------------------------------------> f(x)
	{
		fprintf (logStream,"f(x=n): ");
		
		if(fn_[1] != 0)//-----------------------------------------------------> funktionsgleichungsausgabe b0
		{
			fprintf (logStream,"%.3f +", fn_[1]);	
		}
	
		if(fn_[2] != 0)//-----------------------------------------------------> funktionsgleichungsausgabe b1	
		{
			if(fn_[2] != 1)
			{	
				fprintf (logStream," %.3f x ", fn_[2]);
			}

			if(fn_[2] == 1)
			{	
				fprintf (logStream," x ");
			}

		}

		for (jLauf = 3; jLauf <= ordnung; jLauf++)//--------------------------> funktionsgleichungsausgabe bj
		{
			if(fn_[jLauf] != 0)
			{
				if(fn_[jLauf] != 1)
				{
					fprintf (logStream,"+ %.3f x%i ", fn_[jLauf], jLauf-1);
				}

				if(fn_[jLauf] == 1)
				{
					fprintf (logStream,"+ x%i ", jLauf-1);
				}
				
			}
		}
	}//if(atoi(argv[3]) == 0) end
	
	//-------------------------------------------------------------------------| Protokoll 
	if(atoi(argv[3]) == 1)//--------------------------------------------------> Gvt
	{
		fprintf (protStream,"Ein Zufallsvektor n in Gleichverteilung zwischen nmin = %s und nmax = %s, ", argv[4], argv[5]);
		fprintf (protStream,"Gvt[%s, %s] wurde im Umfang von n = %s Elementen generiert. ", argv[4], argv[5], argv[1]);
	}
	
	if(atoi(argv[3]) == 0)//--------------------------------------------------> f(x)
	{
		fprintf (protStream,"Ein Zufallsvektor n zur Funktion nach f(x=n) = ");
	
		if(fn_[1] != 0)//-----------------------------------------------------> funktionsgleichungsausgabe b0
		{
			fprintf (protStream,"%.3f +", fn_[1]);	
		}
	
		if(fn_[2] != 0)//-----------------------------------------------------> funktionsgleichungsausgabe b1	
		{
			if(fn_[2] != 1)
			{	
				fprintf (protStream," %.3f x ", fn_[2]);
			}

			if(fn_[2] == 1)
			{	
				fprintf (protStream," x ");
			}

		}

		for (jLauf = 3; jLauf <= ordnung; jLauf++)//--------------------------> funktionsgleichungsausgabe bj
		{
			if(fn_[jLauf] != 0)
			{
				if(fn_[jLauf] != 1)
				{
					fprintf (protStream,"+ %.3f x%i ", fn_[jLauf], jLauf-1);
				}

				if(fn_[jLauf] == 1)
				{
					fprintf (protStream,"+ x%i ", jLauf-1);
				}
				
			}
		}
	
		fprintf (protStream,"wurde im Umfang von n = %s Elementen generiert. ", argv[1]);
	}//if(atoi(argv[3]) == 0) end
	
	fprintf (protStream,"Anfangswert (Seed) für die Zufallsberechnung nach Gl.(*) war Seed = %i.\n", seed);
	
	fclose( outStream  );
	fclose( logStream );
	fclose( protStream );

	//system("Ip_GEN_dat.asc");//---------------------------------------------> editieren der outputdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_GEN_log_%i\n",logcode );
	fprintf (batStream, "move Ip_GEN_dat.asc Ip_GEN_log_%i\n", logcode);
	fprintf (batStream, "move Ip_GEN_log.txt Ip_GEN_log_%i\n", logcode);
	fprintf (batStream, "move Ip_GEN_prot.rtf Ip_GEN_log_%i\n", logcode);
	fclose( batStream  );
	
	system ("verschub.bat");
	system ("del verschub.bat");

	printf("%i      \n",logcode);

    _fusszeile();
	
	return 0;
}

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_GEN [1]_[2]_[3]_[4]_[5]\n");
	printf("----------------------------------\n");
	printf("[1] Elemente n im Zufallsvekor n\n");
	printf("[2] (*)|Anfangswert (0)|Zeitwert\n");
	printf("[3] (1)|Gvt (0)|f(x) Ip_GEN_fn.asc\n");
	printf("[4] y Minimalwert\n");
	printf("[5] y Maximalwert\n");
	printf("----------------------------------\n");
	printf("Ip_GEN von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_GEN_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE                                           