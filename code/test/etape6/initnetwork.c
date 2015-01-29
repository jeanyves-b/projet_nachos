/* initnetwork.c
 *	
 * 	Programme pour initialiser les dossier inbox et outbox et network
 */

#include "syscall.h"
	int
main ()
{
	mkdir("network");
	cd("network");
	mkdir("inbox");
	mkdir("outbox");
	return 0;
}
