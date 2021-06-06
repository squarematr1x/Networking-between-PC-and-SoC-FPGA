import socket


class ImageClient():
    def __init__(self):
        self.tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connected = False

    def send_all(self, pixel_data):
        self.tcp_client.sendall(pixel_data)

    def recv_all(self, image_size):
        total_data = b''
        recv_bytes = 0

        while recv_bytes < image_size:
            data = self.tcp_client.recv(8192)

            if not data:
                raise RuntimeError("Error: Socket connection broken.")

            total_data += data
            recv_bytes += len(data)

        return total_data

    def connect(self, host_ip, port):
        if not self.connected:
            self.tcp_client.connect((host_ip, port))
            self.connected = True

    def close(self):
        self.tcp_client.close()
