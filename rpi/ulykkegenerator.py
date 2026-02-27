import random
import pathfinder

def ulykkegenerator(x, y):
	ulykkeX = random.randint(0, x-1)
	ulykkeY = random.randint(0, y-1)
	ulykkespunkt = [ulykkeX, ulykkeY]
	return ulykkespunkt


#def getMoves(2d-liste, startposisjon, sluttposisjon, orientering):
#returnerer liste med trekk



def naresteBil(rutenett, biler, ulykkespunkt):
	#Rutenett er en 2d liste hvor man kan og ikke kan kjøre
	#Biler er en liste med alle bil-objektene
	#Ulykkespunkt er en liste hvor [0] er x- og [1] er y-koordinat
	naresteBil = None
	kortesteAvstand = rutenett.width * rutenett.height

	for bil in biler:
		if(bil.busy):
			continue
		sti = pathfinder.getPath(rutenett, bil.position, ulykkespunkt)
		avstand = len(pathfinder.getMoves(rutenett, sti, bil.orientation))
		if(avstand < kortesteAvstand):
			naresteBil = bil.id
			kortesteAvstand = avstand

	return naresteBil
