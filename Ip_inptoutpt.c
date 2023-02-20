//---------------------------------------------------------------------------| Ip_inptoutpt by Dietmar SCHRAUSSER 2009               

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Ip_calc.h"

void _err_msg (); //---------------------------------------------------------> prozedur fehlermeldung und usage instruktion   
void _head(void); //---------------------------------------------------------> prozedur für kopfzeilen                        

char dbuffer [9],tbuffer [9], datei_in_zeichen, datei_out_zeichen; //---------------------------> buffer fuer time und date, variable f. dateizeichenm
main(int argc, char *argv[]) //----------------------------------------------> main übernimmt n argumente im vektor argv     
{   
	
	
	FILE *inStream, *outStream; //---------------------------------------> streams  
	
	
      
	if (argc != 3) //----------------------------------------------------> was wenn keine oder zuwenig, zuviel argumente? 
	{
   		printf("ERROR, check arguments!\n");
   		_err_msg(argv[0]);
	}
   
	//-------------------------------------------------------------------| def von input und outputdatei:                 
	inStream = fopen( argv[1], "r" );
	outStream = fopen( argv[2], "w" );                                                                                  
   
	if (inStream == NULL) //---------------------------------------------> was wenn keine inputdatei                      
	{
		printf("ERROR, check file %s!\n", argv[1]);
		_err_msg(argv[0]);
	}
  
	_head();



   
	//-------------------------------------------------------------------| Prozedur 
	
	//*input-output--------------------------------------------> via feof funktion mit break
	while(1)
	{
	datei_in_zeichen = fgetc(inStream);//--------------------------------> zeichen lesen von inputdatei
	if (feof (inStream) != 0 ) break;//---------------------------------> bis feof(instream) = 1
	datei_out_zeichen = datei_in_zeichen;//------------------------------> ggf zeichenmanipulationen
	fputc (datei_out_zeichen, outStream);//------------------------------> zeichen in outputdatei schreiben 
	}//*/	
 
	fclose( inStream  );
	fclose( outStream );

	system(argv[2]); //--- editieren von outputdatei
 
	return 0;
}

//---------------------------------------------------------------------------| allgemeine fehlerroutine                       
void _err_msg()
{
	printf("---------------------------------------------------------\n");
	printf("Usage: Ip_inptoutpt [input] [output] \n ");
	printf("[input] ... Eingabe Datei \n");
	printf(" [output] .. Ausgabe Datei \n");
	printf("---------------------------------------------------------\n");
	printf("Ip_inptoutpt by Dietmar Schrausser\n");
        printf("compiled on %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//---------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _head(void) 
{
	printf("\nIp_inptoutpt by Dietmar Schrausser\n");
	printf("compiled on %s @ %s\n", __DATE__, __TIME__);
	printf("computing Ip_inptoutpt:"); 
}

//---------------------------------------------------------------------------| ENDE                                           