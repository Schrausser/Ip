//-----------------------------------------------------------------------------| Ip_Pmn von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

char dbuffer [9],tbuffer [9], datenprm_dateiname[50]; //----------------------> buffer fuer time und date
int  logcode, zeichen_bis_punkt;

float s_ij;

double 	AV_exp[40],//---------------------------------------------------------> Urdatenvektor
		P_AV_exp[30],//-------------------------------------------------------> Permutationsdatenvektor p1
		SP_AV_exp[30];//------------------------------------------------------> Permutationsdatenvektor p2

main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE *inStream, *inprmStream, *outStream, *logStream, *batStream; //------> streams

	int jLauf, iLauf,  
		nA,//-----------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kkA,//----------------------------------------------------------------> Datenmatrix Spaltenvektoren k
		kA=0,//---------------------------------------------------------------> Datensubvektorlänge p1 
		jp2,//----------------------------------------------------------------> Index j zu Subvektorelement p2
		index[21],//----------------------------------------------------------> Schablonenindex Vektor
		zlr=1,//--------------------------------------------------------------> Durchgangszähler 
		sb_=0,//--------------------------------------------------------------> Schablonenverschubindex
		wtr=0,
		ft_n=1, ft_p1=1, ft_p2=1, P_;

	if (argc == 1 ) _profil();//----------------------------------------------> Ip_Pmn profil
	if (argc != 5) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (atoi(argv[2]) == 0 ){ printf("\n[2] Format.0\n");_profil();}//--------> format
	if (atoi(argv[3]) == 0 ){ printf("\n[3] Format.0\n");_profil();}					 
	if (atoi(argv[2]) == atoi(argv[3])) {printf("\n[2][3] j1=j2 .0\n");_profil();}
	if (atoi(argv[4]) == 0 || atoi(argv[4]) > 2) {printf("\n[4] Format.0\n");_profil();}

	if (fopen (argv[1], "r") == NULL  ) //------------------------------------> fehlerroutine inputdatei
	{
   		inStream = fopen( argv[1], "w" );
		
		fprintf( inStream,"1\t1\n"); 
		fprintf( inStream,"2\t1\n"); 
		fprintf( inStream,"3\t1\n"); 
		fprintf( inStream,"4\t1\n"); 
		fprintf( inStream,"5\t2\n");
		fprintf( inStream,"6\t2\n");
		fprintf( inStream,"7\t2\n");
		fprintf( inStream,"8\t2\n");
		fprintf( inStream,"9\t2\n");

		fclose( inStream  );
		system (argv[1]);
		printf("%s\n", argv[1] );
		
	}

	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.

	if ( fopen (datenprm_dateiname, "r") == NULL  ) //------------------------> fehlerroutine datenparameterdatei 
	{
   		inprmStream = fopen( datenprm_dateiname, "w" );

		fprintf( inprmStream,"9\n"); 
		fprintf( inprmStream,"2\n");

		fclose( inprmStream  );
		system (datenprm_dateiname);
		printf("%s\n", datenprm_dateiname);
	}

	//-------------------------------------------------------------------------| einlesen der datenmatrix parameter n, k 
	inprmStream = fopen( datenprm_dateiname, "r" );
	fscanf(inprmStream,"%i\n%i", &nA, &kkA ); 
	fclose( inprmStream  );	

	if ( atoi(argv[2]) >> kkA || atoi(argv[3]) >> kkA ) //--------------------> fehlerroutine jv > kX 
	{
   			printf("\n[2][3] k<j .0\n");_profil();
	}

	if (nA > 20) {printf("\n[1] Rechenumfang.0\n");_profil();}//--------------> Rechenumfang

	

	//-------------------------------------------------------------------------| einlesen von aij nach AV_exp
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)

	for (iLauf = 0; iLauf < nA; iLauf++)
	{
		for (jLauf = 1; jLauf <= kkA; jLauf++)
		{	
			fscanf(inStream,"%f", &s_ij );

			if (jLauf == atoi(argv[2]))	AV_exp[iLauf+1] = s_ij;//-------------> Urdatenvektor Initialisierung
			
			if (jLauf == atoi(argv[3]))	{ 	
			
				if (s_ij < 1 || s_ij > 2)//-----------------------------------> aij nach si2 kompatibilitätsprüfung
				{										
					printf("\n[1] Wertebereich.0\n"); _profil();
				}
				if (s_ij ==1) kA++;//-----------------------------------------> kA = p1 Zähler
			}
		}
	}

	fclose( inStream  );

	if (kA > nA-kA) kA=nA-kA;//-----------------------------------------------> p1<=p2

	for (jLauf=1; jLauf<=20; jLauf++) index[jLauf]=0; //----------------------> Index Initialisierung (zu beachten: Vrn=1/Pmn,Kmn=0 )

	outStream = fopen( "Ip_Pmn.asc", "w" );

	//------------------------------------------------------------------------> Berechnung P=n!/(p1!*p2!)
	for (iLauf=1; iLauf<=nA; iLauf++)		ft_n *=iLauf;//-------------------> n!
	for (iLauf=1; iLauf<=kA; iLauf++)		ft_p1 *=iLauf;//------------------> p1!
	for (iLauf=1; iLauf<=nA-kA; iLauf++)	ft_p2 *=iLauf;//------------------> p2!

	P_= ft_n/(ft_p1*ft_p2);

	

	_kopfzeile();

	//-------------------------------------------------------------------------| Vollständige Permutation (P) bei np=2, P=n!/(p1!*p2!), 
	//                                                                           entsprechend der Kombination K=n!/(k!*(n-k)!), wobei k=p1.
	do 
	{	
		//--------------------------------------------------------------------> Schablonenindexgenerierung
		spr:
		for (jLauf=1; jLauf<=nA-kA+1; jLauf++)
		{				
			if(jLauf == 1) index[1]++;
			if(index[jLauf] == nA-kA+2) 
			{ 
				index[jLauf]    = 1;
				index[jLauf+1] += 1;	
			}
			if( index[jLauf-1] < index[jLauf]) goto spr;//--------------------> Rangindikator
		}
		//--------------------------------------------------------------------> Schablonenverschub
		while( index[1]-1 + sb_ <= nA-kA )
		{
			//----------------------------------------------------------------> Datenzuordnung p1
			for (iLauf=1; iLauf<=kA; iLauf++)
			{
				if( index[iLauf]==0) index[iLauf]=1;
				P_AV_exp[iLauf] = AV_exp[(kA+(index[iLauf]-1+sb_))-iLauf+1];
				
				if (atoi(argv[4]) == 1) fprintf(outStream, "%.0f ", P_AV_exp[iLauf]);// Datenausgabe p1 i
				if (atoi(argv[4]) == 2) fprintf(outStream, "%.3f ", P_AV_exp[iLauf]);// Datenausgabe p2 f
			}
			fprintf(outStream, "\t");//---------------------------------------> Tabulator

			//----------------------------------------------------------------> Vektorspiegelung p2
			jp2=1;
			for (iLauf=1; iLauf<= nA; iLauf++)
			{
				for (jLauf=1; jLauf<= kA; jLauf++)
				{
					if(P_AV_exp[jLauf] == AV_exp[iLauf]) wtr = 1;//-----------> Vektorelementvergleich
				}
				if(wtr == 0) { SP_AV_exp[jp2] = AV_exp[iLauf]; jp2++; }

				wtr=0;
			}
			if (atoi(argv[4]) == 1)//-----------------------------------------> Datenausgabe p2 i
			{
				for (iLauf=1; iLauf<= nA-kA; iLauf++) fprintf(outStream, "%.0f ", SP_AV_exp[iLauf]);
			}
			if (atoi(argv[4]) == 2)//-----------------------------------------> Datenausgabe p2 f
			{
				for (iLauf=1; iLauf<= nA-kA; iLauf++) fprintf(outStream, "%.3f ", SP_AV_exp[iLauf]);
			}
			
			fprintf (outStream,"\t%i\n",zlr); //------------------------------> Dateiausgabe Durchgangszähler
			printf ("%i\r",zlr); //-------------------------------------------> Bildschirmausgabe Durchgangszähler

			zlr++; sb_++;	
		}
		sb_ = 0;
		
	} while (index[kA-1] <= nA-kA);//do end


	fclose( outStream  );
    
	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_Pmn_log.txt", "w" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_Pmn_ %s %s:\n--------------------------\n", dbuffer, tbuffer);
	fprintf (logStream, "P= n!/(p1!p2!), n= %i, p1= %i, p2= %i\n\n", nA, kA, nA-kA);
	fprintf (logStream, "P= %i", P_);

	fclose( logStream  );

	system("Ip_Pmn_log.txt");//-----------------------------------------------> editieren der logdatei
	if(nA<10) system("Ip_Pmn.asc");//-----------------------------------------> editieren der outputdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_Pmn_log_%i\n",logcode );
	fprintf (batStream, "move Ip_Pmn_log.txt Ip_Pmn_log_%i\n", logcode);
	fprintf (batStream, "move Ip_Pmn.asc Ip_Pmn_log_%i\n", logcode);

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
	printf("Ip_Pmn [1]_[2]_[3]_[4]\n");
	printf("-----------------------------------------\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a1, AV\n");
	printf("[3] Index j zu Datenvektor a2, UV\n");
	printf("[4] Datenformat 1|10 2|10.000\n");
	printf("-----------------------------------------\n");
	printf("Ip_Pmn von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_Pmn_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
