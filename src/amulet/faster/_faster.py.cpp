#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/typing.h>

#include <filesystem>

#include <amulet/faster/faster.hpp>

#include <amulet/pybind11_extensions/compatibility.hpp>

namespace py = pybind11;
namespace pyext = Amulet::pybind11_extensions;

void init_module(py::module m)
{
    pyext::init_compiler_config(m);

    py::classh<Amulet::FasterKV> FasterKV(m, "FasterKV");
    FasterKV.def(
        py::init<std::filesystem::path>(),
        py::arg("directory"));
    FasterKV.def(
        "get",
        [](const Amulet::FasterKV& self, py::bytes key) -> py::bytes {
            std::string_view key_view = key;
            std::string value;
            {
                py::gil_scoped_release nogil;
                value = self.get(key_view);
            }
            return py::bytes(value);
        },
        py::arg("key"));
    FasterKV.def(
        "set",
        [](Amulet::FasterKV& self, py::bytes key, py::bytes value) {
            std::string_view key_view = key;
            std::string_view value_view = value;
            {
                py::gil_scoped_release nogil;
                self.set(key_view, value_view);
            }
        },
        py::arg("key"), 
        py::arg("value"));
    FasterKV.def(
        "remove",
        [](Amulet::FasterKV& self, py::bytes key) {
            std::string_view key_view = key;
            {
                py::gil_scoped_release nogil;
                self.remove(key_view);
            }
        },
        py::arg("key"));
}

PYBIND11_MODULE(_faster, m)
{
    m.def("init", &init_module, py::arg("m"));
}
