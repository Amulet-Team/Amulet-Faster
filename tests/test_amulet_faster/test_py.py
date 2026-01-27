import unittest
import tempfile

from amulet.faster import FasterKV


class FasterPythonTestCase(unittest.TestCase):
    def test_py(self) -> None:
        with tempfile.TemporaryDirectory() as tempdir:
            faster_kv = FasterKV(tempdir)
            self.assertEqual(b"test", faster_kv.get(b"hi"))


if __name__ == "__main__":
    unittest.main()
