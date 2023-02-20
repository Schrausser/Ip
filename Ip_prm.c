//-----------------------------------------------------------------------------| Ip_prm von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include <conio.h>

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

char dbuffer [9],tbuffer [9], datenprm_dateiname[50], arg_[1000], arg; 
int  logcode, zeichen_bis_punkt, iLauf; 
	
main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE *inStream, *outStream, *logStream, *batStream; //--------------------> streams

	double	nA=0,//---------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kn=0, kA;//-----------------------------------------------------------> Datenmatrix Spaltenvektoren k

	if (argc == 1 ) _profil();//----------------------------------------------> Ip_prm profil
	if (argc != 2) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (fopen (argv[1], "r") == NULL ) {printf("\n[1] Datenmatrixdatei.0\n");_profil();}//fehlerroutine inputdatei
	
	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.
	
	//-------------------------------------------------------------------------| einlesen von aij nach AV_exp
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)
	while(1)
	{
		fgets (arg_,1000,inStream );//----------------------------------------> zeile einlesen
		if (feof (inStream) != 0 ) break;
		nA++; //--------------------------------------------------------------> n zähler	
	}
	fclose(inStream);
	
	if (nA == 0){printf("\n[1] n=0 .0\n");_profil();}//-----------------------> fehlerroutine n=0
	
	inStream = fopen( argv[1], "r" );//--------------------------------------> definition von datenmatrixdatei A(n,k)
	while(1)
	{
		arg = fgetc (inStream );//--------------------------------------------> zeichen einlesen
		if (feof (inStream) != 0 ) break;
		if(arg == ',') {printf("\n[1] Kommaformat.0\n");_profil();}//---------> fehlerroutine komma
	}
	fclose(inStream);
	
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)
	while(1)
	{
		fscanf (inStream,"%s",arg_ );//---------------------------------------> wert einlesen
		if (feof (inStream) != 0 ) break;
		kn++; //--------------------------------------------------------------> kn zähler	
	}	
	fclose(inStream);

	kA = kn / nA;//-----------------------------------------------------------> k berechnung
	if (kA != floor(kA)){ printf("\n[1] Datenmatrixdatei Format.0\n"); _profil();}// datenmatrixformat (letzten zeilenvektor beachten) 
	
	_kopfzeile();
	outStream = fopen( datenprm_dateiname, "w" );
	fprintf(outStream, "%.0f\n%.0f", nA, kA);//-------------------------------> parameterdateiausgabe
	fclose(outStream);

	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_prm_log.txt", "w" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_prm_ %s %s:\n--------------------------\n", dbuffer, tbuffer);
	fprintf (logStream, "%s, n= %.0f, k= %.0f, kn= %.0f", argv[1], nA, kA, kn );

	fclose( logStream  );

	system("Ip_prm_log.txt");//---------------------------------------------> editieren der logdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_prm_log_%i\n",logcode );
	fprintf (batStream, "move Ip_prm_log.txt Ip_prm_log_%i\n", logcode);
	fprintf (batStream, "move %s Ip_prm_log_%i\n", datenprm_dateiname, logcode);
	fprintf (batStream, "xcopy %s Ip_prm_log_%i>nil\n", argv[1], logcode);

	fclose( batStream  );
	
	system ("verschub.bat");
	system ("del verschub.bat");
	system ("del nil");

	printf("%i      \n",logcode);
	
	_fusszeile();

	return 0;
}

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_prm [1]\n");
	for(iLauf=1; iLauf<=41; iLauf++) cprintf("\xC4"); printf("\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	for(iLauf=1; iLauf<=41; iLauf++) cprintf("\xC4"); printf("\n");
	printf("Ip_prm von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_prm_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
