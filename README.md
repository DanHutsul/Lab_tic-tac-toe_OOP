# Lab_tic-tac-toe_OOP

##
 - [Danyil Hutsul](https://github.com/DanHutsul)

## Usage
* Compile the program and run it

## Adding additional types of AI
* Inherit the AI class interface
```c++
class my_ai: public ai {
    // Your Code
}
```

* Implement public `choose_move(board &b, int symbol)` method
```c++
class my_ai: public ai {
public:
    virtual int choose_move(board &b, int symbol) {
        // Implementation
    }
}
```

* Add `char` to `correct_inputs` array for choosing the AI

* Create a new case in `switch` in `game_master` class constructor
    * Change `new_ai` variable
        ```c++
        new_ai = new my_ai();
        ```
## Prerequisites

 - **C++ compiler** - needs to support **C++17** standard
 - **CMake** 3.15+