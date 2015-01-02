import time
import string

def getData():
	time.sleep(1);
	f = open("/proc/pfcount", "r")
	s = f.readline()
	f.close()
	params = s.split(',')
	jiffies = params[0]
	pfcount = params[1]
	jiffies = int(jiffies.split(":")[1])
	pfcount = int(pfcount.split(":")[1])

	return jiffies, pfcount

def main():
	total = 0
	orig_jiffies, orig_pfcount = getData();
	for i in range(0, 10):
		jiffies, pfcount = getData()

		result = (pfcount - orig_pfcount) / float(jiffies - orig_jiffies)
		result *= 100
		total += result
		print "page_fault per second: " + "{:.4f}".format(result)
	print "avg. val: {:.4f}".format(total / 10)

main();


