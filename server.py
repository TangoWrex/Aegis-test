import socket
from _thread import *
import threading
from time import sleep
import json
from src.server.node import client
from src.server.jobs import add_sensor
import os

json_lock = threading.Lock()

nodes = {}

# thread function

#  https://www.geeksforgeeks.org/socket-programming-multi-threading-python/


def threaded(c: socket.socket, ip: str, port: str):
    while True:

        # data received from client
        mac = c.recv(2048)
        mac = mac.decode("utf-8")

        if not mac:
            print('Bye')
            # lock released on exit
            break

        print(f'mac: {mac}')

        # check if the node is in the macbase
        node = client(mac, ip, port)

        # add sensor to the jobs list
        add_sensor(mac)

        try:
            # TODO: create a function in the client class that does this
            # if our database of clients is empty we will create the first object
            if os.stat("./database/clients.json").st_size == 0:
                # create first node with mac
                first_object = {"sensor_information": [
                    {"mac": node.mac, "ip": node.ipaddr, "port": node.port}]}
                with open("./database/clients.json", "w") as f:
                    json.dump(first_object, f, indent=4)

            # if the node/client exists we can continue our communication with the client
            elif node.check_json(mac):
                print('Node is in the database')

            # else the client does not exist, we want to add them
            else:
                print('Node is not in the database')
                json_lock.acquire()
                new_node = {"mac": node.mac, "ip": node.ipaddr,
                            "port": node.port}
                with open(f'./database/clients.json', 'r+') as f:
                    file_data = json.load(f)

                    file_data["sensor_information"].append(new_node)
                    f.seek(0)
                    json.dump(file_data, f, indent=4)
                json_lock.release()

            c.send(b'ACK')

            # send a message saying true
        except Exception as e:
            print(e)
            print('Error in the database')
            # send a message saying false
            c.send(b'False')

        # send the client a message information them they are registered

        while True:
            # mac_added_to_jobs = False
            checkin = c.recv(2048)
            if checkin == b'checkin':
                # check jobs.json for a job for this mac
                # c.send(b'runsdr')
                with open(f'./database/jobs.json', 'r') as f:
                    jobs = json.load(f)
                    for key, value in jobs.items():
                        for node in value:
                            if node['mac'] == mac:
                                # mac_added_to_jobs = True
                                # if there is a job send it to the client
                                if len(node['jobs']) > 0:
                                    # craft message to send
                                    sensors_job = node['jobs'][0]
                                    c.send(sensors_job.encode('utf-8'))
                                    # remove the job from the jobs.json file
                                    node['jobs'].pop(0)
                                    with open(f'./database/jobs.json', 'w') as f:
                                        json.dump(jobs, f, indent=4)

                                    # os.system(
                                    #     f"ncat --ssl -w 3 -l -p 15051 > saved_files/{mac}wrx.jpg")

                                    # os.system(f"ncat --ssl -w 3 -l -p 15051 > \{app.config['DOWNLOAD_FOLDER']}/{file}")

                                else:
                                    print(f'sending no jobs\n')
                                    c.send(b'no job')

            if checkin == 'exit':
                break

            # we have to check to see if they client has a job
            # check for job
            # if job:
            # send them the command to execute
            # wait for response
            # if no job:
            # send them a message saying no job available
            # the client will wait 30 seconds and then check again

        print("bottom of thread function\n")

    c.close()


def Main():
    host = ""

    # reserve a port on your computer
    # in our case it is 12345 but it
    # can be anything
    port = 5003
    # TODO: change this to a setup_socket function
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((host, port))
    print("socket binded to port", port)

    # put the socket into listening mode
    s.listen(5)
    print("socket is listening")

    # a forever loop until client wants to exit
    while True:
        try:
            # establish connection with client
            c, addr = s.accept()

            # Check data base for their mac address

            print('Connected to :', addr[0], ':', addr[1])

            # Start a new thread and return its identifier
            start_new_thread(threaded, (c, addr[0], addr[1]))
        except KeyboardInterrupt:
            print("Keyboard Interrupt")
            break

    s.close()


if __name__ == '__main__':
    Main()
