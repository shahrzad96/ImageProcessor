import mmap
import struct
from collections import Counter

def most_common_color_per_row(row_data):
    # Interpret row_data as a series of unsigned bytes
    pixels = struct.unpack('{}B'.format(len(row_data)), row_data)
    # Group pixels into BGR tuples
    colors = [tuple(pixels[i:i+3]) for i in range(0, len(pixels), 3)]
    # Find the most common color in the row
    most_common = Counter(colors).most_common(1)[0][0]
    return most_common

def process_image_data():

    with open("map_size.txt", "r") as size_file:
       map_size = int(size_file.read().strip())

    mem_path = "Local\\ImageMap"

    mmapped_file = mmap.mmap(-1, map_size, mem_path, access=mmap.ACCESS_WRITE)
    try:
        # Read metadata (map size, width, and height of the image)
        map_size, width, height = struct.unpack('iii', mmapped_file[:12])
        row_size = width * 3  # Number of bytes per row

        for row in range(height):
            # Extract row data starting at byte offset MetaDataSize(3Bytes) + row * row_size
            start = 12 + row * row_size
            row_data = mmapped_file[start:start + row_size]
            
            common_color = most_common_color_per_row(row_data)
            
            # Pack the color as bytes and write it back to the memory map
            result_pos = 12 + row * 3
            mmapped_file[result_pos:result_pos + 3] = struct.pack('BBB', *common_color)
        
        status_flag_pos = map_size - 4
        # Set the completion flag as a single int
        mmapped_file[status_flag_pos:status_flag_pos + 4] = struct.pack('i', 1)

        print("Processing complete. Results written to memory.")
    finally:
        mmapped_file.close()

if __name__ == "__main__":
    process_image_data()