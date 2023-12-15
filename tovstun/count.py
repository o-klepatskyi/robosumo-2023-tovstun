import os

# Function to count lines in each file and return the total number of lines
def count_lines_in_directory(directory):
    total_lines = 0
    files_line_count = {}

    # Loop through each file in the directory
    for filename in os.listdir(directory):
        
        # Construct the full file path
        file_path = os.path.join(directory, filename)

        # Check if it's a file and not a directory
        if os.path.isfile(file_path):
            
            # Skip counting lines for a specific file
            if filename == 'count.py':
                continue
            
            with open(file_path, 'r') as file:
                # Count the lines in the file
                lines = sum(1 for line in file)
                total_lines += lines
                files_line_count[filename] = lines

    return total_lines, files_line_count

# Current directory
current_directory = '.'

# Count lines in the current directory
total_lines, files_line_count = count_lines_in_directory(current_directory)

# Print the total number of lines and lines in each file
print(f"Total number of lines in the directory: {total_lines}")
for filename, line_count in files_line_count.items():
    print(f"{filename}: {line_count} lines")