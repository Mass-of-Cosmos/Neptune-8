Neptune 8 is my first serious project. A chip 8 Emulator/interpreter written in C++


---------------------


I'm using SDL3 here, to display the output, and trying to write modern C++, with best practices to achive a clean design, however, as this is my first time, it wont be flawless.


---------------------


using https://tobiasvl.github.io/blog/write-a-chip-8-emulator/ guide as a reference and a guide.

---------------------

+why?



\-its mostly for learning purposes (and of course, fun!) and want to satisfy my urge for low-level software engineering development.

---------------------

NOTE: I'm trying to avoid using AI LLMs as much as it makes sense (I don't want to compromise my learning, and I want to ensure the craft remains truly mine.). however, this doesn't mean that i don't use them as a helpful tool.

for difficult parts, like reading a ROM using C++ STL fstream header, and basic Cmake or SDL questions, i ask LLMs for example codes, then after reviewing them, i add them to the project using my own logic. also finding out how to implement some systems programming patterns for the first time is not possible by pure "guessing", so LLMs are useful here to understand the basics.

---------------------
Progress:

<img width="2560" height="1440" alt="Screenshot (268)" src="https://github.com/user-attachments/assets/4a12b1e0-6f4d-4497-81da-19d4e5b1b6f9" />

-looks like the emulator can run lots of games now and they work visualy fine; testing is needed for each opcode though.


-all opcodes have been implemented. testing via open source tools is in progress...
---------------------
Todos:

1) add audio support in the main emulator loop using SDL3  

2) testing all of the instructions, and the correctness of the emulator with existing open source tools (after the emulator is done)

3) Adding a way for the user to get a list of available roms in /roms subfolder, and just select it from the list, the user should be aible to add as many files as he/she want to the /roms folder.

---------------------

third party codes/libraries used in the project:

1) SDL3 (for displaying)

2) Random.h header file by www.learncpp.com (for generating high quality random numbers)




