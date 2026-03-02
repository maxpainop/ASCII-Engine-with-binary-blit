# ASCII Console Engine Template

## A high-performance, lightweight ASCII rendering engine written in C for the Windows Console. This project bypasses standard printf bottlenecks by utilizing the Win32 API to blit raw memory buffers directly to the terminal at a consistent 60 frames per second.
Key Features

    Binary Blitting: Uses WriteConsoleOutputCharacterA for near-instantaneous frame delivery, avoiding the overhead of standard I/O string processing.

    Asset Loading: Implements a binary file reader that maps .txt or .bin ASCII art directly into RAM.

    60 FPS Frame Limiter: High-resolution timing using QueryPerformanceCounter to ensure smooth, consistent playback without excessive CPU utilization.

    Hardened Console: Automatically strips away window decorations (maximize buttons, resizing handles) and hides the cursor to create a dedicated application environment.

## Technical Implementation
The Framebuffer Logic

The engine treats the terminal as a 1D array of characters (the canvas). Each frame, the engine:

    Clears the buffer using a high-speed memset.

    Copies pre-loaded ASCII assets into the buffer via memcpy.

    Injects dynamic data (like the frame counter) using direct index manipulation.

    Pushes the entire buffer to the console in a single API call.

## High-Resolution Timing

To maintain exactly 16.67ms per frame, the engine calculates the "work time" of each cycle and dynamically adjusts the Sleep duration:
C

double time_taken_sec = (double)(end - start) / frequency;
if (time_taken_sec < 0.0166) {
    double time_to_sleep_sec = 0.0166666666666667 - time_taken_sec;
    DWORD sleep_ms = (DWORD)(time_to_sleep_sec * 1000.0);
    if (sleep_ms > 0) Sleep(sleep_ms);
}

## Project Structure

    sumnlight.cpp: The core engine loop, memory management, and Win32 initialization.

    asciitest.txt: The primary asset file (120x30 recommended).

    systeminfo: A custom struct managing the state of the renderer, high-frequency timing, and memory pointers.

## Requirements and Setup

    Environment: Windows OS (required for windows.h).

    Compiler: Visual Studio 2022 (MSVC).

    Setup: * Clone the repository.

        Ensure asciitest.txt is in the same directory as the executable.

        Set the solution platform to x64 and build.

### License

This project is open-source and available under the MIT License.
