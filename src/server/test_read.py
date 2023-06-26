import json

from time import sleep


def main():
    with open(f'./database/clients.json', 'r') as f:
        nodes = json.loads(f.read())
        print(f'were open')
        sleep(100)

    dict_to_hold_jobs = {}

    for key, value in nodes.items():
        # print(key, value)
        for node in value:
            # print(node)
            dict_to_hold_jobs[node['mac']] = {'jobs': []}

    print(dict_to_hold_jobs)


if __name__ == '__main__':
    main()
