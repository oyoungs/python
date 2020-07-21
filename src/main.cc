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

    if (not executor.exec_file("test/python/test.py", error)) {
        std::cerr << error << std::endl;
    }


    if (not executor.exec_file("test/python/NotFound.py", error)) {
        std::cerr << error << std::endl;
    }




    return 0;
}

