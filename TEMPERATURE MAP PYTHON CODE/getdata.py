import socket
import csv

# Define the IP address and port of the ESP8266
ip_address = "192.168.25.204"
port = 5000

# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Open a CSV file for writing
filename = "output.csv"
with open(filename, mode="w", newline="") as file:
    # Create a CSV writer
    writer = csv.writer(file)

    # Write header row
    writer.writerow(["Latitude", "Longitude", "Temperature"])

    try:
        # Connect to the ESP8266
        print("Connecting...")
        s.connect((ip_address, port))
        print("Connected")

        # Set a timeout for the socket to periodically check for data
        s.settimeout(1)  # 1 second timeout

        # Initialize variables to store data
        buffer_list = []  # List to accumulate received data

        # Receive and store data from the ESP8266
        while True:
            try:
                data = s.recv(4096)
            except socket.timeout:
                # Timeout occurred, check for other tasks or continue the loop
                continue

            if not data:
                # Connection closed by the server
                print("Connection closed by the server.")
                break

            # Print the raw received data
            # print(f"Raw received data: {data}")

            # Write the raw received data to the file
            # file.write(f"Raw received data: {data}\n")

            # Accumulate received data in the list
            buffer_list.append(data)

            # Check if a complete set is received (ends with '*')
            if b"*" in buffer_list[-1]:
                # Concatenate all data in the list and decode
                accumulated_data = b"".join(buffer_list).decode('utf-8', errors='ignore')
                print(f"Accumulated data: {accumulated_data}")

                # Write the accumulated data to the file
                file.write(f"{accumulated_data.replace('#', ',')}\n")

                # Process and store the received data
                values = accumulated_data.split('#')
                if len(values) == 2:  # Assuming 2 values separated by '#'
                    long_lat, temp = values
                    long, lat = long_lat.split('°')
                    # Convert to appropriate types if needed
                    long = float(long)
                    lat = float(lat)
                    temp = float(temp)

                    # Print or process the long, lat, and temp values
                    print(f"Long: {long}, Lat: {lat}, Temp: {temp}")

                    # Write the parsed data to the CSV file
                    writer.writerow([lat, long, temp])

                # Clear the buffer list after processing
                buffer_list = []

    except KeyboardInterrupt:
        print("Script terminated by user.")

    finally:
        # Close the socket
        s.close()
        print("Socket closed.")
