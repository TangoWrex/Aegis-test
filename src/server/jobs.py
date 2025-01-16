import json
import os


def create_jobs_db():
    # TODO: make this create jobs DB work properly
    # if jobs data base is empty we will create the first object
    if os.stat("./database/jobs.json").st_size == 0:
        first_job = {"sensor_jobs": []}
        with open(f'./database/jobs.json', 'w') as f:
            json.dump(first_job, f, indent=4)
    # read from the clients.json file and copy all macs to the jobs database
    sensors = {}
    with open(f'./database/clients.json', 'r') as f:
        try:
            all_nodes = json.load(f)
            for key, value in all_nodes.items():
                # print(key, value)
                for node in value:
                    # print(node)
                    sensors[node['mac']] = {'jobs': []}
        except Exception as e:
            # print(e)
            # print('Error in the database')
            return

    with open(f'./database/jobs.json', 'r+') as f:
        first_object = {"sensor_jobs": []}
        job_data = json.load(f)
        # add sensors to "sensors_jobs" key
        for sensor in sensors:
            # TODO: get the lock
            job_data["sensor_jobs"].append(sensor)

    print(f'in create_jobs_db \tsensors: {sensors}\n')


def parse_nodes():
    """Parse nodes from clients.json"""
    with open(f'./database/clients.json', 'r') as f:
        try:
            nodes = json.load(f)
        except:
            return None
    return nodes


def display_node(node_mac: str):
    # to display the node mac address in the control panel they are both returned when it's selected
    # we're simply removing "mac : " from the start of the string
    # it's a set size so we cna use [6:] here
    node_mac = node_mac[6:]
    print(f'node_mac: {node_mac}\n')
    with open(f'./database/clients.json', 'r') as f:
        all_nodes = json.load(f)
        for key, value in all_nodes.items():
            for node in value:
                if node['mac'] == node_mac:
                    return node


def add_job(node_mac: str, job: str):
    # TODO: if sensor information is empty we need to exectue the create_jobs_db() function
    with open(f'./database/jobs.json', 'r+') as f:
        nodes = json.load(f)
        node_in_file = False
        for key, value in nodes.items():
            for node in value:
                if node['mac'] == node_mac:
                    node['jobs'].append(job)
                    node_in_file = True
            if node_in_file is False:
                nodes['sensor_jobs'].append(
                    {'mac': node_mac, 'jobs': [job]})
        f.seek(0)
        json.dump(nodes, f, indent=4)


def add_sensor(node_mac: str):
    print("*add sensors() *\t before try\n")
    try:
        with open(f'./database/jobs.json', 'r+') as f:
            print("*add sensors() *\t adding sensor to jobs database\n")
            nodes = json.load(f)
            nodes['sensor_jobs'].append({'mac': node_mac, "jobs": []})
            f.seek(0)
            json.dump(nodes, f, indent=4)
    except Exception as e:
        print(e)
