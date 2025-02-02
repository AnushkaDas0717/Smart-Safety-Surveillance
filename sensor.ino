import serial
import tkinter as tk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Create serial object
ser = serial.Serial('COM6', 9600)  # Adjust 'COM6' to match your serial port

# Create GUI window
root = tk.Tk()
root.title("Humidity Sensor Monitor")
root.configure(bg='black')  # Set background color to black

# Add purple borders
root.config(highlightbackground="purple", highlightcolor="purple", highlightthickness=3)

# Create frame for plots
frame = tk.Frame(root, bg='black')
frame.pack(fill=tk.BOTH, expand=True)

# Create Matplotlib figure and subplots
fig = Figure(figsize=(6, 6), dpi=100)
ax_temp = fig.add_subplot(311)
ax_humidity = fig.add_subplot(312)
ax_heat_index = fig.add_subplot(313)

# Add labels and title
ax_temp.set_ylabel('Temperature (°C)')
ax_humidity.set_ylabel('Humidity (%)')
ax_heat_index.set_ylabel('Heat Index (°C)')
ax_heat_index.set_xlabel('Time')

# Create Matplotlib canvas
canvas = FigureCanvasTkAgg(fig, master=frame)
canvas.draw()
canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

# Initialize empty lists for storing data
timestamps = []
temperatures = []
humidities = []

# Function to update plots with new data
def update_plots():
    try:
        data = ser.readline().decode().strip().split(',')
        if len(data) == 3:
            timestamp = int(data[0])
            temperature = float(data[1])
            humidity = float(data[2])

            # Append data to lists
            timestamps.append(timestamp)
            temperatures.append(temperature)
            humidities.append(humidity)

            # Update plots
            ax_temp.clear()
            ax_temp.plot(timestamps, temperatures, 'ro-', label='Temperature')
            ax_temp.legend()

            ax_humidity.clear()
            ax_humidity.plot(timestamps, humidities, 'bo-', label='Humidity')
            ax_humidity.legend()

            ax_heat_index.clear()
            ax_heat_index.plot(timestamps, [compute_heat_index(t, h) for t, h in zip(temperatures, humidities)], 'go-', label='Heat Index')
            ax_heat_index.legend()

            # Refresh canvas
            canvas.draw()

        # Schedule the next update
        root.after(100, update_plots)
    except Exception as e:
        print("Error reading serial data:", e)

# Function to compute heat index
def compute_heat_index(temperature, humidity):
    # Simplified heat index computation
    return temperature + humidity * 0.1

# Start updating plots
root.after(100, update_plots)

# Start GUI event loop
root.mainloop()

# Close serial port when GUI is closed
ser.close()
