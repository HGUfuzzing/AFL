#server.py
from socket import *
import sys

if len(sys.argv) != 2 :
    print('Usage :', sys.argv[0], '<Port>')
    sys.exit()

PORT = int(sys.argv[1])

serverSock = socket(AF_INET, SOCK_STREAM)
serverSock.bind(('', PORT))
serverSock.listen()

def recv_file(s, filename) :
    length = int(s.recv(16).decode())
    recved_data = ''
    recved_len = length
    while recved_len > 0 :
        recved_data += s.recv(recved_len).decode('utf-8')
        recved_len = length - len(recved_data)
    
    with open(filename, 'w') as f:
        f.write(recved_data)
    print(length, 'length of a file was sended.')

while True:
        connectionSock, addr = serverSock.accept()
        print(addr, 'is connected.')
        recv_file(connectionSock, 'recv.json') 
        connectionSock.close()
serverSock.close()

