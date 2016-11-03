outp = open('./measurewisefile_msec/allavggtod.txt', 'w')
with open('./measurewisefile/allavggtod.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
		   outp.write('\n')
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()


outp = open('./measurewisefile_msec/allavgclock.txt', 'w')
with open('./measurewisefile/allavgclock.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
		   outp.write('\n')
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()

outp = open('./measurewisefile_msec/allavgcpu.txt', 'w')
with open('./measurewisefile/allavgcpu.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
		   outp.write('\n')
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()


outp = open('./measurewisefile_msec/allavgrt.txt', 'w')
with open('./measurewisefile/allavgrt.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
		   outp.write('\n')
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()


outp = open('./measurewisefile_msec/allavgth.txt', 'w')
with open('./measurewisefile/allavgth.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
		   outp.write('\n')
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()
