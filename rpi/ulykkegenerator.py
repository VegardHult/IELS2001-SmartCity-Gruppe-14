import random

def ulykkegenerator(x, y)
	ulykkeX = random.randint(0, x)
	ulykkeY = random.randint(0, y)
	ulykkespunkt = [ulykkeX, ulykkeY]
	return ulykkespunkt


#def getMoves(2d-liste, startposisjon, sluttposisjon, orientering):
#returnerer liste med trekk



def naresteBil(rutenett, biler, ulykkespunkt):
	#Rutenett er en 2d liste hvor man kan og ikke kan kjøre
	#Biler er en liste med alle bil-objektene
	#Ulykkespunkt er en liste hvor [0] er x- og [1] er y-koordinat
	naresteBil = None
	kortesteAvstand = 0

	for bil in biler:
		if(bil.busy):
			continue
		avstand = len(getMoves(rutenett, bil.posistion, ulykkespunkt, bil.orientation))
		if(avstand < kortesteAvstand):
			naresteBil = bil
			kortesteAvstand = avstand

	return naresteBil
