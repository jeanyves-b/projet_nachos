// frameprovider.h 
//	


#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"

// The following class defines 

class FrameProvider {
	public:
		FrameProvider(int n);	
		~FrameProvider();			// De-allocate the data structures

		int GetEmptyFrame();	// récupère un cadre vide et le 
		// considère comme utilisé
		void ReleaseFrame(int n); // libère un cadre
		int NumAvailFrame(); // retourne le nombre de cadres libres
		Semaphore* s;

	private:
		BitMap *bitmap;
		int numFrames;
};

#endif // FRAMEPROVIDER_H
