// frameprovider.cc 
//

#include "copyright.h"
#include "system.h"
#include "frameprovider.h"

//----------------------------------------------------------------------
// FrameProvider::FrameProvider
//		Création d'un FrameProvider.
//
//----------------------------------------------------------------------

FrameProvider::FrameProvider(int frames)
{
	numFrames = numFrames;
	this->bitmap = new BitMap(frames);
}

//----------------------------------------------------------------------
// FrameProvider::~FrameProvider
//		Libération des ressources.
//
//----------------------------------------------------------------------

FrameProvider::~FrameProvider() {
	delete this->bitmap;
}

//----------------------------------------------------------------------
// FrameProvider::GetEmptyFrame
//		Récupère un cadre vide, le considère comme utilisé et mets les
//			bits de la page physique correspondante à 0
//		Retourne le numéro du cadre si succès, -1 sinon.
//
//----------------------------------------------------------------------

	int
FrameProvider::GetEmptyFrame ()
{
	int free_frame = this->bitmap->Find();
	if (free_frame >= 0)
		bzero(&(machine->mainMemory[free_frame*PageSize]), PageSize);
	return free_frame;
}

//----------------------------------------------------------------------
// FrameProvider::ReleaseFrame
//		Libère le cadre ayant pour identifiant frame_id
//
//----------------------------------------------------------------------

	void
FrameProvider::ReleaseFrame (int frame_id)
{
	this->bitmap->Clear(frame_id);
}

//----------------------------------------------------------------------
// FrameProvider::NumAvailFrame
//		Retourne le nombre de cadres vides
//
//----------------------------------------------------------------------

	int
FrameProvider::NumAvailFrame ()
{
	return this->bitmap->NumClear();
}

