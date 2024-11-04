# Image Processing Pub/Sub System
This project demonstrates a basic publisher-subscriber (pub/sub) system for processing images, where:

## C++ application acts as the publisher.
Python application acts as the subscriber.
The applications communicate through a shared memory-mapped file. The C++ program loads and writes image data to this memory, while the Python program reads and processes the data, identifying the most common color for each row in the image.

### System Components and Workflow
1. Publisher (C++ Application)
The C++ application is responsible for:

Image Loading and Memory Mapping: Using OpenCV, it loads an image and writes its pixel data to a memory-mapped file.
Singleton Design Pattern: The ImageMapping class follows the Singleton pattern to ensure exclusive access to the memory-mapped file, preventing accidental overwrites or multiple access points.
Metadata Storage: Metadata about the image (such as width, height, and total map_size) is stored at the beginning of the memory-mapped file.
Pixel Data Writing: The program iterates through each row of the image and writes each row’s pixel data to the mapped memory. Finally, it sets a completion flag to signal that the image data is ready for processing by the subscriber.

### Key Methods
LoadImage(): Loads the image, calculates width, height, and rowSize, and initializes memMapSize.
Init(): Initializes the memory-mapped file for inter-process communication.
MapImageToMemory(): Writes pixel data row by row to the mapped memory, storing metadata at the start.
ReadResultFromMemory(): Reads processed results (most common row colors) back from the memory.

### 2. Subscriber (Python Application)
The Python application subscribes to the image data by:

Accessing the Memory-Mapped File: It connects to the same memory-mapped file created by the C++ publisher.
Row Color Analysis: For each row, it identifies the most common color using the most_common function from Counter (from Python’s collections module).
Result Writing: It writes the most common color for each row back to the mapped memory for the C++ program to read.
Completion Flag: Sets a completion flag in the memory to indicate when processing is complete.

### Key Methods
most_common_color_per_row(row_data): Computes the most frequent color in a row.
process_image_data(): Reads metadata, processes each row, writes results back to memory, and sets the completion flag.

### System Flow and Communication
Initialization: The C++ application loads the image and maps it to memory.
Data Writing: The image is written row by row, and metadata is placed at the start of the mapped memory.
Data Processing: The Python application processes each row, calculates the most common color, and writes the results to the shared memory.
Result Reading: The C++ application reads the processed results after the completion flag is set.

## Time Complexity
### C++ Application
Image Loading: Loading an image using OpenCV (cv::imread) has a time complexity of 𝑂(width × height) as each pixel must be read.
Writing to Memory: Writing row-by-row is also 𝑂(width × height) because each row is processed once.
Result Reading: Reading back the results is 𝑂(height) as it retrieves the most common color for each row only once.
Overall Complexity: 𝑂(width × height)

### Python Application
Color Calculation: The most_common_color_per_row function computes the most frequent color, which is 𝑂(width) per row due to counting each pixel in the row.
Writing Results: Writing the results back to memory is 𝑂(height).
Overall Complexity: 𝑂(width × height)

## Usage
### C++ Setup
Compile and run the C++ code to load the image and initialize the memory-mapped file.
The image and metadata will be written to shared memory, awaiting processing by Python.

### Python Processing
Run PixelColorCalc.py, which processes the shared memory and writes the most common color per row back to it.
Upon completion, the status flag in memory is updated.

### Result Retrieval
The C++ application can read the processed data (most common colors) once the status flag indicates completion.
Prerequisites
C++: OpenCV library for image handling.
Python: Requires Python 3.12.
