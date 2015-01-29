// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk 
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"

// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known 
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number 
// of files that can be loaded onto the disk.
#define FreeMapFileSize 	(NumSectors / BitsInByte)
#define NumDirEntries 		10
#define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)
#define openFilesNum			10

//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).  
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{ 
	DEBUG('f', "Initializing the file system.\n");
	sem = new Semaphore("sem",1);
	lock = new Semaphore("lock",1);
	if (format) {
		BitMap *freeMap = new BitMap(NumSectors);
		Directory *directory = new Directory(NumDirEntries);
		FileHeader *mapHdr = new FileHeader;
		FileHeader *dirHdr = new FileHeader;

		DEBUG('f', "Formatting the file system.\n");

		// First, allocate space for FileHeaders for the directory and bitmap
		// (make sure no one else grabs these!)
		freeMap->Mark(FreeMapSector);	    
		freeMap->Mark(DirectorySector);

		// Second, allocate space for the data blocks containing the contents
		// of the directory and bitmap files.  There better be enough space!

		ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
		ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

		// Flush the bitmap and directory FileHeaders back to disk
		// We need to do this before we can "Open" the file, since open
		// reads the file header off of disk (and currently the disk has garbage
		// on it!).

		DEBUG('f', "Writing headers back to disk.\n");
		mapHdr->WriteBack(FreeMapSector);    
		dirHdr->WriteBack(DirectorySector);

		// OK to open the bitmap and directory files now
		// The file system operations assume these two files are left open
		// while Nachos is running.

		freeMapFile = new OpenFile(FreeMapSector);
		directoryFile = new OpenFile(DirectorySector);
		
		//Now we add the directory '.' to the root directory
		directory->AddDir(".",DirectorySector);
		directory->AddDir("..",DirectorySector);
		// Once we have the files "open", we can write the initial version
		// of each file back to disk.  The directory at this point is completely
		// empty; but the bitmap has been changed to reflect the fact that
		// sectors on the disk have been allocated for the file headers and
		// to hold the file data for the directory and bitmap.
		
		DEBUG('f', "Writing bitmap and directory back to disk.\n");
		freeMap->WriteBack(freeMapFile);	 // flush changes to disk
		directory->WriteBack(directoryFile);

		if (DebugIsEnabled('f')) {
			freeMap->Print();
			directory->Print();
		}
			delete freeMap; 
			delete directory; 
			delete mapHdr; 
			delete dirHdr;
		currentDir = directoryFile;
		CreateDir("System");
		
	} else {
		// if we are not formatting the disk, just open the files representing
		// the bitmap and directory; these are left open while Nachos is running
		freeMapFile = new OpenFile(FreeMapSector);
		directoryFile = new OpenFile(DirectorySector);
		currentDir = directoryFile;
				
	}
	int i;
	openFileTable = new FileSysEntry[maxOpenFiles];
	for(i=0;i<maxOpenFiles;i++){
		openFileTable[i].used = false;
		openFileTable[i].file = NULL;
		openFileTable[i].count=0;
	}
	
}

FileSystem::~FileSystem(){
  delete freeMapFile;
   if (currentDir != directoryFile){
     delete currentDir;
   }
  delete directoryFile;
  delete sem;
  delete lock;
  delete openFileTable;

}

OpenFile* 
FileSystem::MoveTo(const char* name,char* s){
  OpenFile* f;
   int sector;
  int cpt =0;
  int nb = 0;
  char buf[FileNameMaxLen+1];
  if (name[0] == '/'){
    f = directoryFile;
    cpt++;
  }else{
    f = currentDir;
  }
  Directory* dir = new Directory(NumDirEntries);
  dir->FetchFrom(f);
 
  while (name[cpt] != '\n' && name[cpt]!='\0'){
    if (name[cpt] == '/'){
     buf[nb] = '\0';
     sector = dir->FindDir(buf);//on cherche le nom du repertoire
     if (f != currentDir)
       delete f;
     if(sector == -1)
       return NULL;
     f = new OpenFile(sector); 
     dir->FetchFrom(f);
     nb = 0;
     cpt++;
    }else{
      buf[nb++] = name[cpt++];
    }
  }
  buf[nb]='\0';
  if (s != NULL){
    nb = 0;
    while (buf[nb]!='\0'){
      s[nb] = buf[nb];
      nb++;
    }
    s[nb] = '\0';
  }
  delete dir;
  return f;
} 
      

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk 
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file 
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

	bool
FileSystem::Create(const char *name, int initialSize)
{
	Directory* dir;
	BitMap *freeMap;
	FileHeader *hdr;
	OpenFile* f;
	int sector;
	bool success;
	char filename[FileNameMaxLen+1];
	f = MoveTo(name,filename);
	if (f == NULL)
	  return FALSE;
	
	DEBUG('f', "Creating file %s, size %d\n", name, initialSize);
	dir = new Directory(NumDirEntries);
	dir->FetchFrom(f);
	if (dir->Find(name) != -1)
	success = FALSE;
	else {	
		freeMap = new BitMap(NumSectors);
		freeMap->FetchFrom(freeMapFile);

		sector = freeMap->Find();	// find a sector to hold the file header
		if (sector == -1) 		
			success = FALSE;		// no free block for file header 
		else if (!dir->Add(filename, sector))
			success = FALSE;	// no space in directory
		else {
			hdr = new FileHeader;
			sem->P();
			if (!hdr->Allocate(freeMap, initialSize)){
				sem->V();
				success = FALSE;	// no space on disk for data
			}else {	
				sem->V();
				success = TRUE;
				// everthing worked, flush all changes back to disk
				hdr->WriteBack(sector); 		
				dir->WriteBack(f);
				freeMap->WriteBack(freeMapFile);
			}
			delete hdr;
		}
		delete freeMap;
	}
	delete dir;
	if (f!=currentDir && f != directoryFile)
	  delete f;
	return success;
}


	bool
FileSystem::CreateDir(const char *name)
{
	Directory* dir;
	BitMap *freeMap;
	FileHeader *hdr;
	OpenFile* f;
	int sector;
	bool success;
	char filename[FileNameMaxLen+1];
	f = MoveTo(name,filename);
	if (f == NULL)
	  return FALSE;
	
	DEBUG('f', "Creating Directory %s\n", name);

	dir = new Directory(NumDirEntries);
	dir->FetchFrom(f);
	if (dir->Find(name) != -1)
		success = FALSE;			// file is already in directory
	else {	
		freeMap = new BitMap(NumSectors);
		freeMap->FetchFrom(freeMapFile);
		sector = freeMap->Find();	// find a sector to hold the file header
		if (sector == -1) 		
			success = FALSE;		// no free block for file header 
		else if (!dir->AddDir(filename, sector))
			success = FALSE;	// no space in directory
		else {
			hdr = new FileHeader;
			sem->P();
			if (!hdr->Allocate(freeMap, DirectoryFileSize)){
				sem->V();
				success = FALSE;	// no space on disk for data
			}else {
				sem->V();	
				int parentSector = dir->Find(".");
				success = TRUE;
				// everthing worked, flush all changes back to disk
				hdr->WriteBack(sector); 		
				dir->WriteBack(f);
				freeMap->WriteBack(freeMapFile);
				InitializeDir(sector,parentSector);
			}
			delete hdr;
		}
		delete freeMap;
	}
	delete dir;
	if (f!=currentDir && f != directoryFile)
	  delete f;
	return success;
}

void
FileSystem::InitializeDir(int childSector,int parentSector){
  OpenFile* file = new OpenFile(childSector);
  Directory* dir = new Directory(NumDirEntries);
  ASSERT(NumDirEntries >= 2);
  dir->AddDir(".", childSector);
  dir->AddDir("..",parentSector);
  
  dir->WriteBack(file);
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.  
//	To open a file:
//	  Find the location of the file's header, using the directory 
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

	OpenFile *
FileSystem::Open(const char *name)
{ 			
		OpenFile *openFile = NULL;	
		//int i;
		
		lock->P();
					
		Directory *directory = new Directory(NumDirEntries);
		OpenFile *f;
		int sector;
		char filename[FileNameMaxLen+1];		
			
		f = MoveTo(name,filename);
		
		if (f == NULL)
		  return NULL;
		DEBUG('f', "Opening file %s\n", name);
		directory->FetchFrom(f);
		sector = directory->Find(filename); 
		if (sector >= 0) {		
			//i=FindIndex(sector);
			//if(i == -1 && GetNextEntry() == -1 ) // table de fichiers ouverts pleine
				//return NULL;
			//if(i != -1) // fichier présent dans la table
				//openFile = openFileTable[i].file;
			//else
				openFile = new OpenFile(sector);	// name was found in directory 
			
		}else{//si le fichier n'est pas trouvé on cherche dans le repertoire 'system'
			f = MoveTo("/System/",NULL);
			if (f == NULL){
			return NULL;
			}
			directory->FetchFrom(f);
			sector = directory->Find(filename);
			if (sector >= 0){ 	
				//i=FindIndex(sector);
				//if(i == -1 && GetNextEntry() == -1 ) // table de fichiers ouverts pleine
					//return NULL;
				//if(i != -1) // fichier présent dans la table
					//openFile =  openFileTable[i].file;
				//else
					openFile = new OpenFile(sector);	// name was found in directory 
				
			}
		}
		if(openFile != NULL && FindIndex(sector) != -1 )		
			AddFile(sector,openFile); // on ajoute le fichier dans la table
			
		
		delete directory;
		if (f != currentDir && f != directoryFile)
		  delete f;
		lock->V();
		return openFile;				// return NULL if not found
	
	
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

	bool
FileSystem::Remove(const char *name)
{ 
	Directory *directory;
	OpenFile* f;
	BitMap *freeMap;
	FileHeader *fileHdr;
	int sector;
	char filename[FileNameMaxLen+1];
	
	f = MoveTo(name,filename);
	if (f == NULL)
		return FALSE;
	directory = new Directory(NumDirEntries);
	directory->FetchFrom(f);
	sector = directory->Find(filename);
	if (sector == -1) {
		delete directory;
		return FALSE;			 // file not found 
	}
	fileHdr = new FileHeader;
	fileHdr->FetchFrom(sector);

	freeMap = new BitMap(NumSectors);
	freeMap->FetchFrom(freeMapFile);

	fileHdr->Deallocate(freeMap);  		// remove data blocks
	freeMap->Clear(sector);			// remove header block
	directory->Remove(name);

	freeMap->WriteBack(freeMapFile);		// flush to disk
	directory->WriteBack(currentDir);        // flush to disk
	delete fileHdr;
	delete directory;
	delete freeMap;
	
	return TRUE;
}

int
FileSystem::Cd(const char* name){
  int sector;
  int error = TRUE;
  Directory* dir =new Directory(NumDirEntries);
  OpenFile* f;
  char filename[FileNameMaxLen+1];
  f = MoveTo(name,filename);
  if (f == NULL)
    error =  FALSE;
  else{
    dir->FetchFrom(f);
    //printf("f:%p\ncurrentDir:%p\nfilename:%s\n",f,currentDir,filename);
    if (filename[0] == '\0'){
      currentDir = f;
    }else{
	if (f!=currentDir && f != directoryFile)
	    delete f;
      sector= dir->FindDir(filename);
      if (sector == -1){
	printf("Le repertoire est introuvable\n");
	error = FALSE;
      }else{
	if (currentDir != directoryFile){//le fichier ouvert sur le repertoire root doit rester ouvert
	  delete currentDir;
	}
	currentDir = new OpenFile(sector);
      }
    }
  }

  if (error == true){
    printf("Now in %s\n",name);
    List();
  }
  delete dir;
  return error;
}
  

// remove directory

	bool
FileSystem::RemoveDir(const char *name)
{ 
	Directory *directory;
	Directory *dirToDelete;
	BitMap *freeMap;
	FileHeader *fileHdr;
	OpenFile* dir;
	OpenFile* f;
	int sector;
	char filename[FileNameMaxLen+1];
	
	if( !strncmp(name, ".\0", FileNameMaxLen) ) // si le repertoire à supprimer est le repertoire courant
		return false; 
	f = MoveTo(name,filename);
	if (f == NULL)
	  return FALSE;
	directory = new Directory(NumDirEntries);
	directory->FetchFrom(f);
	if (filename[0] != '\0')
		sector = directory->FindDir(filename);
	else{
		delete directory;
		return FALSE;
	}
		
	
	if (sector == -1) {
		delete directory;
		return FALSE;			 // file not found 
	}
	
	dir = new OpenFile(sector);
	dirToDelete = new Directory(NumDirEntries);
	dirToDelete->FetchFrom(dir);
	if ( !dirToDelete->isEmpty() ){ // repertoire pas vide
			delete directory;
			delete dir;
			delete dirToDelete;
			return false;    // on empeche la suppression
		}
	
	fileHdr = new FileHeader;
	fileHdr->FetchFrom(sector);

	freeMap = new BitMap(NumSectors);
	freeMap->FetchFrom(freeMapFile);

	fileHdr->Deallocate(freeMap);  		// remove data blocks
	freeMap->Clear(sector);			// remove header block
	directory->Remove(name);

	freeMap->WriteBack(freeMapFile);		// flush to disk
	directory->WriteBack(f);        // flush to disk
	delete fileHdr;
	delete directory;
	delete dir;
	delete dirToDelete;
	delete freeMap;
	return TRUE;
} 

// end remove dir


//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

	void
FileSystem::List()
{
	Directory *directory = new Directory(NumDirEntries);

	directory->FetchFrom(currentDir);
	directory->List();
	delete directory;
}

void
FileSystem::ListP(const char* name){
	OpenFile* f;
	OpenFile* dir;
	Directory *directory;
	int sector;
	char filename[FileNameMaxLen+1];
	f = MoveTo(name,filename);
	if (f != NULL){
		directory = new Directory(NumDirEntries);
		directory->FetchFrom(f);
		if( filename[0] != '\0'){
			sector = directory->FindDir(filename);
			if (sector != -1){
				dir = new OpenFile(sector);
				directory->FetchFrom(dir);
				directory->List();
				delete dir;
			}else{
				printf("le chemin est incorrecte\n");
			}
		}else{
			directory->List();
		}
		delete directory;
	}else{
		printf("Le repertoire spécifié est introuvable\n");
	}
}
		
//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

	void
FileSystem::Print()
{
	FileHeader *bitHdr = new FileHeader;
	FileHeader *dirHdr = new FileHeader;
	BitMap *freeMap = new BitMap(NumSectors);
	Directory *directory = new Directory(NumDirEntries);

	printf("Bit map file header:\n");
	bitHdr->FetchFrom(FreeMapSector);
	bitHdr->Print();

	printf("Directory file header:\n");
	dirHdr->FetchFrom(DirectorySector);
	dirHdr->Print();

	freeMap->FetchFrom(freeMapFile);
	freeMap->Print();

	directory->FetchFrom(currentDir);
	directory->Print();

	delete bitHdr;
	delete dirHdr;
	delete freeMap;
	delete directory;
}


//----------------------------------------------------------------------
// 	ajoute un fichier dans la liste des fichiers ouverts 
//  return index du fichier ajouté dans la table -1 en cas d'erreur
//----------------------------------------------------------------------
 
int
FileSystem::AddFile(int secteur,OpenFile* open){
	int i = 0;
		
	i = GetNextEntry(); // cherche une entrée libre
	if(i == -1) //table des entrées est pleine 
		return -1;
	
	openFileTable[i].used = true;
	openFileTable[i].file = open;
	openFileTable[i].count++;
	openFileTable[i].secteur = secteur;


	return i;
}


//----------------------------------------------------------------------
// retourne l'index du fichier dans la table
// return -1 si fichier absent
//----------------------------------------------------------------------
int 
FileSystem::FindIndex(int secteur){
	int i=0;
	
	for(i=0;i<maxOpenFiles;i++){
		if( openFileTable[i].used && (openFileTable[i].secteur == secteur) )
			return i;
	}
	
	return -1;
	
}

//----------------------------------------------------------------------
// FileSysTable::close 
// Décremente le nombre de thread qui ouvre le fichier. 
// Férme le fichier si aucun thread ne l'utilise
//----------------------------------------------------------------------
void 
FileSystem::Close(const char* name){
	int i=0;
		
		lock->P();
		//i = FindIndex(name);
		if(i != -1 ){ 
		openFileTable[i].count--;
		if(openFileTable[i].count == 0 )
			delete openFileTable[i].file;
			openFileTable[i].used = false;
			openFileTable[i].file = NULL;
		}
		lock->V();
}

//----------------------------------------------------------------------
// return la prochaine entrée valide de la table des fichiers ouverts
// -1 si la table est pleine
//----------------------------------------------------------------------
int 
FileSystem::GetNextEntry(){
	int i;
	
    for(i=0;i<maxOpenFiles;i++){
		if( !openFileTable[i].used )
			return i;
	}
	
	return -1;
	
}
