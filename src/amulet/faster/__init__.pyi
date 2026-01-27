from __future__ import annotations

import os
import typing

from . import _faster, _version

__all__: list[str] = ["FasterKV", "compiler_config"]

class FasterKV:
    def __init__(self, directory: os.PathLike | str | bytes) -> None: ...
    def get(self, key: typing.SupportsInt) -> int: ...
    def remove(self, key: typing.SupportsInt) -> None: ...
    def set(self, key: typing.SupportsInt, value: typing.SupportsInt) -> None: ...

def _init() -> None: ...

__version__: str
compiler_config: dict
