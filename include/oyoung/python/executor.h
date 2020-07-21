//
// Created by oyoung on 19-4-9.
//

#ifndef OYOUNG_PYTHON_EXECUTOR_H
#define OYOUNG_PYTHON_EXECUTOR_H

#include <string>



namespace oyoung {
    namespace python {

        class executor {
        public:
            executor(int argc, char **argv);

            bool exec(const std::string &code, std::string &error);

            bool exec_file(const std::string &file, std::string &error);

            ~executor();

        };

    }
}

#endif //TASKCENTER_EXECUTOR_H
