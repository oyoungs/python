# python
C++ python runnner with boost.python

# Dependencies
## Boost.Python
You can use apt install this library on Ubuntu list that:
```bash
~$ sudo apt install libboost-python-dev
```

# Install
```bash
~$ git clone https://github.com/oyoungs/python.git
~$ cd python
~$ cmake -Bbuild -H.
~$ cmake --build build --target install
```
**This library will be installed into /usr/local for default. If change wanted, just add -DCMAKE_INSTALL_PREFIX=/your/install/path**

# Usage:
1. execute python code from string

```cpp
#include <oyoung/python.hpp>
#include <iostream>

int main(int argc, char **argv)
{

    std::string error;
    oyoung::python::executor executor(argc, argv);

    if (not executor.exec(R"(
import json
print json.dumps({
    'project': {
        'name': 'oyoungs/python',
        'src_files': [
            'executor.cc'
        ]
    }
}, indent=2)
)", error)) {
        std::cerr << error << std::endl;
    }




    return 0;
}

```

2. execute python code from string

```cpp
#include <oyoung/python.hpp>
#include <iostream>

int main(int argc, char **argv)
{

   std::string error;
   oyoung::python::executor executor(argc, argv);

   if (not executor.exec_file("test/python/test.py", error)) {
       std::cerr << error << std::endl;
   }

   return 0;
}

```

