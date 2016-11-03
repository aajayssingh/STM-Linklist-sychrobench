outp = open('./measurewisefile_msec/allavggtod.txt', 'w')
with open('./measurewisefile/allavggtod.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   num = num * 1000
		   outp.write(format(num, '.2f'))
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
outp.close()
