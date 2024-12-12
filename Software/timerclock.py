import serial
import time
import argparse
'''
parse current time in linux format to uart to microcontroller 

from terminal: 'python timerclock.py /dev/ttyACM0'

'''
def date_time_parser(args):
	# generate time string in format like __TIME__(19:10:12) and __DATE__ (Oct 20 24) under C language
	
	now = time.gmtime()
	#now = time.localtime()
	
	atCommandString = "AT+SETRTC="
	timeString = time.strftime("%H:%M:%S", now)
	dateString = time.strftime("%b %d %Y", now)
	atCommandString = atCommandString + timeString
	atCommandString = atCommandString + dateString
	atCommandString = atCommandString + '\r\n'
	print(atCommandString)
	serialInterface = args.serial_interface
	try:
		byteString = atCommandString.encode()
		terminal = serial.Serial(serialInterface, 115200)
		terminal.write(byteString)
		answer = terminal.readline()
		print(f'received:{answer}')
		terminal.close()
	except:
		print(f'could not open port {serialInterface} Device or resource busy')
		return 0
	return 1

if __name__ == "__main__":
	
	parser = argparse.ArgumentParser()
	parser.add_argument('serial_interface', type=str, help='COMx on windows or /dev/ttyACM0 on linux')
	args = parser.parse_args()
	
	if(date_time_parser(args)):
		print('RTC set successfully')
	else:
		print('Failed to set RTC')
