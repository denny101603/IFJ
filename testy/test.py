"""
Univerzalni testovaci skript – uprava pro IFJ2018.
Datum: listopad 2018
Autor: Jan Beran, FIT VUT
E-mail: xberan43@stud.fit.vutbr.cz

Skript lze spoustet bez prepinacu, popripade s prepinaci (bude vysvetleno dale).
1) Bez prepinacu skript projizdi testy ve tvaru xin, kde x je cislo od start_test po max_tests a dava je na vstup programu main (v appname).
2) Dale vystup z 1) da do interpretu ic18int a porovna vystup se souborem xcheck.
3) mezi kazdym testem se pta, jestli chcete provest dalsi test.

Prepinace (na jejich poradi nezalezi):
1) --start=NUM
	Pri pouziti prepinace "--start=NUM" zacne probihat testovani az od testu s cislem NUM (standard je od 1, vstup neni nijak osetrovan)
2) --fast=true
	Pri pouziti tohoto prepinace se skript nebude ptat pred kazdym provedenim testu na pokracovani, projee vsechny testy naraz.

Pouziti skriptu:
Predpoklada se, ze testovaci soubory xin a xcheck, stejne jako interpret ic18int a testovany program (main) jsou ve stejne slozce, jako skript.
Pouziti probiha jednoduse. Do konzole se napise "python3 test.py" a skript zacne testovat. Pri pouziti prepinace "--fast=true" se nebude ptat pred kazdym testem 
na pokracovani, standardne se pta.
"""



import subprocess
import sys
max_tests = 14
appname = "./main"
kompname = "./ic18int" 
filein = "in" #program v puvodnim IFJRuby
fileout = "out" #skutecny vystup
filecode = "code" #program v IFJcode18
filecheck = "check" #kontrolni vystup
start_test = 1
fast = 0
rozh = "y"
for arg in sys.argv:
	if arg.startswith("--start="):
		start_test = int(arg.split("--start=",1)[1])
	if arg == "--fast=true":
		fast = 1


for num in range(start_test,max_tests+1):#cislo testu
	if fast == 0:
		rozh = input("Pokracovat v testovani? y/n?")
	if rozh == "y":
		print ("\033[33m","***TEST CISLO ", num, "***", "\033[37m")
		with open(str(num)+filein, "rb", 0) as inputfile, open(str(num)+filecode, "w") as codefile:
			app = subprocess.call([appname], stdin=inputfile, stdout=codefile)
		with open(str(num)+fileout, "w") as outputfile: 	
			komp = subprocess.call([kompname, str(num)+filecode], stdout=outputfile)
		with open(str(num)+fileout) as fin, open(str(num)+filecheck) as fcheck:
			for (linein, linecheck) in zip(fin.readlines(), fcheck.readlines()):
				if linein == linecheck:
					print ("\033[32m ***OK***")
					print("vystup: ", linein)
					print("kontrolni hodnota: ", linecheck)
				else:
					if linecheck.endswith("\n"):# or linein.endswith("\n"):
						print ("\033[32m ***OK***")
						print("vystup: ", linein)
						print("kontrolni hodnota: ", linecheck) 
					else:
						print ("\033[31m ***FAIL***")
						print("vystup: ", linein)
						for char in linein:
							print(ord(char))
						print("kontrolni hodnota: ", linecheck)
						for char in linecheck:
							print(ord(char))
				print()
	else:
		break
print ("\033[94m","***KONEC TESTU***", "\033[37m")
