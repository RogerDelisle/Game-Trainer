# Game-Trainer

The Game Trainer is the implementation of my personal project which I entitle "Supervised Q-Learning in a 2-Player Board Game." It is a platform-independent artificial-intelligence algorithm that learns  to play tic-tac-toe or other 2-token board games from a partly-randomised analytical opponent. This opponent is automated using a data-driven logic that plays defensively but partly randomises every other move (strategic moves like corner and centre squares are weighed favourably but not absolutely). The simple Q-learning algorithm builds chains of situations in which the number of losses in each of available alternatives drives it not to make the same mistake twice, i.e. not to play a move whose inevitable outcome is a loss.

## Status
The source code is currently implemented using the Qt platform but the classes that perform the analytical and the machine-learning are intended to be platform-independent. I'm currently still working on the early machine-learning portion and it will be uploaded in early April 2018. Meanwhile, the user interface and semi-random analytical opponent compile and work well.

## Environment
I've compiled this using Qt Creator 4.5.1 (Based on Qt 5.10.1) and compiling using MSVC 2015, 64-bits on x86. Even though I haven't yet much effort on portability at this early stage of the implementation, your comments on any portability issues you encounter are most welcome.

## Language Features, Classes, Techniques

- C++
- Qt Widgets
- Unconventional User Interface
- Use of STL containers such as std::map<>
- Use of inline lambda functions to streamline code
- Use of templates to promote code reuse across any graphical platforms

# Installation
 Simply compile all the code in Qt Creator using all files in the same directory.

# License
All the source code in this Repository is original and its licensing is modeled after the MIT License (https://opensource.org/licenses/MIT) 

Copyright 2018 Roger M. Delisle

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
