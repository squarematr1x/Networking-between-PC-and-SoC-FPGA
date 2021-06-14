import socket


class ImageClient():
    def __init__(self):
        self.tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connected = False

    def send_all(self, pixel_data):
        assert isinstance(pixel_data, bytes), 'pixel_data should be bytes'
        assert len(pixel_data) > 0, 'pixel_data should be larger than 0'

        self.tcp_client.sendall(pixel_data)

    def recv_all(self, image_size):
        assert image_size > 0, 'image_size should be larger than 0'

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
        assert isinstance(host_ip, str), 'host_ip should be string'
        assert isinstance(port, int), 'port should be integer'

        if not self.connected:
            try:
                self.tcp_client.connect((host_ip, port))
                self.connected = True
            except:
                print("Error: Cannot establish a connection.")

    def close(self):
        self.tcp_client.close()
