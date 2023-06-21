import os
import time
import requests

# Directory to save the images
directory = os.path.join(os.getcwd(), "images")
os.makedirs(directory, exist_ok=True)

# API endpoint URL
api_url = "http://192.168.110.76/image"

# Number of images to capture
num_images = 50

# Delay between each request (in seconds)
delay = 3

# Headers for the request
headers = {
    "Accept": "*/*",
    "X-User-Role": "admin"
}

# Send requests and save images
for i in range(num_images):
    try:
        # Send GET request to the API with headers
        response = requests.get(api_url, headers=headers)

        # Generate a unique filename for each image
        timestamp = time.strftime("%Y%m%d%H%M%S")
        filename = f"image_{timestamp}.jpg"
        filepath = os.path.join(directory, filename)

        # Save the image file
        with open(filepath, "wb") as f:
            f.write(response.content)

        print(f"Image {i+1}/{num_images} saved as {filename}")

    except Exception as e:
        print(f"Error occurred while processing image {i+1}: {e}")

    # Wait for the specified delay
    time.sleep(delay)

print("Image capture complete.")
