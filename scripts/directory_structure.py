import os

def print_directory_structure(root_dir, indent=''):
    for item in os.listdir(root_dir):
        item_path = os.path.join(root_dir, item)
        if os.path.isdir(item_path):
            print(f"{indent}[+] {item}")
            print_directory_structure(item_path, indent + '  ')
        else:
            print(f"{indent}[-] {item}")

# Get the current working directory (cwd)
cwd = os.getcwd()

# Print the directory structure
print_directory_structure(cwd)
