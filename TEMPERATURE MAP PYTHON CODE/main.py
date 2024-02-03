import folium
import pandas as pd

# Load CSV data into a Pandas DataFrame with 'latin1' encoding
csv_file_path = 'C:\Users\monster\PycharmProjects\pythonProject\output2.csv'
df = pd.read_csv(csv_file_path, encoding='latin1')

# Iterate through each row in the DataFrame
for index, row in df.iterrows():
    # Convert latitude and longitude strings to float
    latitude = float(row['Latitude'].split('°')[0].replace(' ', '') + '.' + row['Latitude'].split('°')[1].rstrip('*'))
    longitude = float(row['Longitude'].split('°')[0].replace(' ', '') + '.' + row['Longitude'].split('°')[1].rstrip('*'))

    # Create a folium map centered at the coordinates of the current row
    mymap = folium.Map(location=[latitude, longitude], zoom_start=15)

    # Add a marker for the current data point
    folium.Marker([latitude, longitude], popup=f'Temperature: {row["Temperature"]}').add_to(mymap)

    # Save the map to an HTML file with a unique name based on the index
    map_filename = f'map_{index}.html'
    mymap.save(map_filename)

    print(f'Map saved as {map_filename}')
