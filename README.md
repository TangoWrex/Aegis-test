# Aegis

## About
V1.0
* Aegis is a tool that allows for control over remote sensors. The commands built in are designed to work with DragonOS, an operating system built to allow multiple Software Defined Radios and softwares to be used. 


Git Link:

* <https://github.com/TangoWrex/Aegis>

## Table of contents

Use for instance <https://github.com/ekalinin/github-markdown-toc>:

> * [Title](#Aegist)
>   * [About](#About)
>   * [Table of contents](#table-of-contents)
>   * [Installation](#installation)
>   * [Requirements](#requirements)
>   * [Build](#build)
>   * [Usage](#usage)
>   * [Content](#content)
>   * [Resources (Documentation and other links)](#resources-documentation-and-other-links)
>   * [Contributing / Reporting issues](#contributing--reporting-issues)

## Installation

Clone project
> https://git.cybbh.space/170D/wobc/student-folders/21_002/truett/Aegis.git

### Requirements


set up virtual environment
> $ sudo apt install python3.8-venv
> 
> $ pip3 install venv
> 
> $ pyhon3 -m venv .venv
> 
> $ source .venv/bin/activate

install requirements
> $ pip3 install  -r requirements.txt


gps requirements
gpsd notes
for include gps.h
>    $ sudo apt install gpsd
>    $ sudo apt-get install libgps-dev 

### Build

    $ make clean && make

Build options:

* check 
* debug 
* clean 
* grade 
* profile 
* all
## Usage

Run the python Server
> sudo python3 server.py -p <port>
> *note: if no port is given a default port is chosen 
> example: sudo python3 server.py -p 5003


Run the sensor client

Run the C client
> make clean && make
> sudo ./tutelary <port> <ip> <mac-or-name>
> example: sudo ./tutelary  5003  127.0.0.1  00:x7:z0:0c:32:bb

Run the python dashboard
> sudo python3 dashboard.py

open browser and go to:
> http://localhost:5000



Further testing and instructions are located in doc/testplan.pdf







## Resources (Documentation and other links)

* design document:
    >doc/design.pdf

