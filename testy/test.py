import subprocess
max_tests = 4
appname = "./main"
kompname = "./ic18int" 
filein = "in"
fileout = "out"
filecode = "code"
filecheck = "check"
for num in range(1,max_tests+1):#cislo testu
	print ("\033[33m","***TEST CISLO ", num, "***", "\033[37m")
	with open(str(num)+filein, "rb", 0) as inputfile, open(str(num)+filecode, "w") as codefile:
		app = subprocess.call([appname], stdin=inputfile, stdout=codefile)
	with open(str(num)+fileout, "w") as outputfile: 	
		komp = subprocess.call([kompname, str(num)+filecode], stdout=outputfile)
	with open(str(num)+fileout) as fin, open(str(num)+filecheck) as fcheck:
		for (linein, linecheck) in zip(fin.readlines(), fcheck.readlines()):
			if num == 1:
				if linein == linecheck[:-1]:
					print ("\033[32m ***OK***")
					print ("vystup: ", linein[:-1])
					print("kontrolni hodnota: ", linecheck[:-1])
				else:
					print ("\033[31m ***FAIL***")
					print("vystup: ", linein[:-1])
					print("kontrolni hodnota: ", linecheck[:-1])
			else:
				if linein == linecheck:
					print ("\033[32m ***OK***")
					print("vystup: ", linein[:-1])
					print("kontrolni hodnota: ", linecheck[:-1])
				else:
					print ("\033[31m ***FAIL***")
					print("vystup: ", linein[:-1])
					print("kontrolni hodnota: ", linecheck[:-1])
			print()
print("\033[37m")
	#proc = subprocess.Popen([appname str(num)+filein], stdin = open(str(num+filein, stdout = subprocess.STDOUT)
	#stdout = subprocess.check_output("./main "+str(num)+filein)
	#for line in stdout.splitlines():
	#	print (line)
	#with open(str(num)+filein) as f:
	#	for line in f.readlines():
	#		print line[:-1]
	#print str(num)+filein
	#print str(num)+fileout 






#proc = subprocess.Popen(["./proj2"], stderr=subprocess.STDOUT)
#works in python 3.0+
#for line in proc.stdout:
#stdout = subprocess.check_output("./proj2")
#print "\033[94m", "***PRVNI TEST***"
#stri = "Ahoj! :) "
#for line in stdout.splitlines():
#	print "line: ", line,"*"
#	print "stri: ", stri,"*"
#	if line == stri:
#		print "\033[32m", "***OK***"
#	else:
#		print "\033[31m", "***FAIL***"
			
