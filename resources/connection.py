import socket
import threading


class Connection:

    chat_subscribers = list()
    lock = threading.Lock()

    @classmethod
    def attach(cls, connection):
        with cls.lock:
            cls.chat_subscribers.append(connection)

    @classmethod
    def detach(cls, connection):
        with cls.lock:
            for conn in cls.chat_subscribers:
                if conn == connection:
                    cls.chat_subscribers.remove(conn)

    @classmethod
    def publish(cls, msg):
        with cls.lock:
            for conn in Connection.chat_subscribers:
                conn.send_message(msg)

    @classmethod
    def get_subscribers(cls):
        subscribers = ""
        with cls.lock:
            for conn in cls.chat_subscribers:
                subscribers += conn.chatname + "\n"

        return subscribers

    def __init__(self, conn, addr):
        self.conn = conn
        self.addr = addr
        self.chatname = None

    def close(self):
        Connection.detach(self)
        self.conn.close()
        msg = self.chatname + " has left the chat"
        Connection.publish(msg)

    def publish_entry(self):
        Connection.attach(self)
        curr_chatnames = Connection.get_subscribers()
        msg = self.chatname + " has joined the chat\n" + \
                              "Current chatters...\n" + curr_chatnames

        Connection.publish(msg)

    def publish_message(self, msg):
        full_msg = self.chatname + "> " + msg
        Connection.publish(full_msg)

    def receive_chatname(self):
        data_bytes = self.conn.recv(64)
        self.chatname = data_bytes.decode("utf-8").lstrip("0")

        return self.chatname

    def receive_message(self):
        msg = None
        msg_len = self.receive_message_header()
        if msg_len > 0:
            msg = self.receive_message_body(msg_len)

        return msg

    def receive_message_header(self):
        msg_len = 0
        data_bytes = self.conn.recv(4)
        if data_bytes:
            msg_len = int.from_bytes(data_bytes, "big")

        return msg_len

    def receive_message_body(self, msg_len):
        msg_bytes = self.conn.recv(msg_len)
        msg = msg_bytes.decode("utf-8")

        return msg

    def send_message(self, msg):
        msg_bytes = msg.encode("utf-8")
        msg_len = len(msg_bytes)
        msg_len_bytes = msg_len.to_bytes(4, "big")

        self.send(msg_len_bytes)
        self.send(msg_bytes)

    def send(self, data_bytes):
        data_len = len(data_bytes)
        total_bytes_sent = 0

        while total_bytes_sent < data_len:
            sent_bytes = self.conn.send(data_bytes[total_bytes_sent:])
            if sent_bytes == 0:
                raise socket.error(f"{threading.current_thread().name} " +
                                   "connection closed during send")
            total_bytes_sent += sent_bytes
