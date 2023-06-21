from PIL import Image
import os

def rotate_images(directory):
    for filename in os.listdir(directory):
        if filename.endswith(".jpg"):
            image_path = os.path.join(directory, filename)
            try:
                image = Image.open(image_path)
                rotated_image = image.rotate(-90, expand=True)  # Rotate 90 degrees to the right
                rotated_image = rotated_image.resize(image.size, Image.ANTIALIAS)  # Resize to original dimensions
                rotated_image.save(image_path)
                print(f"Rotated {filename}")
            except Exception as e:
                print(f"Error rotating {filename}: {e}")

# Specify the directory path where your images are located
directory_path = "C:/Users/ZEPHYRUS/Desktop/dataset_personDetetion"

# Call the function to rotate the images in the directory
rotate_images(directory_path)
