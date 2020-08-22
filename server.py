#server.py
import socket
HOST = '127.0.0.1'
PORT = 9999        

c = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
c.bind((HOST, PORT))
c.listen()
s, addr = c.accept()

def send_file(s, filename) {
    print(filename, "is sending.")
    s.sendall('ok'.encode())
    with open(filename, 'rb') as f:
        data = f.read()
    s.sendall(('%16d' % len(data)).encode())
    print('length : %16d' % len(data))
    s.sendall(data)
}


while True:
       data = (s.recv(1024)).decode()
        x, filename, x = data.split('\n', 2)
        print(filename, "is sending.")
        s.sendall('ok'.encode())
        with open(filename, 'rb') as f:
            data = f.read()
        s.sendall(('%16d' % len(data)).encode())
        print('length : %16d' % len(data))
        s.sendall(data)