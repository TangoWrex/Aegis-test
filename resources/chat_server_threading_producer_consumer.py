#!/usr/bin/env python3

import argparse
import logging
import socket
import threading

from connection_with_queue import Connection


def establish_listener(address):
    lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    lsock.bind(address)
    lsock.listen()

    return lsock


def accept_connection(lsock):
    """
    Accept a connection from a client
    """
    conn, address = lsock.accept()
    connection = Connection(conn, address)

    return connection


def service_connection(connection: Connection):
    """
    Service a chat client socket connection.
        (1) Receiving its chatname
        (2) Publish that it has joined the chat by placing in the queue (producer)
        so that consumer can publish
        (3) Receive chat messages and publish to active chatters by
        placing in queue (producer) so that consumer can publish
    """
    thread_name = threading.current_thread().name
    logging.info(f"{thread_name} starting to service connection")
    chatname = connection.receive_chatname()
    prev_thread_name = thread_name
    threading.current_thread().name = chatname + "-thread"
    thread_name = threading.current_thread().name
    logging.info(f"{prev_thread_name} changed name to {thread_name}")

    # publish announcement that chat user joined the chat
    connection.publish_entry()

    try:
        done = False
        while not done:
            chat_msg = connection.receive_message()
            logging.info(f"{thread_name}: received message \"{chat_msg}\"")
            if chat_msg:
                connection.publish_message(chat_msg)
            else:
                done = True
    except OSError as error_msg:
        logging.error(f"{thread_name}: {error_msg}")
    finally:
        connection.close()

    logging.info(f"{thread_name} finished servicing connection. Goodbye...")


def parse_args():
    parser = argparse.ArgumentParser(description="Chat service " +
                                     "(threaded version)",
                                     epilog='Enjoy chatting! :)')
    parser.add_argument("ip", type=str, help="ip address")
    parser.add_argument("port", type=int, help="listening port")
    args = parser.parse_args()
    address = (args.ip, args.port)

    return address


def setup_logging():
    log_format = "%(levelname)s %(asctime)s %(message)s"
    logging.basicConfig(level=logging.INFO,
                        format=log_format, datefmt='%I:%M:%S %p')


def main():
    main_thread_name = threading.current_thread().name
    setup_logging()
    address = parse_args()

    lsock = establish_listener(address)
    logging.info(f"{main_thread_name} listening on {address}")

    # instantiate and start publisher thread
    publisher_thread = threading.Thread(target=Connection.publish, args=(),
                                        name="publisher-thread")

    # make sure this thread dies when main thread dies
    publisher_thread.daemon = True
    logging.info(f"{publisher_thread.name} starting...")
    publisher_thread.start()

    try:
        while True:
            logging.info(f"{main_thread_name} waiting to accept a connection")
            connection = accept_connection(lsock)
            logging.info(f"{main_thread_name} accepted connection " +
                         "from {connection.addr}")
            if connection is not None:
                t = threading.Thread(target=service_connection,
                                     args=(connection,))
                logging.info(f"Starting {t.name} from {main_thread_name}")
                t.start()
    except socket.error as msg:
        logging.error(msg)
    finally:
        lsock.close()


if __name__ == '__main__':
    main()
