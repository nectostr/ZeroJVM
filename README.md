# ZeroJVM
This is a minimalistic implementation of a Java Virtual Machine, designed for Flipper Zero devices, developed as a part of CS263 course.

## Reasons
Flipper Zero is an STM32-based device with different periphery (see additional information at https://flipperzero.one/).  
There's a number of different restrictions preventing from running a usual JVM on it:
- RAM size: Flipper Zero has ~130KB of free memory for both JVM and Java code
- OS API restrictions: a number of common C functions are not available in the Flipper Zero firmware
- Build restrictions: Flipper Zero enforces a specific applicaiton format and build procedure for applications

## Capabilities
The developed runtime supports basic Java classes parsing and execution, in particular:
- Basic Java control instructions (stack operations, program flow control, etc.)
- Basic OOP (full support of static & instance fields and methods, new instances of the entrypoint class)
- Byte/Short/Int/Float math
- Text output to Flipper Zero graphical interface
- Sound generation using Flipper Zero internal speaker


## Instructions
### Preparation
To prepare the repository it's required to properly initialize submodules and download corresponding libraries:
- You would need to install the next libraries:
    - `gcc-arm-none-eabi`
    - `libstdc++-arm-none-eabi-newlib`
    - `gcc-multilib`
- Then initialize submodules and `fbt` (flipper build tool)
    - `git submodule update --init --recursive`
    - `cd unleashed-firmware && ./fbt && cd ..`

### Providing Java class
The ZeroJVM consumes a single Java class object and execute `public static void main` method of this class. The class should be named `Entrypoint`, compiled with Java Compiler (usually, `javac Entrypoint.java`) and resulting `Entrypoint.class` should be located in the same folder (for x86 target) or in the root folder of SD card (for flipper target). 

### Compilation
The repository contains 2 targets - x86 and flipper.  

The x86 target is designed to be compiled for and executed on any x86 architecture, and reuse the same ZeroJVM code to allow implementation debug and development.  

You can make this target with `make x86`.

Flipper target allows you to compile the ZeroJVM for Flipper Zero device. You can use the next targets:
1. `make flipper` - it will copy the needed sources to firmware for compiling and start compilation process
2. `make launch` - this target will copy compiled binary application to the Flipper Zero device and launch

## How to ...?
Most of the answers in this section are supported by examples in `tests` folder.

- Q: How to call `System.out.println`?
    - A: instead of using `System.out.println` this implementation intercepts calls to method named `println` of the class `Entrypoint` with one of the next signatures:
        - `public static void println(String s) {}`
        - `public static void println(int s) {}`
        - `public static void println(float s) {}`
    - You can define these methods with empty implementations in the class, and then call in the program. This allows us to reduce runtime overhead and exclude unnecessary initializations.
- Q: How to make program sleep?
    - A: Define and use `public static void delay(int duration_ms) {}` with an empty implementation in the `Entrypoint` class. Runtime will intercept calls to this method and execute the needed instructions.
- Q: How to produce a sound using Flipper Zero speaker?
    - A: Define and use `public static void beep(float frequency, int duration_ms) {}` with an empty implementation in the `Entrypoint` class. Runtime will intercept calls to this method and execute the needed instructions.