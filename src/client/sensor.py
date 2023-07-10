#! /usr/bin/env python3

"""
    Sensor.py
    This file contains the sensor class which is used to create sensor objects
    and provides the functions or jobs that the sensor can execute.

    Sensor JSON Format
    "sensor_information": [
        {
            "sensor_id":"",
            "sensor_password": "",
            "ip": "0",
            "port": 0,
            "gps_status" : 0,
            "latitude": 0,
            "longitude": 0,
            "sdr": "", // name of the SDR i.e hackrf, rtl-sdr, as of now only two supported sdrs
            "bluetooth": 0,
            "krakensdr": 0
        }

"""
import subprocess
from connection import Connection


class ClientSensor(Connection):
    """A class representing a client sensor for data collection.

    This class inherits from the ConnectionSettings class and provides functionality
    for connecting the sensor to a server and running the client logic.

    :param host: The host address to connect to.
    :type host: str
    :param port: The port number to connect to.
    :type port: int
    :param sdr: The software-defined radio device.
    :type sdr: SDR
    :param bluetooth: Bluetooth boolean.
    :type bluetooth: bool
    :param wifi: The Wi-Fi boolean
    :type wifi: bool
    :param gps: The GPS boolean.
    :type gps: bool
    :param krakensd: The KrakenSDR boolean.
    :type krakensdr: bool
    """

    def __init__(self, host, port, sdr, bluetooth, wifi, gps, krakensdr):
        super().__init__(host, port)
        self.sensor_id = None
        self.sensor_password = None
        self.sdr = sdr
        self.bluetooth = bluetooth
        self.wifi = wifi
        self.gps = gps
        self.krakensdr = krakensdr

    def run(self):
        """Run the client sensor and perform data collection.

        This method connects the client sensor to the server using the provided
        host and port. It then executes the client logic for data collection.

        :return: None
        """
        # Implement the client logic here, using self.host and self.port
        self.connect()

        # send sensor id and password

        # listen for server response,
        # The server will inform us if we failed the id and password check
        # or if we passed and can continue

        # functions check the hardward devices to make sure they are configured properly

        # build our sensor information dictionary (json object)
        # and send it to the server. This should match what the server has in it's database
        # listen for response. If this check fails then there is a mismatch and our server willl send back
        # what the difference is and we can begin troubleshooting

        # if we pass the sensor information check then we can begin waiting for a job from the server

        """

        
        """

    def functions_check(self):
        """ Check if the sensor hardware is configured properly
        """
        pass

    def get_gps(self):
        """Get the GPS coordinates.

        This method gets the GPS coordinates from the GPS device.

        :return: The GPS coordinates.
        :rtype: str
        """
        # Implement the GPS logic here
        pass

    def send_gps(self):
        """Send the GPS coordinates.

        This method sends the GPS coordinates to the server.

        :return: None
        """
        # Implement the send GPS logic here
        pass

    def run_kismet(self, output_directory):
        """run_kismet
        """
        # Set the output file path
        output_file = f"{output_directory}/kismet_capture.pcap"

        # Run Kismet with the desired output file path
        subprocess.run(['kismet', '-c', output_file], check=True)

    def scan_wifi(self):
        """Get the Wi-Fi data.

        This method gets the Wi-Fi data from the Wi-Fi device.

        :return: The Wi-Fi data.
        :rtype: str
        """
        # Implement the Wi-Fi logic here

        # Example usage:
        # TODO: Validate this file path exists and is valid
        output_directory = "/path/to/project/directory"
        self.run_kismet(output_directory)

    def send_wifi_data(self):
        """Send the Wi-Fi data.

        This method sends the Wi-Fi data to the server.

        :return: None
        """
        # Implement the send Wi-Fi logic here
        pass

    def scan_bluetooth(self):
        """Get the Bluetooth data.

        This method gets the Bluetooth data from the Bluetooth device.

        :return: The Bluetooth data.
        :rtype: str
        """
        # Implement the Bluetooth logic here
        pass

    def send_bluetooth_data(self):
        """Send the Bluetooth data.

        This method sends the Bluetooth data to the server.

        :return: None
        """
        # Implement the send Bluetooth logic here
        pass

    def exec_sdr_for_space(self):
        """Execute the SDR for space data collection.

        This method executes the SDR for space data collection.

        :return: None
        """
        # Implement the SDR for space logic here
        pass

    def send_sdr_for_space_data(self):
        """Send the SDR for space data.

        This method sends the SDR for space data to the server.

        :return: None
        """
        # Implement the send SDR for space logic here
        pass

    def passive_scan_and_alert(self):
        """Passively scan and alert.

        This method passively scans for devices and alerts the server if a
        device is found.

        :return: None
        """
        # Implement the passive scan and alert logic here
        pass
