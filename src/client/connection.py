#! /usr/bin/env python3
"""
    Sensor.py
    This file contains the sensor class which is used to create sensor objects
    and provides the functions or jobs that the sensor can execute.

    Sensor JSON Format    
    "sensor_information": [
        {
            "sensor_name":"",
            "sensor_password": "",
            "ip": "127.0.0.1",
            "port": 57670,
            "gps_status" : 0,
            "latitude": "<lat>",
            "longitude": "<long>",
            "hackrf": 1,
            "rtlsdr": 0,
            "bluetooth": "<boolean if bluetooth capable> (1 or 0)"
        }
        
"""

import socket
import time
import sys

MAX_PACKET_LEN = 2048
MAX_FILE_SIZE = 1016
MAX_PATH_LEN = 100


class Connection:
    """ _summary_

    _extended_summary_
    """

    def __init__(self, ipaddress, port):
        self.ipaddr = ipaddress
        self.port = port
        self.sock = None
        self.buffer = b''

    def connect(self):
        """connect _summary_

        _extended_summary_
        """
        # make initial connection to the server

        try:
            self.sock = socket.socket(socket.AF_INET,
                                      socket.SOCK_STREAM)
            # address = (int(ipaddr), int(port))
            address = (self.ipaddr, self.port)
            self.sock.connect(address)

        except ConnectionError as connerr:
            print(f'connection setup failure: {connerr}')
            sys.exit()

    def send_all(self, packet_len: int):
        """
        send_msg

        sends the command 'msg' to the server and
        returns the data recieved
        """
        try:
            if packet_len > MAX_PACKET_LEN:
                raise ValueError(
                    "send_msg: packet_length exceeds \
                        MAX_PACKET_LEN of 2048")

            total_bytes_sent = 0
            while total_bytes_sent <= len(self.buffer):
                # I want to ensure that we aren't sending
                # more than 2048 bytes
                if total_bytes_sent > MAX_PACKET_LEN:
                    raise ValueError(
                        "send_msg: packet_length exceeds \
                            MAX_PACKET_LEN of 2048")
                # If we have to send again we will have the
                # index of where we left off
                bytes_sent = self.sock.send(self.buffer[total_bytes_sent:])
                # if we aren't sending any bytes then \
                # something is wrong.
                if bytes_sent == 0:
                    raise RuntimeError("Connection Error")

                # add and check our byte count before breaking
                total_bytes_sent = total_bytes_sent + bytes_sent
                if total_bytes_sent >= packet_len:
                    break
            return total_bytes_sent
        except ValueError as err:
            print(err)

    def recv_all(self, sock: socket.socket):
        """
        recv_all

        if nothing is recieved after 0.5 seconds then the function will return
        all the data it has recieved
        """
        # set socket to non blocking
        self.sock.setblocking(0)
        all_data = b''
        start = time.time()
        try:
            while True:
                try:
                    # I have to use BlockingIOError because the socket
                    # is non blocking
                    data = sock.recv(MAX_PACKET_LEN)
                    all_data += data
                except BlockingIOError:
                    time.sleep(0.1)

                total_time = time.time() - start
                if total_time > 1:
                    break
            return all_data
        except socket.error as err:
            print(err)

    def send_recv_all_data(self,  packet_len: int):
        """send_recv_all_data

        This function uses the send_all and recv_all functions to
        control communication between the server and the client

        This function send self.buffer after the function has packed it
        with the data to send to the server.

        packet_len  The length of the packet to be sent by sendall

        return: the data recieved from the server
        """
        bytes_sent = self.send_all(packet_len)
        if bytes_sent != packet_len:
            print("send_recv_all_data: bytes sent does not \
equal packet length")
            return None

        server_return = self.recv_all(self.sock)
        if server_return is None:
            print("send_recv_all_data: server return is None")
            return None

        return server_return

    # TODO: Using the SCP library build a function to send the file to the server
