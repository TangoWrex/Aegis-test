#!/usr/bin/env python3

import argparse
import logging
import socket
import threading


def establish_listener(address):
    lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    lsock.bind(address)
    lsock.listen()
    logging.info("Listening on {0}".format(address))

    return lsock


def accept_connection(lsock):
    thread_name = threading.current_thread().name
    logging.info(f"{thread_name} waiting to accept a connection")
    conn, address = lsock.accept()
    logging.info("Accepted connection from {0}".format(address))

    return conn, address


def service_connection(conn, address, connections):
    """
    Service a chat client socket connection by
    receiving chat messages and sending to all active
    chat clients
    """
    thread_name = threading.current_thread().name
    logging.info(f"{thread_name} starting to service connection")

    done = False
    while not done:
        try:
            # blocking call (this thread blocks until there is data)
            data_bytes = conn.recv(1024)
            if data_bytes:
                data_str = data_bytes.decode("utf-8")
                logging.info(f"Received \"{data_str}\" from {address}")

                # send chat message to all connected clients
                for c in connections:
                    c.send(data_bytes)
        except OSError as msg:
            logging.info(msg)
            done = True
            conn.close()
            # remove the chat client from the list of connections
            for c in connections:
                if c == conn:
                    connections.remove(c)
            logging.info(f"Closing connection to {address}")
        finally:
            pass

    logging.info(f"{thread_name} finishing service connection")


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
    setup_logging()
    address = parse_args()
    connections = list()

    lsock = establish_listener(address)
    main_thread_name = threading.current_thread().name
    done = False

    try:
        while not done:
            # blocks until a client connects
            conn, addr = accept_connection(lsock)
            if conn is not None:
                connections.append(conn)
                t = threading.Thread(target=service_connection,
                                     args=(conn, addr, connections))
                logging.info(f"Starting {t.name} from {main_thread_name}")

                # start a thread to service this specific client socket conn
                t.start()

            logging.info(f"{main_thread_name} continuing")

    except socket.error as msg:
            logging.error(msg)
    finally:
        lsock.close()


if __name__ == '__main__':
    main()
