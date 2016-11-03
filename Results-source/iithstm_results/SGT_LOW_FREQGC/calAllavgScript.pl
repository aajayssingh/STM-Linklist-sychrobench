path = './'
pathop = './measurewisefile'

outp = open(pathop + '/allavgclock.txt', 'a')
for i in range(1,11):
	with open(path + format(i) +'0run/rawAnalysis/avg/avgclock.txt', 'r') as inp:
	   for line in inp:
		   try:
			   num = float(line)
			   print('dir {}  '.format(i))
			   print('num {}  '.format(num))
			   outp.write(format(num)+'\n')
		   except ValueError:
			   print('{} is not a number!'.format(line))

outp.close()

outp = open(pathop + '/allavgcpu.txt', 'a')
for i in range(1,11):
	with open(path + format(i) +'0run/rawAnalysis/avg/avgcpu.txt', 'r') as inp:
	   for line in inp:
		   try:
			   num = float(line)
			   print('dir {}  '.format(i))
			   print('num {}  '.format(num))
			   outp.write(format(num)+'\n')
		   except ValueError:
			   print('{} is not a number!'.format(line))

outp.close()

outp = open(pathop + '/allavggtod.txt', 'a')
for i in range(1,11):
	with open(path + format(i) +'0run/rawAnalysis/avg/avggtod.txt', 'r') as inp:
	   for line in inp:
		   try:
			   num = float(line)
			   print('dir {}  '.format(i))
			   print('num {}  '.format(num))
			   outp.write(format(num)+'\n')
		   except ValueError:
			   print('{} is not a number!'.format(line))

outp.close()


outp = open(pathop + '/allavgrt.txt', 'a')
for i in range(1,11):
	with open(path + format(i) +'0run/rawAnalysis/avg/avgrt.txt', 'r') as inp:
	   for line in inp:
		   try:
			   num = float(line)
			   print('dir {}  '.format(i))
			   print('num {}  '.format(num))
			   outp.write(format(num)+'\n')
		   except ValueError:
			   print('{} is not a number!'.format(line))

outp.close()


outp = open(pathop + '/allavgth.txt', 'a')
for i in range(1,11):
	with open(path + format(i) +'0run/rawAnalysis/avg/avgth.txt', 'r') as inp:
	   for line in inp:
		   try:
			   num = float(line)
			   print('dir {}  '.format(i))
			   print('num {}  '.format(num))
			   outp.write(format(num)+'\n')
		   except ValueError:
			   print('{} is not a number!'.format(line))

outp.close()
