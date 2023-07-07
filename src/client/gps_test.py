"""
Python file to test calling a gps function in C that 
returns the lat long from gpsd of the user device
"""
import os
import ctypes

# Set the LD_LIBRARY_PATH environment variable to include the current directory
current_dir = os.path.dirname(os.path.abspath(__file__))
os.environ['LD_LIBRARY_PATH'] = current_dir

# Load the shared library containing the C function
# Replace "your_library.so" with the actual library file name
lib = ctypes.CDLL(f"{current_dir}/test_call.so")

# Define the argument types of the C function
lib.get_value_test.argtypes = (ctypes.POINTER(
    ctypes.c_double), ctypes.POINTER(ctypes.c_double))
lib.get_value_test.restype = ctypes.c_int

# Create variables in Python to store the values
lat = ctypes.c_double()
long = ctypes.c_double()

# Call the C function
result = lib.get_gps(ctypes.byref(lat), ctypes.byref(long))

# Check the result and access the values
if result == 1:
    print("Values successfully retrieved from C function:")
    print("val1 =", lat.value)
    print("val2 =", long.value)
else:
    print("Error occurred while calling the C function.")
