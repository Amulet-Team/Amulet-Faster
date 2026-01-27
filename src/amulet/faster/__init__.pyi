from __future__ import annotations

import os

from . import _faster, _version

__all__: list[str] = ["FasterKV", "compiler_config"]

class FasterKV:
    def __init__(self, directory: os.PathLike | str | bytes) -> None: ...
    def get(self, key: bytes) -> bytes: ...
    def remove(self, key: bytes) -> None: ...
    def set(self, key: bytes, value: bytes) -> None: ...

def _init() -> None: ...

__version__: str
compiler_config: dict
