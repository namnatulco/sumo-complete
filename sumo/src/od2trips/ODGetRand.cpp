/***************************************************************************
                         Get_rand.cpp

	 This subroutine returns <number> of random elements from
	 input vector input

			 usage		 : Get_rand(outputnumber, size, input, output, ini) 
			 outputnumber: number of randoms to return from input (int)
			 size		 : total number of input numbers to chose from (int)
			 input		 : input numbers to chose from (int)
			 output		 : random output numbers (int)
			 init		 : initialize random generator with time (true/false)
                             -------------------
    project              : SUMO		 : 
	subproject           : OD2TRIPS
    begin                : Thu, 12 September 2002
    copyright            : (C) 2002 by DLR/IVF http://ivf.dlr.de/
    author               : Peter Mieth
    email                : Peter.Mieth@dlr.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

# include <cstdlib>
# include <ctime>
# include <cmath>
# include <iostream>
# include <sstream>
# include <fstream>
# include <vector>
# include <cstdlib>
# include <ctime>
# include <cmath>
# include <string>
# include <utils/common/UtilExceptions.h>
# include "ODmatrix.h"  

using namespace std;

int Get_rand (unsigned short number,int maxele, int *input, int *output, bool ini) 
	{
	int i, check, newele, index;
	double rand_space;  

 
	if(number>=maxele) 
	{    
		cout << "More random numbers requiered as elements!" << endl;
        number = maxele-1;
	}
	if(number==0) 
	{    
		cout << "No elements requiered, nothing to do" << endl;
		throw ProcessError();
	}		
    for(int k=0;k<maxele;k++) {
        *(input+k)=k;
    }
	check=0;
	newele=maxele; 
    if(ini) srand(time(0));
	while (check < number) 
	{      
		rand_space = 1. * (newele-1) / RAND_MAX;
		index = (int) (rand() * rand_space);
		if(index==maxele) index--;
		*(output+check) = *(input+index);
		++check;
		for (i=index;i<newele-1;i++) 
			{
				*(input+i)=*(input+i+1);
				if(	*(input+i)<0) {
					cout << "was faul at "<< i << endl;
					throw ProcessError();
				}
			}
		--newele;
		
	}
	return (number);
}
	
