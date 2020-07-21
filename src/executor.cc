//
// Created by oyoung on 19-4-9.
//

#include <oyoung/python/executor.h>
#include <boost/python.hpp>
#include <fstream>

namespace boost {
    namespace python {

        void initialize() { Py_Initialize(); }

        void set_sys_argv(int argc, char **argv) { PySys_SetArgv(argc, argv); }

        void finalize() { Py_Finalize(); }

        class exception : public std::exception {
        public:
            const char *what() const noexcept override;

            std::string type() const {
                return m_type;
            }

            std::string value() const {
                return m_value;
            }

            static std::string name(const std::string &type, const std::string &value) {
                return (std::string(type) += ": ") += value;
            }

            static exception create(const std::string &type, const std::string &value) {
                return exception(type, value);
            }


            exception() = delete;

        private:
            exception(const std::string &type, const std::string &value)
                    : m_type(type), m_value(value), m_what(name(type, value)) {

            }

            std::string m_type;
            std::string m_value;
            std::runtime_error m_what;
        };

        const char *exception::what() const noexcept {
            return m_what.what();
        }

        exception current_exception() {

            std::string value_string;
            std::string type_string("Unfetchable Python Error");

            _object *etype, *evalue, *etraceback;
            PyErr_Fetch(&etype, &evalue, &etraceback);
            PyErr_NormalizeException(&etype, &evalue, &etraceback);

            if (etype) {
                boost::python::handle<> type_handle(etype);
                boost::python::str type_pystr(type_handle);
                boost::python::extract<std::string> type_str(type_pystr);
                if (type_str.check()) {
                    type_string = type_str();
                } else {
                    type_string = "Unknown Exception Type";
                }
            }

            if (evalue) {
                boost::python::handle<> value_handle(evalue);
                boost::python::str value_pystr(value_handle);
                boost::python::extract<std::string> value_str(value_pystr);
                if (value_str.check()) {
                    value_string = value_str();
                } else {
                    value_string = "Unknown Python Error";
                }
            }

            return exception::create(type_string, value_string);
        }
    }
}

namespace oyoung {
    namespace python {

        std::string read_all(const std::string& filepath) {
            std::ifstream input(filepath);

            if (not input.is_open()) {
                throw std::invalid_argument("Open file " + filepath + " failed: No such file");
            }

            return std::string(
                    std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()
            );
        }

        executor::executor(int argc, char **argv) {
            boost::python::initialize();
            boost::python::set_sys_argv(argc, argv);
        }

        executor::~executor() {
            boost::python::finalize();
        }

        bool executor::exec(const std::string &code, std::string &error) {

            try {
                try {
                    auto main = boost::python::import("__main__");
                    auto global = main.attr("__dict__");
                    boost::python::exec(code.c_str(), global, global);
                } catch (const boost::python::error_already_set &) {
                    throw boost::python::current_exception();
                }
            } catch (const boost::python::exception &e) {
                error = e.value();
                return false;
            }

            return true;
        }

        bool executor::exec_file(const std::string &file, std::string &error) {
            try {
                auto code = read_all(file);
                try {
                    auto main = boost::python::import("__main__");
                    auto global = main.attr("__dict__");
                    boost::python::exec(code.c_str(), global, global);
                } catch (const boost::python::error_already_set &) {
                    throw boost::python::current_exception();
                }
            } catch (const boost::python::exception &e) {
                error = e.value();
                return false;
            } catch (const std::exception& e) {
                error = e.what();
                return false;
            }
            return true;
        }
    }
}