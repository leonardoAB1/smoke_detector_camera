import os
import xml.etree.ElementTree as ET
import cv2

dataset_dir = r'C:\Users\ZEPHYRUS\Desktop\dataset_person_detection_mecacueva'
annotations_dir = os.path.join(dataset_dir, 'annotations')
output_dir = os.path.join(dataset_dir, 'dataset_smoker_classification_mecacueva')

# Create the output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Process each annotation file
for filename in os.listdir(annotations_dir):
    if not filename.endswith('.xml'):
        continue
    
    # Read the annotation file
    xml_path = os.path.join(annotations_dir, filename)
    tree = ET.parse(xml_path)
    root = tree.getroot()
    
    # Get the image file name and path
    image_filename = root.find('filename').text
    image_path = os.path.join(dataset_dir, 'JPEGImages', image_filename)
    
    # Read the image
    image = cv2.imread(image_path)
    
    # Process each object in the annotation file
    for obj in root.iter('object'):
        # Get the bounding box coordinates
        xmin = int(obj.find('bndbox/xmin').text)
        ymin = int(obj.find('bndbox/ymin').text)
        xmax = int(obj.find('bndbox/xmax').text)
        ymax = int(obj.find('bndbox/ymax').text)
        
        # Extract the region defined by the bounding box
        region = image[ymin:ymax, xmin:xmax]
        
        # Generate the output file name
        output_filename = f'{os.path.splitext(filename)[0]}_{obj.find("name").text}.jpg'
        output_path = os.path.join(output_dir, output_filename)
        
        # Save the extracted region as a new image
        cv2.imwrite(output_path, region)
        
        print(f'Saved: {output_filename}')
