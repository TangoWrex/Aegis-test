# import sys
# import ipaddress
from sensor import ClientSensor
import sys
import ipaddress


def main():
    """main _summary_

    _extended_summary_
    """
    # TODO: Use configur parser to allow a user to configure their sensor settings
    #

    # ipaddr = sys.argv[1]
    # ipaddr.ip_address(ipaddr)
    # port = sys.argv[2]
    # typecase port to int before passing it to sensor.
    # port = int(port)

    ipaddr = sys.argv[1]
    ipaddress.ip_address(ipaddr)
    port = sys.argv[2]
    # host = '127.0.0.1'
    # port = 5003

    # currently only supports hackrf
    sdr = "hackrf"
    bluetooth = 0
    wifi = 1
    gps = 1
    # Future iterations will use krakenSDR for DF
    krakensdr = 0

    try:
        client = ClientSensor(ipaddr, int(port), sdr,
                              bluetooth, wifi, gps, krakensdr)
        client.run()
    except ValueError as val_err:
        print(f'error: {val_err}')
        return


if __name__ == '__main__':
    try:
        main()
    except (IndexError, ValueError, TypeError) as err:
        print(f"{err}\nusage: python3 main.py <ip> <port>\n")
    except (KeyboardInterrupt, EOFError):
        print("Client Shutting Down\n")
    except (SystemExit, GeneratorExit, BrokenPipeError) as err:
        print(err)
