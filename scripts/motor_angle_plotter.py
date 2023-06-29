import requests
import time
import matplotlib.pyplot as plt

reqUrl = "http://192.168.0.124/motor/get"

headersList = {
    "Accept": "*/*",
    "User-Agent": "Thunder Client (https://www.thunderclient.com)",
    "X-User-Role": "user",
    "Motor-Number": "1"
}

payload = ""
duration = 120  # Set the duration for sending requests (in seconds)
interval = 0.5  # Set the interval between requests (in seconds)

timestamps = []
angles = []

start_time = time.time()
end_time = start_time + duration

plt.close('all')

plt.ion()  # Enable interactive mode for real-time plotting

while time.time() < end_time:
    response = requests.get(reqUrl, headers=headersList)
    data = response.json()

    # Extract angle from the response
    angle = data.get("motor_angle")
    if angle is not None:
        timestamp = time.time() - start_time
        timestamps.append(timestamp)
        angles.append(angle)

        # Plot the moving graph
        plt.clf()  # Clear previous plot
        plt.plot(timestamps, angles)
        plt.grid()
        plt.xlabel("Time (seconds)")
        plt.ylabel("Motor Angle")
        plt.title("Motor Angle vs. Time")
        plt.draw()
        plt.pause(0.001)

    time.sleep(interval)

# Plot the final graph
plt.ioff()  # Disable interactive mode
plt.plot(timestamps, angles)
plt.xlabel("Time (seconds)")
plt.ylabel("Motor Angle")
plt.title("Motor Angle vs. Time")
plt.grid()
plt.show()
