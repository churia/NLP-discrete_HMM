import sys

def main():
	res_f = open(sys.argv[1], 'r')
	out_f = open(sys.argv[2], 'r')

	correct_line = 0

	for index, (res_line, out_line) in enumerate(zip(res_f, out_f)):
		if res_line.strip() in out_line.strip():
			correct_line += 1
	print(correct_line*1.0/(index+1))

if __name__ == '__main__':
	main()
