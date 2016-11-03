total = 0
i = 0
outp = open('./avg/avggtod.txt', 'w')
with open('runGTOD.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   total += num
		   i += 1
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
print('Total of all numbers GTOD: {}'.format(total))
print('Total i: {}'.format(i))
outp.write(format(total/i))
print('AVG of all numbers GTOD: {}'.format(total/i))
outp.close()
print('\n')

total = 0
i = 0
outp = open('./avg/avgcpu.txt', 'w')
with open('runCPU.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   total += num
		   i += 1
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
print('Total of all numbers CPU: {}'.format(total))
print('Total i: {}'.format(i))
outp.write(format(total/i))
print('AVG of all numbers CPU: {}'.format(total/i))
outp.close()
print('\n')

total = 0
i = 0
outp = open('./avg/avgth.txt', 'w')
with open('runTH.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   total += num
		   i += 1
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
print('Total of all numbers TH: {}'.format(total))
print('Total i: {}'.format(i))
outp.write(format(total/i))
print('AVG of all numbers TH: {}'.format(total/i))
outp.close()
print('\n')

total = 0
i = 0
outp = open('./avg/avgrt.txt', 'w')
with open('runRT.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   total += num
		   i += 1
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
print('Total of all numbers RT: {}'.format(total))
print('Total i: {}'.format(i))
outp.write(format(total/i))
print('AVG of all numbers RT: {}'.format(total/i))
outp.close()
print('\n')

total = 0
i = 0
outp = open('./avg/avgclock.txt', 'w')
with open('runCLOCK.txt', 'r') as inp:
   for line in inp:
	   try:
		   num = float(line)
		   total += num
		   i += 1
	   except ValueError:
		   print('{} is not a number!'.format(line))
	
print('Total of all numbers CLOCK: {}'.format(total))
print('Total i: {}'.format(i))
outp.write(format(total/i))
print('AVG of all numbers CLOCK: {}'.format(total/i))
outp.close()
print('\n')
