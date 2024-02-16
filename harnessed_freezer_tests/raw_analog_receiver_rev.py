import serial
import serial.tools.list_ports
from datetime import datetime


def find_arduino_port():
    # Iterate through all available ports
    for port in serial.tools.list_ports.comports():
        try:
            # Try to open the port
            ser = serial.Serial(port.device, baudrate=19200, timeout=1)
            
            # Send a handshake signal to Arduino
            ser.write(b'HelloArduino\n')
            
            # Wait for a response from Arduino
            response = ser.readline().decode().strip()

            # Check if the response matches the expected acknowledgment
            if response == 'ArduinoReady':
                print(f"Arduino found on {port.device}")
                return ser
            else:
                # Close the port if the response is not as expected
                ser.close()
        except serial.SerialException:
            print(f'Failed on {port}')

    print("Arduino not found on any port.")
    return None


def get_log_filename():

    now = datetime.now() # current date and time
    logname = 'Data_' + now.strftime("%Y_%m_%d_t%H_%M_%S") + '.csv'
    return logname


if __name__ == "__main__":

    fname = get_log_filename()

    with open(fname, 'a') as f:
        f.write('Timestamp,A0,A1,A2,A3,A4,A5')

    # Find Arduino and establish a connection
    arduino_serial = find_arduino_port()

    if arduino_serial:
        try:
            while True:
                if arduino_serial.in_waiting > 0:
                    # Read data from Arduino
                    data = arduino_serial.readline().decode().strip()
                    
                    # Process and print the received data
                    print(f"Received data: {data}")

                    timestamp = datetime.now().strftime("%Y_%m_%d_t%H_%M_%S")
                    data_to_write = f"{timestamp},{data}"
                    with open(fname, 'a') as f:
                        f.write(data_to_write)

        except KeyboardInterrupt:
            # Close the serial connection on KeyboardInterrupt
            arduino_serial.close()
            print("\nSerial connection closed.")
    else:
        print("Exiting.")
