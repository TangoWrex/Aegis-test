#!/usr/bin/python3
from venv import create
from flask import Flask, render_template, request, redirect, url_for, flash
from traceback import print_exc
import sys
from src.server.jobs import *

app = Flask(__name__)
app.config['SECRET_KEY'] = 'tutelary'


@app.route('/', methods=['GET', 'POST'])
def home():
    if request.method == 'POST':
        # copy our clients macs to our jobs.json to start our server.
        # need to ensure we don't delete any old jobs or overwrite them
        # TODO: get create_jobs_db() to work properly
        create_jobs_db()
        return redirect(url_for('nodes'))
    else:
        return render_template('home.html')


@app.route('/control_module/<node>', methods=['GET', 'POST'])
def control_module(node=None):
    # find the node in our json list
    sensor = display_node(node)
    # TODO: display the sensor on the control module
    print(f'sensor: {sensor}')
    if request.method == 'POST':
        job = request.form['job']
        print(f'job: {job}')
        add_job(sensor['mac'], job)
    # return the node to our control module html page
    # and render the html page
    # Next capture their input and add it to our job
    return render_template('control_module.html', node=sensor)


@app.route('/nodes', methods=['GET', 'POST'])
def nodes():
    nodes_from_json = parse_nodes()
    if request.method == 'POST':
        # when the user selects a node it will be saved here.
        node_selected = request.form['node']
        print(f'node selected: {node_selected}\n')
        return redirect(url_for('control_module', node=node_selected))
    return render_template('nodes.html', nodes_dict=nodes_from_json)


def main():
    app.run(debug=True)


if __name__ == '__main__':
    try:
        main()
    except IndexError:
        print("IndexError\nusage: python3 dashboard.py")
    except (KeyboardInterrupt, EOFError):
        print("Have a nice day")
    except (SystemExit, GeneratorExit, Exception) as err:
        print_exc()
        pass
