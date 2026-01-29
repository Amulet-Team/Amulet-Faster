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

    py::classh<Amulet::Faster::FasterKV> FasterKV(m, "FasterKV");
    FasterKV.def(
        py::init<std::filesystem::path>(),
        py::arg("directory"));
    // FasterKV.def(
    //     "get",
    //     [](const Amulet::FasterKV& self, py::bytes key) -> py::bytes {
    //         std::string_view key_view = key;
    //         std::string value;
    //         {
    //             py::gil_scoped_release nogil;
    //             value = self.get(key_view);
    //         }
    //         return py::bytes(value);
    //     },
    //     py::arg("key"));
    // FasterKV.def(
    //     "set",
    //     [](Amulet::FasterKV& self, py::bytes key, py::bytes value) {
    //         std::string_view key_view = key;
    //         std::string_view value_view = value;
    //         {
    //             py::gil_scoped_release nogil;
    //             self.set(key_view, value_view);
    //         }
    //     },
    //     py::arg("key"),
    //     py::arg("value"));
    // FasterKV.def(
    //     "remove",
    //     [](Amulet::FasterKV& self, py::bytes key) {
    //         std::string_view key_view = key;
    //         {
    //             py::gil_scoped_release nogil;
    //             self.remove(key_view);
    //         }
    //     },
    //     py::arg("key"));
    FasterKV.def(
        "get",
        [](Amulet::Faster::FasterKV& self, std::uint64_t key) {
            return self.get(key);
        },
        py::arg("key"),
        py::call_guard<py::gil_scoped_release>());
    FasterKV.def(
        "set",
        [](Amulet::Faster::FasterKV& self, std::uint64_t key, std::uint64_t value) {
            self.set(key, value);
        },
        py::arg("key"),
        py::arg("value"),
        py::call_guard<py::gil_scoped_release>());
    FasterKV.def(
        "remove",
        [](Amulet::Faster::FasterKV& self, std::uint64_t key) {
            self.remove(key);
        },
        py::arg("key"),
        py::call_guard<py::gil_scoped_release>());
    FasterKV.def(
        "compact",
        &Amulet::Faster::FasterKV::compact,
        py::call_guard<py::gil_scoped_release>());
}

PYBIND11_MODULE(_faster, m)
{
    m.def("init", &init_module, py::arg("m"));
}
