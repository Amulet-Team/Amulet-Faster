from __future__ import annotations

from typing import TYPE_CHECKING
import logging as _logging

from . import _version

__version__ = _version.get_versions()["version"]

# init a default logger
_logging.basicConfig(level=_logging.INFO, format="%(levelname)s - %(message)s")


def _init() -> None:
    import sys

    from ._faster import init

    init(sys.modules[__name__])


_init()
