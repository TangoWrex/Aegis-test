import time
import socket
from time import sleep
import json


class client(object):
    """client object is created when a connection starts

    port 1 and port 2 are both send via the instructions port from the C server
    """

    def __init__(self, macaddr, ipaddr, port):
        self.ipaddr = ipaddr
        self.mac = macaddr
        self.port = port

    @classmethod
    def recv_timeout(self, the_socket, timeout=1):
        """
        continue recieving data until the server stops sending it
        obviously this will need be to linked to us changing our methods
        """
        the_socket.setblocking(0)
        total_data = b''
        data = ''
        # beginning time
        begin = time.time()
        while 1:
            # if you got some data, then break after timeout
            if total_data and time.time()-begin > timeout:
                break
            elif time.time()-begin > timeout*2:
                break
            try:
                data = the_socket.recv(1024)
                if data:
                    total_data += data.decode("utf-8")
                    # change the beginning time for measurement
                    begin = time.time()
                else:
                    time.sleep(0.1)
            except Exception as e:
                pass
        return total_data

    @classmethod
    def send_msg(self, connection: socket, msg: str):
        """
        send_msg

        sends the command 'msg' to the server and
        returns the data recieved
        """
        connection.send(msg.encode("utf-8"))
        data = self.recv_timeout(connection)
        return data

    @classmethod
    def setup_socket(self, port: int, ip: str):
        """
        given a port and ip address create a socket
        and return it
        """
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        address = (ip, port)
        sock.connect(address)
        return sock

    @classmethod
    def load_json_list(self, content):
        """
        The server uses the linux tree command to
        generate a json objects of the directory.
        This has to be parsed correctly and returned
        as a list of tuples in order for the html
        to be rendered correctly on the flask page
        """
        type_file = []
        name = []
        for key, value in content.items():
            if (type(value) == list):
                for content in value:
                    for (key, value) in content.items():
                        if key == 'type':
                            type_file.append(value)
                        if key == 'name':
                            name.append(value)
        all_content = list(zip(type_file, name))
        return all_content

    @classmethod
    def check_json(self, mac: str):
        # read nodes file
        try:
            print('reading file\n')
            with open(f'./database/clients.json', 'r') as f:
                nodes = json.load(f)
            for key, value in nodes.items():
                for node in value:
                    print(node)
                    if node['mac'] == mac:
                        return True
                # print(key, value)
        except Exception as e:
            print("client: mac not in node\n")
            return False

    @classmethod
    def create_json_node(self, mac, ip, port):
        """
        create_json_node

        creates a json object with the mac address
        as the key and the ip and port as the value
        """
        node = {mac: [ip, port]}
        return node
